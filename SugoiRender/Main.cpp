#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Renderer.h"

void _check_gl_error(const char *file, int line);

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

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

// Function prototypes
sr::Mesh createCube();
void doMovement(sr::Camera& camera);
void mouseMoved(GLfloat xpos, GLfloat ypos, sr::Camera& camera);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = 400, lastY = 300;

bool firstMouse = true;

int main() {
    // Create our window for drawing
    sr::Window window(800, 600, "SugoiRender", true);
    window.SetMouseCursorVisible(false);
	check_gl_error();

    lastFrame = glfwGetTime();

	sr::Shader shader("shader.vert", "shader.frag");
	sr::Renderer renderer(shader);
	sr::Camera& camera = renderer.GetCamera();
	sr::Mesh cube = createCube();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while (window.IsOpen()) {

        sr::Event event;
        while(window.PollEvent(event)) {
            switch(event.type) {
            case sr::Event::WINDOW_CLOSED:
                window.Close();
                break;
            case sr::Event::WINDOW_RESIZED:
                std::cout << "Window resized: " << event.resized.width << " " << event.resized.height << std::endl;
                break;
            case sr::Event::KEY_PRESSED:
                std::cout << "Key pressed: " << event.key.keyCode << std::endl;
                break;
            case sr::Event::KEY_RELEASED:
                std::cout << "Key released: " << event.key.keyCode << std::endl;
                if(event.key.keyCode == GLFW_KEY_ESCAPE) {
                    window.Close();
                }
                break;
            case sr::Event::MOUSE_PRESSED:
                std::cout << "Mouse pressed: " << event.mouseClicked.mouseCode << std::endl;
                break;
            case sr::Event::MOUSE_RELEASED:
                std::cout << "Mouse released: " << event.mouseClicked.mouseCode << std::endl;
                break;
            case sr::Event::MOUSE_MOVED:
                mouseMoved(event.mouseMoved.x, event.mouseMoved.y, camera);
                break;
            case sr::Event::MOUSE_SCROLLED:
                camera.SetZoom(camera.GetZoom() - event.mouseScrolled.yoffset);
                break;
            case sr::Event::MOUSE_ENTERED:
                std::cout << "Mouse entered" << std::endl;
                break;
            case sr::Event::MOUSE_EXITED:
                std::cout << "Mouse exited" << std::endl;
                break;
            }
        }

        doMovement(camera);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);
        
        for(GLuint i = 0; i < 10; ++i) {
			renderer.PushMatrix();
			renderer.Translate(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
			renderer.Rotate(20.0f * i, glm::vec3(1.0f, 0.3f, 0.5f));
			renderer.Render(cube);
			renderer.PopMatrix();
        }

		window.SwapBuffers();
    }

    return 0;
}

void doMovement(sr::Camera& camera) {
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

void mouseMoved(GLfloat xpos, GLfloat ypos, sr::Camera& camera) {
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

sr::Mesh createCube() {
	sr::Mesh mesh;
	mesh.AddTexture(sr::Texture("wood_container.jpg", "Texture1"));
	mesh.AddTexture(sr::Texture("awesome_face.png", "Texture2"));

	glm::vec3 p1(-0.5f, -0.5f,  0.5f);
	glm::vec3 p2( 0.5f, -0.5f,  0.5f);
	glm::vec3 p3( 0.5f,  0.5f,  0.5f);
	glm::vec3 p4(-0.5f,  0.5f,  0.5f);
	glm::vec3 p5( 0.5f, -0.5f, -0.5f);
	glm::vec3 p6(-0.5f, -0.5f, -0.5f);
	glm::vec3 p7(-0.5f,  0.5f, -0.5f);
	glm::vec3 p8( 0.5f,  0.5f, -0.5f);

	glm::vec2 tc00(0.0f, 0.0f);
	glm::vec2 tc01(0.0f, 1.0f);
	glm::vec2 tc10(1.0f, 0.0f);
	glm::vec2 tc11(1.0f, 1.0f);

	glm::vec3 norm;

	GLuint v1, v2, v3, v4, v5, v6, v7, v8;

	// Front
	norm = glm::vec3(0.0f, 0.0f, 1.0f);
	v1 = mesh.AddVertex(p1, norm, tc00);
	v2 = mesh.AddVertex(p2, norm, tc10);
	v3 = mesh.AddVertex(p3, norm, tc11);
	v4 = mesh.AddVertex(p4, norm, tc01);
	mesh.AddTriangle(v1, v2, v3);
	mesh.AddTriangle(v1, v3, v4);

	// Back
	norm = glm::vec3(0.0f, 0.0f, -1.0f);
	v5 = mesh.AddVertex(p5, norm, tc00);
	v6 = mesh.AddVertex(p6, norm, tc10);
	v7 = mesh.AddVertex(p7, norm, tc11);
	v8 = mesh.AddVertex(p8, norm, tc01);
	mesh.AddTriangle(v5, v6, v7);
	mesh.AddTriangle(v5, v7, v8);

	// Right
	norm = glm::vec3(1.0f, 0.0f, 0.0f);
	v2 = mesh.AddVertex(p2, norm, tc00);
	v5 = mesh.AddVertex(p5, norm, tc10);
	v8 = mesh.AddVertex(p8, norm, tc11);
	v3 = mesh.AddVertex(p3, norm, tc01);
	mesh.AddTriangle(v2, v5, v8);
	mesh.AddTriangle(v2, v8, v3);

	// Left
	norm = glm::vec3(-1.0f, 0.0f, 0.0f);
	v6 = mesh.AddVertex(p6, norm, tc00);
	v1 = mesh.AddVertex(p1, norm, tc10);
	v4 = mesh.AddVertex(p4, norm, tc11);
	v7 = mesh.AddVertex(p7, norm, tc01);
	mesh.AddTriangle(v6, v1, v4);
	mesh.AddTriangle(v6, v4, v7);

	// Top
	norm = glm::vec3(0.0f, 1.0f, 0.0f);
	v4 = mesh.AddVertex(p4, norm, tc00);
	v3 = mesh.AddVertex(p3, norm, tc10);
	v8 = mesh.AddVertex(p8, norm, tc11);
	v7 = mesh.AddVertex(p7, norm, tc01);
	mesh.AddTriangle(v4, v3, v8);
	mesh.AddTriangle(v4, v8, v7);

	// Bottom
	norm = glm::vec3(0.0f, -1.0f, 0.0f);
	v6 = mesh.AddVertex(p6, norm, tc00);
	v5 = mesh.AddVertex(p5, norm, tc10);
	v2 = mesh.AddVertex(p2, norm, tc11);
	v1 = mesh.AddVertex(p1, norm, tc01);
	mesh.AddTriangle(v6, v5, v2);
	mesh.AddTriangle(v6, v2, v1);

	mesh.Build();

	return mesh;
}