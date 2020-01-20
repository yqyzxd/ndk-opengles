//
// Created by 史浩 on 2019-12-03.
//

#ifndef NDK_OPENGLES_OFFSCREENSURFACE_H
#define NDK_OPENGLES_OFFSCREENSURFACE_H


#include "BaseEGLSurface.h"

class OffscreenSurface : public BaseEGLSurface{
public:
    OffscreenSurface(EGLCore *eglCore,int width,int height);

    void release();


};


#endif //NDK_OPENGLES_OFFSCREENSURFACE_H
