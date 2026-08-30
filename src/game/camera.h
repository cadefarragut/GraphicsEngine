#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

namespace CameraDefaults {
    constexpr float Yaw = -90.0f;
    constexpr float Pitch = 0.0f;
    constexpr float Speed = 9.0f;
    constexpr float Sensitivity = 0.1f;
    constexpr float Zoom = 45.0f;
}


class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float Yaw = CameraDefaults::Yaw;
    float Pitch = CameraDefaults::Pitch;
    float MovementSpeed = CameraDefaults::Speed;
    float MouseSensitivity = CameraDefaults::Sensitivity;
    float Zoom = CameraDefaults::Zoom;

    Camera(glm::vec3 position,
           float yaw = CameraDefaults::Yaw,
           float pitch = CameraDefaults::Pitch,
           float speed = CameraDefaults::Speed,
           float sensitivity = CameraDefaults::Sensitivity,
           float zoom = CameraDefaults::Zoom);

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};
#endif