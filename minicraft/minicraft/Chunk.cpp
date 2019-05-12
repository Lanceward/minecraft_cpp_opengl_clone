//
//  Chunk.cpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/8/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#include "Chunk.h"
#include <iostream>

int count = 0;
bool past = false;

Block Chunk::bmodels[4] = {Block(VOID), Block(GRASS), Block(WOOD), Block(ROCK)};

Chunk::Chunk(int px, int py, int pz)
{
    wpos_x = px;
    wpos_y = py;
    wpos_z = pz;
    load();
}

Chunk::Chunk(int px, int py, int pz, int blockdatas[16][16][16])
{
    wpos_x = px;
    wpos_y = py;
    wpos_z = pz;
    load(blockdatas);
}

void Chunk::load()
{
    for(int x = 0; x < 16; x++)
    {
        for(int z = 0; z < 16; z++)
        {
            blocksData[x][0][z] = 3;
        }
    }
    for(int x = 0; x < 16; x++)
    {
        for(int z = 0; z < 16; z++)
        {
            blocksData[x][1][z] = 1;
        }
    }
    for(int x = 0; x < 16; x++)
    {
        for(int z = 0; z < 16; z++)
        {
            blocksData[x][2][z] = 1;
        }
    }
    for(int y = 3; y < 16; y++)
    {
        for(int x = 0; x < 16; x++)
        {
            for(int z = 0; z < 16; z++)
            {
                blocksData[x][y][z] = 0;
            }
        }
    }
    initvertices();
}

void Chunk::load(int blockdatas[16][16][16])
{
    for(int x = 0; x < 16; x++)
        for(int y = 0; y < 16; y++)
            for(int z = 0; z < 16; z++)
                blocksData[x][y][z] = blockdatas[x][y][z];
    initvertices();
}

int Chunk::get(int x, int y, int z)
{
    return blocksData[x][y][z];
}

int Chunk::getVLength()
{
    return vlength*5;
}

int Chunk::getILength()
{
    return ilength*3;
}

void Chunk::setBlock(int x, int y, int z, blockType totype)
{
    blocksData[x][y][z] = (int)totype;
    initvertices();
    //unfinished method, use init as substitution
    //updateVertices(x, y, z);
}

void Chunk::getVertices(float verts[], int tlength)
{
    if(tlength != vlength*5)
    {
        std::cout << "ERROR::incomming array doesn't match chunk's verticies length" << std::endl;
    }
    else
    {
        for(int i = 0; i < vlength; i++)
            for(int j = 0; j < 5; j++)
                verts[i*5+j] = cvertices[i][j];
    }
}

void Chunk::getIndices(int indi[], int idxlength)
{
    if(idxlength != ilength*3)
    {
        std::cout << "ERROR::incomming array doesn't match chunk's verticies length" << std::endl;
    }
    else
    {
        for(int i = 0; i < ilength; i++)
            for(int j = 0; j < 3; j++)
                indi[i*3+j] = cindices[i][j];
    }
}

void Chunk::updateVertices(int x, int y, int z)
{
    std::cout << "Doing absolutely nothing" << std::endl;
}


void Chunk::initvertices()
{
    float vertexList[52224][5];
    int indices[26112][3];
    int indexvtx = 0;
    int indexindi = 0;
    for(int x = 0; x < 16; x++)
    {
        for(int y = 0; y < 16; y++)
        {
            for(int z = 0; z < 16; z++)
            {
                if(blocksData[x][y][z] != 0)
                {
                    //std::cout << x << " " << y << " " << z << ": " << blocksData[x][y][z] << std::endl;
                    //std::cout << x << " " << y << " " << z << " idx000 : " << blocksData[0][0][0] << std::endl;
                    float vtx[24][5];
                    int indi[12*3];
                    bmodels[blocksData[x][y][z]].getVertices(vtx);
                    bmodels[blocksData[x][y][z]].getIndices(indi);
                    for(int i = 0; i < 24; i++)
                    {
                        vtx[i][0] += x;
                        vtx[i][1] += y;
                        vtx[i][2] += z;
                    }
                    compareReduceAround(vtx, x, y, z);
                    addVertList(vertexList, indices, indexvtx, indexindi, vtx, 24, indi, 12, x, y, z);
                }
            }
        }
    }
    std::cout << indexvtx << " " << indexindi << std::endl;
    vlength = indexvtx;
    ilength = indexvtx/2;
    
    cvertices = new float[vlength][5];
    for(int i = 0; i < vlength; i++)
        for(int j = 0; j < 5; j++)
            cvertices[i][j] = vertexList[i][j];
    
    cindices = new int[ilength][3];
    for(int i = 0; i < ilength; i++)
        for(int j = 0; j < 3; j++)
            cindices[i][j] = indices[i][j];
}


