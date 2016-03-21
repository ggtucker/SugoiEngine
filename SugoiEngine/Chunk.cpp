#include "Chunk.h"
#include "ChunkManager.h"
#include "Player.h"

#include <random>

const float Chunk::BLOCK_RENDER_SIZE = 1.0f;
const float Chunk::HALF_RENDER_SIZE = BLOCK_RENDER_SIZE / 2.0f;

Chunk::Chunk(sr::Renderer* renderer, ChunkManager* chunkManager) :
	m_renderer{ renderer },
	m_chunkManager{ chunkManager },
	m_blocks{ nullptr },
	m_meshId{ -1 },
	m_cachedMeshId{ -1 },
	m_grid{ 0, 0, 0 },
	m_state{ ChunkState::Initial },
	m_rebuild{ false },
	m_rebuildNeighbors{ false },
	m_empty{ false },
	m_surrounded{ false },
	m_x_minus_full{ false },
	m_x_plus_full{ false },
	m_y_minus_full{ false },
	m_y_plus_full{ false },
	m_z_minus_full{ false },
	m_z_plus_full{ false } {

	m_blocks = new Block**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; ++i) {
		m_blocks[i] = new Block*[CHUNK_SIZE];
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			m_blocks[i][j] = new Block[CHUNK_SIZE];
		}
	}
}

Chunk::~Chunk() {
	Unload();

	for (int i = 0; i < CHUNK_SIZE; ++i) {
		for (int j = 0; j < CHUNK_SIZE; ++j) {
			delete[] m_blocks[i][j];
		}
		delete[] m_blocks[i];
	}
	delete[] m_blocks;
}

void Chunk::Unload() {
	m_state = ChunkState::Unloading;

	if (m_loaded) {
		m_loaded = false;
		std::vector<Chunk*> neighbors = GetNeighbors();
		for (Chunk* neighbor : neighbors) {
			if (neighbor->IsLoaded()) {
				neighbor->UpdateSurroundedFlag();
			}
		}
	}

	ClearMesh();
	ClearMeshCache();
}

void Chunk::Load() {
	m_state = ChunkState::Loading;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 15);
	auto randomBlock = std::bind(distribution, generator);

	// Generate chunk, i.e. load from file
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int z = 0; z < CHUNK_SIZE; ++z) {
			for (int y = 0; y < CHUNK_SIZE; ++y) {
				m_blocks[x][y][z] = Block((BlockType)randomBlock());
				m_blocks[x][y][z].SetActive(true);
			}
		}
	}
	m_loaded = true;

	m_state = ChunkState::Idle;
}

std::vector<Chunk*> Chunk::GetNeighbors() {
	std::vector<Chunk*> neighbors;

	Chunk* chunkXMinus = m_chunkManager->GetChunk(m_grid.x - 1, m_grid.y, m_grid.z);
	Chunk* chunkXPlus = m_chunkManager->GetChunk(m_grid.x + 1, m_grid.y, m_grid.z);
	Chunk* chunkYMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y - 1, m_grid.z);
	Chunk* chunkYPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y + 1, m_grid.z);
	Chunk* chunkZMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z - 1);
	Chunk* chunkZPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z + 1);

	if (chunkXMinus != nullptr)
		neighbors.push_back(chunkXMinus);
	if (chunkXPlus != nullptr)
		neighbors.push_back(chunkXPlus);
	if (chunkYMinus != nullptr)
		neighbors.push_back(chunkYMinus);
	if (chunkYPlus != nullptr)
		neighbors.push_back(chunkYPlus);
	if (chunkZMinus != nullptr)
		neighbors.push_back(chunkZMinus);
	if (chunkZPlus != nullptr)
		neighbors.push_back(chunkZPlus);

	return neighbors;
}

