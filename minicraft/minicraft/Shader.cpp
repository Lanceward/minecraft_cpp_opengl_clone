//
//  Shader.cpp
//  testopengl
//
//  Created by Lianfeng Shi on 5/2/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode = getShaderSourceCode(vertexPath);
    std::string fragmentCode = getShaderSourceCode(fragmentPath);
    
    //std::cout << vertexCode << std::endl << fragmentCode << std::endl;
    
    const char* vshaderCode = vertexCode.c_str();
    const char* fshaderCode = fragmentCode.c_str();
    unsigned int vid, fid;
    // vertex shader
    vid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vid, 1, &vshaderCode, NULL);
    glCompileShader(vid);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vid, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    fid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fid, 1, &fshaderCode, NULL);
    glCompileShader(fid);
    // check for compiling error
    glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vid, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link the two shaders
    ID = glCreateProgram();
    glAttachShader(ID, vid);
    glAttachShader(ID, fid);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vid);
    glDeleteShader(fid);
}

void Shader::use()
{
    glUseProgram(ID);
}
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::getShaderSourceCode(const char* filepath)
{
    std::string content;
    std::ifstream shaderFile;
    std::cout << filepath << std::endl;

    //shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(filepath);
        std::string line = "";
        while(!shaderFile.eof())
        {
            std::getline(shaderFile, line);
            content.append(line + "\n");
        }
        shaderFile.close();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return content;
}
