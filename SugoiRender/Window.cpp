#include "Window.h"

namespace sr {
CircularQueue<Event> Window::eventQueue;

Window::Window() : window{ nullptr }, isWindowOpen{ false } {
}

Window::Window(GLuint width, GLuint height, const GLchar* title, GLboolean resizable) : window{ nullptr }, isWindowOpen { false } {
    Create(width, height, title, resizable);
}

Window::~Window() {
	glfwTerminate();
}

void Window::Create(GLuint width, GLuint height, const GLchar* title, GLboolean resizable) {
	if (this->isWindowOpen) {
		throw new std::runtime_error("Window.Create() has already been called");
	}

    this->width = width;
    this->height = height;
    this->isWindowOpen = true;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* sharedContext = glfwGetCurrentContext();
    this->window = glfwCreateWindow(width, height, title, nullptr, sharedContext);
    if(window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        this->isWindowOpen = false;
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetCursorEnterCallback(window, mouse_enter_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetWindowSizeCallback(window, resize_callback);
    glfwSetWindowCloseCallback(window, close_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;

    // Initialize GLEW to setup the OpenGL Function pointers
    if(glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
    }

    // Define the viewport dimensions
    glViewport(0, 0, width, height);
}

void Window::SetMouseCursorVisible(GLboolean visible) {
    if(visible) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Window::Close() {
    this->isWindowOpen = false;
}

void Window::SwapBuffers() {
	glfwSwapBuffers(this->window);
}

GLboolean Window::PollEvent(Event& event) {
    glfwPollEvents();
    return eventQueue.Pop(event);
}

GLboolean Window::IsOpen() const {
	return this->isWindowOpen;
}

GLuint Window::GetWidth() const {
	return this->width;
}

GLuint Window::GetHeight() const {
	return this->height;
}

void Window::key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode) {
    Event keyEvent;
    keyEvent.key.keyCode = key;
    keyEvent.key.alt = Keyboard::IsKeyPressed(GLFW_KEY_LEFT_ALT);
    keyEvent.key.control = Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL);
    keyEvent.key.shift = Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT);
    keyEvent.key.system = Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SUPER);

    if(action == GLFW_PRESS) {
        Keyboard::KeyDown(key);
        keyEvent.type = Event::KEY_PRESSED;
		eventQueue.Push(keyEvent);
    } else if(action == GLFW_RELEASE) {
        Keyboard::KeyUp(key);
        keyEvent.type = Event::KEY_RELEASED;
		eventQueue.Push(keyEvent);
    }
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Event mouseEvent;
    mouseEvent.mouseClicked.mouseCode = button;

    if(action == GLFW_PRESS) {
        mouseEvent.type = Event::MOUSE_PRESSED;
		eventQueue.Push(mouseEvent);
    } else if(action == GLFW_RELEASE) {
        mouseEvent.type = Event::MOUSE_RELEASED;
		eventQueue.Push(mouseEvent);
    }
}

void Window::mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
    Event mouseEvent;
    mouseEvent.type = Event::MOUSE_MOVED;
    mouseEvent.mouseMoved.x = xpos;
    mouseEvent.mouseMoved.y = ypos;
	eventQueue.Push(mouseEvent);
}

void Window::mouse_enter_callback(GLFWwindow* window, int entered) {
    Event mouseEvent;
    if(entered) {
        mouseEvent.type = Event::MOUSE_ENTERED;
    } else {
        mouseEvent.type = Event::MOUSE_EXITED;
    }
	eventQueue.Push(mouseEvent);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Event mouseEvent;
    mouseEvent.type = Event::MOUSE_SCROLLED;
    mouseEvent.mouseScrolled.xoffset = xoffset;
    mouseEvent.mouseScrolled.yoffset = yoffset;
	eventQueue.Push(mouseEvent);
}

void Window::resize_callback(GLFWwindow* window, int width, int height) {
    Event resizeEvent;
    resizeEvent.type = Event::WINDOW_RESIZED;
    resizeEvent.resized.width = width;
    resizeEvent.resized.height = height;
	eventQueue.Push(resizeEvent);
}

void Window::close_callback(GLFWwindow* window) {
    if(glfwWindowShouldClose(window)) {
        Event windowClosedEvent;
        windowClosedEvent.type = Event::WINDOW_CLOSED;
		eventQueue.Push(windowClosedEvent);
    }
}


}