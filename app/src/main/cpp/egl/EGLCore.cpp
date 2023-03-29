//
// Created by 史浩 on 2020-01-12.
//

#include "EGLCore.h"

EGLCore::EGLCore() {
    init(NULL);
}
EGLCore::EGLCore(EGLContext shareContext) {
    init(shareContext);
}
EGLCore::~EGLCore() {
    release();
}

/**
 * 目的，创建EGLContext
 * @param shareContext
 * @param flags
 * @return
 */
bool EGLCore::init(EGLContext shareContext) {

    if (mEGLDisplay!=EGL_NO_DISPLAY){

        ALOGE("EGL already set up");
        return false;
    }
    //1.获取eglDisplay
    mEGLDisplay=eglGetDisplay(EGL_DEFAULT_DISPLAY);
    //再次判断
    if (mEGLDisplay==EGL_NO_DISPLAY){
        //unable to getEGLDisplay
        return false;
    }
    //2.初始化egldiaplay
    //EGL_FALSE 0
    if(eglInitialize(mEGLDisplay,0,0)==EGL_FALSE){
        mEGLDisplay=EGL_NO_DISPLAY;
        return false;
    }

    EGLint numConfigs;
    EGLint attribs[]={EGL_BUFFER_SIZE,32,
                      EGL_ALPHA_SIZE,8,
                      EGL_BLUE_SIZE,8,
                      EGL_GREEN_SIZE,8,
                      EGL_RED_SIZE,8,
                      EGL_RENDERABLE_TYPE,EGL_OPENGL_ES2_BIT,
                      EGL_SURFACE_TYPE,EGL_WINDOW_BIT,
                      EGL_NONE};
    //3.设置EGLDisplay参数
    if (eglChooseConfig(mEGLDisplay,attribs,&mEGLConfig,1,&numConfigs)==EGL_FALSE){
        release();
        return false;
    }

    if(shareContext==NULL){
        shareContext=EGL_NO_CONTEXT;
    }
    //4.创建EGLContext
    EGLint eglContextAttributes[]={EGL_CONTEXT_CLIENT_VERSION,2,
                                EGL_NONE};
    /*
     * EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list
     */
    mEGLContext=eglCreateContext(mEGLDisplay,mEGLConfig,shareContext,eglContextAttributes);


    //获取eglPresentationTimeAndroid方法的地址
    eglPresentationTimeANDROID=(EGL_PRESENTATION_TIME_ANDROIDPROC)eglGetProcAddress("eglPresentationTimeANDROID");

    return true;
}
EGLContext EGLCore::getEGLContext() {
    return mEGLContext;
}

EGLSurface EGLCore::createWindowSurface(ANativeWindow *window) {
   /* if(window==NULL){
        return NULL;
    }
    int attribs[]={EGL_NONE};
    //EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list
    EGLSurface eglSurface=eglCreateWindowSurface(mEGLDisplay,mEGLConfig,window,attribs);
    if(eglSurface==NULL){
        return NULL;
    }
    return eglSurface;*/
    EGLSurface surface = NULL;
    EGLint format;
    if (!eglGetConfigAttrib(mEGLDisplay, mEGLConfig, EGL_NATIVE_VISUAL_ID, &format)) {
        ALOGE("eglGetConfigAttrib() returned error %d", eglGetError());
        release();
        return surface;
    }
    ANativeWindow_setBuffersGeometry(window, 0, 0, format);
    if (!(surface = eglCreateWindowSurface( mEGLDisplay, mEGLConfig, window, 0))) {
        ALOGE("eglCreateWindowSurface() returned error %d", eglGetError());
    }
    return surface;
}

EGLSurface EGLCore::createOffscreenSurface(int width, int height) {
    int attribs[]={EGL_WIDTH,width,
                   EGL_HEIGHT,height,
                   EGL_NONE};
    EGLSurface eglSurface=eglCreatePbufferSurface(mEGLDisplay,mEGLConfig,attribs);
    if (eglSurface==NULL){
        return NULL;
    }
    return eglSurface;
}

EGLBoolean EGLCore::makeCurrent(EGLSurface eglSurface) {
    if (mEGLDisplay==EGL_NO_DISPLAY){
        return EGL_FALSE;
    }
    return eglMakeCurrent(mEGLDisplay,eglSurface,eglSurface,mEGLContext);
}

EGLBoolean EGLCore::makeCurrent(EGLSurface eglDrawSurface, EGLSurface eglReadSurface) {
    if (mEGLDisplay==EGL_NO_DISPLAY){
        return EGL_FALSE;
    }
    return eglMakeCurrent(mEGLDisplay,eglDrawSurface,eglReadSurface,mEGLContext);
}

EGLBoolean EGLCore::swapBuffers(EGLSurface eglSurface) {
    return eglSwapBuffers(mEGLDisplay,eglSurface);
}
void EGLCore::setPresentationTime(EGLSurface eglSurface, long nsecs) {
    eglPresentationTimeANDROID(mEGLDisplay,eglSurface,nsecs);
}

bool EGLCore::isCurrent(EGLSurface eglSurface) {
    return mEGLContext==eglGetCurrentContext()
           && eglSurface ==eglGetCurrentSurface(EGL_DRAW);
}

int EGLCore::querySurface(EGLSurface eglSurface, int what) {
    int value;
    eglQuerySurface(mEGLDisplay,eglSurface,what,&value);
    return value;
}

const char* EGLCore::queryString(int what) {
    return eglQueryString(mEGLDisplay,what);
}

/*int EGLCore::getGLVersion() {
    return mGlVersion;
}*/

void EGLCore::checkEGLError(const char *msg) {
    int error;
    if((error=eglGetError()) != EGL_SUCCESS){
        //error
    }
}

void EGLCore::release(){
    if (mEGLDisplay!=EGL_NO_DISPLAY){
        eglMakeCurrent(mEGLDisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,mEGLContext);
        eglDestroyContext(mEGLDisplay,mEGLContext);
        eglReleaseThread();
        eglTerminate(mEGLDisplay);
    }
    mEGLDisplay=EGL_NO_DISPLAY;
    mEGLContext=EGL_NO_CONTEXT;
    mEGLConfig=NULL;
}

void EGLCore::releaseSurface(EGLSurface eglSurface) {
    eglDestroySurface(mEGLDisplay,eglSurface);
}