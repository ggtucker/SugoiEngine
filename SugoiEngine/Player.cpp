#include "Player.h"
#include <SugoiRender/Keyboard.h>

const glm::vec3 Player::PLAYER_CENTER_OFFSET = glm::vec3(0.0f, 0.3f, 0.0f);

Player::Player(sr::Renderer* renderer, ChunkManager* chunkManager, float walkSpeed, float runSpeed) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	//m_textureId{ textureId },
	m_canJump{ false },
	//m_meshId{ -1 },
	m_radius{ 0.2f },
	m_halfHeight{ 0.4f },
	m_cachedChunk{ nullptr },
	m_gravityDirection{ 0.0f, -1.0f, 0.0f },
	m_walkSpeed{ walkSpeed },
	m_runSpeed{ runSpeed } {

	m_transform.position = glm::vec3(0.0f, 50.0f, 0.0f);
	UpdateGridPosition();

	//createDebugMesh();
}

Player::~Player() {
	//m_renderer->DeleteMesh(m_meshId);
}

void Player::Update(float deltaTime) {

	UpdateGridPosition();

	UpdateMovement(deltaTime);

	//DebugPrintBlockPosition();

	UpdatePhysics(deltaTime);

}

void Player::DestroyBlock() {
	for (int i = 0; i <= 6; ++i) {

		float dist = (float)i * 0.5f;

		glm::vec3 worldPos = m_transform.position + m_transform.forward * dist;

		Chunk* blockChunk = m_chunkManager->GetChunkFromWorldPosition(worldPos.x, worldPos.y, worldPos.z);
		if (blockChunk) {
			glm::ivec3 blockPos = Chunk::GetBlockPosition(worldPos.x, worldPos.y, worldPos.z);
			if (blockChunk->Destroy(blockPos.x, blockPos.y, blockPos.z)) {
				return;
			}
		}
	}
}

void Player::DebugPrintBlockPosition() {
	Chunk* blockChunk = m_chunkManager->GetChunkFromWorldPosition(m_transform.position.x, m_transform.position.y - Chunk::BLOCK_RENDER_SIZE, m_transform.position.z);
	if (blockChunk) {
		glm::ivec3 blockPos = Chunk::GetBlockPosition(m_transform.position.x, m_transform.position.y - Chunk::BLOCK_RENDER_SIZE, m_transform.position.z);
		glm::vec3 blockWorldPos = blockChunk->GetBlockPosition(blockPos.x, blockPos.y, blockPos.z);

		std::cout << "Block[" << blockPos.x << " " << blockPos.y << " " << blockPos.z << "] ";
		std::cout << "BlockWorld[" << blockWorldPos.x << " " << blockWorldPos.y << " " << blockWorldPos.z << "] ";
		std::cout << "Player[" << m_transform.position.x << " " << m_transform.position.y << " " << m_transform.position.z << "]" << std::endl;
	}
}

void Player::UpdateGridPosition() {
	int gridX = (int)(m_transform.position.x / Chunk::CHUNK_SIZE);
	int gridY = (int)(m_transform.position.y / Chunk::CHUNK_SIZE);
	int gridZ = (int)(m_transform.position.z / Chunk::CHUNK_SIZE);

	if (gridX != m_grid.x || gridY != m_grid.y || gridZ != m_grid.z || m_cachedChunk == nullptr) {
		m_grid = glm::vec3(gridX, gridY, gridZ);
		m_cachedChunk = m_chunkManager->GetChunkFromWorldPosition(
			m_transform.position.x,
			m_transform.position.y,
			m_transform.position.z);
	}
}

void Player::UpdateMovement(float deltaTime) {

	float speed = m_walkSpeed;

	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		speed = m_runSpeed;
	}

	speed *= deltaTime;

	glm::vec3 direction;

	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_W)) {
		direction += m_moveTransform.forward;
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_S)) {
		direction -= m_moveTransform.forward;
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_A)) {
		direction -= m_moveTransform.right;
	}
	if (sr::Keyboard::IsKeyPressed(GLFW_KEY_D)) {
		direction += m_moveTransform.right;
	}

	if (glm::length(direction) > 0.0f) {
		MoveAbsolute(glm::normalize(direction), speed);
	}
}

void Player::UpdatePhysics(float deltaTime) {

	glm::vec3 acceleration = (m_gravityDirection * 9.81f);
	m_velocity += acceleration * deltaTime;

	// Ensure we don't go through any blocks by dividing position delta
	// until it is less than the block size.
	glm::vec3 movement = m_velocity;
	glm::vec3 posDelta = m_velocity * deltaTime;
	int numSteps = 1;
	while (glm::length(posDelta) >= Chunk::BLOCK_RENDER_SIZE) {
		++numSteps;
		posDelta = m_velocity * (deltaTime / numSteps);
	}

	glm::vec3 normal;
	for (int step = 1; step <= numSteps; ++step) {

		float deltaTimeToUse = (deltaTime / numSteps) * step;
		glm::vec3 posToCheck = GetBottomPosition() + movement * deltaTimeToUse;

		if (CheckCollision(posToCheck, GetBottomPosition(), &normal, &movement)) {
			m_velocity = glm::vec3();
			m_canJump = true;
			break;
		}

		m_transform.position += movement * (deltaTime / numSteps);
	}

}

