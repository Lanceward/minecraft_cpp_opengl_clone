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
    genVertices();
}

blockType Block::getType() const
{
    return btype;
}

void Block::getIndices(int indi[]) const
{
    for(int i = 0; i<36; i++)
        indi[i] = indices_m[i];
}

void Block::getVertices(float vt[]) const
{
    for(int v = 0; v < 24; v++)
        for(int d = 0; d < 5; d++)
            vt[v*5+d] = vertices[v][d];
}

void Block::getVertices(float vt[][5]) const
{
    for(int v = 0; v < 24; v++)
        for(int d = 0; d < 5; d++)
            vt[v][d] = vertices[v][d];
}

void Block::genVertices()
{
    int idx = btype - 1;
    vertices = new float[24][5];
    //float vertf[120];
    //give bottom texture coord
    for(int v = 0; v < 4; v++)
    {
        for(int d = 0; d < 3; d++)
        {
            vertices[v][d] = (float)vertecies_m[v][d];
            //cout << vertf[v*5+d] << endl;
        }
        vertices[v][4] = (float)(32+(1-v/2)*16)/48;
        vertices[v][3] = (float)(idx*16+v%2*16)/48;
        //cout << vertf[v*5+3] << endl << vertf[v*5+4] << endl;
    }
    //give top texture coord
    for(int v = 0; v < 4; v++)
    {
        for(int d = 0; d < 3; d++)
        {
            vertices[4+v][d] = (float)vertecies_m[4+v][d];
            //cout << vertf[20+v*5+d] << endl;
        }
        vertices[4+v][4] = (float)((1-v/2)*16)/48;
        vertices[4+v][3] = (float)(idx*16+v%2*16)/48;
        //cout << vertf[20+v*5+3] << endl << vertf[20+v*5+4] << endl;
    }
    
    for(int s = 2; s<6; s++)
    {
        for(int v = 0; v < 4; v++)
        {
            for(int d = 0; d < 3; d++)
                vertices[s*4+v][d] = vertecies_m[s*4+v][d];
            vertices[s*4+v][4] = (float)(16+(1-v/2)*16)/48;
            vertices[s*4+v][3] = (float)(idx*16+v%2*16)/48;
        }
    }
}
