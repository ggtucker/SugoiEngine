#pragma once

#include <glm/glm.hpp>
#include <stack>
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"

namespace sr {
class Renderer {
public:
	Renderer();
	explicit Renderer(const Shader& shader);
	explicit Renderer(const Camera& camera);
	Renderer(const Shader& shader, const Camera& camera);

	void Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const;
	void Render(const Mesh& mesh) const;

	void PushMatrix();
	void PopMatrix();

	void Translate(GLfloat x, GLfloat y, GLfloat z);
	void Scale(GLfloat x, GLfloat y, GLfloat z);

	void Rotate(GLfloat degrees, glm::vec3 rotationAxis);
	void RotateX(GLfloat degrees);
	void RotateY(GLfloat degrees);
	void RotateZ(GLfloat degrees);

	void EnableImmediateMode(GLenum mode);
	void DisableImmediateMode();
	void ImmediateColor(GLfloat red, GLfloat green, GLfloat blue);
	void ImmediateColorAlpha(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
	void ImmediateVertex(GLfloat x, GLfloat y, GLfloat z);
	void ImmediateNormal(GLfloat x, GLfloat y, GLfloat z);
	void ImmediateTexCoord(GLfloat s, GLfloat t);

	Shader& GetShader();
	const Shader& GetShader() const;
	Camera& GetCamera();
	const Camera& GetCamera() const;

private:
	Shader shader;
	Camera camera;
	std::stack<glm::mat4> model;

	void updateMVP() const;
};
}