#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Window.h"
#include "Keyboard.h"

GLfloat vertices[] = {
    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};

// Define our camera
sr::Camera camera;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Function prototypes
void do_movement();
void mouse_moved(GLfloat xpos, GLfloat ypos);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = 400, lastY = 300;

bool firstMouse = true;

GLuint generateDefaultVBO() {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return VBO;
}

/*GLuint generateDefaultEBO() {
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    return EBO;
}*/

int main() {
    // Create our window for drawing
    sr::Window window(800, 600, "SugoiRender", true);
    window.SetMouseCursorVisible(false);

    // Build and compile our shader program
    sr::Shader shader("shader.vert", "shader.frag");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // Generate default vertex buffer object
    glBindVertexArray(VAO);
    GLuint VBO = generateDefaultVBO();
    //GLuint EBO = generateDefaultEBO();
    glBindVertexArray(0);

    // Load and create a texture
    sr::Texture texture1("wood_container.jpg");
    sr::Texture texture2("awesome_face.png");

    glEnable(GL_DEPTH_TEST);

    lastFrame = glfwGetTime();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (window.isOpen()) {

        sr::Event event;
        while(window.pollEvent(event)) {
            switch(event.type) {
            case sr::Event::WINDOW_CLOSED:
                window.close();
                break;
            case sr::Event::KEY_PRESSED:
                std::cout << "Key pressed: " << event.key.keyCode << std::endl;
                break;
            case sr::Event::KEY_RELEASED:
                std::cout << "Key released: " << event.key.keyCode << std::endl;
                if(event.key.keyCode == GLFW_KEY_ESCAPE) {
                    window.close();
                }
                break;
            case sr::Event::MOUSE_MOVED:
                mouse_moved(event.mouseMoved.x, event.mouseMoved.y);
                break;
            case sr::Event::MOUSE_SCROLLED:
                camera.SetZoom(camera.GetZoom() - event.mouseScrolled.yoffset);
                break;
            }
        }

        do_movement();

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind textures
        texture1.BindTexture("Texture1", shader.GetProgram(), 0);
        texture2.BindTexture("Texture2", shader.GetProgram(), 1);
        
        // Model/view/projection matrices
        glm::mat4 view;
        view = camera.GetViewMatrix();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.GetZoom()), (GLfloat) WIDTH / HEIGHT, 0.1f, 100.0f);

        // Activate shader
        shader.Use();

        // Bind model/view/projection matrices
        GLuint modelLoc = glGetUniformLocation(shader.GetProgram(), "model");
        GLuint viewLoc = glGetUniformLocation(shader.GetProgram(), "view");
        GLuint projectionLoc = glGetUniformLocation(shader.GetProgram(), "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Draw container
        glBindVertexArray(VAO);
        for(GLuint i = 0; i < 10; ++i) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = glm::radians(20.0f * i);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window.getWindow());
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void do_movement() {
    GLfloat cameraSpeed = camera.GetMovementSpeed() * deltaTime;
    if(sr::Keyboard::IsKeyPressed(GLFW_KEY_W)) {
        camera += cameraSpeed * camera.GetFront();
    }
    if(sr::Keyboard::IsKeyPressed(GLFW_KEY_S)) {
        camera -= cameraSpeed * camera.GetFront();
    }
    if(sr::Keyboard::IsKeyPressed(GLFW_KEY_A)) {
        camera -= cameraSpeed * camera.GetRight();
    }
    if(sr::Keyboard::IsKeyPressed(GLFW_KEY_D)) {
        camera += cameraSpeed * camera.GetRight();
    }
}

void mouse_moved(GLfloat xpos, GLfloat ypos) {
    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= camera.GetMouseSensitivity();
    yoffset *= camera.GetMouseSensitivity();

    camera.SetYaw(camera.GetYaw() + xoffset);
    camera.SetPitch(camera.GetPitch() + yoffset);
}