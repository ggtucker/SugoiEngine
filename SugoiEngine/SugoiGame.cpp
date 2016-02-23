#include "SugoiGame.h"

SugoiGame* SugoiGame::c_instance = nullptr;

SugoiGame* SugoiGame::GetInstance() {
	if (!c_instance)
		c_instance = new SugoiGame;

	return c_instance;
}

void SugoiGame::Create(GameSettings settings) {
	m_settings = settings;
	m_window = new sr::Window(m_settings.windowWidth, m_settings.windowHeight, "SugoiEngine", false);
	m_window->SetMouseCursorVisible(false);
	m_window->SetPosition(700, 200);

	sr::Shader shader("../Resources/Shaders/Cube/shader.vert", "../Resources/Shaders/Cube/shader.frag");
	m_renderer = new sr::Renderer(shader);

	int textureId;
	m_renderer->LoadTexture(&textureId, "textures.png", "Texture");

	m_chunkManager = new ChunkManager(m_renderer, textureId);

	//m_renderer->LoadTexture(&textureId, "Player.png", "Texture");
	m_player = new Player(m_renderer, m_chunkManager, m_settings.walkSpeed, m_settings.runSpeed);
	//m_cameraDistance = 7.0f;
	//m_renderer->GetCamera().SetDistanceFromPoint(m_player->GetPosition(), m_cameraDistance);

	m_chunkManager->SetPlayer(m_player);

	m_chunkManager->CreateNewChunk(0, 0, 0);

	m_leftMousePressed = false;
	m_rightMousePressed = false;

	m_lastTime = glfwGetTime();
}

void SugoiGame::Destroy() {
	if (c_instance) {
		delete m_chunkManager;
		delete m_player;

		delete m_renderer;
		delete m_window;

		delete c_instance;
	}
}

void SugoiGame::PollEvents() {
	sr::Event event;
	while (m_window->PollEvent(event)) {
		switch (event.type) {

		case sr::Event::WINDOW_CLOSED:
			m_window->Close();
			break;

		case sr::Event::KEY_PRESSED:
			KeyPressed(event.key.keyCode, event.key.alt, event.key.control, event.key.shift, event.key.system);
			break;

		case sr::Event::KEY_RELEASED:
			KeyReleased(event.key.keyCode, event.key.alt, event.key.control, event.key.shift, event.key.system);
			break;

		case sr::Event::MOUSE_PRESSED:
			if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_1) {
				MouseLeftPressed();
			}
			else if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_2) {
				MouseRightPressed();
			}
			else if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_3) {
				MouseMiddlePressed();
			}
			break;

		case sr::Event::MOUSE_RELEASED:
			if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_1) {
				MouseLeftReleased(event.mouseClicked.x, event.mouseClicked.y);
			}
			else if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_2) {
				MouseRightReleased();
			}
			else if (event.mouseClicked.mouseCode == GLFW_MOUSE_BUTTON_3) {
				MouseMiddleReleased();
			}
			break;

		case sr::Event::MOUSE_MOVED:
			MouseMoved(event.mouseMoved.x, event.mouseMoved.y);
			break;

		case sr::Event::MOUSE_SCROLLED:
			MouseScroll(event.mouseScrolled.xoffset, event.mouseScrolled.yoffset);
			break;
		}
	}
}

void SugoiGame::Update() {
	GLfloat currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;

	m_chunkManager->Update();
	m_player->Update(m_deltaTime);
	UpdateCamera();
}

void SugoiGame::Render() {

	m_renderer->Clear(0.2f, 0.3f, 0.3f, 1.0f);

	m_chunkManager->Render();

	//m_player->Render();

	m_window->SwapBuffers();
}

void SugoiGame::UpdateCamera() {

	sr::Camera& camera = m_renderer->GetCamera();
	Transform transform = m_player->GetTransform();

	

	transform.position.y += m_player->GetHeight();
	camera.SetTransform(transform);
	//camera.SetDistanceFromPoint(target, m_cameraDistance);
}

void SugoiGame::KeyPressed(int keyCode, bool alt, bool control, bool shift, bool system) {
	
}

void SugoiGame::KeyReleased(int keyCode, bool alt, bool control, bool shift, bool system) {
	if (keyCode == GLFW_KEY_ESCAPE) {
		m_window->Close();
	}
	else if (keyCode == GLFW_KEY_SPACE) {
		m_player->Jump(5.0f);
	}
}

void SugoiGame::MouseLeftPressed() {
	m_leftMousePressed = true;
}

void SugoiGame::MouseLeftReleased(double x, double y) {
	m_leftMousePressed = false;
	m_player->DestroyBlock();
	m_renderer->PushMatrix();
		glm::vec3 mouseWorldPos = m_renderer->UnProject(glm::vec3(x, y, 0.0f), glm::vec4(0.0f, 0.0f, m_window->GetWidth(), m_window->GetHeight()));
	m_renderer->PopMatrix();
	std::cout << mouseWorldPos.x << " " << mouseWorldPos.y << " " << mouseWorldPos.z << std::endl;
}

void SugoiGame::MouseRightPressed() {
	m_rightMousePressed = true;
}

void SugoiGame::MouseRightReleased() {
	m_rightMousePressed = false;
}

void SugoiGame::MouseMiddlePressed() {
}

void SugoiGame::MouseMiddleReleased() {
}

void SugoiGame::MouseScroll(float x, float y) {
	/*m_cameraDistance -= y;
	if (m_cameraDistance <= m_settings.minCameraDistance) {
		m_cameraDistance = m_settings.minCameraDistance;
	}
	if (m_cameraDistance >= m_settings.maxCameraDistance) {
		m_cameraDistance = m_settings.maxCameraDistance;
	}*/
}

void SugoiGame::MouseMoved(float x, float y) {
	GLfloat yawDelta = x - m_lastMouseX;
	GLfloat pitchDelta = y - m_lastMouseY;
	m_lastMouseX = x;
	m_lastMouseY = y;
	
	yawDelta *= m_settings.mouseSensitivity;
	pitchDelta *= m_settings.mouseSensitivity;
	
	m_player->Rotate(-pitchDelta, -yawDelta);
}