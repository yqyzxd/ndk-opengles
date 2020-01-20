//
// Created by 史浩 on 2019-12-03.
//

#ifndef NDK_OPENGLES_GLUTILS_H
#define NDK_OPENGLES_GLUTILS_H

#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#define PI 3.1415926535897932384626433832795f
    typedef struct {
        GLfloat m[16];
    } ESMatrix;

    //创建program
    GLuint  createProgram(const char* vertexShader, const char* fragmentShader);
    //加载shader
    GLuint loadShader(GLenum type, const char* shaderSrc);
    //查询活动的统一变量uniform
    void checkActiveUniform(GLuint program);
    //创建texture
    GLuint createTexture(GLenum type);
    //使用bytes创建texture
    GLuint createTextureWithBytes(unsigned char* bytes,int width,int height);
    //使用旧texture创建新texture
    GLuint createTextureWithOldTexture(GLuint texture, unsigned char* bytes,int width,int height);
    //创建FBO和Texture
     void createFrameBuffer(GLuint* framebuffer,GLuint* texture,int width,int height);

// 创建size个FBO和Texture
    void createFrameBuffers(GLuint* frambuffers, GLuint* textures, int width, int height, int size);
    //检查是否出错
    void checkGlError(const char* op);
    //缩放

    //平移
    //旋转
    //正交投影矩阵
    //视椎体
    //透视矩阵
    //产生一个单位矩阵
    //矩阵相乘










#ifdef __cplusplus
};
#endif

#endif //NDK_OPENGLES_GLUTILS_H
