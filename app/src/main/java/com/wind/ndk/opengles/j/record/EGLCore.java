package com.wind.ndk.opengles.j.record;

import android.opengl.EGLConfig;
import android.opengl.EGLContext;
import android.opengl.EGLDisplay;
import android.opengl.EGLExt;
import android.opengl.EGLSurface;
import android.view.Surface;

import static android.opengl.EGL14.*;
import static android.opengl.EGL14.EGL_BLUE_SIZE;
import static android.opengl.EGL14.EGL_DEFAULT_DISPLAY;
import static android.opengl.EGL14.EGL_GREEN_SIZE;
import static android.opengl.EGL14.EGL_NONE;
import static android.opengl.EGL14.EGL_NO_DISPLAY;
import static android.opengl.EGL14.EGL_OPENGL_ES2_BIT;
import static android.opengl.EGL14.EGL_RED_SIZE;
import static android.opengl.EGL14.EGL_RENDERABLE_TYPE;
import static android.opengl.EGL14.eglChooseConfig;
import static android.opengl.EGL14.eglGetDisplay;
import static android.opengl.EGL14.eglInitialize;

/**
 * Created By wind
 * on 2020-01-20
 *
 * EGL环境类
 */
public class EGLCore {

    EGLDisplay mEGLDisplay;
    EGLConfig mEGLConfig;
    EGLContext mEGLContext;
    public EGLCore(EGLContext shareEGLContext){
        //1， 创建 EGL环境
        createEGL(shareEGLContext);
    }

    private void createEGL(EGLContext shareEGLContext) {
        //1，获取显示设备, 默认设备：手机屏幕
        mEGLDisplay=eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mEGLDisplay==null || mEGLDisplay==EGL_NO_DISPLAY){
            throw new IllegalStateException("eglGetDisplay failed");
        }
        //2, 初始化设备
        int []version=new int[2];
        if (!eglInitialize(mEGLDisplay,version,0,version,1)){
            throw new IllegalStateException("eglInitialize failed");
        }

        /**
         *  EGLDisplay dpy,
         *         int[] attrib_list,
         *         int attrib_listOffset,
         *         EGLConfig[] configs,
         *         int configsOffset,
         *         int config_size,
         *         int[] num_config,
         *         int num_configOffset
         */
        //3, 选择配置
        int[] attrib_list ={
                //指定像素格式 rgba
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                //指定渲染 api 类型
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,//版本
                //告诉EGL，创建surface的行为必须是视频编解码器所能兼容的
                EGLExt.EGL_RECORDABLE_ANDROID, 1,
                EGL_NONE// 结尾符
        };
        int[] num_config=new int[1];
        EGLConfig[] eglConfigs=new EGLConfig[1];
        if(!eglChooseConfig(mEGLDisplay,//显示设备
                attrib_list,//属性列表
                0,
                eglConfigs,
                0,
                1,
                num_config,
                0)){
            throw new IllegalStateException("eglChooseConfig failed");
        }

        mEGLConfig=eglConfigs[0];
        //4， 创建上下文
        int[] ctx_attrib_list = {
                EGL_CONTEXT_CLIENT_VERSION, 2,//context 版本
                EGL_NONE
        };
        mEGLContext = eglCreateContext(
                mEGLDisplay, //显示设备
                mEGLConfig,//上一步获取的配置
                shareEGLContext,// 共享上下文，传绘制线程（GLThread）中的EGL上下文，共享资源（发生关系）
                ctx_attrib_list,//上下文的属性列表
                0);

        if (mEGLContext==null ||mEGLContext==EGL_NO_CONTEXT){
            throw new IllegalStateException("eglCreateContext failed");
        }
    }


    public EGLSurface createWindowSurface(Surface surface){
        int[] attrib_list = {
                EGL_NONE
        };
        EGLSurface eglSurface=eglCreateWindowSurface(mEGLDisplay,mEGLConfig,surface,attrib_list,0);

        if (eglSurface==null || eglSurface==EGL_NO_SURFACE){
            return null;
        }
        return eglSurface;
    }

    public boolean makeCurrent(EGLSurface eglSurface){
        return eglMakeCurrent(mEGLDisplay,eglSurface,eglSurface,mEGLContext);
    }
    public void releaseSurface(EGLSurface eglSurface){
        eglDestroySurface(mEGLDisplay,eglSurface);
    }
    public boolean swapBuffers(EGLSurface eglSurface){
        return eglSwapBuffers(mEGLDisplay,eglSurface);
    }

    public boolean eglPresentationTimeANDROID(EGLSurface eglSurface,long timestamp){
        //刷新mEGLSurface的时间戳
        //如果设置不合理，编码的时候会采取丢帧或以低质量的编码方式进行编码
        return EGLExt.eglPresentationTimeANDROID(mEGLDisplay, eglSurface, timestamp);
    }
    public void release(){
        eglMakeCurrent(mEGLDisplay,EGL_NO_SURFACE,EGL_NO_SURFACE,EGL_NO_CONTEXT);
        eglDestroyContext(mEGLDisplay,mEGLContext);
        eglReleaseThread();
        eglTerminate(mEGLDisplay);
    }
}
