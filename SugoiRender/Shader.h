#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Texture.h"

namespace sr {
class Shader {
public:
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void LoadShaderFiles(const GLchar* vertexPath, const GLchar* fragmentPath);

	GLuint GetProgram() const;
	void Use() const;

	void BindTexture(const Texture& texture, GLuint textureNum) const;
	void UnbindTexture(GLuint num) const;
	
private:
    GLuint program;

	void loadShaderFile(const GLchar* filePath, std::string& shaderSource);
	GLuint compileShader(GLenum shaderType, const GLchar* shaderSource);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadAndCompileShader(GLenum shaderType, const GLchar* filePath);
};
}
