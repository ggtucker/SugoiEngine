#include "Renderer.h"

#include "GLError.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sr {

    std::string Renderer::TextureDirectory = "Resources/Textures/";

Renderer::Renderer() : Renderer(Shader(), Camera()) {}

Renderer::Renderer(const Shader& shader) : Renderer(shader, Camera()) {}

Renderer::Renderer(const Camera& camera) : Renderer(Shader(), camera) {}

Renderer::Renderer(const Shader& shader, const Camera& camera) : shader{ shader }, camera{ camera } {
	model.push(glm::mat4());
	glEnable(GL_DEPTH_TEST);
	check_gl_error();
}

void Renderer::Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	check_gl_error();
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

void Renderer::BindTextureToShader(GLint textureId, GLint textureIndex) {
	shader.BindTexture(texturePool[textureId], textureIndex);
	check_gl_error();
}

/*********************
* Cube Maps
**********************/

bool Renderer::LoadCubeMapTexture (std::vector<const GLchar*>&& faces) {
    bool loaded = false;
    GLint textureId = texturePool.create(std::move(faces));
    
    


}

void Renderer::BindCubeMapTexture (unsigned int id) {

}

void Renderer::EmptyCubeMapTextureIndex (unsigned int textureIndex) {

}

void Renderer::DisableCubeMapTexture () {

}


void Renderer::UpdateMVP() {
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
	shader.Use();
	UpdateMVP();
	meshPool[meshId].Render();
}

void Renderer::CreateMesh(GLint* meshId) {
	*meshId = meshPool.create();
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

bool Renderer::CubeInFrustum(glm::vec3 center, float x, float y, float z) {
	FrustumResult result = camera.CubeInFrustum(center, x, y, z);
	return result != FrustumResult::Outside;
}

}