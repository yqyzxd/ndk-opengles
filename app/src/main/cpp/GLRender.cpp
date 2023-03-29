//
// Created by 史浩 on 2020-01-12.
//

#include <unistd.h>
#include "GLRender.h"
#include "gles/GlUtils.h"
#include "libpng/png_decoder.h"
#include "pngdecoder/file_png_decoder.h"

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
    ALOGE("GLRender surfaceCreated");
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
        checkGlError("glTexParameteri");
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
                             "}\n";

    const char* fragmentSource="precision mediump float;\n"
                               "varying vec2 v_Coord;\n"
                               "uniform sampler2D u_Texture;\n"
                               "void main(){\n"
                               "    gl_FragColor=texture2D(u_Texture,v_Coord);\n"
                               "}\n";
    static const char* PIC_PREVIEW_VERTEX_SHADER_2 =
            "attribute vec4 a_Position;    \n"
            "attribute vec2 a_Coord;   \n"
            "varying vec2 v_Coord;     \n"
            "void main(void)               \n"
            "{                            \n"
            "   gl_Position = a_Position;  \n"
            "   v_Coord = a_Coord;  \n"
            "}                            \n";
    static const char* PIC_PREVIEW_FRAG_SHADER_2 =
            "varying highp vec2 v_Coord;\n"
            "uniform sampler2D u_Texture;\n"
            "void main() {\n"
            "  gl_FragColor = texture2D(u_Texture, v_Coord);\n"
            "}\n";



    //创建filter
    mScreenFilter=new ScreenFilter(PIC_PREVIEW_VERTEX_SHADER_2,PIC_PREVIEW_FRAG_SHADER_2);

    //mTriangle=new Triangle();
    //mTriangle->init();

}

void GLRender::surfaceChanged(int width, int height) {
    ALOGE("GLRender surfaceChanged");
    mWindowSurface->makeCurrent();
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    mScreenFilter->onReady(width,height);
    char* fileName="/storage/emulated/0/Android/data/com.wind.ndk.opengles/files/1.png";
    PngDecoder* decoder=new FilePngDecoder(fileName);
    png_bytep bytes=decoder->decode();
    mScreenFilter->updateTexImage(mTextureId, bytes, decoder->getWidth(), decoder->getHeight());
    ALOGE("GLRender surfaceChanged updateTexImage width:%d,height:%d",decoder->getWidth(),decoder->getHeight());
    mScreenFilter->onDrawFrame(mTextureId);
    // mTriangle->onDraw(width,height);
    mWindowSurface->swapBuffers();

}



void GLRender::surfaceDestroyed() {
    ALOGE("GLRender surfaceDestroyed");
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
    mScreenFilter->onDrawFrame(mTextureId);
    ALOGE("GLRender updateTexImage width:%d,height:%d",width,height);
   /* ALOGE("GLRender updateTexImage width:%d,height:%d",width,height);
    if (mWindowSurface== nullptr){
        ALOGE("GLRender updateTexImage mWindowSurface is null");
        return;
    }
    mWindowSurface->makeCurrent();
    mScreenFilter->updateTexImage(mTextureId, bytes, width, height);

    sleep(1);
    mScreenFilter->onDrawFrame(mTextureId);
    //mTriangle->onDraw(width,height);
    bool swap=mWindowSurface->swapBuffers();
   ALOGE("GLRender swapBuffers:%d",swap);
    */

}
