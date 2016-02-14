#pragma once

#include <SugoiRender/Renderer.h>
#include "ChunkManager.h"

class Player {
public:
	Player(sr::Renderer* renderer, ChunkManager* chunkManager);
	~Player();

	void Update(float deltaTime);
	void Render();

	// Player accessors and mutators
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetForward() { return m_forward; }
	glm::vec3 GetRight() { return m_right; }
	glm::vec3 GetUp() { return m_up; }
	void SetForward(glm::vec3 forward);

	// Movement
	void Move(float speed);
	void MoveAbsolute(glm::vec3 direction, float speed);

	/* PUBLIC STATIC MEMBERS*/
	static const glm::vec3 PLAYER_CENTER_OFFSET;

private:
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;

	// Mesh
	int m_meshId;

	// Chunk location
	glm::vec3 m_grid;
	Chunk* m_cachedChunk;

	// Player physics
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_gravityDirection;

	// Local axis
	glm::vec3 m_targetForward;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	glm::vec3 m_up;

	// Previous frame's position
	glm::vec3 m_previousPosition;
	glm::vec3 m_positionDelta;

	void createDebugMesh();
};