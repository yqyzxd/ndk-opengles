//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_IGLRENDER_H
#define NDK_OPENGLES_IGLRENDER_H

#include <android/native_window.h>

class IGLRender{
public:
    IGLRender() {};
    virtual ~IGLRender() {}; //注意，最好定义此虚析构函数，够避免子类不能正常调用析构函数；如果定义为纯虚析构函数，则必须带定义体，因为子类隐含调用该析构函数。

    virtual void surfaceCreated(ANativeWindow* window)=0;
    virtual void surfaceChanged(int width,int height)=0;
    virtual void surfaceDestroyed(void)=0;
};

#endif //NDK_OPENGLES_IGLRENDER_H
