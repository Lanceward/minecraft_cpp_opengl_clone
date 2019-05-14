
//
//  main.cpp
//  testopengl
//
//  Created by Lianfeng Shi on 4/29/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include "Block.h"
#include "Chunk.h"
#include "ChunkBuffer.h"

//funcs
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processinput_movepov(GLFWwindow* window, double timeDifference);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

double lastX = 400;
double lastY = 300;
bool firstMouse = true;


Camera camera = Camera();


// Define main function
int main()
{
    //Initialize everything
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    
    //Create window and check if succeed
    //----------------------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //Set the current context to *window* and check if succeed
    //--------------------------------------------------------
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //Set viewpoint, make it change when the window size change
    //---------------------------------------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);
    //END OF WINDOW SETUP
    
    
    //Input Setup
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    //START OF SHADER, DATA AND STUFFS
    
    //Build and compile our shaders and shader program
    Shader shaderProgram = Shader("./shaders/vertexShader.vs", "./shaders/fragmentShader.fs");
    
    // textures generations
    //---------------------
    Texture texture1 = Texture("texture_basic.png");
    
    
    //World Generation
    //----------------
    const int cnumber = 4;
    
    Chunk c[cnumber] = {Chunk(0, 0, 0), Chunk(1, 0, 0), Chunk(0, 0, 1), Chunk(1, 0, 1)};
    
    ChunkBuffer *cBuffer[cnumber];
    for(int i = 0; i < cnumber; i++)
        cBuffer[i] = new ChunkBuffer(&c[i]);

    shaderProgram.use();
    texture1.bind(GL_TEXTURE0);
    shaderProgram.setInt("texture1", 0);
    camera.SetCMType(CONSTRICED_FLAT);

    unsigned int modelLoc, viewLoc, projectionLoc;
    modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
    
    //projection matrix
    glm::mat4 projection;
    glm::mat4 model;

    projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH)/SCR_HEIGHT, 0.1f, 100.0f);
    model = glm::mat4(1.0f);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //Looping time!
    double previousTime = glfwGetTime();
    while(!glfwWindowShouldClose(window))
    {
        //Input
        processinput_movepov(window, glfwGetTime()-previousTime);
        previousTime = glfwGetTime();
        
        //Rendering code here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderProgram.use();
        texture1.bind(GL_TEXTURE0);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        
        for(int b = 0; b < cnumber; b++)
        {
            std::cout << cBuffer[b]->getChunk()->wpos_x*16.0f << " " << cBuffer[b]->getChunk()->wpos_y*16.0f << " " << cBuffer[b]->getChunk()->wpos_z*16.0f << std::endl;
            
            model = glm::translate(glm::mat4(1.0f), glm::vec3(cBuffer[b]->getChunk()->wpos_x*16.0f, cBuffer[b]->getChunk()->wpos_y*16.0f, cBuffer[b]->getChunk()->wpos_z*16.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            //std::cout << cBuffer[b]->getChunk()->getVLength() << " " << cBuffer[b]->getChunk()->getILength() << std::endl;
            cBuffer[b]->bind();
            cBuffer[b]->draw();
        }
        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //terminate and clear all the craps
    glfwTerminate();
    
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void processinput_movepov(GLFWwindow* window, double timeDifference)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, timeDifference);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, timeDifference);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFTWARD, timeDifference);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHTWARD, timeDifference);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UPWARD, timeDifference);
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWNWRD, timeDifference);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}
