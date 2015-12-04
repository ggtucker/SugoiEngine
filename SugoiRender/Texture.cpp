#include "Texture.h"

namespace sr {
Texture::Texture(const GLchar* imagePath, const std::string& name) : name{ name } {
	init(imagePath);
}

void Texture::init(const GLchar* imagePath) {
	// First load the image into memory using SOIL
	int width, height;
	unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == '\0') {
		std::cout << "Unable to load image for texture: " << imagePath << std::endl;
	}

	// Bind our new OpenGL texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	// Set texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image data into our OpenGL texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Free image data from memory and unbind OpenGL texture
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
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