#pragma once

#include "BlockType.h"

class Block {
public:
	Block() : m_active{ true }, m_blockType{ BlockType::Default } {}
	~Block() {}

	bool IsActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }

private:
	bool m_active;
	BlockType m_blockType;
};