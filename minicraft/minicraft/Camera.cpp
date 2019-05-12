//
//  Camera.cpp
//  testopengl
//
//  Created by Lianfeng Shi on 5/4/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

void Camera::SetCMType(CM_Type type)
{
    Type = type;
}


glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position+Front, WorldUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    glm::vec3 MoveFront;
    switch (Type) {
        case UNCONSTRICED:
            MoveFront = Front;
            break;
        case CONSTRICED_FLAT:
            MoveFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
            break;
        default:
            MoveFront = Front;
    }
        
    float displacement = deltaTime * MovementSpeed;
    switch(direction)
    {
        case FORWARD:
            Position += MoveFront * displacement;
            break;
        case BACKWARD:
            Position -= MoveFront * displacement;
            break;
        case LEFTWARD:
            Position -= Right * displacement;
            break;
        case RIGHTWARD:
            Position += Right * displacement;
            break;
        case UPWARD:
            Position += WorldUp * displacement;
            break;
        case DOWNWRD:
            Position -= WorldUp * displacement;
            break;
        default:
            std::cout << "Enum has some problem" << std::endl;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;
    
    Yaw += xoffset;
    Pitch += yoffset;
    
    if(Pitch > 89.0f)
        Pitch = 89.0f;
    if(Pitch < -89.0f)
        Pitch = -89.0f;
    if(Yaw > 360.0f)
        Yaw -= 360;
    if(Yaw < -360.0f)
        Yaw += 360;
    
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;

    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    //std::cout << front.x << " " << front.y << " " << front.z << std::endl;
    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
}
