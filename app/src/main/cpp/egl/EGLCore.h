//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_EGLCORE_H
#define NDK_OPENGLES_EGLCORE_H
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/native_window.h>
#include "../log.h"
//android-specific extension
//定义一个函数指针
typedef EGLBoolean (EGLAPIENTRYP EGL_PRESENTATION_TIME_ANDROIDPROC)(EGLDisplay display,EGLSurface surface,khronos_stime_nanoseconds_t time);
class EGLCore {

public:
    EGLCore();
    EGLCore(EGLContext shareContext);
    ~EGLCore();

    /**
     * 用于初始化EGLContext
     * @param shareContext  共享的EGLContext
     * @param flags
     * @return
     */
    bool init(EGLContext shareContext);
    /**
     * 释放EGLContext相关资源
     */
    void release();
    /**
     *  获取EGLContext
     */
    EGLContext getEGLContext();

    /**
     * 通过ANativeWindow创建EGLSurface
     * @param window
     * @return
     */
    EGLSurface createWindowSurface(ANativeWindow* window);
    /**
     * 创建离屏EGLSurface
     * @param width   surface宽
     * @param height  surface高
     * @return
     */
    EGLSurface createOffscreenSurface(int width,int height);
    /**
     * 释放eglSurface
     * @param eglSurface
     */
    void releaseSurface(EGLSurface eglSurface);

    /**
     * 切换到当前的eglSurface
     * @param eglSurface
     * @return
     */
    EGLBoolean makeCurrent(EGLSurface eglSurface);
    EGLBoolean makeCurrent(EGLSurface eglDrawSurface,EGLSurface eglReadSurface);

    /**
     * 交换显示帧缓存
     * @param eglSurface
     * @return
     */
    EGLBoolean swapBuffers(EGLSurface eglSurface);

    /**
     * 设置pts
     * @param eglSurface
     * @param nsecs
     */
    void setPresentationTime(EGLSurface eglSurface, long nsecs);

    /**
     * 判断eglSurface是否属于当前上下文
     * @param eglSurface
     * @return
     */
    bool isCurrent(EGLSurface eglSurface);

    int querySurface(EGLSurface eglSurface,int what);
    const char* queryString(int what);
    //int getGLVersion();
    void checkEGLError(const char* msg);

private:
    EGLContext mEGLContext=EGL_NO_CONTEXT;
    EGLConfig  mEGLConfig=NULL;
    EGLDisplay mEGLDisplay=EGL_NO_DISPLAY;

    //设置时间戳的方法
    EGL_PRESENTATION_TIME_ANDROIDPROC eglPresentationTimeANDROID=NULL;

    EGLConfig getConfig(int flags,int version);
};


#endif //NDK_OPENGLES_EGLCORE_H
