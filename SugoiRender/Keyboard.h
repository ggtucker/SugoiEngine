#pragma once

#include <array>
#include <GL/glew.h>

namespace sr {
namespace Keyboard {

bool IsKeyPressed(GLuint keyCode);

void KeyDown(GLuint keyCode);

void KeyUp(GLuint keyCode);

}
}
