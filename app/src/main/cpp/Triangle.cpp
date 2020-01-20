//
// Created by 史浩 on 2019-12-03.
//

#include "Triangle.h"
#include "gles/GlUtils.h"

const GLint COORDS_PER_VERTEX=3;
const GLint vertexStride=COORDS_PER_VERTEX*4;

Triangle::Triangle() {

}
Triangle::~Triangle() {

}

int Triangle::init() {
    char vertexShader[]="attribute vec4 a_position;\n"
                        "attribute vec4 a_color;\n"
                        "varying vec4 v_color;\n"
                        "void main()\n"
                        "{\n"
                        "   gl_Position=a_position;\n"
                        "   v_color=a_color;\n"
                        "}\n";

    char fragmentShader[]="precision mediump float;\n"
                          "varying vec4 v_color;\n"
                          "void main()\n"
                          "{\n"
                          "     gl_FragColor=v_color;\n"
                          "}\n";



    program=createProgram(vertexShader,fragmentShader);

    if (program<=0){
        return -1;
    }
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    return 0;
}

void Triangle::onDraw(int width, int height) {
    GLfloat vertices[]={0.0f,1.0f,0.0f,
                        -1.0f,-1.0f,0.0f,
                        1.0f,-1.0f,0.0f};

    GLfloat color[]={
            1.0f,0.0f,0.0f,1.0f
    };
    GLint vertexCount= sizeof(vertices)/(sizeof(vertices[0])*3);
    //设置视口
    glViewport(0,0,width,height);
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    GLint positionPosition=glGetAttribLocation(program,"a_position");
    GLint colorPosition=glGetAttribLocation(program,"a_color");
    glVertexAttribPointer(positionPosition,COORDS_PER_VERTEX,GL_FLOAT, false,0,vertices);
    glEnableVertexAttribArray(positionPosition);


    glVertexAttrib4fv(colorPosition,color);

    glDrawArrays(GL_TRIANGLES,0,vertexCount);
    glDisableVertexAttribArray(positionPosition);
}

void Triangle::destroy() {
    if(program>0){
        glDeleteProgram(program);
    }
    program=-1;
}