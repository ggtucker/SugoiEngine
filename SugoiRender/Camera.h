#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// Default camera values
const static GLfloat YAW         = -90.0f;
const static GLfloat PITCH       =  0.0f;
const static GLfloat SPEED       =  3.0f;
const static GLfloat SENSITIVITY =  0.05f;
const static GLfloat ZOOM        =  45.0f;

class Camera {
public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));

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

#endif