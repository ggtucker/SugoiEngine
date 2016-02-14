#pragma once

#include <glm/glm.hpp>
#include <DataStructures\TIndexMemoryPool.h>
#include <stack>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderManager.h"

namespace sr {

    enum ERenderMode {
        e_renderModeWireframe,
        e_renderModeSolid,
        e_renderModeShaded,
        e_renderModeTextured,
        e_renderModeTexturedLighting,
        e_renderModeCubeMap,
        e_lastRenderMode
    };

    enum EProjectionMode {
        e_projectionModePerspective = 0,
        e_projectionModeOrthographic,
        e_projectionMode2D
    };

    enum EPrimitiveMode {
        e_primitivePoints = 0,
        e_primitiveLines,
        e_primitiveTriangles,
        e_primitiveTriangleStrips,
        e_primitiveTriangleFans,
        e_primitiveQuads
    };

    enum ECullMode {
        e_cullModeNoCull = 0,
        e_cullModeBack,
        e_cullModeFront
    };

class Renderer {
public:
	Renderer();
	explicit Renderer(const Shader& shader);
	explicit Renderer(const Camera& camera);
	Renderer(const Shader& shader, const Camera& camera);

	void Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void SetWireframe(bool active);

	// Texture
	void LoadTexture(GLint* textureId, const GLchar* imagePath, const std::string& name);
	void BindTexture(GLint textureId);
	void BindTextureUnit(GLint textureId, GLint textureIndex);
	void BindTextureToShader(GLint textureId, GLint textureIndex, EShaderType type = e_shaderDefault);

    // Cube Maps
    // TODO this should be refactored to take a CubeMap type or a vector
    GLint LoadCubeMapTexture (std::vector<const GLchar*>&& faces);

    void RenderSkybox (GLuint id);

   //void BindCubeMapTexture (unsigned int id);
   //void EmptyCubeMapTextureIndex (unsigned int textureIndex);
   //void DisableCubeMapTexture ();

	// Matrix stack
	void UpdateMVP (const Shader& shader);
	void PushMatrix();
	void PopMatrix();

	// Matrix transformations
	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Scale(GLfloat x, GLfloat y, GLfloat z);
	void Rotate(GLfloat degrees, glm::vec3 rotationAxis);
	void RotateX(GLfloat degrees);
	void RotateY(GLfloat degrees);
	void RotateZ(GLfloat degrees);

	// Mesh
	void RenderMesh(GLuint meshId);
	void CreateMesh(GLint* meshId);
    void CreateSkyboxMesh (GLint* meshId);
	void DeleteMesh(GLint meshId);
	void FinishMesh(GLint meshId);
	void ClearMesh(GLint meshId);
	void GetMeshInfo(GLint meshId, int* numVerts, int* numTriangles);
	GLuint AddVertexToMesh(GLuint meshId, const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);
	void AddTriangleToMesh(GLuint meshId, GLuint v1, GLuint v2, GLuint v3);

	// Camera
	bool CubeInFrustum(glm::vec3 center, float x, float y, float z);

	Camera& GetCamera() { return camera; }

    // Rendering Code
    void SetRenderModeContext (ERenderMode mode);

    // Temporary
    static std::string TextureDirectory;

    void SetActiveShader (EShaderType type) { m_shaderManager.SetActiveShader(type); }

private:
    ShaderManager m_shaderManager;
	Camera camera;
	std::stack<glm::mat4> model;

	TIndexMemoryPool<Mesh, 1024> meshPool;
	TIndexMemoryPool<Texture, 16> texturePool;

};
}