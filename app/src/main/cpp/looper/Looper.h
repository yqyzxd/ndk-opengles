//
// Created by 史浩 on 2019-12-05.
//

#ifndef NDK_OPENGLES_LOOPER_H
#define NDK_OPENGLES_LOOPER_H

#include <pthread.h>
#include <semaphore.h>

//消息结构体
struct LooperMessage{
    int what;
    int arg1;
    int arg2;
    void* obj;
    LooperMessage* next;
    bool quit;
};

class Looper {

public:
    Looper();
    Looper&operator=(const Looper& ) = delete;
    Looper(Looper&)= delete;
    virtual ~Looper();

    //发送消息
    void postMessage(int what, bool flush= false);
    void postMessage(int what,void* obj, bool flush= false);
    void postMessage(int what,int arg1,int arg2, bool flush= false);
    void postMessage(int what,int arg1,int arg2,void* obj, bool flush= false);


    //退出looper循环
    void quit();

    //处理消息
    virtual void handleMessage(LooperMessage* msg);


private:
    //添加消息
    void addMessage(LooperMessage* msg, bool flush);
    //消息线程句柄
    static void* trampoline(void* p);
    //循环体
    void loop();

    LooperMessage* head;
    //线程相关
    pthread_t worker;
    sem_t headwriteproject;
    sem_t headdataavailable;
    bool running;

};


#endif //NDK_OPENGLES_LOOPER_H
