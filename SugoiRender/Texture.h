#pragma once

#include <iostream>
#include <SOIL2/SOIL2.h>
#include <GL/glew.h>

class Texture {
public:
    Texture(const GLchar* imagePath);
    void BindTexture(const GLchar* uniformName, GLuint program, GLint textureNum);
private:
    GLuint textureId;
};

Texture::Texture(const GLchar* imagePath) {
    // First load the image into memory using SOIL
    int width, height;
    unsigned char* image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
    if(image == '\0') {
        std::cout << "Unable to load image for texture: " << imagePath << std::endl;
    }

    // Bind our new OpenGL texture
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

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

void Texture::BindTexture(const GLchar* uniformName, GLuint program, GLint textureNum) {
    if(textureNum >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        std::cout << "Cannot bind '" << uniformName << "' to GL_TEXTURE" << textureNum << ". Not enough texture units." << std::endl;
    }
    glActiveTexture(GL_TEXTURE0 + textureNum);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glUniform1i(glGetUniformLocation(program, uniformName), textureNum);
}