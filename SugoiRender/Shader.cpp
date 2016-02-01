#include "Shader.h"

#include <stdexcept>
#include <cassert>

namespace sr {

Shader::Shader() {}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	LoadShaderFiles(vertexPath, fragmentPath);
}

void Shader::LoadShaderFiles(const GLchar* vertexPath, const GLchar* fragmentPath) {
	GLuint vertexShader = loadAndCompileShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = loadAndCompileShader(GL_FRAGMENT_SHADER, fragmentPath);
	this->program = createShaderProgram(vertexShader, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint Shader::GetProgram() const {
    return this->program;
}

void Shader::Use() const {
	glUseProgram(this->program);
}

void Shader::loadShaderFile(const GLchar* filePath, std::string& shaderSource) {
	// Stream for loading shader file
	std::ifstream shaderFile;
	// Ensures ifstream object can throw exceptions
	shaderFile.exceptions(std::ifstream::badbit);
	try {
		// Open shader file
		shaderFile.open(filePath);
		// Read file buffer contents into stream
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		// Close file handle
		shaderFile.close();
		// Convert stream into GLchar array
		shaderSource = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << filePath << std::endl;
	}
}

GLuint Shader::compileShader(GLenum shaderType, const GLchar* shaderSource) {
	assert(shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER);
	// Create and compile shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	// Detect and log any compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}

GLuint Shader::createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
	// Create shader program and link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Detect and log any linking errors
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
	return shaderProgram;
}

GLuint Shader::loadAndCompileShader(GLenum shaderType, const GLchar* filePath) {
	std::string shaderString;
	loadShaderFile(filePath, shaderString);
	return compileShader(shaderType, shaderString.c_str());
}
}