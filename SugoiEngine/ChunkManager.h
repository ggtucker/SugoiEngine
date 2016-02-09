#pragma once

#include <map>
#include <vector>
#include <thread>
#include <SugoiRender\Renderer.h>
#include "Chunk.h"

struct ChunkCoordKey {
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

using ChunkList = std::vector<Chunk*>;
using ChunkCoordKeyList = std::vector<ChunkCoordKey>;

class ChunkManager {
public:
	ChunkManager(sr::Renderer* renderer);
	~ChunkManager();

	static void _UpdateChunksThread(void* pData);
	void UpdateChunksThread();

	Chunk* GetChunk(int x, int y, int z);
	void CreateNewChunk(int x, int y, int z);

	// Render
	void Render();

private:
	sr::Renderer* m_renderer;
	std::map<ChunkCoordKey, Chunk*> m_chunkMap;

	// Threads
	std::thread* m_updateChunksThread;
	std::mutex m_chunkMapMutex;
	bool m_updateThreadActive;
	bool m_updateThreadFinished;
	
	// Step lock
	bool m_stepLockEnabled;
	bool m_updateStepLock;
};