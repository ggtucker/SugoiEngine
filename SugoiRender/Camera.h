#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SugoiMath\Plane3D.h>
#include <SugoiMath\Transform.h>
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

	Transform GetTransform() const { return m_transform; }
	glm::vec3 GetPosition() const { return m_transform.position; }
	glm::vec3 GetForward() const { return m_transform.forward; }
	glm::vec3 GetRight() const { return m_transform.right; }
	glm::vec3 GetUp() const { return m_transform.up; }
	glm::vec3 GetWorldUp() const { return m_worldUp; }
	GLfloat GetZoom() const { return m_zoom; }

	// Frustum
	FrustumResult CubeInFrustum(glm::vec3 center, float x, float y, float z);

	void SetTransform(const Transform& transform) { m_transform = transform; }
	void SetPosition(glm::vec3 position) { m_transform.position = position; }
	
	// View attributes
	void LookAt(glm::vec3 target);
	void RotateAround(glm::vec3 target, GLfloat pitchDelta, GLfloat yawDelta);
	void SetDistanceFromPoint(glm::vec3 target, GLfloat dist);
    void SetZoom(GLfloat zoom);
	void SetPitch(GLfloat pitch);
	void SetYaw(GLfloat yaw);

	// Mutable projection options
	void SetAspectRatio(GLfloat aspect);
	void SetNearPlane(GLfloat near);
	void SetFarPlane(GLfloat far);

private:

	Transform m_transform;
	glm::vec3 m_worldUp;

	// Projection options
	GLfloat m_aspect;
	GLfloat m_zoom;
	GLfloat m_near;
	GLfloat m_far;

	// Rotation
	GLfloat m_pitch;
	GLfloat m_yaw;

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
