#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "Texture.h"

namespace sr {
class Shader {
public:
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void LoadShaderFiles(const GLchar* vertexPath, const GLchar* fragmentPath);

	void BindTexture(const sr::Texture& tex, int index) const;

	GLuint GetProgram() const;
	void Use() const;

private:
    GLuint program;

	void loadShaderFile(const GLchar* filePath, std::string& shaderSource);
	GLuint compileShader(GLenum shaderType, const GLchar* shaderSource);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadAndCompileShader(GLenum shaderType, const GLchar* filePath);
};
}
