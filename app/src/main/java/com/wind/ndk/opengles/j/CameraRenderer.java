package com.wind.ndk.opengles.j;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.EGL14;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Environment;

import com.wind.ndk.opengles.CameraHelper;
import com.wind.ndk.opengles.j.filter.CameraFilter;
import com.wind.ndk.opengles.j.filter.ScreenFilter;
import com.wind.ndk.opengles.j.record.MyMediaRecorder;

import java.io.File;
import java.io.IOException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created By wind
 * on 2020-01-13
 */
public class CameraRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private GLSurfaceView mGLSurfaceView;
    CameraHelper mCameraHelper;
    SurfaceTexture mSurfaceTexture;
    int []mTexture=new int[1];
    private float[] mtx=new float[16];

    CameraFilter mCameraFilter;
    ScreenFilter mScreenFilter;

    MyMediaRecorder mMediaRecorder;


    private Speed mSpeed = Speed.MODE_NORMAL;

    public enum Speed {
        MODE_EXTRA_SLOW, MODE_SLOW, MODE_NORMAL, MODE_FAST, MODE_EXTRA_FAST
    }

    public CameraRenderer(GLSurfaceView glSurfaceView){
        this.mGLSurfaceView=glSurfaceView;
    }



    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        mCameraHelper=new CameraHelper(Camera.CameraInfo.CAMERA_FACING_FRONT);

        GLES20.glGenTextures(1,mTexture,0);
        mSurfaceTexture=new SurfaceTexture(mTexture[0]);
        mSurfaceTexture.setOnFrameAvailableListener(this);


        mSurfaceTexture.getTransformMatrix(mtx);

        mCameraFilter=new CameraFilter(mGLSurfaceView.getContext());
        mCameraFilter.setMatrix(mtx);


        mScreenFilter=new ScreenFilter(mGLSurfaceView.getContext());

        EGLContext eglContext=EGL14.eglGetCurrentContext();
        File externalDir=mGLSurfaceView.getContext().getExternalFilesDir(Environment.DIRECTORY_MOVIES);
        mMediaRecorder=new MyMediaRecorder(mGLSurfaceView.getContext(),
                eglContext,480,640, externalDir.getAbsolutePath()+"/test.mp4"
        );
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        mCameraFilter.onReady(width,height);
        mScreenFilter.onReady(width,height);

      //  System.out.println("externalDir:"+externalDir.getAbsolutePath());

        GLES20.glViewport(0,0,width,height);
        GLES20.glClearColor(0,0,0,1);
        mCameraHelper.startPreview(mSurfaceTexture);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT|GLES20.GL_DEPTH_BUFFER_BIT);
        mSurfaceTexture.updateTexImage();
        mSurfaceTexture.getTransformMatrix(mtx);
        mCameraFilter.setMatrix(mtx);

        int textureId=mCameraFilter.onDrawFrame(mTexture[0]);
        mScreenFilter.onDrawFrame(textureId);


        mMediaRecorder.encodeFrame(textureId,mSurfaceTexture.getTimestamp());
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        mGLSurfaceView.requestRender();
    }

    /**
     * 开始录制
     */
    public void startRecording(){
        float speed = 1.0f;
        switch (mSpeed){
            case MODE_EXTRA_SLOW:
                speed = 0.3f;
                break;
            case MODE_SLOW:
                speed = 0.5f;
                break;
            case MODE_NORMAL:
                speed = 1.0f;
                break;
            case MODE_FAST:
                speed = 1.5f;
                break;
            case MODE_EXTRA_FAST:
                speed = 3.0f;
                break;

        }
        try {
            if (mMediaRecorder!=null)
                mMediaRecorder.start(speed);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void stopRecording(){
        if (mMediaRecorder!=null)
            mMediaRecorder.stop();
    }

    public void setSpeed(Speed speed) {
        mSpeed = speed;
    }


}
