#pragma once

#include <SugoiRender\Renderer.h>
#include "Block.h"

class ChunkManager;

class Chunk {
public:
	static const int CHUNK_SIZE = 16;
	static const int NUM_NEIGHBORS = 6;
	static const float BLOCK_RENDER_SIZE;
	static const float HALF_RENDER_SIZE;

	Chunk(sr::Renderer* renderer, ChunkManager* chunkManager);
	~Chunk();

	// Creation and destruction
	void Unload();
	void Setup();
	bool IsSetup() const { return m_setup; }
	void SetCreated(bool created) { m_created = created; }
	bool IsCreated() { return m_created; }

	// Position
	float GetX() const { return m_position.x; }
	float GetY() const { return m_position.y; }
	float GetZ() const { return m_position.z; }
	glm::vec3 GetPosition() const { return m_position; }
	void SetPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }

	// Grid position
	int GetGridX() const { return m_grid.x; }
	int GetGridY() const { return m_grid.y; }
	int GetGridZ() const { return m_grid.z; }
	glm::ivec3 GetGrid() const { return m_grid; }
	void SetGrid(int x, int y, int z) { m_grid = glm::ivec3(x, y, z); }

	// Neighbors
	std::vector<Chunk*> GetNeighbors();

	// Active
	bool GetActive(int x, int y, int z);

	// Flags
	void UpdateWallFlags();
	void UpdateSurroundedFlag();
	void UpdateEmptyFlag();
	
	// Rebuild
	void CreateMesh();
	void RebuildMesh();
	void SetNeedsRebuild(bool rebuild, bool rebuildNeighbors);
	bool NeedsRebuild() const { return m_rebuild; }

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
	glm::vec3 m_position;
	glm::ivec3 m_grid;

	// Setup and creation flags
	bool m_setup;
	bool m_created;
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

	void addCubeToMesh(int x, int y, int z);
};