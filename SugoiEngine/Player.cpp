#include "Player.h"

const glm::vec3 Player::PLAYER_CENTER_OFFSET = glm::vec3(0.0f, 0.3f, 0.0f);

Player::Player(sr::Renderer* renderer, ChunkManager* chunkManager) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	m_meshId{ -1 },
	m_cachedChunk{ nullptr },
	m_position{ 0.0f, 17.0f, 0.0f },
	m_gravityDirection{ 0.0f, -1.0f, 0.0f },
	m_targetForward{ 0.0f, 0.0f, 1.0f },
	m_forward{ 0.0f, 0.0f, 1.0f },
	m_right{ 1.0f, 0.0f, 0.0f },
	m_up{ 0.0f, 1.0f, 0.0f } {

	createDebugMesh();
}

Player::~Player() {
	m_renderer->DeleteMesh(m_meshId);
}

void Player::Update(float deltaTime) {
	
}

void Player::Render() {
	if (m_meshId != -1) {
		m_renderer->PushMatrix();
			float angle = -glm::atan(m_forward.z, m_forward.x);
			m_renderer->RotateY(angle);
			m_renderer->RenderMesh(m_meshId);
		m_renderer->PopMatrix();
	}
}

void Player::Move(float speed) {
	m_position += m_targetForward * speed;
}

void Player::MoveAbsolute(glm::vec3 direction, float speed) {
	m_position += (direction * speed);
}

void Player::SetForward(glm::vec3 forward) {
	m_forward = glm::normalize(forward);
	m_right = glm::normalize(glm::cross(m_forward, m_up));
	m_up = glm::normalize(glm::cross(m_right, m_forward));

	m_targetForward = m_forward;
	m_targetForward.y = 0.0f;
	m_targetForward = glm::normalize(m_targetForward);
}

void Player::createDebugMesh() {
	m_renderer->CreateMesh(&m_meshId);
	m_renderer->AddCubeToMesh(
		m_meshId, glm::vec3(), glm::vec3(0.5f, 1.0f, 0.5f),
		glm::vec2(0.0f, 0.0f), glm::vec2(1.1f, 1.1f),
		false, false, false, false, false, false);
	m_renderer->FinishMesh(m_meshId);
}