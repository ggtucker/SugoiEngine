#pragma once

#include <glm/glm.hpp>

// Default camera values
const GLfloat YAW         = -90.0f;
const GLfloat PITCH       =  0.0f;
const GLfloat SPEED       =  3.0f;
const GLfloat SENSITIVITY =  0.05f;
const GLfloat ZOOM        =  45.0f;

class Camera {
public:
    Camera(glm::vec3 pos);

    glm::mat4 GetViewMatrix();

    Camera& operator+=(const glm::vec3& delta);
    Camera& operator-=(const glm::vec3& delta);

    glm::vec3 GetPosition();
    glm::vec3 GetFront();
    glm::vec3 GetRight();
    glm::vec3 GetWorldUp();
    GLfloat GetPitch();
    GLfloat GetYaw();
    GLfloat GetMovementSpeed();
    GLfloat GetMouseSensitivity();
    GLfloat GetZoom();
    void SetPosition(glm::vec3 pos);
    void SetWorldUp(glm::vec3 up);
    void SetPitch(GLfloat pitch);
    void SetYaw(GLfloat yaw);
    void SetMovementSpeed(GLfloat movementSpeed);
    void SetMouseSensitivity(GLfloat mouseSensitivity);
    void SetZoom(GLfloat zoom);
private:
    // Camera attributes
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 worldUp;
    // Euler angles
    GLfloat pitch;
    GLfloat yaw;
    // Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    void updateVectors();
};

Camera::Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f)) : 
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