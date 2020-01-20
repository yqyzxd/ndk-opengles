//
// Created by 史浩 on 2019-12-03.
//

#ifndef NDK_OPENGLES_WINDOWSURFACE_H
#define NDK_OPENGLES_WINDOWSURFACE_H


#include "BaseEGLSurface.h"

class WindowSurface :public BaseEGLSurface {

public:
    WindowSurface(EGLCore* eglCore,ANativeWindow* window);
    WindowSurface(EGLCore* eglCore,ANativeWindow* window, bool releaseSurface);
    ~WindowSurface();

    void release();
    void recreate(EGLCore* eglCore);

private:
    ANativeWindow* mSurface;
    bool mReleaseSurface;
};


#endif //NDK_OPENGLES_WINDOWSURFACE_H
