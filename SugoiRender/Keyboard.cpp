#include "Keyboard.h"

namespace sr {
bool Keyboard::IsKeyPressed(GLuint keyCode) {
    return keyStates[keyCode];
}

void Keyboard::KeyDown(GLuint keyCode) {
    keyStates[keyCode] = true;
}

void Keyboard::KeyUp(GLuint keyCode) {
    keyStates[keyCode] = false;
}
}