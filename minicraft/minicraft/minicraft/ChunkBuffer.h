//
//  ChunkBuffer.hpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/14/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef ChunkBuffer_h
#define ChunkBuffer_h

#include "Chunk.h"

class ChunkBuffer
{
public:
    ChunkBuffer(Chunk *c);
    ~ChunkBuffer();
    void init();
    void bind();
    void draw();
    unsigned int getVAO();
    unsigned int getVBO();
    unsigned int getEBO();
    Chunk* getChunk();

private:
    Chunk *C;
    unsigned int VAOid, VBOid, EBOid;
};

#endif /* ChunkBuffer_h */
