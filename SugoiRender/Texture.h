#pragma once

#include <GL/glew.h>
#include <iostream>
#include <SOIL2/SOIL2.h>

namespace sr {
class Texture {
public:
    Texture(const GLchar* imagePath, const std::string& name);
	GLuint GetId() const;
	std::string& GetName();
	const std::string& GetName() const;
private:
    GLuint id;
	std::string name;

	void init(const GLchar* imagePath);
};
}
