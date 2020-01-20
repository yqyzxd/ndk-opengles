package com.wind.ndk.opengles.j.record;

import android.content.Context;
import android.opengl.EGLContext;
import android.opengl.EGLSurface;
import android.view.Surface;

import com.wind.ndk.opengles.j.filter.ScreenFilter;

/**
 * Created By wind
 * on 2020-01-20
 */
public class EGLRenderer {


    EGLCore mEGLCore;
    EGLSurface mEGLSurface;
    ScreenFilter mScreenFilter;
    public EGLRenderer(Context context, EGLContext shareContext, Surface surface){
        this.mEGLCore=new EGLCore(shareContext);

        mEGLSurface=mEGLCore.createWindowSurface(surface);


        boolean makeCurrent=mEGLCore.makeCurrent(mEGLSurface);
        if (!makeCurrent){
            throw new IllegalStateException("makeCurrent failed");
        }
        //waring: makeCurrent之后才能使用opengles api创建opengl程序，否则GLES20.glCreateShader(type)返回0
        mScreenFilter=new ScreenFilter(context);
    }





    public void onReady(int width,int height){
        mScreenFilter.onReady(width,height);
    }


    /**
     *
     * @param textureID
     * @param timestamp 单位微秒
     */
    public void draw(int textureID, long timestamp){

        //渲染到虚拟屏幕
        mScreenFilter.onDrawFrame(textureID);
        //刷新mEGLSurface的时间戳
        //如果设置不合理，编码的时候会采取丢帧或以低质量的编码方式进行编码
        mEGLCore.eglPresentationTimeANDROID(mEGLSurface,timestamp);
        //交换缓冲数据（看资料《EGL接口解析与理解 》eglSwapBuffers接口实现说明）
        boolean swapBuffers=mEGLCore.swapBuffers(mEGLSurface);
        if (!swapBuffers){
            throw new IllegalStateException("swapBuffers failed");
        }
    }


    public void release() {
        mEGLCore.releaseSurface(mEGLSurface);
        mEGLCore.release();

        mEGLCore=null;
    }
}
