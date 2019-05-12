//
//  block.hpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/8/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef Block_h
#define Block_h
#include "block_model.h"

class Block
{
    blockType btype;
    float (*vertices)[5];
    int *indices;
    
public:
    Block(blockType t);
    blockType getType() const;
    void getIndices(int indi[]) const;
    void getVertices(float vt[]) const;
    void getVertices(float vt[][5]) const;
    void genVertices();

};

#endif /* block_h */
