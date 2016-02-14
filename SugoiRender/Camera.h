#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SugoiMath\Plane3D.h>
#include <array>

// Default camera values
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

	glm::vec3 GetRelativePosition() const { return m_position; }
	glm::vec3 GetWorldPosition() const { return m_worldPosition + m_position; }
	glm::vec3 GetFront() const { return m_front; }
	glm::vec3 GetRight() const { return m_right; }
	glm::vec3 GetWorldUp() const { return m_worldUp; }
	GLfloat GetZoom() const { return m_zoom; }

	// Frustum
	FrustumResult CubeInFrustum(glm::vec3 center, float x, float y, float z);

	// Mutable view attributes
	void SetRelativePosition(glm::vec3 position) { m_position = position; }
	void SetWorldPosition(glm::vec3 position);
	void SetWorldUp(glm::vec3 worldUp);
	
	void SetRelativeDistanceFromPoint(glm::vec3 target, GLfloat dist);
	void LookAt(glm::vec3 target);
	void RotateAroundPoint(glm::vec3 target, GLfloat pitchDelta, GLfloat yawDelta);

	// Mutable input options
    void SetZoom(GLfloat zoom);

	// Mutable projection options
	void SetAspectRatio(GLfloat aspect);
	void SetNearPlane(GLfloat near);
	void SetFarPlane(GLfloat far);

private:

    // View attributes
    glm::vec3 m_position;
	glm::vec3 m_worldPosition;
    glm::vec3 m_worldUp;

	// Derived view attributes
	glm::vec3 m_front;
	glm::vec3 m_right;
	glm::vec3 m_up;

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
