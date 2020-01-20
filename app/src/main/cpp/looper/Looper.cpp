//
// Created by 史浩 on 2019-12-05.
//

#include "Looper.h"


Looper::Looper() {

    head = NULL;
    /*该函数初始化由 sem 指向的信号对象，并给它一个初始的整数值 value。
    pshared 控制信号量的类型，值为 0 代表该信号量用于多线程间的同步，值如果大于 0 表示可以共享，用于多个相关进程间的同步
   */
    sem_init(&headdataavailable, 0, 0);
    sem_init(&headwriteproject, 0, 1);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&worker, &attr, trampoline, this);
    running = true;
}

Looper::~Looper() {
    if (running) {
        quit();
    }
}

void *Looper::trampoline(void *p) {
    Looper *looper = static_cast<Looper *>(p);
    looper->loop();
    return 0;
}

void Looper::loop() {
    while (true) {

        /*
         * sem_wait 是一个阻塞的函数，测试所指定信号量的值，它的操作是原子的。
         * 若 sem value > 0，则该信号量值减去 1 并立即返回。
         * 若sem value = 0，则阻塞直到 sem value > 0，此时立即减去 1，然后返回。
         */
        //wait for available message
        sem_wait(&headdataavailable);

        //get next available message
        sem_wait(&headwriteproject);

        LooperMessage *msg = head;
        if (msg == NULL) {
            sem_post(&headwriteproject);
            continue;
        }
        head = msg->next;
        sem_post(&headwriteproject);

        if (msg->quit) {
            delete msg;
            return;
        }

        handleMessage(msg);
        delete msg;
    }

}

void Looper::quit() {
    LooperMessage *msg = new LooperMessage();
    msg->what = 0;
    msg->obj = NULL;
    msg->next = NULL;
    msg->quit = true;
    addMessage(msg, false);
    void *retval;
    //等待worker线程结束
    pthread_join(worker, &retval);
    //销毁锁
    //该函数用于对用完的信号量的清理。 成功则返回 0，失败返回 -1
    sem_destroy(&headdataavailable);
    sem_destroy(&headwriteproject);
    running = false;
}

/**
 *
 * @param msg
 * @param flush  是否清空消息队列
 */
void Looper::addMessage(LooperMessage *msg, bool flush) {
    sem_wait(&headwriteproject);
    LooperMessage *h = head;
    if (flush) {
        //清空消息队列
        while (h) {
            LooperMessage *next = h->next;
            delete h;
            h = next;
        }
        h = NULL;
    }
    if (h) {
        while (h->next) {
            h = h->next;
        }
        h->next = msg;
    } else {
        head = msg;
    }
    //把指定的信号量 sem 的值加 1，唤醒正在等待该信号量的任意线程。
    sem_post(&headwriteproject);
    sem_post(&headdataavailable);

}


void Looper::postMessage(int what, bool flush) {
    postMessage(what, 0, 0, NULL, flush);
}

void Looper::postMessage(int what, void *obj, bool flush) {
    postMessage(what, 0, 0, obj, flush);
}

void Looper::postMessage(int what, int arg1, int arg2, bool flush) {
    postMessage(what, arg1, arg2, NULL, flush);
}

void Looper::postMessage(int what, int arg1, int arg2, void *obj, bool flush) {
    LooperMessage *looperMessage = new LooperMessage();
    looperMessage->what = what;
    looperMessage->arg1 = arg1;
    looperMessage->arg2 = arg2;
    looperMessage->obj = obj;
    looperMessage->next = NULL;
    looperMessage->quit = false;
    addMessage(looperMessage, flush);
}

void Looper::handleMessage(LooperMessage *msg) {

}

