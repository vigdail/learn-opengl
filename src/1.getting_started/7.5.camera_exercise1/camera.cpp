#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CameraExersice1
{

    FPSCamera::FPSCamera() : position(glm::vec3(0.0f, 0.0f, 0.0f)), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                             yaw(YAW), pitch(PITCH), speed(SPEED), sensitivity(SENSITIVITY), fov(FOV)
    {
        updateVectors();
    }

    FPSCamera::FPSCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : position(position), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up),
                                                                                     yaw(yaw), pitch(pitch), speed(SPEED), sensitivity(SENSITIVITY), fov(FOV)
    {
        updateVectors();
    }

    glm::mat4 FPSCamera::getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void FPSCamera::move(CameraMovement direction, float dt)
    {
        switch (direction)
        {
        case CameraMovement::FORWARD:
            position += front * speed * dt;
            break;
        case CameraMovement::BACKWARD:
            position -= front * speed * dt;
            break;
        case CameraMovement::LEFT:
            position -= right * speed * dt;
            break;
        case CameraMovement::RIGHT:
            position += right * speed * dt;
            break;
        default:
            break;
        }

        position.y = 0.0f;
    }
    void FPSCamera::handleMouseMovement(float dx, float dy, bool contrainPitch)
    {

        const float sensetivity = 0.1f;
        dx *= sensetivity;
        dy *= sensetivity;

        yaw += dx;
        pitch += dy;

        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
        if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }

        updateVectors();
    }

    void FPSCamera::handleMouseScroll(float dy)
    {
        fov -= dy;

        if (fov > 45.0f)
        {
            fov = 45.0f;
        }
        if (fov < 1.0f)
        {
            fov = 1.0f;
        }
    }

    void FPSCamera::updateVectors()
    {
        glm::vec3 _front;
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(_front);

        right = glm::cross(front, worldUp);
        up = glm::normalize(glm::cross(right, front));
    }

} // namespace CameraExersice1
