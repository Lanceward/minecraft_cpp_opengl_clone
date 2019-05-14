//
//  ChunkBuffer.cpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/14/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "ChunkBuffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

ChunkBuffer::ChunkBuffer(Chunk *c)
{
    C = c;
    glGenVertexArrays(1, &VAOid);
    glGenBuffers(1, &VBOid);
    glGenBuffers(1, &EBOid);
    init();
}

ChunkBuffer::~ChunkBuffer()
{
    glDeleteVertexArrays(1, &VAOid);
    glDeleteBuffers(1, &VBOid);
    glDeleteBuffers(1, &EBOid);
}

void ChunkBuffer::init()
{
    float v[C->getVLength()];
    C->getVertices(v, (int)sizeof(v)/sizeof(v[0]));
    int i[C->getILength()];
    C->getIndices(i, (int)sizeof(i)/sizeof(i[0]));
    
    
    glBindVertexArray(VAOid);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v ,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOid);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(i), i, GL_STATIC_DRAW);
    cout << C->getVLength() << " " << C->getILength() << endl;
    //coord attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
    // buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO;
    // keep the EBO bound.
    glBindVertexArray(0);
}

void ChunkBuffer::bind()
{
    glBindVertexArray(VAOid);
}

void ChunkBuffer::draw()
{
    glDrawElements(GL_TRIANGLES, C->getILength(), GL_UNSIGNED_INT, 0);
}

unsigned int ChunkBuffer::getVAO()
{
    return VAOid;
}

unsigned int ChunkBuffer::getEBO()
{
    return EBOid;
}

unsigned int ChunkBuffer::getVBO()
{
    return VBOid;
}

Chunk* ChunkBuffer::getChunk()
{
    return C;
}