ChunkCoordKeyList Chunk::GetMissingNeighbors() {
	ChunkCoordKeyList missing;

	Chunk* chunkXMinus = m_chunkManager->GetChunk(m_grid.x - 1, m_grid.y, m_grid.z);
	Chunk* chunkXPlus = m_chunkManager->GetChunk(m_grid.x + 1, m_grid.y, m_grid.z);
	Chunk* chunkYMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y - 1, m_grid.z);
	Chunk* chunkYPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y + 1, m_grid.z);
	Chunk* chunkZMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z - 1);
	Chunk* chunkZPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z + 1);

	if (chunkXMinus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x - 1, m_grid.y, m_grid.z));
	if (chunkXPlus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x + 1, m_grid.y, m_grid.z));
	if (chunkYMinus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x, m_grid.y - 1, m_grid.z));
	if (chunkYPlus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x, m_grid.y + 1, m_grid.z));
	if (chunkZMinus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x, m_grid.y, m_grid.z - 1));
	if (chunkZPlus == nullptr)
		missing.push_back(ChunkCoordKey(m_grid.x, m_grid.y, m_grid.z + 1));

	return missing;
}

bool Chunk::Destroy(int x, int y, int z) {
	if (GetActive(x, y, z)) {
		m_blocks[x][y][z].SetActive(false);
		RebuildMesh();
		return true;
	}
	return false;
}

bool Chunk::GetActive(int x, int y, int z) {
	return m_blocks[x][y][z].IsActive();
}

glm::vec3 Chunk::GetBlockPosition(int x, int y, int z) {
	glm::vec3 position = GetPosition();
	return glm::vec3(
		position.x + x * Chunk::BLOCK_RENDER_SIZE,
		position.y + y * Chunk::BLOCK_RENDER_SIZE,
		position.z + z * Chunk::BLOCK_RENDER_SIZE);
}

void Chunk::UpdateWallFlags() {

	int x_minus = 0;
	int x_plus = 0;
	int y_minus = 0;
	int y_plus = 0;
	int z_minus = 0;
	int z_plus = 0;

	for (int a = 0; a < CHUNK_SIZE; ++a) {
		for (int b = 0; b < CHUNK_SIZE; ++b) {
			if (GetActive(0, a, b))
				++x_minus;
			if (GetActive(CHUNK_SIZE - 1, a, b))
				++x_plus;
			if (GetActive(a, 0, b))
				++y_minus;
			if (GetActive(a, CHUNK_SIZE - 1, b))
				++y_plus;
			if (GetActive(a, b, 0))
				++z_minus;
			if (GetActive(a, b, CHUNK_SIZE - 1))
				++z_plus;
		}
	}

	m_x_minus_full = false;
	m_x_plus_full = false;
	m_y_minus_full = false;
	m_y_plus_full = false;
	m_z_minus_full = false;
	m_z_plus_full = false;

	int wall_size = CHUNK_SIZE * CHUNK_SIZE;

	if (x_minus == wall_size)
		m_x_minus_full = true;
	if (x_plus == wall_size)
		m_x_plus_full = true;
	if (y_minus == wall_size)
		m_y_minus_full = true;
	if (y_plus == wall_size)
		m_y_plus_full = true;
	if (z_minus == wall_size)
		m_z_minus_full = true;
	if (z_plus == wall_size)
		m_z_plus_full = true;
}

void Chunk::UpdateSurroundedFlag() {
	Chunk* chunkXMinus = m_chunkManager->GetChunk(m_grid.x - 1, m_grid.y, m_grid.z);
	Chunk* chunkXPlus = m_chunkManager->GetChunk(m_grid.x + 1, m_grid.y, m_grid.z);
	Chunk* chunkYMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y - 1, m_grid.z);
	Chunk* chunkYPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y + 1, m_grid.z);
	Chunk* chunkZMinus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z - 1);
	Chunk* chunkZPlus = m_chunkManager->GetChunk(m_grid.x, m_grid.y, m_grid.z + 1);

	if (chunkXMinus != nullptr && chunkXMinus->IsLoaded() && chunkXMinus->m_x_plus_full &&
		chunkXPlus != nullptr  && chunkXPlus->IsLoaded() && chunkXPlus->m_x_minus_full &&
		chunkYMinus != nullptr && chunkYMinus->IsLoaded() && chunkYMinus->m_y_plus_full &&
		chunkYPlus != nullptr  && chunkYPlus->IsLoaded() && chunkYPlus->m_y_minus_full &&
		chunkZMinus != nullptr && chunkZMinus->IsLoaded() && chunkZMinus->m_z_plus_full &&
		chunkZPlus != nullptr  && chunkZPlus->IsLoaded() && chunkZPlus->m_z_minus_full) {

		m_surrounded = true;
	} else {

		m_surrounded = false;
	}
}

