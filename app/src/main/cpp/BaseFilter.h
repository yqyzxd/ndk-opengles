//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_BASEFILTER_H
#define NDK_OPENGLES_BASEFILTER_H

#include "gles/GlUtils.h"
class BaseFilter {

public:
    BaseFilter(const char* vertexSource, const char* fragmentSource);
    virtual ~BaseFilter(){};
    void onReady(int width,int height);

    virtual GLuint onDrawFrame(GLuint textureId)=0;

protected:
    GLuint mProgram;
    int mWidth;
    int mHeight;
};


#endif //NDK_OPENGLES_BASEFILTER_H
