#include "Mesh.h"

#include "GLError.h"
#include <stdexcept>

namespace sr {
Mesh::Mesh() {}

Mesh::Mesh(const Mesh& other) {
	
}

Mesh::Mesh(Mesh&& other)
	: VAO{ other.VAO }, VBO{ other.VBO }, EBO{ other.EBO },
	vertices{ std::move(other.vertices) }, indices{ std::move(other.indices) }, textures{ std::move(other.textures) } {

	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::vector<Texture>& textures)
	: vertices{ vertices }, indices{ indices }, textures{ textures } {

	this->Build();
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	check_gl_error();
}

Mesh& Mesh::operator=(Mesh&& other) {
	assert(this != &other);
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);
	check_gl_error();
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->vertices = std::move(other.vertices);
	this->indices = std::move(other.indices);
	this->textures = std::move(other.textures);
	other.VAO = 0;
	other.VBO = 0;
	other.EBO = 0;
	return *this;
}

void Mesh::Render(const Shader& shader) const {
	assert(this->VAO);

	for (GLuint i = 0; i < this->textures.size(); ++i) {
		const Texture& tex = this->textures[i];
		tex.Bind(i);
		glUniform1i(glGetUniformLocation(shader.GetProgram(), tex.GetName().c_str()), i);
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->textures.size(); ++i) {
		this->textures[i].Unbind(i);
	}

	check_gl_error();
}

GLuint Mesh::AddVertex(Vertex&& vertex) {
	this->vertices.push_back(vertex);
	return this->vertices.size() - 1;
}

GLuint Mesh::AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) {
	Vertex v;
	v.Position = position;
	v.Normal = normal;
	v.TexCoords = texCoords;
	return AddVertex(std::move(v));
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
	size_t noffset = offsetof(Vertex, Normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)noffset);

	// Vertex texture coords
	glEnableVertexAttribArray(2);
	size_t toffset = offsetof(Vertex, TexCoords);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)toffset);

	glBindVertexArray(0);

	check_gl_error();
}

}