#pragma once

#include <glm\glm.hpp>

namespace sm {
class Plane3D {
public:
	Plane3D();
	Plane3D(glm::vec3 normal, glm::vec3 point);
	Plane3D(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);

	float GetDistance(glm::vec3 point);

private:
	glm::vec3 m_normal;
	float m_d;
};
}