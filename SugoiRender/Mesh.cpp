#include "Mesh.h"

#include "GLError.h"
#include <stdexcept>
#include <GLFW/glfw3.h>

namespace sr {

Mesh::Mesh() : VAO{ 0 }, VBO{ 0 }, EBO{ 0 }, finalized{ false } {}

Mesh::~Mesh() {
	Clear();
}

GLuint Mesh::AddVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) {
	if (!finalized) {
		Vertex v;
		v.Position = position;
		v.Normal = normal;
		v.TexCoords = texCoords;
		this->vertices.push_back(v);
		return this->vertices.size() - 1;
	}
	return -1;
}

void Mesh::AddTriangle(GLuint v1, GLuint v2, GLuint v3) {
	if (!finalized) {
		this->indices.push_back(v1);
		this->indices.push_back(v2);
		this->indices.push_back(v3);
	}
}

GLuint Mesh::BuildOnlyVerts () {
    // This is too strict, verts are only required.
    if (this->vertices.empty()) {
        return -1;
    }
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*)0);

    glBindVertexArray(0);

    return 0;

}

void Mesh::Clear() {
	finalized = false;
	this->vertices.clear();
	this->indices.clear();
	if (this->VAO)
		glDeleteVertexArrays(1, &this->VAO);
	if (this->VBO)
		glDeleteBuffers(1, &this->VBO);
	if (this->EBO)
		glDeleteBuffers(1, &this->EBO);
	check_gl_error();

	this->VAO = 0;
	this->VBO = 0;
	this->EBO = 0;
}

void Mesh::Build() {
    // This is too strict, verts are only required.
	if (this->vertices.empty()) {
		return;
	}

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

	finalized = true;
}

void Mesh::Render() {
	if (finalized) {
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		check_gl_error();
	}
}

}