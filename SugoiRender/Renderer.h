#pragma once

#include <glm/glm.hpp>
#include <DataStructures\TIndexMemoryPool.h>
#include <stack>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

namespace sr {
class Renderer {
public:
	Renderer();
	Renderer(const Renderer& other);
	Renderer(Renderer&& other);
	explicit Renderer(const Shader& shader);
	explicit Renderer(const Camera& camera);
	Renderer(const Shader& shader, const Camera& camera);

	void Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void RenderMesh(GLuint meshId);

	void PushMatrix();
	void PopMatrix();

	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Scale(GLfloat x, GLfloat y, GLfloat z);

	void Rotate(GLfloat degrees, glm::vec3 rotationAxis);
	void RotateX(GLfloat degrees);
	void RotateY(GLfloat degrees);
	void RotateZ(GLfloat degrees);

	void SetRenderMode(GLenum mode);
	void EnableImmediateMode(GLenum mode);
	void DisableImmediateMode();
	void ImmediateColor(GLfloat red, GLfloat green, GLfloat blue);
	void ImmediateColorAlpha(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void ImmediateVertex(GLfloat x, GLfloat y, GLfloat z);
	void ImmediateNormal(GLfloat x, GLfloat y, GLfloat z);
	void ImmediateTexCoord(GLfloat s, GLfloat t);

	void CreateMesh(GLint* meshId);
	void DeleteMesh(GLint meshId);
	void FinishMesh(GLint meshId);
	void ClearMesh(GLint meshId);
	void GetMeshInfo(GLint meshId, int* numVerts, int* numTriangles);
	GLuint AddVertexToMesh(GLuint meshId, const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);
	void AddTriangleToMesh(GLuint meshId, GLuint v1, GLuint v2, GLuint v3);

	Shader& GetShader();
	Camera& GetCamera();

private:
	Shader shader;
	Camera camera;
	std::stack<glm::mat4> model;

	TIndexMemoryPool<Mesh, 1024> meshPool;

	void updateMVP();
};
}