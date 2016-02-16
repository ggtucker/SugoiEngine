#include "Renderer.h"

#include "GLError.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SkyboxVertices.h"

namespace sr {

    std::string Renderer::TextureDirectory = "Resources/Textures/";

Renderer::Renderer() : Renderer(Shader(), Camera()) {}

Renderer::Renderer(const Shader& shader) : Renderer(shader, Camera()) {}

Renderer::Renderer(const Camera& camera) : Renderer(Shader(), camera) {}

Renderer::Renderer(const Shader& shader, const Camera& camera) : camera{ camera } {
    m_shaderManager.BuildDefaultShaders();
    if (shader.ShaderType() != e_shaderInvalid)
        m_shaderManager.AddShader(shader);

	model.push(glm::mat4());
	glEnable(GL_DEPTH_TEST);
	check_gl_error();
}

void Renderer::Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	check_gl_error();
}

void Renderer::SetWireframe(bool active) {
	if (active) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Renderer::LoadTexture(GLint* textureId, const GLchar* imagePath, const std::string& name) {
	*textureId = texturePool.create(imagePath, name);
}

void Renderer::BindTexture(GLint textureId) {
	texturePool[textureId].Bind();
}

void Renderer::BindTextureUnit(GLint textureId, GLint textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	BindTexture(textureId);
}

void Renderer::BindTextureToShader(GLint textureId, GLint textureIndex, EShaderType type) {
	m_shaderManager.GetShaderByEnum(type).BindTexture(texturePool[textureId], textureIndex);
	check_gl_error();
}

/*********************
* Cube Maps
**********************/

GLint Renderer::LoadCubeMapTexture (std::vector<const GLchar*>&& faces) {
    return texturePool.create(std::move(faces));
}

void Renderer::UpdateMVP (const Shader& shader) {
	glm::mat4 _proj = camera.GetProjectionMatrix();
	glm::mat4 _view = camera.GetViewMatrix();
	glm::mat4 _model = model.top();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(_proj));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(_model));
	check_gl_error();
}

void Renderer::PushMatrix() {
	model.push(model.top());
}

void Renderer::PopMatrix() {
	model.pop();
	if (model.empty()) {
		model.push(glm::mat4());
	}
}

void Renderer::Translate(GLfloat x, GLfloat y, GLfloat z) {
	model.top() = glm::translate(model.top(), glm::vec3(x, y, z));
}

void Renderer::Scale(GLfloat x, GLfloat y, GLfloat z) {
	model.top() = glm::scale(model.top(), glm::vec3(x, y, z));
}

void Renderer::Rotate(GLfloat degrees, glm::vec3 rotationAxis) {
	model.top() = glm::rotate(model.top(), degrees, rotationAxis);
}

