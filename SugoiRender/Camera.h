#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SugoiMath\Plane3D.h>
#include <array>

// Default camera values
const static GLfloat YAW         = -90.0f;
const static GLfloat PITCH       =  0.0f;
const static GLfloat SPEED       =  3.0f;
const static GLfloat SENSITIVITY =  0.05f;
const static GLfloat ZOOM        =  45.0f;
const static GLfloat ASPECT      =  4.0f / 3.0f;
const static GLfloat NEAR        =  0.1f;
const static GLfloat FAR         =  100.0f;

enum class FrustumResult {
	Outside = 0,
	Intersect,
	Inside
};

namespace sr {
class Camera {
public:
	Camera();
	Camera(const Camera& other);
    explicit Camera(const glm::vec3& pos);

	Camera& operator=(const Camera& other);

    glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

    Camera& operator+=(const glm::vec3& delta);
    Camera& operator-=(const glm::vec3& delta);

	glm::vec3 GetPosition() const { return m_pos; }
	glm::vec3 GetFront() const { return m_front; }
	glm::vec3 GetRight() const { return m_right; }
	glm::vec3 GetWorldUp() const { return m_worldUp; }
	GLfloat GetPitch() const { return m_pitch; }
	GLfloat GetYaw() const { return m_yaw; }
	GLfloat GetMovementSpeed() const { return m_movementSpeed; }
	GLfloat GetMouseSensitivity() const { return m_mouseSensitivity; }
	GLfloat GetZoom() const { return m_zoom; }

	// Frustum
	FrustumResult CubeInFrustum(glm::vec3 center, float x, float y, float z);

	// Mutable view attributes
	void SetPosition(glm::vec3 pos) { m_pos = pos; }
	void SetWorldUp(glm::vec3 worldUp);
	void SetPitch(GLfloat pitch);
	void SetYaw(GLfloat yaw);

	// Mutable input options
	void SetMovementSpeed(GLfloat movementSpeed) { m_movementSpeed = movementSpeed; }
	void SetMouseSensitivity(GLfloat mouseSensitivity) { m_mouseSensitivity = mouseSensitivity; }
    void SetZoom(GLfloat zoom);

	// Mutable projection options
	void SetAspectRatio(GLfloat aspect);
	void SetNearPlane(GLfloat near);
	void SetFarPlane(GLfloat far);

private:
    // View attributes
    glm::vec3 m_pos;
    glm::vec3 m_worldUp;
	GLfloat m_pitch;
	GLfloat m_yaw;

	// Derived view attributes
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;

    // Input options
    GLfloat m_movementSpeed;
    GLfloat m_mouseSensitivity;

	// Projection options
	GLfloat m_aspect;
	GLfloat m_zoom;
	GLfloat m_near;
	GLfloat m_far;

	// Frustum planes
	enum {
		FRUSTOM_TOP = 0,
		FRUSTOM_BOTTOM,
		FRUSTOM_LEFT,
		FRUSTOM_RIGHT,
		FRUSTOM_NEAR,
		FRUSTOM_FAR
	};
	std::array<sm::Plane3D, 6> m_planes;

    void updateVectors();
	void updateFrustum();
};
}
