#pragma once

#include <SugoiRender/Window.h>
#include <SugoiRender/Renderer.h>
#include <SugoiRender/Event.h>
#include "ChunkManager.h"
#include "Player.h"
#include "GameSettings.h"

class SugoiGame {
public:
	static SugoiGame* GetInstance();

	void Create(GameSettings settings);
	void Destroy();

	bool IsRunning() { return m_window && m_window->IsOpen(); }
	void PollEvents();
	void Update();
	void Render();

	// Camera
	void UpdateCamera();

	// Input functions
	void KeyPressed(int keyCode, bool alt, bool control, bool shift, bool system);
	void KeyReleased(int keyCode, bool alt, bool control, bool shift, bool system);
	void MouseLeftPressed();
	void MouseLeftReleased(double x, double y);
	void MouseRightPressed();
	void MouseRightReleased();
	void MouseMiddlePressed();
	void MouseMiddleReleased();
	void MouseScroll(float x, float y);
	void MouseMoved(float x, float y);

private:
	GameSettings m_settings;
	sr::Window* m_window;
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;
	Player* m_player;

	//float m_cameraDistance;

	float m_lastTime;
	float m_deltaTime;

	float m_lastMouseX;
	float m_lastMouseY;

	bool m_leftMousePressed;
	bool m_rightMousePressed;

	static SugoiGame* c_instance;
};