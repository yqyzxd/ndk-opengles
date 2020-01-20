//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_GLLOOPER_H
#define NDK_OPENGLES_GLLOOPER_H

#include "looper/Looper.h"
#include "GLRender.h"

enum {
    kMsgSurfaceCreated,
    kMsgSurfaceChanged,
    kMsgSurfaceDestroyed,
    kMsgUpdateTexImage
};
class GLLooper: public Looper {

public:
    GLLooper();
    virtual ~GLLooper();

    virtual void handleMessage(LooperMessage* msg);

private:
    GLRender* mRender;
};


#endif //NDK_OPENGLES_GLLOOPER_H
