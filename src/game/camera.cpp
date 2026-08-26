#include "camera.h"
#include "../Config.h"

Camera::Camera(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity, float zoom)
{
    this->Position = position;
    this->Front = glm::vec3(0.0f, 0.0f, -3.0f);
    this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->MovementSpeed = speed;
    this->MouseSensitivity = sensitivity;
    this->Zoom = zoom;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() {
    float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    return glm::perspective(glm::radians(Zoom), aspect, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 flatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 flatRight = glm::normalize(glm::cross(flatFront, glm::vec3(0.0f, 1.0f, 0.0f)));

    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front.x * velocity;
    if (direction == BACKWARD)
        Position -= Front.x * velocity;
    if (direction == LEFT)
        Position -= Front.y * velocity;
    if (direction == RIGHT)
        Position += Front.y * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}



void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 90.0f)   
        Zoom = 90.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

