#include "Keyboard.h"

#define MAX_KEY_STATES 1024

namespace sr {
namespace Keyboard {

static std::array<bool, MAX_KEY_STATES> keyStates;

bool IsKeyPressed(GLuint keyCode) {
	return keyStates[keyCode];
}

void KeyDown(GLuint keyCode) {
	keyStates[keyCode] = true;
}

void KeyUp(GLuint keyCode) {
	keyStates[keyCode] = false;
}

}
}