void Chunk::UpdateEmptyFlag() {
	int numVerts;
	int numTriangles;
	m_renderer->GetMeshInfo(m_meshId, &numVerts, &numTriangles);
	m_empty = (numVerts == 0 && numTriangles == 0);
}

void Chunk::Render() {

	int meshToUse = (m_cachedMeshId != -1) ?  m_cachedMeshId : m_meshId;

	if (meshToUse != -1) {
		m_renderer->PushMatrix();
			glm::vec3 pos = GetPosition();
			m_renderer->Translate(pos.x, pos.y + Chunk::HALF_RENDER_SIZE, pos.z);
			m_renderer->RenderMesh(meshToUse);
		m_renderer->PopMatrix();
	}
}

void Chunk::RebuildMesh() {

	m_state = ChunkState::RebuildingMesh;

	CacheMesh();
	CreateMesh();

	UpdateWallFlags();
	UpdateSurroundedFlag();
	
	std::vector<Chunk*> neighbors = GetNeighbors();
	for (Chunk* neighbor : neighbors) {
		if (neighbor->IsLoaded()) {
			neighbor->UpdateSurroundedFlag();
			if (m_rebuildNeighbors) {
				neighbor->SetNeedsRebuild(true, false);
			}
		}
	}
	m_rebuildNeighbors = false;
	m_rebuild = false;
	
	m_state = ChunkState::RebuildComplete;
}

void Chunk::CreateMesh() {

	ClearMesh();

	if (m_meshId == -1) {
		m_renderer->CreateMesh(&m_meshId);
	}

	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			for (int z = 0; z < CHUNK_SIZE; ++z) {

				if (m_blocks[x][y][z].IsActive()) {

					bool activeXMinus = false;
					bool activeXPlus = false;
					bool activeYMinus = false;
					bool activeYPlus = false;
					bool activeZMinus = false;
					bool activeZPlus = false;

					if (x > 0)
						activeXMinus = m_blocks[x - 1][y][z].IsActive();
					if (x < CHUNK_SIZE - 1)
						activeXPlus = m_blocks[x + 1][y][z].IsActive();

					if (y > 0)
						activeYMinus = m_blocks[x][y - 1][z].IsActive();
					if (y < CHUNK_SIZE - 1)
						activeYPlus = m_blocks[x][y + 1][z].IsActive();

					if (z > 0)
						activeZMinus = m_blocks[x][y][z - 1].IsActive();
					if (z < CHUNK_SIZE - 1)
						activeZPlus = m_blocks[x][y][z + 1].IsActive();

					float sx = (float)x * Chunk::BLOCK_RENDER_SIZE;
					float sy = (float)y * Chunk::BLOCK_RENDER_SIZE;
					float sz = (float)z * Chunk::BLOCK_RENDER_SIZE;

					BlockType blockType = m_blocks[x][y][z].GetType();
					m_renderer->AddCubeToMesh(
						m_meshId, glm::vec3(sx, sy, sz), glm::vec3(Chunk::HALF_RENDER_SIZE),
						Block::GetTopLeftUV(blockType), Block::GetBottomRightUV(blockType),
						activeXMinus, activeXPlus,
						activeYMinus, activeYPlus,
						activeZMinus, activeZPlus);
				}
			}
		}
	}
}

