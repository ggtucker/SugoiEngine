#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

namespace sr {
class Shader {
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	GLuint GetProgram();
	void Use();
private:
    GLuint program;

	void loadShaderFile(const GLchar* filePath, std::string& shaderSource);
	GLuint compileShader(GLenum shaderType, const GLchar* shaderSource);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadAndCompileShader(GLenum shaderType, const GLchar* filePath);
};
}

#endif