#pragma once

#include <SugoiRender/Renderer.h>
#include <SugoiMath/Transform.h>
#include <SugoiMath/Plane3D.h>
#include "ChunkManager.h"

class Player {
public:
	Player(sr::Renderer* renderer, ChunkManager* chunkManager, int textureId);
	~Player();

	void Update(float deltaTime);
	void Render();

	void UpdateGridPosition();
	void UpdatePhysics(float deltaTime);
	bool CheckCollision(glm::vec3 position, glm::vec3 previousPosition, glm::vec3* normal, glm::vec3* deltaPos);

	// Player accessors and mutators
	glm::vec3 GetPosition() { return m_transform.position; }
	glm::vec3 GetForward() { return m_transform.forward; }
	glm::vec3 GetRight() { return m_transform.right; }
	glm::vec3 GetUp() { return m_transform.up; }
	void SetForward(glm::vec3 forward);

	// Movement
	void Jump(float jumpVelocity);
	void Move(float speed);
	void MoveAbsolute(glm::vec3 direction, float speed);

	/* PUBLIC STATIC MEMBERS*/
	static const glm::vec3 PLAYER_CENTER_OFFSET;

private:
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;
	int m_textureId;

	// Transform
	Transform m_transform;
	bool m_canJump;

	// Mesh
	int m_meshId;

	// Chunk location
	glm::vec3 m_grid;
	Chunk* m_cachedChunk;

	// Player physics
	glm::vec3 m_velocity;
	glm::vec3 m_gravityDirection;

	// Previous frame's position
	glm::vec3 m_previousPosition;
	glm::vec3 m_positionDelta;

	void createDebugMesh();
};