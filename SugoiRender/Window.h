#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Keyboard.h"
#include "Event.h"

namespace sr {
class Window {
public:
    Window();
    Window(GLuint width, GLuint height, const GLchar* title, GLboolean resizable);
    void Create(GLuint width, GLuint height, const GLchar* title, GLboolean resizable);
    void SetMouseCursorVisible(GLboolean visible);
    void close();
    GLboolean isOpen();
    GLboolean pollEvent(Event& event);
    GLFWwindow* getWindow();
private:
    GLFWwindow* window;
    GLboolean isWindowOpen;
    GLuint width;
    GLuint height;

    static const GLuint MAX_PENDING = 16;
    static Event eventQueue[MAX_PENDING];
    static GLuint queueHead;
    static GLuint queueTail;

    static GLuint queue_size();
    static void push_event(Event event);
    static GLboolean pop_event(Event& event);

    static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void close_callback(GLFWwindow* window);
};
}

#endif