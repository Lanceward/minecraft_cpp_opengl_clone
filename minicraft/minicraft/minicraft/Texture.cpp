//
//  Texture.cpp
//  testopengl
//
//  Created by Lianfeng Shi on 5/4/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "Texture.h"
#include <stb_image.h>

Texture::Texture(std::string texturePath)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    load(texturePath);
}

void Texture::load(std::string textureName)
{
    //Currently only support RGBA
    glBindTexture(GL_TEXTURE_2D, ID);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(("./textures/"+textureName).c_str(), &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned int GlTextureNum)
{
    if(GlTextureNum > GL_TEXTURE0+15)
    {
        std::cout << "ERROR::Opengl don't support this many textures" << std::endl;
    }
    else
    {
        glActiveTexture(GlTextureNum);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}
