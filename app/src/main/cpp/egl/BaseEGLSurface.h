//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_BASEEGLSURFACE_H
#define NDK_OPENGLES_BASEEGLSURFACE_H


#include "EGLCore.h"
#include <GLES2/gl2.h>
class BaseEGLSurface {

public:
    BaseEGLSurface(EGLCore* eglCore);
    ~BaseEGLSurface();

    //创建窗口Surface
    void createWindowSurface(ANativeWindow* window);
    //创建离屏Surface
    void createOffscreenSurface(int width,int height);
    //获取宽度
    int getWidth();
    //获取高度
    int getHeight();
    //释放EGLSurface
    void releaseEglSurface();
    //切换当前上下文
    void makeCurrent();
    //交换缓冲区 显示图像
    bool swapBuffers();
    //设置显示时间戳
    void setPresentationTime(long nsecs);
    //获取当前帧缓冲
    char* getCurrentFrame();

protected:
    EGLCore* mEGLCore;
    EGLSurface mEGLSurface;
    int mWidth;
    int mHeight;
};


#endif //NDK_OPENGLES_BASEEGLSURFACE_H
