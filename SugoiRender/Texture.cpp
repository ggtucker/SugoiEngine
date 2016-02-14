#include "Texture.h"
#include <cassert>
#include <SOIL2/SOIL2.h>
#include "GLError.h"

#define INVALID_TEXTURE ((unsigned int)-1)

namespace sr {

Texture::Texture() : id{ INVALID_TEXTURE } {}

Texture::Texture(const Texture& other) : id{ INVALID_TEXTURE } {
	*this = other;
}

Texture::Texture(Texture&& other) : id{ other.id }, name{ std::move(other.name) }, width{ other.width }, height{ other.height } {
	other.id = 0;
	other.width = 0;
	other.height = 0;
}

Texture::Texture(const GLchar* imagePath, const std::string& name) : id{ INVALID_TEXTURE }, name { name } {
	Load(imagePath);
}

Texture::Texture (std::vector<const GLchar*>&& faces) {
    LoadCubeMap(std::move(faces));
}

Texture::~Texture() {
	if (IsLoaded()) {
		Clear();
	}
}

Texture& Texture::operator=(const Texture& other) {
	this->name = other.name;
	if (!other.IsLoaded()) {
		this->id = INVALID_TEXTURE;
		this->width = 0;
		this->height = 0;
		return *this;
	}
	unsigned char* image = new unsigned char[other.width*other.height << 2];
	other.Bind();
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	Load(image, other.width, other.height);
	delete[] image;
	return *this;
}

Texture& Texture::operator=(Texture&& other) {
	assert(this != &other);
	glDeleteTextures(1, &this->id);
	this->id = other.id;
	this->name = std::move(other.name);
	this->width = other.width;
	this->height = other.height;
	other.id = 0;
	other.width = 0;
	other.height = 0;
	return *this;
}

void Texture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->id);
	check_gl_error();
}

void Texture::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, NULL);
	check_gl_error();
}

void Texture::Bind(GLuint textureNum) const {
	assert(textureNum < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	glActiveTexture(GL_TEXTURE0 + textureNum);
	Bind();
}

void Texture::Unbind(GLuint textureNum) const {
	assert(textureNum < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);
	glActiveTexture(GL_TEXTURE0 + textureNum);
	Unbind();
}

void Texture::Clear() {
	glDeleteTextures(1, &this->id);
	this->id = INVALID_TEXTURE;
	this->width = 0;
	this->height = 0;
	name.clear();
}

void Texture::Load(const GLchar* imagePath) {
	int w, h;
	unsigned char* image = SOIL_load_image(imagePath, &w, &h, 0, SOIL_LOAD_RGB);
	assert(image);
	Load(image, w, h);
	SOIL_free_image_data(image);
}

void Texture::Load(const unsigned char* image, unsigned int w, unsigned int h) {
	if (w > (unsigned int)1 << 31 || h > (unsigned int)1 << 31 || (unsigned long long)w*h > (unsigned long long)1 << 31 || image == '\0') {
		return;
	}

	if (IsLoaded()) {
		Clear();
	}

	this->width = w;
	this->height = h;

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();
}

void Texture::LoadCubeMap (std::vector<const GLchar*>&& faces) {
    glGenTextures(1, &id);
    glActiveTexture(GL_TEXTURE0);

    int width, height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    for (GLuint i = 0; i < faces.size(); ++i) {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
            width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    check_gl_error();
}


bool Texture::IsLoaded() const {
	return (this->id != INVALID_TEXTURE);
}

GLuint Texture::GetId() const {
	return this->id;
}

std::string& Texture::GetName() {
	return this->name;
}

const std::string& Texture::GetName() const {
	return this->name;
}

}