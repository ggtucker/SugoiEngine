#include "Player.h"

const glm::vec3 Player::PLAYER_CENTER_OFFSET = glm::vec3(0.0f, 0.3f, 0.0f);

Player::Player(sr::Renderer* renderer, ChunkManager* chunkManager, int textureId) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	m_textureId{ textureId },
	m_meshId{ -1 },
	m_cachedChunk{ nullptr },
	m_gravityDirection{ 0.0f, -1.0f, 0.0f } {

	m_transform.position = glm::vec3(0.0f, 17.0f, 0.0f);

	createDebugMesh();
}

Player::~Player() {
	m_renderer->DeleteMesh(m_meshId);
}

void Player::Update(float deltaTime) {
	
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
		m_meshId, glm::vec3(), glm::vec3(0.5f, 1.0f, 0.5f),
		glm::vec2(0.0f, 0.0f), glm::vec2(1.1f, 1.1f),
		false, false, false, false, false, false);
	m_renderer->FinishMesh(m_meshId);
}