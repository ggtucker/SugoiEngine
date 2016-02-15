#pragma once

#include <map>
#include <vector>
#include <thread>
#include <SugoiRender\Renderer.h>
#include "Chunk.h"

class Player;

using ChunkList = std::vector<Chunk*>;

class ChunkManager {
public:
	ChunkManager(sr::Renderer* renderer, int textureId);
	~ChunkManager();

	static void _UpdateChunksThread(void* pData);
	void UpdateChunksThread();

	Player* GetPlayer() { return m_player; }
	void SetPlayer(Player* player) { m_player = player; }

	Chunk* GetChunk(int x, int y, int z);
	void CreateNewChunk(int x, int y, int z);
	void UnloadChunk(Chunk* chunk);

	Chunk* GetChunkFromWorldPosition(float x, float y, float z);

	void Update();
	void Render();

private:
	sr::Renderer* m_renderer;
	Player* m_player;
	std::map<ChunkCoordKey, Chunk*> m_chunkMap;

	// Texture map
	int m_textureId;

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