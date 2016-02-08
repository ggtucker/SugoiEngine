#include <SugoiRender/Window.h>
#include <SugoiRender/Camera.h>
#include <SugoiRender/Mesh.h>
#include <SugoiRender/Renderer.h>
#include <SugoiRender\Texture.h>
#include "Chunk.h"

//glm::vec3 cubepositions[] = {
//	glm::vec3(0.0f,  0.0f,  0.0f),
//	glm::vec3(2.0f,  5.0f, -15.0f),
//	glm::vec3(-1.5f, -2.2f, -2.5f),
//	glm::vec3(-3.8f, -2.0f, -12.3f),
//	glm::vec3(2.4f, -0.4f, -3.5f),
//	glm::vec3(-1.7f,  3.0f, -7.5f),
//	glm::vec3(1.3f, -2.0f, -2.5f),
//	glm::vec3(1.5f,  2.0f, -2.5f),
//	glm::vec3(1.5f,  0.2f, -1.5f),
//	glm::vec3(-1.3f,  1.0f, -1.5f)
//};

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
	sr::Window window(800, 600, "SugoiEngine", true);
	window.SetMouseCursorVisible(false);

	lastFrame = glfwGetTime();

	sr::Shader shader("shader.vert", "shader.frag");
	sr::Renderer renderer(shader);
	sr::Camera& camera = renderer.GetCamera();

	Chunk chunk;
	chunk.CreateMesh(renderer);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (window.IsOpen()) {

		sr::Event event;
		while (window.PollEvent(event)) {
			switch (event.type) {
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
				if (event.key.keyCode == GLFW_KEY_ESCAPE) {
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

		sr::Texture tex("wood_container.jpg", "Texture");
		//sr::Texture tex("awesome_face.png", "Texture");

		tex.Bind();
		glUniform1i(glGetUniformLocation(shader.GetProgram(), tex.GetName().c_str()), 0);

		renderer.Clear(0.2f, 0.3f, 0.3f, 1.0f);
		chunk.Render(renderer);
		window.SwapBuffers();
	}

	return 0;
}

void doMovement(sr::Camera& camera) {
	GLfloat cameraSpeed = camera.GetMovementSpeed() * deltaTime;
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_W)) {
		camera += cameraSpeed * camera.GetFront();
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_S)) {
		camera -= cameraSpeed * camera.GetFront();
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_A)) {
		camera -= cameraSpeed * camera.GetRight();
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_D)) {
		camera += cameraSpeed * camera.GetRight();
	}
}

void mouseMoved(GLfloat xpos, GLfloat ypos, sr::Camera& camera) {
	if (firstMouse) {
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