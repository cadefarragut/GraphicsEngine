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
    SPACE
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float GRAVITY = 20.0f;

#define SCREEN_WIDTH 1920.0f
#define SCREEN_HEIGHT 1080.0f

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 flatFront;
    glm::vec3 flatRight;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float velocityY = 0.0f;
    float velocityX = 0.0f;
    bool onGround = false;
    bool isSprinting = false;

    Camera(glm::vec3 position);

    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};
#endif