void Renderer::RotateX(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Renderer::RotateY(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Renderer::RotateZ(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Renderer::RenderMesh(GLuint meshId) {
    const Shader& shader = m_shaderManager.GetActiveShader();
    shader.Use();
	UpdateMVP(shader);
	meshPool[meshId].Render();
}

void Renderer::CreateMesh(GLint* meshId) {
	*meshId = meshPool.create();
}

void Renderer::CreateSkyboxMesh (GLint* meshId) {
    CreateMesh(meshId);

   // for (int i = 0; i < numSkyboxVerts; ++i) {
   //     meshPool[mesh
   // }
}

void Renderer::DeleteMesh(GLint meshId) {
	meshPool.deallocate(meshId);
}

void Renderer::FinishMesh(GLint meshId) {
	meshPool[meshId].Build();
}

void Renderer::ClearMesh(GLint meshId) {
	meshPool[meshId].Clear();
}

void Renderer::GetMeshInfo(GLint meshId, int* numVerts, int* numTriangles) {
	*numVerts = meshPool[meshId].GetNumVertices();
	*numTriangles = meshPool[meshId].GetNumTriangles();
}

GLuint Renderer::AddVertexToMesh(GLuint meshId, const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) {
	return meshPool[meshId].AddVertex(position, normal, texCoords);
}

void Renderer::AddTriangleToMesh(GLuint meshId, GLuint v1, GLuint v2, GLuint v3) {
	meshPool[meshId].AddTriangle(v1, v2, v3);
}

void Renderer::AddCubeToMesh(
	GLuint meshId, glm::vec3 center, glm::vec3 halfSize,
	glm::vec2 tc00, glm::vec2 tc11,
	bool activeXMinus, bool activeXPlus,
	bool activeYMinus, bool activeYPlus,
	bool activeZMinus, bool activeZPlus) {

	glm::vec3 p1(center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z);
	glm::vec3 p2(center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z);
	glm::vec3 p3(center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z);
	glm::vec3 p4(center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z);
	glm::vec3 p5(center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z);
	glm::vec3 p6(center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z);
	glm::vec3 p7(center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z);
	glm::vec3 p8(center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z);

	// Get texture coordinates based on block type
	glm::vec2 tc01 = glm::vec2(tc00.x, tc11.y);
	glm::vec2 tc10 = glm::vec2(tc11.x, tc00.y);

	glm::vec3 norm;

	GLuint v1, v2, v3, v4, v5, v6, v7, v8;

	// Front
	if (!activeZPlus) {
		norm = glm::vec3(0.0f, 0.0f, 1.0f);
		v1 = AddVertexToMesh(meshId, p1, norm, tc00);
		v2 = AddVertexToMesh(meshId, p2, norm, tc10);
		v3 = AddVertexToMesh(meshId, p3, norm, tc11);
		v4 = AddVertexToMesh(meshId, p4, norm, tc01);
		AddTriangleToMesh(meshId, v1, v2, v3);
		AddTriangleToMesh(meshId, v1, v3, v4);
	}

	// Back
	if (!activeZMinus) {
		norm = glm::vec3(0.0f, 0.0f, -1.0f);
		v5 = AddVertexToMesh(meshId, p5, norm, tc00);
		v6 = AddVertexToMesh(meshId, p6, norm, tc10);
		v7 = AddVertexToMesh(meshId, p7, norm, tc11);
		v8 = AddVertexToMesh(meshId, p8, norm, tc01);
		AddTriangleToMesh(meshId, v5, v6, v7);
		AddTriangleToMesh(meshId, v5, v7, v8);
	}

	// Right
	if (!activeXPlus) {
		norm = glm::vec3(1.0f, 0.0f, 0.0f);
		v2 = AddVertexToMesh(meshId, p2, norm, tc00);
		v5 = AddVertexToMesh(meshId, p5, norm, tc10);
		v8 = AddVertexToMesh(meshId, p8, norm, tc11);
		v3 = AddVertexToMesh(meshId, p3, norm, tc01);
		AddTriangleToMesh(meshId, v2, v5, v8);
		AddTriangleToMesh(meshId, v2, v8, v3);
	}

	// Left
	if (!activeXMinus) {
		norm = glm::vec3(-1.0f, 0.0f, 0.0f);
		v6 = AddVertexToMesh(meshId, p6, norm, tc00);
		v1 = AddVertexToMesh(meshId, p1, norm, tc10);
		v4 = AddVertexToMesh(meshId, p4, norm, tc11);
		v7 = AddVertexToMesh(meshId, p7, norm, tc01);
		AddTriangleToMesh(meshId, v6, v1, v4);
		AddTriangleToMesh(meshId, v6, v4, v7);
	}

	// Top
	if (!activeYPlus) {
		norm = glm::vec3(0.0f, 1.0f, 0.0f);
		v4 = AddVertexToMesh(meshId, p4, norm, tc00);
		v3 = AddVertexToMesh(meshId, p3, norm, tc10);
		v8 = AddVertexToMesh(meshId, p8, norm, tc11);
		v7 = AddVertexToMesh(meshId, p7, norm, tc01);
		AddTriangleToMesh(meshId, v4, v3, v8);
		AddTriangleToMesh(meshId, v4, v8, v7);
	}

	// Bottom
	if (!activeYMinus) {
		norm = glm::vec3(0.0f, -1.0f, 0.0f);
		v6 = AddVertexToMesh(meshId, p6, norm, tc00);
		v5 = AddVertexToMesh(meshId, p5, norm, tc10);
		v2 = AddVertexToMesh(meshId, p2, norm, tc11);
		v1 = AddVertexToMesh(meshId, p1, norm, tc01);
		AddTriangleToMesh(meshId, v6, v5, v2);
		AddTriangleToMesh(meshId, v6, v2, v1);
	}
}

bool Renderer::CubeInFrustum(glm::vec3 center, float x, float y, float z) {
	sm::CollisionResult result = camera.CubeInFrustum(center, x, y, z);
	return result != sm::CollisionResult::Outside;
}


void Renderer::SetRenderModeContext (ERenderMode mode) {
    switch (mode) {
    case ERenderMode::e_renderModeWireframe:
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
        break;
    case ERenderMode::e_renderModeSolid:
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    case ERenderMode::e_renderModeShaded:
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    case ERenderMode::e_renderModeTextured:
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    case ERenderMode::e_renderModeTexturedLighting:
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
        break;
    };
}

}