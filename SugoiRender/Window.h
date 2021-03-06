#pragma once

#include <GL/glew.h>
#include <array>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Keyboard.h"
#include "Event.h"
#include "DataStructures/TCircularQueue.h"

namespace sr {
class Window {
public:
    Window();
    Window(GLuint width, GLuint height, const GLchar* title, GLboolean resizable);
	~Window();

    void Create(GLuint width, GLuint height, const GLchar* title, GLboolean resizable);
    void SetMouseCursorVisible(GLboolean visible);
	void SwapBuffers();
    GLboolean PollEvent(Event& event);

	bool IsOpen() { return isWindowOpen; }
	void Close() { isWindowOpen = false; }

	void SetPosition(int x, int y);

	GLboolean IsOpen() const;
	GLuint GetWidth() const;
	GLuint GetHeight() const;
private:
    GLFWwindow* window;
    GLboolean isWindowOpen;
    GLuint width;
    GLuint height;

	static TCircularQueue<Event> eventQueue;

    static void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_enter_callback(GLFWwindow* window, int entered);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void resize_callback(GLFWwindow* window, int width, int height);
    static void close_callback(GLFWwindow* window);
};
}
