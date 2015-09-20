#include "Camera.h"

namespace sr {
Camera::Camera(glm::vec3 pos) : 
    pos(pos),
    up(),
    front(),
    right(),
    worldUp(0.0f, 1.0f, 0.0f),
    pitch(PITCH),
    yaw(YAW),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM) {

    updateVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

Camera& Camera::operator+=(const glm::vec3& delta) {
    this->pos += delta;
    return *this;
}

Camera& Camera::operator-=(const glm::vec3& delta) {
    this->pos -= delta;
    return *this;
}

glm::vec3 Camera::GetPosition() {
    return this->pos;
}

glm::vec3 Camera::GetFront() {
    return this->front;
}

glm::vec3 Camera::GetRight() {
    return this->right;
}

glm::vec3 Camera::GetWorldUp() {
    return this->worldUp;
}

GLfloat Camera::GetPitch() {
    return this->pitch;
}

GLfloat Camera::GetYaw() {
    return this->yaw;
}

GLfloat Camera::GetMovementSpeed() {
    return this->movementSpeed;
}

GLfloat Camera::GetMouseSensitivity() {
    return this->mouseSensitivity;
}

GLfloat Camera::GetZoom() {
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

void Camera::updateVectors() {
    // Recalculate front vector
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    tempFront.y = sin(glm::radians(this->pitch));
    tempFront.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    front = glm::normalize(tempFront);

    // Recalculate right and up vectors
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
}