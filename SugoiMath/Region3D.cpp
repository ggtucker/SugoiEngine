#include "Region3D.h"

namespace sm {

Collision Region3D::PointInRegion(glm::vec3 point) {

	for (int i = 0; i < NumPlanes(); ++i) {
		float distance = GetPlane(i).GetDistance(point);
		if (distance < 0.0f) {
			return Collision::Outside;
		}
	}
	
	return Collision::Inside;
}

Collision Region3D::CubeInRegion(glm::vec3 center, glm::vec3 halfSize) {
	Collision result = Collision::Inside;

	float x = halfSize.x;
	float y = halfSize.y;
	float z = halfSize.z;

	for (int i = 0; i < NumPlanes(); ++i) {
		int out = 0;
		int in = 0;

		if (GetPlane(i).GetDistance(center + glm::vec3(-x, -y, -z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(x, -y, -z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(-x, -y, z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(x, -y, z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(-x, y, -z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(x, y, -z)) < 0.0f)
			++out;
		else
			++in;

		if (GetPlane(i).GetDistance(center + glm::vec3(-x, y, z)) < 0.0f)
			++out;
		else
			in++;

		if (GetPlane(i).GetDistance(center + glm::vec3(x, y, z)) < 0.0f)
			++out;
		else
			in++;

		// If all corners are outside the region
		if (!in) {
			return Collision::Outside;
		}
		// If some corners are outside and others are inside	
		else if (out) {
			result = Collision::Intersect;
		}
	}

	return result;
}

}