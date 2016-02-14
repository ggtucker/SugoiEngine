#pragma once

#include <glm\glm.hpp>

enum BlockType {
	BLOCK_DEFAULT = 0,
	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_WATER,
	BLOCK_STONE,
	BLOCK_WOOD,
	BLOCK_SAND,
	BLOCK_NUM_TYPES
};

class Block {
public:
	/* PUBLIC FUNCTIONS */
	Block() : m_active{ true }, m_blockType{ BLOCK_DEFAULT } {}
	Block(BlockType type) : m_active{ true }, m_blockType{ type } {}
	~Block() {}

	bool IsActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }
	BlockType GetType() { return m_blockType; }

	/* PUBLIC STATIC FUNCTIONS */
	static glm::vec2 GetTopLeftUV(BlockType type);
	static glm::vec2 GetBottomLeftUV(BlockType type);
	static glm::vec2 GetTopRightUV(BlockType type);
	static glm::vec2 GetBottomRightUV(BlockType type);

	/* PUBLIC MEMBERS */
	static const int TEXTURE_MAP_LENGTH = 256;
	static const int TEXTURE_LENGTH = 16;
	static const int TEXTURES_PER_ROW = TEXTURE_MAP_LENGTH / TEXTURE_LENGTH;
	static const float TEXTURE_UV_LENGTH;

private:
	/* PRIVATE MEMBERS */
	bool m_active;
	BlockType m_blockType;
};

