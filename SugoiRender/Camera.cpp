#include "Camera.h"

namespace sr {
Camera::Camera() : Camera(glm::vec3()) {}

Camera::Camera(const Camera& other) :
	m_pos{ other.m_pos },
	m_worldUp{ other.m_worldUp },
	m_pitch{ other.m_pitch },
	m_yaw{ other.m_yaw },
	m_front{ other.m_front },
	m_right{ other.m_right },
	m_up{ other.m_up },
	m_movementSpeed{ other.m_movementSpeed },
	m_mouseSensitivity{ other.m_mouseSensitivity },
	m_zoom{ other.m_zoom },
	m_aspect{ other.m_aspect },
	m_near{ other.m_near },
	m_far{ other.m_far } {}

Camera::Camera(const glm::vec3& pos) :
	m_pos{ pos },
	m_worldUp{ 0.0f, 1.0f, 0.0f },
	m_pitch{ PITCH },
	m_yaw{ YAW },
	m_movementSpeed{ SPEED },
	m_mouseSensitivity{ SENSITIVITY },
	m_zoom{ ZOOM },
	m_aspect{ ASPECT },
	m_near{ NEAR },
	m_far{ FAR }
{
	updateVectors();
}

Camera& Camera::operator=(const Camera& other) {
	if (this != &other) {
		m_pos = other.m_pos;
		m_worldUp = other.m_worldUp;
		m_pitch = other.m_pitch;
		m_yaw = other.m_yaw;
		m_front = other.m_front;
		m_right = other.m_right;
		m_up = other.m_up;
		m_movementSpeed = other.m_movementSpeed;
		m_mouseSensitivity = other.m_mouseSensitivity;
		m_zoom = other.m_zoom;
		m_aspect = other.m_aspect;
		m_near = other.m_near;
		m_far = other.m_far;
	}
	return *this;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_pos, m_pos + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return glm::perspective(glm::radians(GetZoom()), m_aspect, m_near, m_far);
}

Camera& Camera::operator+=(const glm::vec3& delta) {
    m_pos += delta;
    return *this;
}

Camera& Camera::operator-=(const glm::vec3& delta) {
    m_pos -= delta;
    return *this;
}

FrustumResult Camera::CubeInFrustum(glm::vec3 center, float x, float y, float z) {
	FrustumResult result = FrustumResult::Inside;

	for (int i = 0; i < m_planes.size(); ++i)
	{
		// Reset counters for corners in and out
		int out = 0;
		int in = 0;

		if (m_planes[i].GetDistance(center + glm::vec3(-x, -y, -z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(x, -y, -z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(-x, -y, z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(x, -y, z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(-x, y, -z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(x, y, -z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(-x, y, z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		if (m_planes[i].GetDistance(center + glm::vec3(x, y, z)) < 0)
		{
			out++;
		}
		else
		{
			in++;
		}

		// If all corners are out
		if (!in)
		{
			return FrustumResult::Outside;
		}
		// If some corners are out and others are in	
		else if (out)
		{
			result = FrustumResult::Intersect;
		}
	}

	return result;
}

void Camera::SetWorldUp(glm::vec3 worldUp) {
    m_worldUp = worldUp;
    updateVectors();
}

void Camera::SetPitch(GLfloat pitch) {
    m_pitch = pitch;
    if(m_pitch > 89.0f) {
        m_pitch = 89.0f;
    } else if(m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }
    updateVectors();
}

void Camera::SetYaw(GLfloat yaw) {
    m_yaw = yaw;
    updateVectors();
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

void Camera::updateVectors() {
    // Recalculate front vector
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    tempFront.y = sin(glm::radians(m_pitch));
    tempFront.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    m_front = glm::normalize(tempFront);

    // Recalculate right and up vectors
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));

	updateFrustum();
}

void Camera::updateFrustum() {

	glm::vec3 nc = m_pos + m_front * m_near;
	glm::vec3 fc = m_pos + m_front * m_far;

	float tang = (float)glm::tan(glm::radians(m_zoom) * 0.5);
	float nearHeight = m_near * tang;
	float nearWidth = nearHeight * m_aspect;
	float farHeight = m_far * tang;
	float farWidth = farHeight * m_aspect;

	glm::vec3 nearTopLeft = nc + m_up * nearHeight - m_right * nearWidth;
	glm::vec3 nearTopRight = nc + m_up * nearHeight + m_right * nearWidth;
	glm::vec3 nearBottomLeft = nc - m_up * nearHeight - m_right * nearWidth;
	glm::vec3 nearBottomRight = nc - m_up * nearHeight + m_right * nearWidth;

	glm::vec3 farTopLeft = fc + m_up * farHeight - m_right * farWidth;
	glm::vec3 farTopRight = fc + m_up * farHeight + m_right * farWidth;
	glm::vec3 farBottomLeft = fc - m_up * farHeight - m_right * farWidth;
	glm::vec3 farBottomRight = fc - m_up * farHeight + m_right * farWidth;

	m_planes[FRUSTOM_TOP] = sm::Plane3D(nearTopRight, nearTopLeft, farTopLeft);
	m_planes[FRUSTOM_BOTTOM] = sm::Plane3D(nearBottomLeft, nearBottomRight, farBottomRight);
	m_planes[FRUSTOM_LEFT] = sm::Plane3D(nearTopLeft, nearBottomLeft, farBottomLeft);
	m_planes[FRUSTOM_RIGHT] = sm::Plane3D(nearBottomRight, nearTopRight, farBottomRight);
	m_planes[FRUSTOM_NEAR] = sm::Plane3D(nearTopLeft, nearTopRight, nearBottomRight);
	m_planes[FRUSTOM_FAR] = sm::Plane3D(farTopRight, farTopLeft, farBottomLeft);
}

}