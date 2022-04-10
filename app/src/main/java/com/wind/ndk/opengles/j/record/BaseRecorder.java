package com.wind.ndk.opengles.j.record;

import android.media.MediaCodec;
import android.media.MediaMuxer;
import android.os.Handler;
import android.os.HandlerThread;

import java.io.IOException;
import java.util.Map;

public abstract class BaseRecorder {
    protected MediaMuxer mMediaMuxer;
    protected Handler mHandler;
    protected MediaCodec mMediaCodec;
    protected boolean mRecording;

    protected int mTrackIndex=-1;
    public BaseRecorder(MediaMuxer mediaMuxer) {
        this.mMediaMuxer = mediaMuxer;
    }

    public void start() throws IOException {
        mRecording=true;
        HandlerThread handlerThread = new HandlerThread("EncodeThread");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());
    }


    public void queueEvent(Runnable runnable) {
        mHandler.post(runnable);
    }


    public void stop(){


        mTrackIndex=-1;
        mRecording=false;
        queueEvent(new Runnable() {
            @Override
            public void run() {

                if (mMediaCodec!=null){
                    mMediaCodec.stop();
                    mMediaCodec.release();
                    mMediaCodec=null;

                    if (mOnRecordStateChangedListener!=null)
                        mOnRecordStateChangedListener.onRecordStop();
                }

                mHandler.getLooper().quitSafely();
                mHandler=null;
            }
        });

    }

    public boolean isAddTrack(){
       return mTrackIndex>=0;
    }

    public boolean isRecording(){
        return mRecording;
    }

    protected OnRecordListener mOnRecordStateChangedListener;
    public void setOnRecordListener(OnRecordListener listener){
        mOnRecordStateChangedListener=listener;
    }

    interface OnRecordListener{
        void onAddTrack(String mimeType);
        void onRecordStop();

        void onFramePrepared(Frame frame);
    }

}
