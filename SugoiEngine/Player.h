#pragma once

#include <SugoiRender/Renderer.h>
#include <SugoiMath/Transform.h>
#include <SugoiMath/Plane3D.h>
#include "ChunkManager.h"

class Player {
public:
	Player(sr::Renderer* renderer, ChunkManager* chunkManager, float walkSpeed, float runSpeed);
	~Player();

	void Update(float deltaTime);
	void DestroyBlock();
	//void Render();

	void DebugPrintBlockPosition();
	void UpdateGridPosition();
	void UpdateMovement(float deltaTime);
	void UpdatePhysics(float deltaTime);
	bool CheckCollision(glm::vec3 position, glm::vec3 previousPosition, glm::vec3* normal, glm::vec3* deltaPos);

	// Player accessors and mutators
	Transform GetTransform() { return m_transform; }
	glm::vec3 GetBottomPosition() { return m_transform.position - Transform::WORLD_UP * m_halfHeight; }
	glm::vec3 GetPosition() { return m_transform.position; }
	glm::vec3 GetForward() { return m_transform.forward; }
	glm::vec3 GetRight() { return m_transform.right; }
	glm::vec3 GetUp() { return m_transform.up; }
	void SetForward(glm::vec3 forward);
	void UpdateMoveForward();

	// Movement
	void Jump(float jumpVelocity);
	void Move(float speed);
	void MoveAbsolute(glm::vec3 direction, float speed);
	void Rotate(GLfloat pitchDelta, GLfloat yawDelta);

	float GetHeight() { return m_halfHeight * 2; }

	/* PUBLIC STATIC MEMBERS*/
	static const glm::vec3 PLAYER_CENTER_OFFSET;

private:
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;
	//int m_textureId;

	// Transform
	Transform m_transform;
	Transform m_moveTransform;
	bool m_canJump;

	// Mesh
	//int m_meshId;
	float m_radius;
	float m_halfHeight;

	// Chunk location
	glm::vec3 m_grid;
	Chunk* m_cachedChunk;

	// Player physics
	glm::vec3 m_velocity;
	glm::vec3 m_gravityDirection;
	float m_walkSpeed;
	float m_runSpeed;

	//void createDebugMesh();
};