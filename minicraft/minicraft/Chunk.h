//
//  Chunk.hpp
//  minicraft
//
//  Created by Lianfeng Shi on 5/8/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef Chunk_h
#define Chunk_h

#include <string.h>
#include "Block.h"


class Chunk
{
public:
    //block models
    static Block bmodels[4];
    //place in the world
    int wpos_x, wpos_y, wpos_z;
    //surrounding chunks;
    Chunk *fx, *bx, *fy, *by, *fz, *bz;

    Chunk(int px, int py, int pz);
    Chunk(int px, int py, int pz, int blockdatas[16][16][16]);
    void load();
    void load(int blockdatas[16][16][16]);
    
    int get(int x, int y, int z);
    int getVLength();
    int getILength();
    
    void setBlock(int x, int y, int z, blockType totype);
    
    void getVertices(float verts[], int tlength);
    void getIndices(int indi[], int idxlength);
    
private:
    
    //blocks data
    int blocksData[16][16][16];
    //chunk vertices data
    float (*cvertices)[5];
    int (*cindices)[3];
    //vertices
    int vlength;
    int ilength;

    void initvertices();
    void updateVertices(int x, int y, int z);
    
    void compareReduceAround(float verts[][5], int x, int y, int z);
    void compareReduce(float verts[][5], int x1, int y1, int z1, int x2, int y2, int z2, int disidxs, int disidxc);
    void compare(float verts[][5], int enType1, int disidx1, int enType2, int disidx2);
    void addVertList(float targetVerts[][5], int targetIndices[][3], int &indexvtx, int &indexindi, float dataVerts[][5], int vtxl, int dataIndi[], int indil, int x, int y, int z);
    bool checkvoid(float dataVerts[][5], int idx);
};

#endif /* Chunk_h */
