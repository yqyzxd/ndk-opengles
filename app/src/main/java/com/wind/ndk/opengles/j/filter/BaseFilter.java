package com.wind.ndk.opengles.j.filter;

import android.opengl.GLES20;

import com.wind.ndk.opengles.j.ShaderHelper;

/**
 * Created By wind
 * on 2020-01-12
 */
public abstract class BaseFilter {

    protected int mProgram;
    protected int mWidth;
    protected int mHeight;
    public BaseFilter(String vertexSource,String fragmentSource){
        //1. 编译着色器源码返回shaderId
        int vertexShaderId= ShaderHelper.compileShader(GLES20.GL_VERTEX_SHADER,vertexSource);
        int fragmentShaderId=ShaderHelper.compileShader(GLES20.GL_FRAGMENT_SHADER,fragmentSource);

        //2. 创建着色器程序
        mProgram=ShaderHelper.linkProgram(vertexShaderId,fragmentShaderId);
        System.out.println("mProgram:"+mProgram);

    }


    public  void onReady(int width, int height){
        mWidth=width;
        mHeight=height;
    }

    public abstract int onDrawFrame(int textureId);
}
