#pragma once

#include <glm/glm.hpp>

namespace sr {
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
}