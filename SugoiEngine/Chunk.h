#pragma once

#include <SugoiRender\Renderer.h>
#include <vector>
#include "Block.h"

class Player;

class ChunkManager;

struct ChunkCoordKey {
	ChunkCoordKey(int x, int y, int z) : x{ x }, y{ y }, z{ z } {}
	int x;
	int y;
	int z;
};

inline bool const operator==(const ChunkCoordKey& l, const ChunkCoordKey& r) {
	return l.x == r.x && l.y == r.y && l.z == r.z;
};

inline bool const operator<(const ChunkCoordKey& l, const ChunkCoordKey& r) {
	if (l.x < r.x)  return true;
	if (l.x > r.x)  return false;

	if (l.y < r.y)  return true;
	if (l.y > r.y)  return false;

	if (l.z < r.z)  return true;
	if (l.z > r.z)  return false;

	return false;
};

using ChunkCoordKeyList = std::vector<ChunkCoordKey>;

enum class ChunkState {
	Initial,
	Loading,
	Unloading,
	RebuildingMesh,
	RebuildComplete,
	Idle
};

class Chunk {
public:
	/* PUBLIC FUNCTIONS */

	// Construction & destruction
	Chunk(sr::Renderer* renderer, ChunkManager* chunkManager);
	~Chunk();

	// Creation and destruction
	void Unload();
	void Load();
	void SetUnloading() { m_state = ChunkState::Unloading; }
	bool IsLoaded() { return m_loaded; }
	bool IsEmpty() { return m_empty; }

	// Chunk state accessors
	ChunkState GetState() { return m_state; }
	bool IsInitialState() { return m_state == ChunkState::Initial; }
	bool IsLoading() { return m_state == ChunkState::Loading; }
	bool IsUnloading() { return m_state == ChunkState::Unloading; }
	bool IsRebuildingMesh() { return m_state == ChunkState::RebuildingMesh; }
	bool IsRebuildComplete() const { return m_state == ChunkState::RebuildComplete; }
	bool IsIdle() { return m_state == ChunkState::Idle; }

	// World position
	float GetX() const { return GetWorldCoord(m_grid.x); }
	float GetY() const { return GetWorldCoord(m_grid.y); }
	float GetZ() const { return GetWorldCoord(m_grid.z); }
	glm::vec3 GetPosition() const { return GetWorldPosition(m_grid.x, m_grid.y, m_grid.z); }
	glm::vec3 GetCenter() const { return GetPosition() + glm::vec3(Chunk::CHUNK_SIZE * Chunk::HALF_RENDER_SIZE); }

	// Grid position
	int GetGridX() const { return m_grid.x; }
	int GetGridY() const { return m_grid.y; }
	int GetGridZ() const { return m_grid.z; }
	glm::ivec3 GetGrid() const { return m_grid; }
	void SetGrid(int x, int y, int z) { m_grid = glm::ivec3(x, y, z); }

	// Neighbors
	std::vector<Chunk*> GetNeighbors();
	ChunkCoordKeyList GetMissingNeighbors();

	// Active
	bool Destroy(int x, int y, int z);
	bool GetActive(int x, int y, int z);
	glm::vec3 GetBlockPosition(int x, int y, int z);

	// Flags
	void UpdateWallFlags();
	void UpdateSurroundedFlag();
	void UpdateEmptyFlag();
	
	// Rebuild
	void RebuildMesh();
	void CreateMesh();
	void CompleteMesh();
	void CacheMesh();
	void ClearMesh();
	void ClearMeshCache();
	void SetNeedsRebuild(bool rebuild, bool rebuildNeighbors);
	bool NeedsRebuild() const { return m_rebuild; }

	// Render
	void Render();

	// Chunk sorting by closeness to camera
	bool operator<(const Chunk &w) const;
	static bool ClosestToCamera(const Chunk *lhs, const Chunk *rhs);

	// Static position functions
	static float GetWorldCoord(int c);
	static int GetChunkCoord(float c);
	static glm::vec3 GetWorldPosition(int x, int y, int z);
	static glm::vec3 GetWorldCenter(int x, int y, int z);
	static glm::ivec3 GetChunkPosition(float x, float y, float z);
	static glm::ivec3 GetBlockPosition(float x, float y, float z);

public:
	/* PUBLIC MEMBERS */
	static const int CHUNK_SIZE = 16;
	static const int NUM_NEIGHBORS = 6;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_RENDER_SIZE;

private:
	/* PRIVATE MEMBERS */
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;

	Block*** m_blocks;
	int m_meshId;
	int m_cachedMeshId;
	glm::ivec3 m_grid;

	// Setup and creation flags
	ChunkState m_state;
	bool m_loaded;
	bool m_rebuild;
	bool m_rebuildNeighbors;

	// Flags for empty chunk and completely surrounded
	bool m_empty;
	bool m_surrounded;

	// Whether a surface edge is completely filled
	bool m_x_minus_full;
	bool m_x_plus_full;
	bool m_y_minus_full;
	bool m_y_plus_full;
	bool m_z_minus_full;
	bool m_z_plus_full;
};