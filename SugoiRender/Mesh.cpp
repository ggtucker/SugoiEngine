#include "Mesh.h"

#include <stdexcept>

namespace sr {
Mesh::Mesh() {}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
	: vertices{ vertices }, indices{ indices }, textures{ textures } {

	this->Build();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Render(const Shader& shader) const {
	if (this->VAO == NULL) {
		throw std::invalid_argument("Cannot render: must call Mesh::Build() first");
	}

	for (GLuint i = 0; i < this->textures.size(); ++i) {
		shader.BindTexture(this->textures[i], i);
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->textures.size(); ++i) {
		shader.UnbindTexture(i);
	}
}

GLuint Mesh::AddVertex(const Vertex& vertex) {
	this->vertices.push_back(vertex);
	return this->vertices.size() - 1;
}

GLuint Mesh::AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) {
	Vertex v;
	v.Position = position;
	v.Normal = normal;
	v.TexCoords = texCoords;
	return AddVertex(v);
}

void Mesh::AddTriangle(GLuint v1, GLuint v2, GLuint v3) {
	this->indices.push_back(v1);
	this->indices.push_back(v2);
	this->indices.push_back(v3);
}

void Mesh::AddTexture(const Texture& texture) {
	this->textures.push_back(texture);
}

void Mesh::Build() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

}