bool Player::CheckCollision(glm::vec3 position, glm::vec3 previousPosition, glm::vec3* normal, glm::vec3* deltaPos) {

	sm::Region3D blockCollideRegion = Block::GetCollideRegion();

	int numChecks = 1 + (int)(m_radius / (Chunk::BLOCK_RENDER_SIZE));

	for (int x = -numChecks; x <= numChecks; x++) {
		for (int y = -numChecks; y <= numChecks; y++) {
			for (int z = -numChecks; z <= numChecks; z++) {

				glm::vec3 blockApproxWorldPos = position + glm::vec3(
					x * Chunk::BLOCK_RENDER_SIZE,
					y * Chunk::BLOCK_RENDER_SIZE,
					z * Chunk::BLOCK_RENDER_SIZE);

				Chunk* blockChunk = m_chunkManager->GetChunkFromWorldPosition(blockApproxWorldPos.x, blockApproxWorldPos.y, blockApproxWorldPos.z);
				if (!blockChunk || !blockChunk->IsLoaded()) {
					continue;
				}

				glm::ivec3 blockPos = Chunk::GetBlockPosition(blockApproxWorldPos.x, blockApproxWorldPos.y, blockApproxWorldPos.z);

				bool active = blockChunk->GetActive(blockPos.x, blockPos.y, blockPos.z);
				if (active) {
					glm::vec3 blockWorldPos = blockChunk->GetBlockPosition(blockPos.x, blockPos.y, blockPos.z);

					glm::vec3 previousDiff = blockWorldPos - previousPosition;
					glm::vec3 diff = blockWorldPos - position;

					sm::Collision previousCollision = blockCollideRegion.SphereInRegion(blockWorldPos - previousPosition, m_radius);
					sm::Collision currentCollision = blockCollideRegion.SphereInRegion(blockWorldPos - position, m_radius);

					if (previousCollision.result == sm::CollisionResult::Outside
						&& currentCollision.result == sm::CollisionResult::Inside) {

						for (int i = 0; i < previousCollision.insidePlane.size(); ++i) {

							if (previousCollision.insidePlane[i] == false
								&& currentCollision.insidePlane[i] == true) {

								*normal += blockCollideRegion.GetPlane(i).m_normal;
							}
						}

						if (glm::length(*normal) <= 1.0f) {
							if (glm::length(*normal) > 0.0f) {

								*normal = glm::normalize(*normal);

								float dotProduct = glm::dot(*normal, *deltaPos);
								*normal *= dotProduct;

								*deltaPos -= *normal;
							}
						}

						return true;
					}
				}
			}
		}
	}

	return false;
}

//void Player::Render() {
//	if (m_textureId != -1) {
//		m_renderer->BindTextureToShader(m_textureId, 0);
//	}
//	if (m_meshId != -1) {
//		m_renderer->PushMatrix();
//			float angle = -glm::atan(m_transform.forward.z, m_transform.forward.x);
//			m_renderer->Translate(m_transform.position.x, m_transform.position.y, m_transform.position.z);
//			m_renderer->RotateY(angle);
//			m_renderer->RenderMesh(m_meshId);
//		m_renderer->PopMatrix();
//	}
//}

void Player::Jump(float jumpVelocity) {
	if (!m_canJump) return;

	m_canJump = false;
	m_velocity += Transform::WORLD_UP * jumpVelocity;
}

void Player::Move(float speed) {
	MoveAbsolute(m_moveTransform.forward, speed);
}

void Player::MoveAbsolute(glm::vec3 direction, float speed) {

	// Ensure we don't go through any blocks by dividing position delta
	// until it is less than the block size.
	glm::vec3 movement = direction;
	glm::vec3 posDelta = direction * speed;
	int numSteps = 1;
	while (glm::length(posDelta) >= Chunk::BLOCK_RENDER_SIZE) {
		++numSteps;
		posDelta = direction * (speed / numSteps);
	}

	glm::vec3 normal;
	for (int step = 1; step <= numSteps; ++step) {

		float speedToUse = (speed / numSteps) * step;
		glm::vec3 posToCheck = GetPosition() + movement * speedToUse;

		if (CheckCollision(posToCheck, GetPosition(), &normal, &movement)) {
		}

		m_transform.position += movement * (speed / numSteps);
	}

}

void Player::Rotate(GLfloat pitchDelta, GLfloat yawDelta) {
	m_transform.Rotate(m_transform.right, pitchDelta);
	m_transform.Rotate(m_transform.up, yawDelta);
	UpdateMoveForward();
}

void Player::SetForward(glm::vec3 forward) {
	m_transform.SetForward(forward);
	UpdateMoveForward();
}

void Player::UpdateMoveForward() {
	glm::vec3 forward = m_transform.forward;
	forward.y = 0.0f;
	m_moveTransform.SetForward(forward);
}

//void Player::createDebugMesh() {
//	m_renderer->CreateMesh(&m_meshId);
//	m_renderer->AddCubeToMesh(
//		m_meshId, glm::vec3(), glm::vec3(m_radius, m_halfHeight, m_radius),
//		glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f),
//		false, false, false, false, false, false);
//	m_renderer->FinishMesh(m_meshId);
//}