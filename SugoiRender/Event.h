#pragma once

#include <GL/glew.h>

namespace sr {
class Event {
public:
    enum EventType {
        UNKNOWN = -1,
        WINDOW_CLOSED,
        WINDOW_RESIZED,
        KEY_PRESSED,
        KEY_RELEASED,
        MOUSE_PRESSED,
        MOUSE_RELEASED,
        MOUSE_MOVED,
        MOUSE_SCROLLED,
        MOUSE_ENTERED,
        MOUSE_EXITED
    };

    struct SizeEvent {
        GLuint width;
        GLuint height;
    };

    struct KeyEvent {
        int keyCode;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };

    struct MouseClickedEvent {
        int mouseCode;
		double x;
		double y;
    };

    struct MouseMovedEvent {
        GLfloat x;
        GLfloat y;
    };

    struct MouseScrolledEvent {
        GLfloat xoffset;
        GLfloat yoffset;
    };

    EventType type;

    union {
        SizeEvent resized;
        KeyEvent key;
        MouseClickedEvent mouseClicked;
        MouseMovedEvent mouseMoved;
        MouseScrolledEvent mouseScrolled;
    };
};
}
