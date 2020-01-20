//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_GLRENDER_H
#define NDK_OPENGLES_GLRENDER_H

#include <android/native_window.h>
#include "egl/IGLRender.h"
#include "egl/EGLCore.h"
#include "egl/WindowSurface.h"
#include "ScreenFilter.h"
#include "Triangle.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>



class GLRender : public IGLRender {

public:
    GLRender();

    virtual ~GLRender();

    virtual void surfaceCreated(ANativeWindow *window);
    virtual void surfaceChanged(int width, int height);
    virtual void surfaceDestroyed(void);

    void updateTexImage(void* bytes,int width,int height);

private:
    EGLCore *mEGLCore;
    WindowSurface *mWindowSurface;
    ScreenFilter *mScreenFilter;
    GLuint mTextureId;

    Triangle* mTriangle;
};


#endif //NDK_OPENGLES_GLRENDER_H
