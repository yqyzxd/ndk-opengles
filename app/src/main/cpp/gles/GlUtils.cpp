//
// Created by 史浩 on 2019-12-03.
//

#include "GlUtils.h"
#include "../log.h"
#ifdef __cplusplus
extern "C" {
#endif

GLuint createProgram(const char* vertexShader, const char* fragmentShader){


    GLuint vertex=loadShader(GL_VERTEX_SHADER,vertexShader);
    if (vertex==0){
        return 0;
    }
    GLuint fragment=loadShader(GL_FRAGMENT_SHADER,fragmentShader);
    if (fragment==0){
        glDeleteShader(vertex);
        return 0;
    }

    GLuint program=glCreateProgram();
    if(program ==0){
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    //绑定shader
    glAttachShader(program,vertex);
    glAttachShader(program,fragment);

    //链接program程序
    glLinkProgram(program);
    //检查链接状态
    GLint link=0;
    glGetProgramiv(program,GL_LINK_STATUS,&link);
    if (!link){
        GLint infoLen=0;
        glGetProgramiv(program,GL_INFO_LOG_LENGTH,&infoLen);
       if(infoLen>1){
         char* infoLog=(char*)  malloc(sizeof(char) * infoLen);

         glGetProgramInfoLog(program,infoLen,NULL,infoLog);
         //打印infoLog
         ALOGE("Shader infoLog:%s",infoLog);
         free(infoLog);

       }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(program);
        return 0;

    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;




}

GLuint loadShader(GLenum type, const char* shaderSrc){

    GLuint shader=glCreateShader(type);
    //上传着色器源码
    glShaderSource(shader,1,&shaderSrc,NULL);
    //编译
    glCompileShader(shader);
    //检查编译状态
    GLint compiled;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
    if (!compiled){
        GLint infoLen=0;
        //查询日志的长度判断是否有日志产生
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);
        if (infoLen>1){
            //分配一个足以存储日志信息的字符串
            char* infoLog= (char *)(malloc(sizeof(char) * infoLen));
            //检索日志信息
            glGetShaderInfoLog(shader,infoLen,NULL,infoLog);
            ALOGE("Shader loadShader log:%s",infoLog);
            //打印infoLog信息
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    ALOGE("Shader loadShader shader:%d",shader);
    return shader;
}

/**
 * 创建Texture
 * @param type  texture类型 OES或者sampler2D
 * @return
 */
GLuint createTexture(GLenum type){

    GLuint textureId;
    //设置解包对齐
   // glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1,&textureId);
    glBindTexture(type,textureId);
    //设置放大缩小模式
    glTexParameteri(type,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(type,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(type,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(type,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    checkGlError("createTexture");

    glBindTexture(type,0);
    return textureId;
}

GLuint createTextureWithBytes(unsigned char* bytes,int width,int height){
    if(bytes ==NULL){
        return 0;
    }
    GLuint textureId;
    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_2D,textureId);
    //利用像素创建texture
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,bytes);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    return textureId;
}
/**
 * 使用旧texture创建新texture，宽高应小于等于旧的texture，最好是相等
 * 一般用于刷新视频帧这样的情形
 *
 */

GLuint createTextureWithOldTexture(GLuint texture, unsigned char* bytes,int width,int height){
    if (texture==0){
        return createTextureWithBytes(bytes,width,height);
    }
    glBindTexture(GL_TEXTURE_2D,texture);
    //更新Texture
    glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width,height,GL_NONE,GL_TEXTURE_2D,bytes);
    return texture;
}
//创建FBO和Texture
void createFrameBuffer(GLuint* framebuffer,GLuint* texture,int width,int height){
    createFrameBuffers(framebuffer,texture,width,height,1);
}

// 创建size个FBO和Texture
void createFrameBuffers(GLuint* frambuffers, GLuint* textures, int width, int height, int size){
    glGenFramebuffers(size,frambuffers);
    glGenTextures(size,textures);

    for (int i = 0; i < size; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER,frambuffers[i]);
        glBindTexture(GL_TEXTURE_2D,textures[i]);
        //创建一个没有像素的texture
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D,0);
        glBindFramebuffer(GL_FRAMEBUFFER,0);

    }
}
//检查是否出错
void checkGlError(const char* op){
   for(GLint error=glGetError();error;error=glGetError()){
       ALOGE( "[GLES2] after %s() glError (0x%x)\n", op, error);
   }
}

#ifdef __cplusplus
};
#endif