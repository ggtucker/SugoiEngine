#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform {
public:
	/* PUBLIC FUNCTIONS */

	Transform();

	void SetForward(glm::vec3 direction);

	void LookAt(glm::vec3 target);
	void RotateAround(glm::vec3 target, glm::vec3 axis, float angle);
	void SetDistanceFromPoint(glm::vec3 target, float distance);

	/* PUBLIC MEMBERS */

	// Transform
	glm::vec3 position;
	glm::vec3 scale;
	glm::quat rotation;

	// Direction
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
};