#include "Camera.h"

namespace sr {
Camera::Camera() : Camera(glm::vec3()) {}

Camera::Camera(const glm::vec3& pos) :
	pos{ pos },
	worldUp{ 0.0f, 1.0f, 0.0f },
	pitch{ PITCH },
	yaw{ YAW },
	movementSpeed{ SPEED },
	mouseSensitivity{ SENSITIVITY },
	zoom{ ZOOM },
	aspect{ ASPECT },
	near{ NEAR },
	far{ FAR }
{
	updateVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return glm::perspective(glm::radians(GetZoom()), this->aspect, this->near, this->far);
}

Camera& Camera::operator+=(const glm::vec3& delta) {
    this->pos += delta;
    return *this;
}

Camera& Camera::operator-=(const glm::vec3& delta) {
    this->pos -= delta;
    return *this;
}

glm::vec3 Camera::GetPosition() const {
    return this->pos;
}

glm::vec3 Camera::GetFront() const {
    return this->front;
}

glm::vec3 Camera::GetRight() const {
    return this->right;
}

glm::vec3 Camera::GetWorldUp() const {
    return this->worldUp;
}

GLfloat Camera::GetPitch() const {
    return this->pitch;
}

GLfloat Camera::GetYaw() const {
    return this->yaw;
}

GLfloat Camera::GetMovementSpeed() const {
    return this->movementSpeed;
}

GLfloat Camera::GetMouseSensitivity() const {
    return this->mouseSensitivity;
}

GLfloat Camera::GetZoom() const {
    return this->zoom;
}

void Camera::SetPosition(glm::vec3 pos) {
    this->pos = pos;
}

void Camera::SetWorldUp(glm::vec3 up) {
    this->worldUp = worldUp;
    updateVectors();
}

void Camera::SetPitch(GLfloat pitch) {
    this->pitch = pitch;
    if(this->pitch > 89.0f) {
        this->pitch = 89.0f;
    } else if(this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }
    updateVectors();
}

void Camera::SetYaw(GLfloat yaw) {
    this->yaw = yaw;
    updateVectors();
}

void Camera::SetMovementSpeed(GLfloat movementSpeed) {
    this->movementSpeed = movementSpeed;
}

void Camera::SetMouseSensitivity(GLfloat mouseSensitivity) {
    this->mouseSensitivity = mouseSensitivity;
}

void Camera::SetZoom(GLfloat zoom) {
    this->zoom = zoom;
    if(this->zoom <= 1.0f) {
        this->zoom = 1.0f;
    }
    if(this->zoom >= 45.0f) {
        this->zoom = 45.0f;
    }
}

void Camera::SetAspectRatio(GLfloat aspect) {
	this->aspect = aspect;
}

void Camera::SetNearPlane(GLfloat near) {
	this->near = near;
}

void Camera::SetFarPlane(GLfloat far) {
	this->far = far;
}

void Camera::updateVectors() {
    // Recalculate front vector
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    tempFront.y = sin(glm::radians(this->pitch));
    tempFront.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->front = glm::normalize(tempFront);

    // Recalculate right and up vectors
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
}