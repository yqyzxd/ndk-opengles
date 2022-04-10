package com.wind.ndk.opengles.j.record;

import android.media.MediaCodec;

import java.nio.ByteBuffer;

public class Frame {

    private ByteBuffer byteBuffer;
    private MediaCodec.BufferInfo bufferInfo;
    private int trackIndex;
    public Frame(ByteBuffer byteBuffer, MediaCodec.BufferInfo bufferInfo,int trackIndex) {
        this.byteBuffer = byteBuffer;
        this.bufferInfo = bufferInfo;
        this.trackIndex=trackIndex;
    }

    public MediaCodec.BufferInfo getBufferInfo() {
        return bufferInfo;
    }

    public ByteBuffer getByteBuffer() {
        return byteBuffer;
    }

    public int getTrackIndex() {
        return trackIndex;
    }
}
