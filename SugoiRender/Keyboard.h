#pragma once

#include <array>
#include <GL/glew.h>

#define MAX_KEY_STATES 1024

namespace sr {
namespace Keyboard {

	static std::array<bool, MAX_KEY_STATES> keyStates;

	bool IsKeyPressed(GLuint keyCode);

	void KeyDown(GLuint keyCode);

	void KeyUp(GLuint keyCode);
}
}
