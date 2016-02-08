#pragma once

#include <GL/glew.h>
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"

namespace sr {
class Mesh {
public:
	Mesh();
	Mesh(const Mesh& other);
	Mesh(Mesh&& other);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures);
	~Mesh();

	Mesh& operator=(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other);

	void Render(const Shader& shader) const;

	GLuint AddVertex(Vertex&& vertex);
	GLuint AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords);
	void AddTriangle(GLuint v1, GLuint v2, GLuint v3);
	void AddTexture(const Texture& texture);
	void Build();

private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLuint VAO, VBO, EBO;
};
}