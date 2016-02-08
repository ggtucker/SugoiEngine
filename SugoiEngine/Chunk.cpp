#include "Chunk.h"

Chunk::Chunk() : m_position{0.0f, 0.0f, 0.0f} {
	m_blocks = new Block**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		m_blocks[i] = new Block*[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			m_blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_blocks[i][j];
		}
		delete[] m_blocks[i];
	}
	delete[] m_blocks;
}

void Chunk::Render(sr::Renderer& renderer) {
	renderer.PushMatrix();
		//renderer.EnableImmediateMode(GL_QUADS);
		//renderer.ImmediateColorAlpha(1.0f, 1.0f, 1.0f, 1.0f);
		//renderer.SetRenderMode(GL_RENDER);
		renderer.Translate(m_position.x, m_position.y, m_position.z);
		if (m_meshId != -1) {
			renderer.RenderMesh(m_meshId);
		}
	renderer.PopMatrix();
}

void Chunk::CreateMesh(sr::Renderer& renderer) {
	renderer.CreateMesh(&m_meshId);
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			for (int z = 0; z < CHUNK_SIZE; ++z) {
				if (m_blocks[x][y][z].IsActive()) {
					addCubeToMesh(renderer, x, y, z);
				}
			}
		}
	}
	renderer.FinishMesh(m_meshId);
}

void Chunk::addCubeToMesh(sr::Renderer& renderer, int x, int y, int z) {
	glm::vec3 p1(x - Block::BLOCK_RENDER_SIZE, y - Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
	glm::vec3 p2(x + Block::BLOCK_RENDER_SIZE, y - Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
	glm::vec3 p3(x + Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
	glm::vec3 p4(x - Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z + Block::BLOCK_RENDER_SIZE);
	glm::vec3 p5(x + Block::BLOCK_RENDER_SIZE, y - Block::BLOCK_RENDER_SIZE, z - Block::BLOCK_RENDER_SIZE);
	glm::vec3 p6(x - Block::BLOCK_RENDER_SIZE, y - Block::BLOCK_RENDER_SIZE, z - Block::BLOCK_RENDER_SIZE);
	glm::vec3 p7(x - Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z - Block::BLOCK_RENDER_SIZE);
	glm::vec3 p8(x + Block::BLOCK_RENDER_SIZE, y + Block::BLOCK_RENDER_SIZE, z - Block::BLOCK_RENDER_SIZE);

	glm::vec2 tc00(0.0f, 0.0f);
	glm::vec2 tc01(0.0f, 1.0f);
	glm::vec2 tc10(1.0f, 0.0f);
	glm::vec2 tc11(1.0f, 1.0f);

	glm::vec3 norm;

	GLuint v1, v2, v3, v4, v5, v6, v7, v8;

	// Front
	norm = glm::vec3(0.0f, 0.0f, 1.0f);
	v1 = renderer.AddVertexToMesh(m_meshId, p1, norm, tc00);
	v2 = renderer.AddVertexToMesh(m_meshId, p2, norm, tc10);
	v3 = renderer.AddVertexToMesh(m_meshId, p3, norm, tc11);
	v4 = renderer.AddVertexToMesh(m_meshId, p4, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v1, v2, v3);
	renderer.AddTriangleToMesh(m_meshId, v1, v3, v4);

	// Back
	norm = glm::vec3(0.0f, 0.0f, -1.0f);
	v5 = renderer.AddVertexToMesh(m_meshId, p5, norm, tc00);
	v6 = renderer.AddVertexToMesh(m_meshId, p6, norm, tc10);
	v7 = renderer.AddVertexToMesh(m_meshId, p7, norm, tc11);
	v8 = renderer.AddVertexToMesh(m_meshId, p8, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v5, v6, v7);
	renderer.AddTriangleToMesh(m_meshId, v5, v7, v8);

	// Right
	norm = glm::vec3(1.0f, 0.0f, 0.0f);
	v2 = renderer.AddVertexToMesh(m_meshId, p2, norm, tc00);
	v5 = renderer.AddVertexToMesh(m_meshId, p5, norm, tc10);
	v8 = renderer.AddVertexToMesh(m_meshId, p8, norm, tc11);
	v3 = renderer.AddVertexToMesh(m_meshId, p3, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v2, v5, v8);
	renderer.AddTriangleToMesh(m_meshId, v2, v8, v3);

	// Left
	norm = glm::vec3(-1.0f, 0.0f, 0.0f);
	v6 = renderer.AddVertexToMesh(m_meshId, p6, norm, tc00);
	v1 = renderer.AddVertexToMesh(m_meshId, p1, norm, tc10);
	v4 = renderer.AddVertexToMesh(m_meshId, p4, norm, tc11);
	v7 = renderer.AddVertexToMesh(m_meshId, p7, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v6, v1, v4);
	renderer.AddTriangleToMesh(m_meshId, v6, v4, v7);

	// Top
	norm = glm::vec3(0.0f, 1.0f, 0.0f);
	v4 = renderer.AddVertexToMesh(m_meshId, p4, norm, tc00);
	v3 = renderer.AddVertexToMesh(m_meshId, p3, norm, tc10);
	v8 = renderer.AddVertexToMesh(m_meshId, p8, norm, tc11);
	v7 = renderer.AddVertexToMesh(m_meshId, p7, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v4, v3, v8);
	renderer.AddTriangleToMesh(m_meshId, v4, v8, v7);

	// Bottom
	norm = glm::vec3(0.0f, -1.0f, 0.0f);
	v6 = renderer.AddVertexToMesh(m_meshId, p6, norm, tc00);
	v5 = renderer.AddVertexToMesh(m_meshId, p5, norm, tc10);
	v2 = renderer.AddVertexToMesh(m_meshId, p2, norm, tc11);
	v1 = renderer.AddVertexToMesh(m_meshId, p1, norm, tc01);
	renderer.AddTriangleToMesh(m_meshId, v6, v5, v2);
	renderer.AddTriangleToMesh(m_meshId, v6, v2, v1);
}