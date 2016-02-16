#include "Region3D.h"

namespace sm {

CollisionResult Region3D::PointInRegion(glm::vec3 point) {

	for (int i = 0; i < NumPlanes(); ++i) {
		Plane3D plane = GetPlane(i);
		float distance = plane.GetDistance(point);

		if (distance < 0.0f) {
			return CollisionResult::Outside;
		}
	}
	
	return CollisionResult::Inside;
}

CollisionResult Region3D::CubeInRegion(glm::vec3 center, glm::vec3 halfSize) {
	CollisionResult result = CollisionResult::Inside;

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
			return CollisionResult::Outside;
		}
		// If some corners are outside and others are inside	
		else if (out) {
			result = CollisionResult::Intersect;
		}
	}

	return result;
}

}