package com.wind.ndk.opengles.j.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

public class BufferHelper {

    public static FloatBuffer getFloatBuffer(float[] vertices) {

        //分配一块本地内存（不受GC管理）
        FloatBuffer floatBuffer=ByteBuffer
                .allocateDirect(vertices.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();

        floatBuffer.put(vertices);
        floatBuffer.position(0);
        return floatBuffer;
    }
}
