#pragma once

#include <GL/glew.h>
#include <cassert>
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "Uncopyable.h"

namespace sr {
class Mesh : private Uncopyable {
public:
	Mesh();
	~Mesh();

	GLuint AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);
	void AddTriangle(GLuint v1, GLuint v2, GLuint v3);

	void Clear();
	void Build();
	void Render();

	int GetNumVertices() { return vertices.size(); }
	int GetNumTriangles() { return indices.size() / 3; }

private:
	GLuint VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	bool finalized;
};
}