//
// Created by 史浩 on 2020-01-12.
//

#include "ScreenFilter.h"

ScreenFilter::ScreenFilter(const char* vertexSource,const char* fragmentSource) : BaseFilter(
        vertexSource, fragmentSource) {

    //获取属性location
    mPositionLocation=glGetAttribLocation(mProgram,"a_Position");
    mCoordLocation=glGetAttribLocation(mProgram,"a_Coord");
    mTextureLocation=glGetUniformLocation(mProgram,"u_Texture");


    checkGlError("ScreenFilter init");
    ALOGE("ScreenFilter mPositionLocation  %d",mPositionLocation);
    ALOGE("ScreenFilter mCoordLocation  %d",mCoordLocation);
    ALOGE("ScreenFilter mTextureLocation  %d",mTextureLocation);

}
ScreenFilter::~ScreenFilter() {

}

void ScreenFilter::updateTexImage(GLuint textureId,void *pixels, int width, int height) {
    //if (pixels) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        if (checkGlError("glBindTexture")) {
            return;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        checkGlError("glTexImage2D");
        glBindTexture(GL_TEXTURE_2D, 0);
    //}

}

GLuint ScreenFilter::onDrawFrame(GLuint textureId) {
    glViewport(0,0,mWidth,mHeight);
    ALOGE("ScreenFilter onDrawFrame mProgram:%d,mWidth:%d,mHeight:%d",mProgram,mWidth,mHeight);
//设置一个颜色状态
    //glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    //使能颜色状态的值来清屏
    //glClear(GL_COLOR_BUFFER_BIT);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(mProgram);

    ALOGE("ScreenFilter onDrawFrame textureId:%d",textureId);
    //顶点坐标
    //static GLfloat vertexArray[]={ -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    static const GLfloat vertexArray[] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
    //纹理坐标
   // static GLfloat textureCoordArray[]={ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
    static const GLfloat textureCoordArray[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
   //屏幕坐标
    static GLfloat screenCoordArray[]={
            0.0f,0.0f,
            1.0f,0.0f,
            0.0f,1.0f,
            1.0f,1.0f
    };

    glVertexAttribPointer(mPositionLocation,2,GL_FLOAT, false,0,vertexArray);
    glEnableVertexAttribArray(mPositionLocation);

    glVertexAttribPointer(mCoordLocation,2,GL_FLOAT, false,0,textureCoordArray);
    glEnableVertexAttribArray(mCoordLocation);
    ALOGE("ScreenFilter onDrawFrame mTextureLocation %d",mTextureLocation);
    //纹理操作相关
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureId);
    //tell the texture uniform sampler to use this texture in the shader by telling it to read from texture unit 0.
    glUniform1i(mTextureLocation,0);
    checkGlError("glBindTexture");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mCoordLocation);
    glBindTexture(GL_TEXTURE_2D,0);
    checkGlError("glDrawArrays");
    return textureId;
}