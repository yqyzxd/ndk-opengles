//
// Created by 史浩 on 2020-01-12.
//

#include "GLLooper.h"

GLLooper::GLLooper() {
    mRender=new GLRender();
}

GLLooper::~GLLooper() {
    delete mRender;
    mRender=0;
}

void GLLooper::handleMessage(LooperMessage *msg) {
    if(mRender==NULL){
        return;
    }
    switch (msg->what){
        case kMsgSurfaceCreated:

            mRender->surfaceCreated((ANativeWindow *)(msg->obj));
            break;
        case kMsgSurfaceChanged:
            mRender->surfaceChanged(msg->arg1,msg->arg2);
            break;
        case kMsgSurfaceDestroyed:
            mRender->surfaceDestroyed();
            break;
        case kMsgUpdateTexImage:
            mRender->updateTexImage((msg->obj), msg->arg1, msg->arg2);
            break;
    }
}