#pragma once

#include <array>
#include <GL/glew.h>

#define MAX_COMMAND_INDEX 1024

namespace InputHandler {
    enum KeyState {
        PRESSED = true,
        RELEASED = false
    };

    static std::array<KeyState, MAX_COMMAND_INDEX> keyStates;

    KeyState IsKeyPressed(GLuint keyCode) {
        return keyStates[keyCode];
    }

    void KeyDown(GLuint keyCode) {
        keyStates[keyCode] = PRESSED;
    }

    void KeyUp(GLuint keyCode) {
        keyStates[keyCode] = RELEASED;
    }
}