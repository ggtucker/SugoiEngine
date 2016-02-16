#include "Transform.h"

Transform::Transform() :
	position{ 0.0f, 0.0f, 0.0f },
	scale{ 1.0f, 1.0f, 1.0f },
	forward{ 0.0f, 0.0f, 1.0f },
	right{ 1.0f, 0.0f, 0.0f },
	up{ 0.0f, 1.0f, 0.0f } {}

void Transform::SetForward(glm::vec3 direction) {
	forward = glm::normalize(direction);
	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(right, forward));
}

void Transform::LookAt(glm::vec3 target) {
	SetForward(target - position);
}

void Transform::RotateAround(glm::vec3 target, glm::vec3 axis, float angle) {
	glm::vec4 camFocusVector = glm::vec4(glm::normalize(position - target), 1.0f);
	position = glm::vec3(glm::rotate(glm::mat4(), angle, axis) * camFocusVector) + target;
	LookAt(target);
}

void Transform::SetDistanceFromPoint(glm::vec3 target, float distance) {
	position = target - forward * distance;
}