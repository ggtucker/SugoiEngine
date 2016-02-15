#include "Player.h"

const glm::vec3 Player::PLAYER_CENTER_OFFSET = glm::vec3(0.0f, 0.3f, 0.0f);

Player::Player(sr::Renderer* renderer, ChunkManager* chunkManager, int textureId) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	m_textureId{ textureId },
	m_canJump{ false },
	m_radius{ (1.0f / 14.0f) },
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

	glm::vec3 acceleration = (m_gravityDirection * 9.81f);
	m_velocity += acceleration * deltaTime;

	// Ensure we don't go through any blocks by dividing position delta
	// until it is less than the block size.
	glm::vec3 posDelta = m_velocity * deltaTime;
	int numDivisions = 1;
	while (glm::length(posDelta) >= Chunk::BLOCK_RENDER_SIZE) {
		++numDivisions;
		posDelta = m_velocity * (deltaTime / numDivisions);
	}

	glm::vec3 normal;
	int numStepsBeforeCollision = numDivisions;
	for (int i = 0; i < numDivisions; ++i) {
		float deltaTimeToUse = (deltaTime / numDivisions) * (i+1);
		glm::vec3 position = m_transform.position + m_velocity * deltaTimeToUse;
		if (CheckCollision(position, m_previousPosition, &normal, &posDelta)) {
			m_velocity = glm::vec3();
			m_canJump = true;
		}
		else if (numStepsBeforeCollision == 0) {
			numStepsBeforeCollision = i;
		}
	}

	if (numStepsBeforeCollision > 0) {
		float deltaTimeToUse = ((deltaTime / numDivisions) * numStepsBeforeCollision);
		m_positionDelta = m_velocity * deltaTimeToUse;
		m_transform.position += m_velocity * deltaTimeToUse;

		m_previousPosition = m_transform.position;
	}

}

bool Player::CheckCollision(glm::vec3 position, glm::vec3 previousPosition, glm::vec3* normal, glm::vec3* deltaPos) {

	bool collision = false;

	int numChecks = 1 + (int)(m_radius / (Chunk::BLOCK_RENDER_SIZE));

	for (int x = -numChecks; x <= numChecks; x++) {
		for (int y = -numChecks; y <= numChecks; y++) {
			for (int z = -numChecks; z <= numChecks; z++) {

				glm::vec3 blockWorldPos = position + glm::vec3(
					x * Chunk::BLOCK_RENDER_SIZE,
					y * Chunk::BLOCK_RENDER_SIZE,
					z * Chunk::BLOCK_RENDER_SIZE);

				Chunk* blockChunk = m_chunkManager->GetChunkFromWorldPosition(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);
				glm::ivec3 blockPos = Chunk::GetBlockPosition(blockWorldPos.x, blockWorldPos.y, blockWorldPos.z);

				if (x == 0 && y == 0 && z == 0) {
					std::cout << "Block: " << blockPos.x << " " << blockPos.y << " " << blockPos.z << std::endl;
				}

				bool active = !blockChunk ? false : blockChunk->GetActive(blockPos.x, blockPos.y, blockPos.z);
				if (!active) {
					if (!blockChunk || !blockChunk->IsLoaded()) {
						//*deltaPos = glm::vec3();
						//return true;
					}
				}
				else {
					sm::Plane3D planes[6];
					planes[0] = sm::Plane3D(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f));
					planes[1] = sm::Plane3D(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f));
					planes[2] = sm::Plane3D(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, Chunk::BLOCK_RENDER_SIZE, 0.0f));
					planes[3] = sm::Plane3D(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -Chunk::BLOCK_RENDER_SIZE, 0.0f));
					planes[4] = sm::Plane3D(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, Chunk::BLOCK_RENDER_SIZE));
					planes[5] = sm::Plane3D(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -Chunk::BLOCK_RENDER_SIZE));

					bool insideCache[6];

					for (int i = 0; i < 6; ++i) {
						glm::vec3 pointToCheck = blockWorldPos - previousPosition;
						float distance = planes[i].GetDistance(pointToCheck);
						insideCache[i] = (distance >= -m_radius); // Previous position was intersecting or inside block
					}

					int inside = 0;

					for (int i = 0; i < 6; ++i) {
						glm::vec3 pointToCheck = blockWorldPos - position;
						float distance = planes[i].GetDistance(pointToCheck);
						if (distance >= -m_radius) { // New position is intersecting or inside block
							++inside;
							if (!insideCache[i]) {
								*normal += planes[i].m_normal;
							}
						}
					}

					if (inside == 6) {
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