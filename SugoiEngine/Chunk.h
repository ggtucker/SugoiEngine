#pragma once

#include <SugoiRender\Renderer.h>
#include "Block.h"

class Chunk {
public:
	static const int CHUNK_SIZE = 16;

	Chunk();
	~Chunk();

	//void Update(float dt);
	void Render(sr::Renderer& renderer);
	void CreateMesh(sr::Renderer& renderer);

private:
	Block*** m_blocks;
	unsigned int m_meshId;
	glm::vec3 m_position;

	void addCubeToMesh(sr::Renderer& renderer, int x, int y, int z);
};