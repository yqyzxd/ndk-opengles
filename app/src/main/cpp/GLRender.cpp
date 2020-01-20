//
// Created by 史浩 on 2020-01-12.
//

#include "GLRender.h"
#include "gles/GlUtils.h"

GLRender::GLRender() {
    mEGLCore=NULL;
    mWindowSurface=NULL;
    mScreenFilter=NULL;
    mTriangle=NULL;

}

GLRender::~GLRender() {
    if (mEGLCore){
        mEGLCore->release();
        delete mEGLCore;
        mEGLCore=NULL;
    }
}

void GLRender::surfaceCreated(ANativeWindow *window) {
    if (mEGLCore==NULL){
        mEGLCore=new EGLCore();
        glGenTextures(1,&mTextureId);
        glBindTexture(GL_TEXTURE_2D,mTextureId);
        //设置放大缩小模式
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D,0);
        ALOGE("GLRender surfaceCreated mTextureId:%d",mTextureId);
    }
    mWindowSurface=new WindowSurface(mEGLCore,window);
    mWindowSurface->makeCurrent();
    if (mScreenFilter!=NULL){
        delete mScreenFilter;
        mScreenFilter=NULL;
    }

    const char* vertexSource="attribute vec4 a_Position;\n"
                             "attribute vec2 a_Coord;\n"
                             "varying vec2 v_Coord;\n"
                             "\n"
                             "void main(){\n"
                             "    gl_Position=a_Position;\n"
                             "    v_Coord=a_Coord;\n"
                             "}";

    const char* fragmentSource="precision mediump float;\n"
                               "varying vec2 v_Coord;\n"
                               "uniform sampler2D u_Texture;\n"
                               "void main(){\n"
                               "    gl_FragColor=texture2D(u_Texture,v_Coord);\n"
                               "}";
    //创建filter
    mScreenFilter=new ScreenFilter(vertexSource,fragmentSource);

    mTriangle=new Triangle();
    mTriangle->init();
}

void GLRender::surfaceChanged(int width, int height) {
    mWindowSurface->makeCurrent();
    glClearColor(1.0f,1.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    mScreenFilter->onReady(width,height);
    mTriangle->onDraw(width,height);
    mWindowSurface->swapBuffers();

}



void GLRender::surfaceDestroyed() {
    if(mTriangle){
        mTriangle->destroy();
        delete mTriangle;
        mTriangle=NULL;
    }
    if (mWindowSurface){
        mWindowSurface->release();
        delete mWindowSurface;
        mWindowSurface=NULL;
    }
    if (mEGLCore){
        mEGLCore->release();
        delete mEGLCore;
        mEGLCore=NULL;
    }
}

void GLRender::updateTexImage(void *bytes, int width, int height) {
    ALOGE("GLRender updateTexImage mTextureId:%d",mTextureId);
    mWindowSurface->makeCurrent();
    mScreenFilter->updateTexImage(bytes,  width,  height);
    mScreenFilter->onDrawFrame(mTextureId);
    bool swap=mWindowSurface->swapBuffers();
   ALOGE("GLRender swapBuffers:%d",swap);

}