void Chunk::compareReduceAround(float verts[][5], int x, int y, int z)
{
    compareReduce(verts,  x, y, z,  x-1, y, z,  4, 5);
    compareReduce(verts,  x, y, z,  x+1, y, z,  5, 4);
    compareReduce(verts,  x, y, z,  x, y-1, z,  0, 1);
    compareReduce(verts,  x, y, z,  x, y+1, z,  1, 0);
    compareReduce(verts,  x, y, z,  x, y, z-1,  3, 2);
    compareReduce(verts,  x, y, z,  x, y, z+1,  2, 3);

}

void Chunk::compareReduce(float verts[][5], int x1, int y1, int z1, int x2, int y2, int z2, int disidx1, int disidx2)
{
    if(x2 >= 0 && x2 < 16 && y2 >= 0 && y2 < 16 && z2 >= 0 && z2 < 16)
    {
        compare(verts, entityType[blocksData[x1][y1][z1]], disidx1, entityType[blocksData[x2][y2][z2]], disidx2);
    }
}

void Chunk::compare(float verts[][5], int enType1, int disidx1, int enType2, int disidx2)
{
    if(typeChar[enType1][disidx1] == typeChar[enType2][disidx2])
    {
        for(int i = disidx1*4; i < disidx1*4+4; i++)
            for(int d = 0; d < 5; d++)
                verts[i][d] = -1.0f;
    }
}

void Chunk::addVertList(float (*targetVerts)[5], int (*targetIndices)[3], int &indexvtx, int &indexindi, float (*dataVerts)[5], int vtxl, int dataIndi[], int indil, int x, int y, int z)
{
    int idxdata = 0;
    while(idxdata < vtxl)
    {
        if(!checkvoid(dataVerts, idxdata))
        {
            /* EXC_BAD_ACCESS BUG FROM HERE
             * Explanation: (int)indexvtx is larger than the size of (float[][])targetVerts and thus the index pointer flow into
             * (int[][])blocksData's storage place and fucked shits up
             */
            for(int i = idxdata; i < idxdata + 4; i++)
            {
                for(int j = 0; j < 5; j++)
                {
                    targetVerts[indexvtx][j] = dataVerts[i][j];
                }
                //std::cout << indexvtx << std::endl;
                indexvtx++;
            }
            /* TO HERE
             */
            
            targetIndices[indexindi/2][0] = indexindi;
            targetIndices[indexindi/2][1] = indexindi+1;
            targetIndices[indexindi/2][2] = indexindi+2;
            targetIndices[indexindi/2+1][0] = indexindi+1;
            targetIndices[indexindi/2+1][1] = indexindi+2;
            targetIndices[indexindi/2+1][2] = indexindi+3;
            indexindi += 4;

        }
        idxdata+=4;
    }
}

bool Chunk::checkvoid(float dataVerts[][5], int idx)
{
    bool v = true;
    for(int i = idx; i < idx+4; i++)
    {
        //std::cout << dataVerts[idx][0] << " " << dataVerts[idx][1] << " " << dataVerts[idx][2] << std::endl;
        v = v && (dataVerts[idx][3] < -0.5 && dataVerts[idx][4] < -0.5);
    }
    return v;
}
