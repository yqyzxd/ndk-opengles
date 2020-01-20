package com.wind.ndk.opengles.j;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.wind.ndk.opengles.j.filter.ScreenFilter;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created By wind
 * on 2020-01-12
 */
public class MyGLRenderer implements GLSurfaceView.Renderer {
    MyGLSurfaceView mGlSurfaceView;
    ScreenFilter mScreenFilter;
    int []textures;
    public MyGLRenderer(MyGLSurfaceView glSurfaceView) {
        mGlSurfaceView = glSurfaceView;
    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

        mScreenFilter=new ScreenFilter(mGlSurfaceView.getContext());
        textures=new int[1];
        /**
         *          int n,              创建纹理个数
         *         int[] textures,      存放纹理id数组
         *         int offset           数组中存放位置从哪里开始
         */
        GLES20.glGenTextures(1,textures,0);
        //绑定纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,textures[0]);
        /**
         * 设置纹理参数
         * GL_TEXTURE_MAG_FILTER 纹理需要放大时，使用哪个过滤算法
         * GL_TEXTURE_MIN_FILTER 纹理需要缩小时，使用哪个过滤算法
         * GL_LINEAR 线性过滤
         * GL_NEAREST最邻近过滤
         *
         * GL_TEXTURE_WRAP_S 纹理s方向扩展方式
         * GL_TEXTURE_WRAP_T 纹理t方向扩展方式 这里指定为GL_CLAMP_TO_EDGE
         * GL_CLAMP_TO_EDGE 扩展边缘像素
         * GL_REPEAT 重复显示
         *
         */
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_MAG_FILTER,GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_MIN_FILTER,GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_WRAP_S,GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_WRAP_T,GLES20.GL_CLAMP_TO_EDGE);
        //设置完之后需要解绑纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,0);
      }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        mScreenFilter.onReady(width,height);

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        //设置清屏色
        GLES20.glClearColor(1,0,0,1);
        //清除颜色缓冲区
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);


        mScreenFilter.onDrawFrame(textures[0]);

    }
}
