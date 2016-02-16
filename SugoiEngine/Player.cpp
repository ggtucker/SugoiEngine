#include "Player.h"

const glm::vec3 Player::PLAYER_CENTER_OFFSET = glm::vec3(0.0f, 0.3f, 0.0f);

Player::Player(sr::Renderer* renderer, ChunkManager* chunkManager, int textureId) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	m_textureId{ textureId },
	m_canJump{ false },
	m_meshId{ -1 },
	m_cachedChunk{ nullptr },
	m_gravityDirection{ 0.0f, -1.0f, 0.0f } {

	m_transform.position = glm::vec3(0.0f, 50.0f, 0.0f);

	createDebugMesh();
}

Player::~Player() {
	m_renderer->DeleteMesh(m_meshId);
}

void Player::Update(float deltaTime) {

	UpdateGridPosition();

	UpdatePhysics(deltaTime);

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

void Player::UpdatePhysics(float deltaTime) {

	m_positionDelta = glm::vec3();

	glm::vec3 acceleration = (m_gravityDirection * 9.81f);
	m_velocity += acceleration * deltaTime;

	// Ensure we don't go through any blocks by dividing position delta
	// until it is less than the block size.
	glm::vec3 posDelta = m_velocity * deltaTime;
	int numSteps = 1;
	while (glm::length(posDelta) >= Chunk::BLOCK_RENDER_SIZE) {
		++numSteps;
		posDelta = m_velocity * (deltaTime / numSteps);
	}

	glm::vec3 normal;
	for (int step = 1; step <= numSteps; ++step) {

		float deltaTimeToUse = (deltaTime / numSteps) * step;

		glm::vec3 posToCheck = m_transform.position + m_velocity * deltaTimeToUse;
		if (CheckCollision(posToCheck, m_previousPosition, &normal, &posDelta)) {
			m_velocity = glm::vec3();
			m_canJump = true;
			break;
		}

		// No collision
		m_positionDelta += posDelta;
		m_transform.position += posDelta;
	}

	m_previousPosition = m_transform.position;
}

bool Player::CheckCollision(glm::vec3 position, glm::vec3 previousPosition, glm::vec3* normal, glm::vec3* deltaPos) {

	sm::Region3D region = Block::GetCollideRegion();

	bool collision = false;

	float playerRadius = 0.2f;

	int numChecks = 1 + (int)(playerRadius / (Chunk::BLOCK_RENDER_SIZE));

	for (int x = -numChecks; x <= numChecks; x++) {
		for (int y = -numChecks; y <= numChecks; y++) {
			for (int z = -numChecks; z <= numChecks; z++) {

				glm::vec3 blockApproxWorldPos = position + glm::vec3(
					x * Chunk::BLOCK_RENDER_SIZE,
					y * Chunk::BLOCK_RENDER_SIZE,
					z * Chunk::BLOCK_RENDER_SIZE);

				Chunk* blockChunk = m_chunkManager->GetChunkFromWorldPosition(blockApproxWorldPos.x, blockApproxWorldPos.y, blockApproxWorldPos.z);
				glm::ivec3 blockPos = Chunk::GetBlockPosition(blockApproxWorldPos.x, blockApproxWorldPos.y, blockApproxWorldPos.z);

				if (blockChunk && x == 0 && y == -1 && z == 0) {
					std::cout << "[" << blockPos.x << " " << blockPos.y << " " << blockPos.z << "] ";
					std::cout << blockChunk->GetGridX() << " " << blockChunk->GetGridY() << " " << blockChunk->GetGridZ() << std::endl;
				}

				bool active = !blockChunk ? false : blockChunk->GetActive(blockPos.x, blockPos.y, blockPos.z);
				if (!active) {
					if (!blockChunk || !blockChunk->IsLoaded()) {
						//*deltaPos = glm::vec3();
						//return true;
					}
				}
				else {
					glm::vec3 blockWorldPos = blockChunk->GetBlockPosition(blockPos.x, blockPos.y, blockPos.z);

					int numPlanesInside = 0;

					for (int i = 0; i < region.NumPlanes(); ++i) {
						// Determine whether previous position was inside region
						float lastDistance = region.GetPlane(i).GetDistance(blockWorldPos - previousPosition);
						bool lastPositionInside = (lastDistance >= 0.0f);

						// Determine whether new position is inside region
						float distance = region.GetPlane(i).GetDistance(blockWorldPos - position);
						bool positionInside = (distance >= 0.0f);

						if (positionInside) {
							++numPlanesInside;
							if (!lastPositionInside) {
								*normal += region.GetPlane(i).m_normal;
							}
						}
					}

					if (numPlanesInside == 6) {
						if (glm::length(*normal) <= 1.0f)
						{
							if (glm::length(*normal) > 0.0f)
							{
								*normal = glm::normalize(*normal);
							}

							float dotResult = glm::dot(*normal, *deltaPos);
							*normal *= dotResult;

							*deltaPos -= *normal;

							collision = true;
						}
					}
				}
			}
		}
	}

	return collision;
}

void Player::Render() {
	if (m_textureId != -1) {
		m_renderer->BindTextureToShader(m_textureId, 0);
	}
	if (m_meshId != -1) {
		m_renderer->PushMatrix();
			float angle = -glm::atan(m_transform.forward.z, m_transform.forward.x);
			m_renderer->Translate(m_transform.position.x, m_transform.position.y, m_transform.position.z);
			m_renderer->RotateY(angle);
			m_renderer->RenderMesh(m_meshId);
		m_renderer->PopMatrix();
	}
}

void Player::Jump(float jumpVelocity) {
	if (!m_canJump) return;

	m_canJump = false;
	m_velocity += m_transform.up * jumpVelocity;
}

void Player::Move(float speed) {
	m_transform.position += m_transform.forward * speed;
}

void Player::MoveAbsolute(glm::vec3 direction, float speed) {
	m_transform.position += (direction * speed);
}

void Player::SetForward(glm::vec3 forward) {
	forward.y = 0.0f;
	m_transform.SetForward(forward);
}

void Player::createDebugMesh() {
	m_renderer->CreateMesh(&m_meshId);
	m_renderer->AddCubeToMesh(
		m_meshId, glm::vec3(), glm::vec3(0.2f, 0.4f, 0.2f),
		glm::vec2(0.0f, 0.0f), glm::vec2(1.1f, 1.1f),
		false, false, false, false, false, false);
	m_renderer->FinishMesh(m_meshId);
}