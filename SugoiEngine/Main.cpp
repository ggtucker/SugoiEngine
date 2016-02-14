#include <SugoiRender/Window.h>
#include <SugoiRender/Camera.h>
#include <SugoiRender/Mesh.h>
#include <SugoiRender/Renderer.h>
#include <SugoiRender/Texture.h>
#include <SugoiRender/GLError.h>
#include "ChunkManager.h"
#include "Player.h"
#include "SugoiGame.h"
#include <SugoiRender/SkyboxManager.h>
#include "SugoiRender/GLError.h"

// Function prototypes
void doMovement(sr::Camera& camera);
void mouseMoved(GLfloat xpos, GLfloat ypos, sr::Camera& camera);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastX = 400, lastY = 300;

bool firstMouse = true;

int main() {

	GameSettings settings;
	settings.windowWidth = 800;
	settings.windowHeight = 600;
	settings.movementSpeed = 3.0f;
	settings.mouseSensitivity = 0.001f;


	SugoiGame* game = SugoiGame::GetInstance();
	game->Create(settings);

	while (game->IsRunning()) {

		game->PollEvents();


		game->Update();


		game->Render();

	}

	game->Destroy();

	return 0;
}