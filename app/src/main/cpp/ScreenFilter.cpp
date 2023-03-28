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

void ScreenFilter::updateTexImage(void *bytes, int width, int height) {
    this->mBytes=bytes;
    this->texWidth=width;
    this->texHeight=height;

    ALOGE("ScreenFilter updateTexImage width %d height %d",width,height);

}

GLuint ScreenFilter::onDrawFrame(GLuint textureId) {
    ALOGE("ScreenFilter onDrawFrame mProgram:%d",mProgram);
//设置一个颜色状态
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    //使能颜色状态的值来清屏
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,mWidth,mHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(mProgram);

    ALOGE("ScreenFilter onDrawFrame textureId:%d",textureId);
    //顶点坐标
    static GLfloat vertexArray[]={
            -1.0f,1.0f,
            1.0f,1.0f,
            -1.0f,-1.0f,
            1.0f,-1.0f
    };
//纹理坐标
    static GLfloat textureCoordArray[]={
            0.0f,1.0f,
            1.0f,1.0f,
            0.0f,0.0f,
            1.0f,0.0f
    };
//屏幕坐标
    static GLfloat screenCoordArray[]={
            0.0f,0.0f,
            1.0f,0.0f,
            0.0f,1.0f,
            1.0f,1.0f
    };


    //mPositionLocation=glGetAttribLocation(mProgram,"a_Position");
    //mCoordLocation=glGetAttribLocation(mProgram,"a_Coord");
    //mTextureLocation=glGetUniformLocation(mProgram,"u_Texture");

    glVertexAttribPointer(mPositionLocation,2,GL_FLOAT, false,0,vertexArray);
    glEnableVertexAttribArray(mPositionLocation);


    glVertexAttribPointer(mCoordLocation,2,GL_FLOAT, false,0,screenCoordArray);
    glEnableVertexAttribArray(mCoordLocation);

    glUniform1i(mTextureLocation,0);
    ALOGE("ScreenFilter onDrawFrame mTextureLocation %d",mTextureLocation);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,textureId);
    checkGlError("glBindTexture");
    ALOGE("ScreenFilter onDrawFrame before glTexImage2D texWidth:%d,texHeight:%d",texWidth,texHeight);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texWidth,texHeight,0,GL_RGBA,GL_UNSIGNED_BYTE,mBytes);
    ALOGE("ScreenFilter onDrawFrame before glDrawArrays");
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
   /* glDisableVertexAttribArray(mPositionLocation);
    glDisableVertexAttribArray(mCoordLocation);*/
    glBindTexture(GL_TEXTURE_2D,0);


    return textureId;
}