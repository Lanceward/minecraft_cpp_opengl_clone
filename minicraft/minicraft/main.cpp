
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
    Chunk c1 = Chunk(0, 0, 0);
    Chunk c2 = Chunk(1, 0, 0);
    float vert1[c1.getVLength()], vert2[c2.getVLength()];
    c1.getVertices(vert1, c1.getVLength());
    c2.getVertices(vert2, c2.getVLength());
    int indi1[c1.getILength()], indi2[c2.getILength()];
    c1.getIndices(indi1, c1.getILength());
    c2.getIndices(indi2, c2.getILength());

    
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    
    std::cout << sizeof(vert1) << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert1), vert1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indi1), indi1, GL_STATIC_DRAW);
    //coord attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
    // buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    // keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
    // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
    // it's not directly necessary.
    glBindVertexArray(0);
    
    
    // Uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //std::cout << texture2 << texture1 << std::endl;
    shaderProgram.use();
    texture1.bind(GL_TEXTURE0);
    shaderProgram.setInt("texture1", 0);
    camera.SetCMType(CONSTRICED_FLAT);
    
    //projection matrix
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)(SCR_WIDTH)/SCR_HEIGHT, 0.1f, 100.0f);
    unsigned int modelLoc, viewLoc, projectionLoc;
    modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
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
        glBindVertexArray(VAO);
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, c1.getILength(), GL_UNSIGNED_INT, 0);
        
        
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
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
