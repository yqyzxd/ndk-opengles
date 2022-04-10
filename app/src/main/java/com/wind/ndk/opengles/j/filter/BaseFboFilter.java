package com.wind.ndk.opengles.j.filter;

import static android.opengl.GLES20.GL_COLOR_ATTACHMENT0;
import static android.opengl.GLES20.GL_FRAMEBUFFER;
import static android.opengl.GLES20.GL_RGBA;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_UNSIGNED_BYTE;
import static android.opengl.GLES20.glBindFramebuffer;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glDeleteFramebuffers;
import static android.opengl.GLES20.glDeleteTextures;
import static android.opengl.GLES20.glFramebufferTexture2D;
import static android.opengl.GLES20.glGenFramebuffers;
import static android.opengl.GLES20.glTexImage2D;

import android.content.Context;
import android.opengl.GLES11Ext;

import com.wind.ndk.opengles.j.util.BufferHelper;
import com.wind.ndk.opengles.j.util.TextureHelper;

import java.nio.FloatBuffer;

public class BaseFboFilter extends BaseFilter{
    protected int[] mFrameBuffers;
    protected int[] mFrameBufferTextures;

    public BaseFboFilter(Context context, int vertexSourceId, int fragmentSourceId) {
        super(context, vertexSourceId, fragmentSourceId);
    }


    @Override
    protected FloatBuffer getTextureBuffer() {
        float[] texture = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
        };
        return BufferHelper.getFloatBuffer(texture);
    }

    @Override
    public void onReady(int width, int height) {
        super.onReady(width, height);
        if (mFrameBuffers != null) {
            releaseFrameBuffers();
        }
        //FBO
        //1， 创建fbo
        mFrameBuffers = new int[1];
        //int n, fbo个数
        //int[] framebuffers, 保存fbo id的数组
        //int offset 数组中第几个来保存
        glGenFramebuffers(mFrameBuffers.length, mFrameBuffers, 0);

        //2，创建属于fbo的纹理
        mFrameBufferTextures = new int[1];
        //生成并配置纹理
        TextureHelper.genTextures(mFrameBufferTextures);

        //3, 让fbo与纹理发生关系
        //生成2d纹理图像
        // 目标 2d纹理 + 等级 + 格式 +宽 + 高 + 边界 + 格式 + 数据类型(byte) + 像素数据
        glBindTexture(GL_TEXTURE_2D, mFrameBufferTextures[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                null);

        //让fbo与纹理绑定起来
        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffers[0]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                mFrameBufferTextures[0], 0);

        //解绑
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    @Override
    public int onDrawFrame(int textureId) {
        //这里是因为要渲染到FBO缓存中，而不是直接显示到屏幕上
        glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffers[0]);
        super.onDrawFrame(textureId);

        //解绑fbo
        glBindTexture(getTextureTarget(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        return textureID;//Bug
        return mFrameBufferTextures[0];//返回fbo的纹理id
    }

    @Override
    public void release() {
        super.release();
        releaseFrameBuffers();
    }

    private void releaseFrameBuffers() {
        //删除fbo的纹理
        if (mFrameBufferTextures != null) {
            glDeleteTextures(1, mFrameBufferTextures, 0);
            mFrameBufferTextures = null;
        }
        //删除fbo
        if (mFrameBuffers != null) {
            glDeleteFramebuffers(1, mFrameBuffers, 0);
            mFrameBuffers = null;
        }
    }
}
