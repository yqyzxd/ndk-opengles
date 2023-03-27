#include "raw.h"


//
// Created by wind on 2023/3/27.
//
Raw::Raw(png_bytep pixels,int width,int height) {
    this->width=width;
    this->height=height;
    this->pixels=pixels;
}
Raw::~Raw(){
    if (pixels){
        delete[] pixels;
    }
}