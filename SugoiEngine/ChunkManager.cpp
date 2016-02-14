#include "ChunkManager.h"

#include <algorithm>

ChunkManager::ChunkManager(sr::Renderer* renderer, int textureId) :
		m_renderer{ renderer },
		m_textureId{ textureId },
		m_updateChunksThread{ nullptr },
		m_loadRadius{ 50.0f },
		m_updateThreadActive{ true },
		m_updateThreadFinished{ false },
		m_stepLockEnabled{ false },
		m_updateStepLock{ true } {

	m_updateChunksThread = new std::thread(_UpdateChunksThread, this);
}

ChunkManager::~ChunkManager() {
	m_stepLockEnabled = false;
	m_updateStepLock = true;
	m_updateThreadActive = false;
	while (m_updateThreadFinished == false) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void ChunkManager::_UpdateChunksThread(void* pData) {
	ChunkManager* chunkManager = (ChunkManager*)pData;
	chunkManager->UpdateChunksThread();
}

void ChunkManager::UpdateChunksThread() {
	//std::cout << "Thread started" << std::endl;
	while (m_updateThreadActive) {

		while (m_stepLockEnabled && m_updateStepLock) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		ChunkList updateList;
		ChunkList rebuildList;
		ChunkCoordKeyList addKeyList;

		// STEP 1: PUT ALL CREATED CHUNKS IN `UPDATE LIST`
		//std::cout << "STEP 1" << std::endl;

		m_chunkMapMutex.lock();
		std::map<ChunkCoordKey, Chunk*>::iterator it;
		for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
			Chunk* chunk = it->second;
			if (!chunk->IsUnloading()) {
				updateList.push_back(chunk);
			}
		}
		m_chunkMapMutex.unlock();

		//std::cout << "updatelist:" << updateList.size() << std::endl;

		// STEP 2: FIND CHUNKS TO ADD (OR UNLOAD IF THEY'RE TOO FAR)
		//std::cout << "STEP 2" << std::endl;

		int numAddedChunks = 0;
		const int MAX_NUM_CHUNKS_ADD = 10;
		std::sort(updateList.begin(), updateList.end(), Chunk::ClosestToCamera);
		for (unsigned int i = 0; i < updateList.size(); ++i) {

			Chunk* chunk = updateList[i];

			if (chunk) {
				glm::vec3 chunkCenter = chunk->GetCenter();
				glm::vec3 cameraPos = m_renderer->GetCamera().GetPosition();
				float cameraDistance = glm::length(chunkCenter - cameraPos);

				if (cameraDistance > m_loadRadius) {
					chunk->SetUnloading();
				}
				else if (numAddedChunks < MAX_NUM_CHUNKS_ADD) {

					ChunkCoordKeyList missing = chunk->GetMissingNeighbors();

					if (!chunk->IsEmpty() || chunk->GetGridY() == 0) {

						for (ChunkCoordKey key : missing) {

							if (std::find(addKeyList.begin(), addKeyList.end(), key) == addKeyList.end()) {

								glm::vec3 chunkCenter = Chunk::GetWorldCenter(key.x, key.y, key.z);
								float cameraDistance = glm::length(chunkCenter - cameraPos);

								if (cameraDistance <= m_loadRadius && key.y == 0) {
									addKeyList.push_back(key);
									++numAddedChunks;
								}
							}
						}
					}
				}
			}
		}
		updateList.clear();

		// STEP 3: ADD CHUNKS
		//std::cout << "STEP 3: " << addKeyList.size() << std::endl;

		for (unsigned int i = 0; i < addKeyList.size(); ++i) {
			ChunkCoordKey key = addKeyList[i];
			CreateNewChunk(key.x, key.y, key.z);
		}
		addKeyList.clear();

		// STEP 4: CHECK FOR REBUILD CHUNKS
		//std::cout << "STEP 4" << std::endl;

		m_chunkMapMutex.lock();
		for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
			Chunk* chunk = it->second;
			if (!chunk->IsUnloading() && chunk->NeedsRebuild()) {
				rebuildList.push_back(chunk);
			}
		}
		m_chunkMapMutex.unlock();

		// STEP 5: REBUILD CHUNKS
		//std::cout << "STEP 5" << std::endl;

		int numRebuildChunks = 0;
		const int MAX_NUM_CHUNKS_REBUILD = 30;
		for (unsigned int i = 0; i < rebuildList.size() && numRebuildChunks < MAX_NUM_CHUNKS_REBUILD; ++i) {
			Chunk* chunk = rebuildList[i];
			chunk->RebuildMesh();
			++numRebuildChunks;
		}
		rebuildList.clear();

		if (m_stepLockEnabled && !m_updateStepLock) {
			m_updateStepLock = true;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	m_updateThreadFinished = true;
}

Chunk* ChunkManager::GetChunk(int x, int y, int z) {
	ChunkCoordKey key(x, y, z);

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
	ChunkCoordKey key(x, y, z);
	std::cout << "Create chunk: " << key.x << " " << key.y << " " << key.z << std::endl;

	Chunk* chunk = new Chunk(m_renderer, this);
	chunk->SetGrid(x, y, z);

	m_chunkMapMutex.lock();
	m_chunkMap[key] = chunk;
	m_chunkMapMutex.unlock();

	chunk->Load();
	chunk->SetNeedsRebuild(true, true);
	chunk->RebuildMesh();

	// UpdateChunkNeighbors(chunk, x, y, z);
}

void ChunkManager::UnloadChunk(Chunk* chunk) {
	ChunkCoordKey key(chunk->GetGridX(), chunk->GetGridY(), chunk->GetGridZ());
	std::cout << "Unload chunk: " << key.x << " " << key.y << " " << key.z << std::endl;

	// Remove from map
	m_chunkMapMutex.lock();
	std::map<ChunkCoordKey, Chunk*>::iterator it = m_chunkMap.find(key);
	if (it != m_chunkMap.end()) {
		m_chunkMap.erase(key);
	}
	m_chunkMapMutex.unlock();

	// Unload and delete chunk
	delete chunk;
}

void ChunkManager::Update() {
	ChunkList unloadList;

	m_chunkMapMutex.lock();
	std::map<ChunkCoordKey, Chunk*>::iterator it;
	for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
		Chunk* chunk = it->second;
		if (chunk->IsUnloading()) {
			unloadList.push_back(chunk);
		}
		else if (chunk->IsRebuildComplete()) {
			chunk->CompleteMesh();
		}
	}
	m_chunkMapMutex.unlock();
	
	int numUnloadChunks = 0;
	const static int MAX_NUM_CHUNKS_UNLOAD = 1;
	for (unsigned int i = 0; i < unloadList.size() && numUnloadChunks < MAX_NUM_CHUNKS_UNLOAD; ++i) {
		Chunk* chunk = unloadList[i];
		UnloadChunk(chunk);
	}
}

void ChunkManager::Render() {
	if (m_textureId != -1) {
		m_renderer->BindTexture(m_textureId);
	}
	m_renderer->PushMatrix();
		m_chunkMapMutex.lock();
		std::map<ChunkCoordKey, Chunk*>::iterator it;
		for (it = m_chunkMap.begin(); it != m_chunkMap.end(); ++it) {
			Chunk* chunk = it->second;
			if (chunk->IsLoaded()) {

				glm::vec3 chunkCenter = chunk->GetCenter();
				float chunkSize = Chunk::CHUNK_SIZE * Chunk::BLOCK_RENDER_SIZE;

				if (m_renderer->CubeInFrustum(chunkCenter, chunkSize, chunkSize, chunkSize)) {
					chunk->Render();
				}
			}
		}
		m_chunkMapMutex.unlock();
	m_renderer->PopMatrix();
}