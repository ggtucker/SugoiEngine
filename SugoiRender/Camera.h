#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// Default camera values
const static GLfloat YAW         = -90.0f;
const static GLfloat PITCH       =  0.0f;
const static GLfloat SPEED       =  3.0f;
const static GLfloat SENSITIVITY =  0.05f;
const static GLfloat ZOOM        =  45.0f;
const static GLfloat ASPECT      =  4.0f / 3.0f;
const static GLfloat NEAR        =  0.1f;
const static GLfloat FAR         =  100.0f;

namespace sr {
class Camera {
public:
	Camera();
	Camera(const Camera& other);
	Camera(Camera&& other);
    explicit Camera(const glm::vec3& pos);

	Camera& operator=(const Camera& other);
	Camera& operator=(Camera&& other);

    glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

    Camera& operator+=(const glm::vec3& delta);
    Camera& operator-=(const glm::vec3& delta);

    glm::vec3 GetPosition() const;
    glm::vec3 GetFront() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetWorldUp() const;
    GLfloat GetPitch() const;
    GLfloat GetYaw() const;
    GLfloat GetMovementSpeed() const;
    GLfloat GetMouseSensitivity() const;
    GLfloat GetZoom() const;

	// Mutable view attributes
    void SetPosition(glm::vec3 pos);
    void SetWorldUp(glm::vec3 up);
    void SetPitch(GLfloat pitch);
    void SetYaw(GLfloat yaw);

	// Mutable input options
    void SetMovementSpeed(GLfloat movementSpeed);
    void SetMouseSensitivity(GLfloat mouseSensitivity);
    void SetZoom(GLfloat zoom);

	// Mutable projection options
	void SetAspectRatio(GLfloat aspect);
	void SetNearPlane(GLfloat near);
	void SetFarPlane(GLfloat far);

private:
    // View attributes
    glm::vec3 pos;
    glm::vec3 worldUp;
	GLfloat pitch;
	GLfloat yaw;

	// Derived view attributes
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

    // Input options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

	// Projection options
	GLfloat aspect;
	GLfloat near;
	GLfloat far;

    void updateVectors();
};
}
