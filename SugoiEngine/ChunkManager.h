#pragma once

#include <map>
#include <vector>
#include <thread>
#include <SugoiRender\Renderer.h>
#include "Chunk.h"

using ChunkList = std::vector<Chunk*>;

class ChunkManager {
public:
	ChunkManager(sr::Renderer* renderer);
	~ChunkManager();

	static void _UpdateChunksThread(void* pData);
	void UpdateChunksThread();

	Chunk* GetChunk(int x, int y, int z);
	void CreateNewChunk(int x, int y, int z);
	void UnloadChunk(Chunk* chunk);

	void Update();
	void Render();

private:
	sr::Renderer* m_renderer;
	std::map<ChunkCoordKey, Chunk*> m_chunkMap;

	// Load radius
	float m_loadRadius;

	// Threads
	std::thread* m_updateChunksThread;
	std::mutex m_chunkMapMutex;
	bool m_updateThreadActive;
	bool m_updateThreadFinished;
	
	// Step lock
	bool m_stepLockEnabled;
	bool m_updateStepLock;
};