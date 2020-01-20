//
// Created by 史浩 on 2019-12-03.
//

#include "OffscreenSurface.h"

OffscreenSurface::OffscreenSurface(EGLCore *eglCore,int width,int height) : BaseEGLSurface(eglCore) {


    createOffscreenSurface(width,height);
}


void OffscreenSurface::release() {
    releaseEglSurface();
}