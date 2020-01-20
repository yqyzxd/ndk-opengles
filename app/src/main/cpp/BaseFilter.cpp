//
// Created by 史浩 on 2020-01-12.
//

#include "BaseFilter.h"

BaseFilter::BaseFilter(const char *vertexSource, const char *fragmentSource) {

    mProgram=createProgram(vertexSource,fragmentSource);
}

void BaseFilter::onReady(int width, int height) {
    mWidth=width;
    mHeight=height;

    //glViewport(0,0,mWidth,mHeight);
}