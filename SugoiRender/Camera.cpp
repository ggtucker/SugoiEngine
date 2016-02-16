#include "Camera.h"

namespace sr {
Camera::Camera() : Camera(glm::vec3()) {}

Camera::Camera(const Camera& other) :
	m_transform{ other.m_transform },
	m_zoom{ other.m_zoom },
	m_aspect{ other.m_aspect },
	m_near{ other.m_near },
	m_far{ other.m_far } {}

Camera::Camera(const glm::vec3& pos) :
	m_zoom{ ZOOM },
	m_aspect{ ASPECT },
	m_near{ NEAR },
	m_far{ FAR }
{
}

Camera& Camera::operator=(const Camera& other) {
	if (this != &other) {
		m_transform = other.m_transform;
		m_zoom = other.m_zoom;
		m_aspect = other.m_aspect;
		m_near = other.m_near;
		m_far = other.m_far;
	}
	return *this;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_transform.position, m_transform.position + m_transform.forward, m_transform.up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return glm::perspective(glm::radians(m_zoom), m_aspect, m_near, m_far);
}

Camera& Camera::operator+=(const glm::vec3& delta) {
    m_transform.position += delta;
    return *this;
}

Camera& Camera::operator-=(const glm::vec3& delta) {
    m_transform.position -= delta;
    return *this;
}

sm::Collision Camera::CubeInFrustum(glm::vec3 center, float x, float y, float z) {
	return m_frustum.CubeInRegion(center, glm::vec3(x, y, z));
}

void Camera::SetDistanceFromPoint(glm::vec3 target, GLfloat dist) {
	m_transform.SetDistanceFromPoint(target, dist);
	updateFrustum();
}

void Camera::LookAt(glm::vec3 target) {
	m_transform.LookAt(target);
	updateFrustum();
}

void Camera::RotateAround(glm::vec3 target, GLfloat pitchDelta, GLfloat yawDelta) {
	GLfloat distance = glm::distance(m_transform.position, target);
	m_transform.RotateAround(target, m_transform.right, pitchDelta);
	m_transform.RotateAround(target, m_transform.up, yawDelta);
	SetDistanceFromPoint(target, distance);
}

void Camera::SetZoom(GLfloat zoom) {
    m_zoom = zoom;
    if(m_zoom <= 1.0f) {
        m_zoom = 1.0f;
    }
    if(m_zoom >= 45.0f) {
        m_zoom = 45.0f;
    }
}

void Camera::SetAspectRatio(GLfloat aspect) {
	m_aspect = aspect;
}

void Camera::SetNearPlane(GLfloat near) {
	m_near = near;
}

void Camera::SetFarPlane(GLfloat far) {
	m_far = far;
}

void Camera::updateFrustum() {

	glm::vec3 nc = m_transform.position + m_transform.forward * m_near;
	glm::vec3 fc = m_transform.position + m_transform.forward * m_far;

	float tang = (float)glm::tan(glm::radians(m_zoom) * 0.5);
	float nearHeight = m_near * tang;
	float nearWidth = nearHeight * m_aspect;
	float farHeight = m_far * tang;
	float farWidth = farHeight * m_aspect;

	glm::vec3 nearTopLeft = nc + m_transform.up * nearHeight - m_transform.right * nearWidth;
	glm::vec3 nearTopRight = nc + m_transform.up * nearHeight + m_transform.right * nearWidth;
	glm::vec3 nearBottomLeft = nc - m_transform.up * nearHeight - m_transform.right * nearWidth;
	glm::vec3 nearBottomRight = nc - m_transform.up * nearHeight + m_transform.right * nearWidth;

	glm::vec3 farTopLeft = fc + m_transform.up * farHeight - m_transform.right * farWidth;
	glm::vec3 farTopRight = fc + m_transform.up * farHeight + m_transform.right * farWidth;
	glm::vec3 farBottomLeft = fc - m_transform.up * farHeight - m_transform.right * farWidth;
	glm::vec3 farBottomRight = fc - m_transform.up * farHeight + m_transform.right * farWidth;

	m_frustum.Clear();
	m_frustum.AddPlane(sm::Plane3D(nearTopRight, nearTopLeft, farTopLeft)); // Top
	m_frustum.AddPlane(sm::Plane3D(nearBottomLeft, nearBottomRight, farBottomRight)); // Bottom
	m_frustum.AddPlane(sm::Plane3D(nearTopLeft, nearBottomLeft, farBottomLeft)); // Left
	m_frustum.AddPlane(sm::Plane3D(nearBottomRight, nearTopRight, farBottomRight)); // Right
	m_frustum.AddPlane(sm::Plane3D(nearTopLeft, nearTopRight, nearBottomRight)); // Near
	m_frustum.AddPlane(sm::Plane3D(farTopRight, farTopLeft, farBottomLeft)); // Far
}

}