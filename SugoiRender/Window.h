#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Keyboard.h"
#include "Event.h"

#define MAX_PENDING 16

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

    static std::array<Event, MAX_PENDING> eventQueue;
    static GLuint queueHead;
    static GLuint queueTail;

    static void push_event(Event event);
    static GLboolean pop_event(Event& event);

    static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_enter_callback(GLFWwindow* window, int entered);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void resize_callback(GLFWwindow* window, int width, int height);
    static void close_callback(GLFWwindow* window);
};
}

#endif