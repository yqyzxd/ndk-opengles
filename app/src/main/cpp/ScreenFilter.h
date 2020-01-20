//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_SCREENFILTER_H
#define NDK_OPENGLES_SCREENFILTER_H


#include "BaseFilter.h"
#include "log.h"


class ScreenFilter : public BaseFilter{
public:
    ScreenFilter(const char *vertexSource, const char *fragmentSource);
    virtual ~ScreenFilter();
    virtual GLuint onDrawFrame(GLuint textureId);

    void updateTexImage(void* bytes,int width,int height);

private:
    GLint  mPositionLocation;
    GLint  mCoordLocation;
    GLint  mTextureLocation;

    void* mBytes;
    int texWidth;
    int texHeight;
};


#endif //NDK_OPENGLES_SCREENFILTER_H
