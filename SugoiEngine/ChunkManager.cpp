#include "ChunkManager.h"

#include <algorithm>

ChunkManager::ChunkManager(sr::Renderer* renderer) :
		m_renderer{ renderer },
		m_updateThreadActive{ true },
		m_updateThreadFinished{ false },
		m_stepLockEnabled{ false },
		m_updateStepLock{ true } {

	//m_updateChunksThread = new std::thread(_UpdateChunksThread, this);
}

ChunkManager::~ChunkManager() {
	m_stepLockEnabled = false;
	m_updateStepLock = true;
	m_updateThreadActive = false;
	//while (m_updateThreadFinished == false) {
	//	Sleep(200);
	//}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void _UpdateChunksThread(void* pData) {
	ChunkManager* chunkManager = (ChunkManager*)pData;
	chunkManager->UpdateChunksThread();
}

void ChunkManager::UpdateChunksThread() {
	while (m_updateThreadActive) {

		while (m_stepLockEnabled && m_updateStepLock) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		ChunkList updateList;
		ChunkCoordKeyList addList;
		ChunkList rebuildList;
		ChunkList unloadList;

		m_chunkMapMutex.lock();
		std::map<ChunkCoordKey, Chunk*>::iterator it;
		for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
			Chunk* chunk = it->second;
			updateList.push_back(chunk);
		}
		m_chunkMapMutex.unlock();

		int numAddedChunks = 0;
		const int MAX_NUM_CHUNKS_ADD = 10;
		std::sort(updateList.begin(), updateList.end(), Chunk::ClosestToCamera);
		for (unsigned int i = 0; i < updateList.size(); ++i) {

			Chunk* chunk = updateList[i];

			if (chunk) {
				int gridX = chunk->GetGridX();
				int gridY = chunk->GetGridY();
			}
		}
	}

	m_updateThreadFinished = true;
}

Chunk* ChunkManager::GetChunk(int x, int y, int z) {
	ChunkCoordKey key;
	key.x = x;
	key.y = y;
	key.z = z;

	m_chunkMapMutex.lock();
	std::map<ChunkCoordKey, Chunk*>::iterator it = m_chunkMap.find(key);
	if (it != m_chunkMap.end()) {
		Chunk* chunk = m_chunkMap[key];
		m_chunkMapMutex.unlock();
		return chunk;
	}
	m_chunkMapMutex.unlock();

	return nullptr;
}

void ChunkManager::CreateNewChunk(int x, int y, int z) {
	ChunkCoordKey key;
	key.x = x;
	key.y = y;
	key.z = z;

	Chunk* chunk = new Chunk(m_renderer, this);
	
	float xPos = x * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
	float yPos = y * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);
	float zPos = z * (Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE);

	chunk->SetPosition(xPos, yPos, zPos);
	chunk->SetGrid(x, y, z);

	m_chunkMapMutex.lock();
	m_chunkMap[key] = chunk;
	m_chunkMapMutex.unlock();

	chunk->Setup();
	chunk->SetNeedsRebuild(true, true);
	chunk->RebuildMesh();
	chunk->SetCreated(true);

	// UpdateChunkNeighbors(chunk, x, y, z);
}

void ChunkManager::Render() {
	m_renderer->PushMatrix();
		m_chunkMapMutex.lock();
		std::map<ChunkCoordKey, Chunk*>::iterator it;
		for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
			Chunk* chunk = it->second;
			if (chunk && chunk->IsCreated()) {
				chunk->Render();
			}
		}
		m_chunkMapMutex.unlock();
	m_renderer->PopMatrix();
}