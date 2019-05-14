//
//  Camera.hpp
//  testopengl
//
//  Created by Lianfeng Shi on 5/4/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFTWARD,
    RIGHTWARD,
    UPWARD,
    DOWNWRD,
};

enum CM_Type {
    UNCONSTRICED,
    CONSTRICED_FLAT
};

const float YAW         =  0.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;



class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    CM_Type Type = UNCONSTRICED;
    // Euler Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    
    //funcs
    Camera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void SetCMType(CM_Type type);

private:
    void updateCameraVectors();
};

#endif /* Camera_hpp */
