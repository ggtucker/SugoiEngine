#pragma once

#include <SugoiRender\Renderer.h>
#include <vector>
#include "Block.h"

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

class Chunk {
public:
	static const int CHUNK_SIZE = 16;
	static const int NUM_NEIGHBORS = 6;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_RENDER_SIZE;

	// Static position functions
	static float GetWorldX(int x) { return x * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE; }
	static float GetWorldY(int y) { return y * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE; }
	static float GetWorldZ(int z) { return z * Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE; }
	static glm::vec3 GetWorldPosition(int x, int y, int z) { return glm::vec3(GetWorldX(x), GetWorldY(y), GetWorldZ(z)); }
	static glm::vec3 GetWorldCenter(int x, int y, int z) { return GetWorldPosition(x, y, z) + glm::vec3(Chunk::CHUNK_SIZE * Chunk::HALF_RENDER_SIZE); }

	Chunk(sr::Renderer* renderer, ChunkManager* chunkManager);
	~Chunk();

	// Creation and destruction
	void Unload();
	void Setup();
	bool IsSetup() const { return m_setup; }
	void SetCreated(bool created) { m_created = created; }
	bool IsCreated() { return m_created; }
	bool IsEmpty() { return m_empty; }
	void SetUnloading(bool unloading) { m_unloading = unloading; }
	bool IsUnloading() { return m_unloading; }

	// World position
	float GetX() const { return GetWorldX(m_grid.x); }
	float GetY() const { return GetWorldY(m_grid.y); }
	float GetZ() const { return GetWorldZ(m_grid.z); }
	glm::vec3 GetPosition() const { return glm::vec3(GetWorldX(m_grid.x), GetWorldY(m_grid.y), GetWorldZ(m_grid.z)); }
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
	bool GetActive(int x, int y, int z);

	// Flags
	void UpdateWallFlags();
	void UpdateSurroundedFlag();
	void UpdateEmptyFlag();
	
	// Rebuild
	void CreateMesh();
	void RebuildMesh();
	void CompleteMesh();
	void CacheMesh();
	void ClearMeshCache();
	void SetNeedsRebuild(bool rebuild, bool rebuildNeighbors);
	bool NeedsRebuild() const { return m_rebuild; }
	bool IsRebuildComplete() const { return m_rebuildComplete; }

	// Render
	void Render();

	// Chunk sorting by closeness to camera
	bool operator<(const Chunk &w) const;
	static bool ClosestToCamera(const Chunk *lhs, const Chunk *rhs);

private:
	/* Private members */
	sr::Renderer* m_renderer;
	ChunkManager* m_chunkManager;

	Block*** m_blocks;
	int m_meshId;
	int m_cachedMeshId;
	glm::ivec3 m_grid;

	// Setup and creation flags
	bool m_setup;
	bool m_created;
	bool m_unloading;
	bool m_rebuild;
	bool m_rebuildNeighbors;
	bool m_rebuildComplete;

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

	void addCubeToMesh(int x, int y, int z);
};