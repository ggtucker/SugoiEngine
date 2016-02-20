#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "Texture.h"

namespace sr {

enum EShaderType {
    e_shaderInvalid = -1,
    e_shaderDefault,
    e_shaderCubeMap,
    e_shaderLast,
    e_shaderCount = e_shaderLast - e_shaderDefault
};
class Shader {

    friend class ShaderManager;
public:
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void LoadShaderFiles(const GLchar* vertexPath, const GLchar* fragmentPath);

	void BindTexture(const sr::Texture& tex, int index) const;

	GLuint GetProgram() const;
	void Use() const;

    void SetShaderType (EShaderType type) { m_shaderType = type; }
    EShaderType ShaderType () const { return m_shaderType; }

private:
    GLuint program;
    EShaderType m_shaderType;
	void loadShaderFile(const GLchar* filePath, std::string& shaderSource);
	GLuint compileShader(GLenum shaderType, const GLchar* shaderSource);
	GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadAndCompileShader(GLenum shaderType, const GLchar* filePath);
};
}