void Chunk::CompleteMesh() {

	m_renderer->FinishMesh(m_meshId);
	UpdateEmptyFlag();
	ClearMeshCache();
	m_state = ChunkState::Idle;
}

void Chunk::CacheMesh() {
	ClearMeshCache();
	m_cachedMeshId = m_meshId;
	m_meshId = -1;
}

void Chunk::ClearMesh() {
	if (m_meshId != -1) {
		m_renderer->DeleteMesh(m_meshId);
		m_meshId = -1;
	}
}

void Chunk::ClearMeshCache() {
	if (m_cachedMeshId != -1) {
		m_renderer->DeleteMesh(m_cachedMeshId);
		m_cachedMeshId = -1;
	}
}

void Chunk::SetNeedsRebuild(bool rebuild, bool rebuildNeighbors) {
	m_rebuild = rebuild;
	m_rebuildNeighbors = rebuildNeighbors;
}

bool Chunk::operator<(const Chunk &w) const
{
	glm::vec3 cameraPos = m_renderer->GetCamera().GetPosition();
	int cameraX = cameraPos.x / (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
	int cameraY = cameraPos.y / (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
	int cameraZ = cameraPos.z / (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
	
	int distance = abs(cameraX - m_grid.x) + abs(cameraY - m_grid.y) + abs(cameraZ - m_grid.z);
	int wDistance = abs(cameraX - w.GetGridX()) + abs(cameraY - w.GetGridY()) + abs(cameraZ - w.GetGridZ());

	return(distance < wDistance);
}

bool Chunk::ClosestToCamera(const Chunk *lhs, const Chunk *rhs)
{
	if ((*lhs) < (*rhs)) { return true; }
	if ((*rhs) < (*lhs)) { return false; }
	return (*lhs) < (*rhs);
}

float Chunk::GetWorldCoord(int c) {
	return (float)(c * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
}

glm::vec3 Chunk::GetWorldPosition(int x, int y, int z) {
	return glm::vec3(GetWorldCoord(x), GetWorldCoord(y), GetWorldCoord(z));
}

glm::vec3 Chunk::GetWorldCenter(int x, int y, int z) {
	return GetWorldPosition(x, y, z) + glm::vec3(Chunk::CHUNK_SIZE * Chunk::HALF_RENDER_SIZE);
}

int Chunk::GetChunkCoord(float c) {
	int c_coord = (int)((c + Chunk::HALF_RENDER_SIZE) / (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE));
	return c <= -Chunk::HALF_RENDER_SIZE ? c_coord - 1 : c_coord;
}

glm::ivec3 Chunk::GetChunkPosition(float x, float y, float z) {
	return glm::ivec3(GetChunkCoord(x), GetChunkCoord(y), GetChunkCoord(z));
}

glm::ivec3 Chunk::GetBlockPosition(float x, float y, float z) {

	glm::ivec3 blockPos = glm::ivec3(
		(int)((glm::abs(x) + Chunk::HALF_RENDER_SIZE) / Chunk::BLOCK_RENDER_SIZE) % Chunk::CHUNK_SIZE,
		(int)((glm::abs(y) + Chunk::HALF_RENDER_SIZE) / Chunk::BLOCK_RENDER_SIZE) % Chunk::CHUNK_SIZE,
		(int)((glm::abs(z) + Chunk::HALF_RENDER_SIZE) / Chunk::BLOCK_RENDER_SIZE) % Chunk::CHUNK_SIZE);

	if (x < 0.0f && blockPos.x != 0) {
		blockPos.x = Chunk::CHUNK_SIZE - blockPos.x;
	}
	if (y < 0.0f && blockPos.y != 0) {
		blockPos.y = Chunk::CHUNK_SIZE - blockPos.y;
	}
	if (z < 0.0f && blockPos.z != 0) {
		blockPos.z = Chunk::CHUNK_SIZE - blockPos.z;
	}
	return blockPos;
}