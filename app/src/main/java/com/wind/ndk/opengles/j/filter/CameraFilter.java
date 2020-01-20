package com.wind.ndk.opengles.j.filter;

import android.content.Context;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;

import com.wind.ndk.opengles.R;
import com.wind.ndk.opengles.j.GLESUtils;
import com.wind.ndk.opengles.j.TextResoureReader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created By wind
 * on 2020-01-13
 */
public class CameraFilter extends BaseFilter {

    private int mPositionLocation;
    private int mCoordLocation;
    private int mMatrixLocation;
    private int mTextureLocation;


    private int[] mFrameBuffer = new int[1];
    private int[] mFrameBufferTexture = new int[1];

    FloatBuffer mVertexBuffer;
    FloatBuffer mCoordBuffer;

    private float[] matrix;
    public CameraFilter(Context context) {
        super(TextResoureReader.readFromRaw(context, R.raw.camera_vertex),
                TextResoureReader.readFromRaw(context, R.raw.camera_fragment));


        mPositionLocation = GLES20.glGetAttribLocation(mProgram, "a_Position");
        mCoordLocation = GLES20.glGetAttribLocation(mProgram, "a_Coord");
        mMatrixLocation = GLES20.glGetUniformLocation(mProgram, "u_Matrix");
        mTextureLocation = GLES20.glGetUniformLocation(mProgram, "u_Texture");


        mVertexBuffer = ByteBuffer
                .allocateDirect(4 * 2 * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        float[] vertex={
                -1,-1,
                1,-1,
                -1,1,
                1,1
        };
        mVertexBuffer.put(vertex);

        mCoordBuffer= ByteBuffer
                .allocateDirect(4 * 2 * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer();

        float[] textureCoords={
                0,0,
                0,1,
                1,0,
                1,1
        };
        mCoordBuffer.put(textureCoords);
    }

    @Override
    public void onReady(int width, int height) {
        super.onReady(width, height);

        //生成fbo
        GLES20.glGenFramebuffers(1, mFrameBuffer, 0);
        //绑定当前fbo
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer[0]);
        //生成纹理
       /* GLES20.glGenTextures(1, mFrameBufferTexture, 0);
        //绑定当前纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, mFrameBufferTexture[0]);
        //设置纹理参数
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);*/
        GLESUtils.genTextures(mFrameBufferTexture);

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,mFrameBufferTexture[0]);
        //给纹理创建一张空白图
        GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGBA, width, height,
                0, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, null);

        //将纹理和fbo绑定
        GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER,
                GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D,
                mFrameBufferTexture[0], 0);

        //解绑纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
        //解绑fbo
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
    }

    @Override
    public int onDrawFrame(int textureId) {

        //绑定fbo
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFrameBuffer[0]);
        GLES20.glUseProgram(mProgram);

        mVertexBuffer.position(0);
        GLES20.glVertexAttribPointer(mPositionLocation,
                2,GLES20.GL_FLOAT,false,0,
                mVertexBuffer);
        GLES20.glEnableVertexAttribArray(mPositionLocation);

        mCoordBuffer.position(0);
        GLES20.glVertexAttribPointer(mCoordLocation,
                2,GLES20.GL_FLOAT,false,0,
                mCoordBuffer);
        GLES20.glEnableVertexAttribArray(mCoordLocation);

        //设置matrix
        GLES20.glUniformMatrix4fv(mMatrixLocation,1,false,matrix,0);


        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,textureId);
        GLES20.glUniform1i(mTextureLocation,0);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP,0,4);

        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,0);
        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER,0);



        return mFrameBufferTexture[0];
    }

    public void setMatrix(float[] mtx){
        this.matrix=mtx;
    }
}
