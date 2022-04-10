package com.wind.ndk.opengles.j.filter;

import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniformMatrix4fv;

import android.content.Context;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;

import com.wind.ndk.opengles.R;
import com.wind.ndk.opengles.j.GLESUtils;
import com.wind.ndk.opengles.j.TextResourceReader;
import com.wind.ndk.opengles.j.util.BufferHelper;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created By wind
 * on 2020-01-13
 */
public class CameraFilter extends BaseFboFilter {
    private float[] matrix;
    private int uMatrixLocation;
    public CameraFilter(Context context) {
        super(context, R.raw.camera_vertex, R.raw.camera_fragment);
    }
    protected FloatBuffer getTextureBuffer() {
        float[] texture = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,
        };
        return BufferHelper.getFloatBuffer(texture);
    }
    @Override
    protected void getLocation(int program) {
        super.getLocation(program);


        uMatrixLocation = glGetUniformLocation(program, "u_Matrix");
    }

    @Override
    protected void inflateLocation() {

        //变换矩阵
        glUniformMatrix4fv(uMatrixLocation, 1, false, matrix, 0);
    }

    @Override
    protected int getTextureTarget() {
        return GLES11Ext.GL_TEXTURE_EXTERNAL_OES;
    }


    public void setMatrix(float[] mtx) {
        matrix = mtx;
    }
}
