#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <iostream>
#include <SOIL2/SOIL2.h>
#include <GL/glew.h>

namespace sr {
class Texture {
public:
    Texture(const GLchar* imagePath);
    void BindTexture(const GLchar* uniformName, GLuint program, GLint textureNum);
private:
    GLuint textureId;
};
}

#endif