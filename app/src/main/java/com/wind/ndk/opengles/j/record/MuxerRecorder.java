package com.wind.ndk.opengles.j.record;

import android.content.Context;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.opengl.EGLContext;
import android.os.Handler;
import android.os.HandlerThread;
import android.view.Surface;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Queue;
import java.util.concurrent.LinkedBlockingDeque;

/**
 * Created By wind
 * on 2020-01-19
 */
public class MuxerRecorder implements Runnable,BaseRecorder.OnRecordListener {


    //复用器
    MediaMuxer mMediaMuxer;
    private VideoRecorder mVideoRecorder;
    private AudioRecorder mAudioRecorder;
    private LinkedBlockingDeque<Frame> mFrameQueue;
    public MuxerRecorder(Context context, EGLContext shareEGLContext, int width, int height, String outputPath) throws IOException{


        /**
         *  创建封装器（复用器）
         */
        mMediaMuxer = new MediaMuxer(outputPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);


        mVideoRecorder=new VideoRecorder(mMediaMuxer,context,shareEGLContext,width,height);


        mAudioRecorder=new AudioRecorder(mMediaMuxer,44100,1);

        mVideoRecorder.setOnRecordListener(this);
        mAudioRecorder.setOnRecordListener(this);


        mFrameQueue=new LinkedBlockingDeque<>();


    }
    private boolean mRecording;
    private Thread mMuxerThread;
    public void start(float speed) throws IOException {
        mRecording=true;
        mFrameQueue.clear();
        mVideoRecorder.setSpeed(speed);
        mVideoRecorder.start();
        mAudioRecorder.start();

        mMuxerThread=new Thread(this);


    }


    public void stop(){
        mRecording=false;
        mVideoRecorder.stop();
        mAudioRecorder.stop();
        mMuxerStarted=false;


    }


    public void encodeFrame(int textureId, long timestamp) {
        mVideoRecorder.encodeFrame(textureId,timestamp);
    }
    private boolean mMuxerStarted;
    @Override
    public void onAddTrack(String mimeType) {
        if (!mMuxerStarted && mVideoRecorder.isAddTrack()&& mAudioRecorder.isAddTrack()){
            mMediaMuxer.start();
            mMuxerStarted=true;
            mMuxerThread.start();
        }

    }



    @Override
    public void onRecordStop() {
        if (!mVideoRecorder.isRecording()&& !mAudioRecorder.isRecording()){
            if (mMediaMuxer!=null){
                try {
                    mMediaMuxer.stop();
                    mMediaMuxer.release();
                }catch (Exception e){
                    e.printStackTrace();
                }

                mMediaMuxer=null;
            }
        }
    }

    @Override
    public void onFramePrepared(Frame frame) {
        mFrameQueue.offer(frame);
    }

    @Override
    public void run() {
        while (mRecording){
            if (!mFrameQueue.isEmpty()){
                Frame frame=mFrameQueue.poll();
                mMediaMuxer.writeSampleData(frame.getTrackIndex(),frame.getByteBuffer(),frame.getBufferInfo());
            }
        }
    }
}
