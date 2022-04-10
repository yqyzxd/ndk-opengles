package com.wind.ndk.opengles.j.record;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.media.MediaMuxer;
import android.media.MediaRecorder;

import java.io.IOException;
import java.nio.ByteBuffer;

public class AudioRecorder extends BaseRecorder {
    private int mSampleRate;
    private int mChannelCount;

    public AudioRecorder(int sampleRate, int channelCount) {
        mSampleRate = sampleRate;
        mChannelCount = channelCount;
    }


    private AudioRecord mAudioRecord;
    private int mBufferSize;

    @Override
    public void start() throws IOException {
        super.start();
        mStop=false;
        // aac
        initAudioEncoder(MediaFormat.MIMETYPE_AUDIO_AAC, mSampleRate, mChannelCount);

        mBufferSize = AudioRecord.getMinBufferSize(mSampleRate, mChannelCount, AudioFormat.ENCODING_PCM_16BIT);
        mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRate, mChannelCount, AudioFormat.ENCODING_PCM_16BIT, mBufferSize);

        queueEvent(new RecordRunnable());


    }
    private boolean mStop;
    @Override
    public void stop() {
        super.stop();
        mStop=true;
        queueEvent(new Runnable() {
            @Override
            public void run() {

                if (mMediaCodec!=null){
                    mMediaCodec.stop();
                    mMediaCodec.release();
                    mMediaCodec=null;
                    mTrackIndex=-1;
                    mRecording=false;
                    if (mOnRecordStateChangedListener!=null)
                        mOnRecordStateChangedListener.onRecordStop();
                }

                mHandler.getLooper().quitSafely();
                mHandler=null;
            }
        });
    }

    private void initAudioEncoder(String mineType, int sampleRate, int channel) {
        try {
            mMediaCodec = MediaCodec.createEncoderByType(mineType);
            MediaFormat audioFormat = MediaFormat.createAudioFormat(mineType, sampleRate, channel);
            audioFormat.setInteger(MediaFormat.KEY_BIT_RATE, sampleRate*16*channel);
            audioFormat.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC);
            audioFormat.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, 20*1024);
            mMediaCodec.configure(audioFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);


        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private long mStartTime;
    private class RecordRunnable implements Runnable {

        @Override
        public void run() {

            mAudioRecord.startRecording();

            mMediaCodec.start();
            mStartTime=System.nanoTime()/1000l;
            while (mRecording && !mStop/*mAudioRecord.getRecordingState() == AudioRecord.RECORDSTATE_RECORDING*/) {
                byte[] audioSamples = new byte[mBufferSize];
                int size = mAudioRecord.read(audioSamples, 0, mBufferSize);
                if (size > 0) {
                    int inputBufferIndex = mMediaCodec.dequeueInputBuffer(0);
                    if (inputBufferIndex >= 0) {
                        ByteBuffer byteBuffer = mMediaCodec.getInputBuffers()[inputBufferIndex];
                        byteBuffer.clear();
                        byteBuffer.put(audioSamples);
                        mMediaCodec.queueInputBuffer(inputBufferIndex, 0, size, getPTSUs(), 0);
                    }

                    MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();


                    while (true) {

                        int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(bufferInfo, 10_000);
                        System.out.println("AudioRecord run outputBufferIndex->"+outputBufferIndex);
                        if (outputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                            break;//稍后重试
                        } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                            MediaFormat outputFormat = mMediaCodec.getOutputFormat();
                            mTrackIndex = mMediaMuxer.addTrack(outputFormat);
                            if (mOnRecordStateChangedListener != null) {
                                mOnRecordStateChangedListener.onAddTrack(outputFormat.getString(MediaFormat.KEY_MIME));
                            }
                        } else if (outputBufferIndex >= 0) {
                            ByteBuffer outputBuffer = mMediaCodec.getOutputBuffers()[outputBufferIndex];
                            if (outputBuffer!=null) {
                                //如果取到 outputBuffer 是配置信息
                                if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                                    bufferInfo.size = 0;
                                }

                                if (bufferInfo.size != 0) {
                                    //偏移位置
                                    outputBuffer.position(bufferInfo.offset);
                                    //可读写的总长度
                                    outputBuffer.limit(bufferInfo.offset + bufferInfo.size);
                                    //bufferInfo.presentationTimeUs = getPTSUs();
                                    //写数据（输出）
                                    if (mOnRecordStateChangedListener != null) {
                                        mOnRecordStateChangedListener.onFramePrepared(new Frame(outputBuffer, bufferInfo, mTrackIndex));
                                    }
                                    prevOutputPTSUs = bufferInfo.presentationTimeUs;

                                }
                            }
                            //一定要. 使用完输出缓冲区，就可以回收了，让mMediaCodec 能继续使用
                            mMediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                            //结束
                            if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                                System.out.println("AudioRecord break");
                                break;
                            }
                        }


                    }
                }
            }
        }
    }
    private long prevOutputPTSUs;
    private long getPTSUs() {
       /* long result = System.nanoTime() / 1000L;
        result= result < prevOutputPTSUs ? prevOutputPTSUs : result;
        return result-mStartTime;*/

        long result = System.nanoTime() / 1000L;
        return result < prevOutputPTSUs ? prevOutputPTSUs : result;
    }


}
