//
// Created by wind on 2023/3/27.
//

#ifndef NDK_PNG_RAW_H
#define NDK_PNG_RAW_H

#include <cstdint>
#include "png.h"
struct Raw{
    int width;
    int height;
    uint8_t* pixels;
    Raw(png_bytep pixels,int width,int height);
    virtual ~Raw();
};


#endif //NDK_PNG_RAW_H
