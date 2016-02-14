#include "Plane3D.h"

namespace sm {
Plane3D::Plane3D() : m_d{ 0.0f } {}

Plane3D::Plane3D(glm::vec3 normal, glm::vec3 point) : m_normal{ glm::normalize(normal) } {
	m_d = -glm::dot(normal, point);
}

Plane3D::Plane3D(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 a1 = v1 - v2;
	glm::vec3 a2 = v3 - v2;

	m_normal = glm::cross(a2, a1);
	m_normal = glm::normalize(m_normal);

	m_d = -glm::dot(m_normal, v2);
}

float Plane3D::GetDistance(glm::vec3 point) {
	return (m_d + dot(m_normal, point));
}
}