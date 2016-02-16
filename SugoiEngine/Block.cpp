#include "Block.h"
#include "Chunk.h"
const float Block::TEXTURE_UV_LENGTH = (float) Block::TEXTURE_LENGTH / (float) Block::TEXTURE_MAP_LENGTH;

glm::vec2 Block::GetTopLeftUV(BlockType type) {
	float u = (float)(type % TEXTURES_PER_ROW) * TEXTURE_UV_LENGTH;
	float v = (float)(TEXTURES_PER_ROW - (type / TEXTURES_PER_ROW)) * TEXTURE_UV_LENGTH;
	return glm::vec2(u, v);
}

glm::vec2 Block::GetBottomLeftUV(BlockType type) {
	return GetTopLeftUV(type) + glm::vec2(0.0f, -TEXTURE_UV_LENGTH);
}

glm::vec2 Block::GetTopRightUV(BlockType type) {
	return GetTopLeftUV(type) + glm::vec2(TEXTURE_UV_LENGTH, 0.0f);
}

glm::vec2 Block::GetBottomRightUV(BlockType type) {
	return GetTopLeftUV(type) + glm::vec2(TEXTURE_UV_LENGTH, -TEXTURE_UV_LENGTH);
}

sm::Region3D Block::GetCollideRegion() {
	static sm::Region3D collideRegion;
	if (collideRegion.NumPlanes() == 0) {
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f)));
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(-Chunk::BLOCK_RENDER_SIZE, 0.0f, 0.0f)));
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, Chunk::BLOCK_RENDER_SIZE, 0.0f)));
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -Chunk::BLOCK_RENDER_SIZE, 0.0f)));
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, Chunk::BLOCK_RENDER_SIZE)));
		collideRegion.AddPlane(sm::Plane3D(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -Chunk::BLOCK_RENDER_SIZE)));
	}
	return collideRegion;
}