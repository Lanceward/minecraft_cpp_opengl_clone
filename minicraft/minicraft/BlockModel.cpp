//
//  block.cpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/8/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "Block.h"
#include <iostream>
using namespace std;

Block::Block(blockType t)
{
    btype = t;
    genVertices(vertices);
}

blockType Block::getType()
{
    return btype;
}

void Block::getIndices(int indi[])
{
    for(int i = 0; i<36; i++)
        indi[i] = indices_m[i];
}

void Block::getVertices(float vt[])
{
    for(int i = 0; i < 120; i++)
        vt[i] = vertices[i];
}

void Block::genVertices(float vertf[])
{
    //float vertf[120];
    //give bottom texture coord
    for(int v = 0; v < 4; v++)
    {
        for(int d = 0; d < 3; d++)
        {
            vertf[v*5+d] = (float)vertecies_m[v*3+d];
            //cout << vertf[v*5+d] << endl;
        }
        vertf[v*5+4] = (float)(32+(1-v/2)*16)/48;
        vertf[v*5+3] = (float)(btype*16+v%2*16)/48;
        //cout << vertf[v*5+3] << endl << vertf[v*5+4] << endl;
    }
    //give top texture coord
    for(int v = 0; v < 4; v++)
    {
        for(int d = 0; d < 3; d++)
        {
            vertf[20+v*5+d] = (float)vertecies_m[12+v*3+d];
            //cout << vertf[20+v*5+d] << endl;
        }
        vertf[20+v*5+4] = (float)((1-v/2)*16)/48;
        vertf[20+v*5+3] = (float)(btype*16+v%2*16)/48;
        //cout << vertf[20+v*5+3] << endl << vertf[20+v*5+4] << endl;
    }
    
    for(int s = 2; s<6; s++)
    {
        for(int v = 0; v < 4; v++)
        {
            for(int d = 0; d < 3; d++)
                vertf[s*20+v*5+d] = vertecies_m[s*12+v*3+d];
            vertf[s*20+v*5+4] = (float)(16+(1-v/2)*16)/48;
            vertf[s*20+v*5+3] = (float)(btype*16+v%2*16)/48;
        }
    }
}
