//
//  Texture.h
//  testopengl
//
//  Created by Lianfeng Shi on 5/4/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef Texture_h
#define Texture_h


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <string.h>

class Texture
{
public:
    unsigned int ID;
    
    Texture(std::string textureName);
    void load(std::string textureName);
    void bind(unsigned int GlTextureNum);
    void bind();
    
};

#endif /* Texture_h */

