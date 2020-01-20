//
// Created by 史浩 on 2019-12-03.
//

#ifndef NDK_OPENGLES_TRIANGLE_H
#define NDK_OPENGLES_TRIANGLE_H


class Triangle {

public:
    Triangle();
    virtual ~Triangle();

    int init();
    void onDraw(int width,int height);
    void destroy();

private:
    int program;
};


#endif //NDK_OPENGLES_TRIANGLE_H
