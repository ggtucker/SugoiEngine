#pragma once

#include <vector>
#include "Plane3D.h"

namespace sm {

enum CollisionResult {
	Outside = 0,
	Intersect,
	Inside
};

class Region3D {
public:

	Region3D() {}
	void Clear() { m_planes.clear(); }
	void AddPlane(Plane3D plane) { m_planes.push_back(plane); }
	Plane3D& GetPlane(unsigned int index) { return m_planes[index]; }
	unsigned int NumPlanes() { return m_planes.size(); }

	CollisionResult PointInRegion(glm::vec3 point);
	CollisionResult CubeInRegion(glm::vec3 center, glm::vec3 halfSize);

private:
	std::vector<Plane3D> m_planes;
};

}