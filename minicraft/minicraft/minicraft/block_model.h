//
//  block_m.h
//  minicraft
//
//  Created by Lianfeng Shi on 5/8/19.
//  Copyright © 2019 Lianfeng Shi. All rights reserved.
//

#ifndef block_model_h
#define block_model_h

enum blockType{
    VOID = 0,
    GRASS = 1,
    WOOD = 2,
    ROCK = 3
};

//Characteristic of types in blockType. are they block? are they air? are they stairs? check entityType
const static int entityType[4] = {
    0,
    1,
    1,
    1
};

//for each type in entityType, there are six faces what is the nature of each faces? check typeChar
//by index: bottom, top, front, back, left, right
const int typeChar[2][6] = {
    {-1, -1, -1, -1, -1, -1},
    {0, 0, 0, 0, 0, 0}
};

const float vertecies_m[24][3] = {
    //Bottom
    {0.0f, 0.0f, 1.0f},    //0
    {1.0f, 0.0f, 1.0f},    //1
    {0.0f, 0.0f, 0.0f},    //2
    {1.0f, 0.0f, 0.0f},    //3
    
    
    //Top
    {0.0f, 1.0f, 1.0f},    //4
    {1.0f, 1.0f, 1.0f},    //5
    {0.0f, 1.0f, 0.0f},    //6
    {1.0f, 1.0f, 0.0f},    //7
    
    
    //Front
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    
    
    //Back
    {0.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    
    
    //Left
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    
    
    //Right
    {1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
};

const int indices_m[] = {
    //Bottom
    0, 1, 2,
    1, 2, 3,
    
    //Top
    4, 5, 6,
    5, 6, 7,
    
    //Front
    8, 9, 10,
    9, 10, 11,
    
    //Back
    12, 13, 14,
    13, 14, 15,
    
    //Left
    16, 17, 18,
    17, 18, 19,
    
    //Right
    20, 21, 22,
    21, 22, 23,
};

#endif /* block_model_h */
