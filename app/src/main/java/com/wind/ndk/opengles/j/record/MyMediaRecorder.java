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

/**
 * Created By wind
 * on 2020-01-19
 */
public class MyMediaRecorder {

    MediaCodec mMediaCodec;
    Surface mInputSurface;
    private Handler mHandler;
    private int mWidth;
    private int mHeight;
    //复用器
    MediaMuxer mMediaMuxer;
    private String mOutputPath;
    EGLRenderer mEGLRenderer;
    private boolean mStarted;
    private Context mContext;
    private EGLContext mShareEGLContext;

    private int index;
    public MyMediaRecorder(Context context, EGLContext shareEGLContext, int width, int height, String outputPath) {
        this.mWidth = width;
        this.mHeight = height;
        this.mOutputPath = outputPath;

        this.mContext=context;
        this.mShareEGLContext=shareEGLContext;



    }


    public void start(float speed) throws IOException {


        /**
         * 1, 创建 MediaCodec编码器
         */
        //创建哪种视频编码器(H.264)
        mMediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
        /**
         * 2，配置编码器
         */
        //视频格式
        MediaFormat videoFormat = MediaFormat
                .createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, mWidth, mHeight);
        //比特率（码率） 1500 Kbps
        videoFormat.setInteger(MediaFormat.KEY_BIT_RATE, 1500_000);
        //帧率 30fps
        videoFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 30);
        //颜色格式（从Surface中获取）
        videoFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT,
                MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
        //关键帧间隔
        videoFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 20);
        //配置编码器
        mMediaCodec.configure(videoFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        /**
         * 3，创建输入Surface（虚拟屏）
         */
        mInputSurface = mMediaCodec.createInputSurface();
        /**
         * 4, 创建封装器（复用器）
         */
        mMediaMuxer = new MediaMuxer(mOutputPath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);


        /**
         * 5, 配置 EGL 环境
         */
        HandlerThread handlerThread = new HandlerThread("encodeThread");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());
        mHandler.post(new Runnable() {
            @Override
            public void run() {

                mEGLRenderer=new EGLRenderer(mContext,mShareEGLContext,mInputSurface);
                mEGLRenderer.onReady(mWidth,mHeight);
                mMediaCodec.start();
                mStarted=true;
            }
        });
    }


    public void stop(){
        mStarted=false;

        mHandler.post(new Runnable() {
            @Override
            public void run() {
                getEncodedData(true);
                if (mMediaCodec!=null){
                    mMediaCodec.stop();
                    mMediaCodec.release();
                    mMediaCodec=null;
                }
                if (mMediaMuxer!=null){
                    try {
                        mMediaMuxer.stop();
                        mMediaMuxer.release();
                    }catch (Exception e){
                        e.printStackTrace();
                    }

                    mMediaMuxer=null;
                }
                if (mInputSurface!=null){
                    mInputSurface.release();
                    mInputSurface=null;
                }
                mEGLRenderer.release();
                mHandler.getLooper().quitSafely();
                mHandler=null;
            }
        });
    }

    public void encodeFrame(final int textureId, final long timestamp){
        if (!mStarted){
            return;
        }
        if (mHandler!=null){
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    //把纹理图像画到了虚拟屏上
                    if (mEGLRenderer!=null){
                        mEGLRenderer.draw(textureId,timestamp);
                    }
                    //然后从编码器中的输出缓冲区去获取编码后的数据
                    getEncodedData(false);
                }
            });
        }
    }

    private void getEncodedData(boolean endOfStream){
        if (endOfStream){
            //不录，发送一个通知给mMediaCodec
            mMediaCodec.signalEndOfInputStream();
        }
        MediaCodec.BufferInfo bufferInfo=new MediaCodec.BufferInfo();
        while (true){
            int status=mMediaCodec.dequeueOutputBuffer(bufferInfo,10_000);
            if (status==MediaCodec.INFO_TRY_AGAIN_LATER){
                //如果是 endOfStream == true： 录制. 继续循环，表示不会等待 接收新的 编码图像数据
                //保证所有待编码的数据 都能编码完
                if (!endOfStream) {
                    //如果是 endOfStream == false： 结束录制. 跳出循环
                    break;
                }
            }else if (status==MediaCodec.INFO_OUTPUT_FORMAT_CHANGED){//输出格式变更了
                MediaFormat outputFormat=mMediaCodec.getOutputFormat();
                index=mMediaMuxer.addTrack(outputFormat);
                //开始封装
                mMediaMuxer.start();
            }else if (status==MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED){

            }else {
                ByteBuffer outputBuffer=mMediaCodec.getOutputBuffer(status);
                if (outputBuffer!=null){
                    //如果取到 outputBuffer 是配置信息
                    if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG)!=0){
                        bufferInfo.size=0;
                    }
                    if (bufferInfo.size!=0){
                        outputBuffer.position(bufferInfo.offset);
                        //可读写的总长度
                        outputBuffer.limit(bufferInfo.offset+bufferInfo.size);

                        //写数据（输出）
                        try {
                            mMediaMuxer.writeSampleData(index, outputBuffer, bufferInfo);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }
                //一定要. 使用完输出缓冲区，就可以回收了，让mMediaCodec 能继续使用
                mMediaCodec.releaseOutputBuffer(status, false);
                //结束
                if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    break;
                }
            }
        }
    }
}
