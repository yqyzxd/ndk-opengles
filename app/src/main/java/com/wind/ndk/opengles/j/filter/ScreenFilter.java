package com.wind.ndk.opengles.j.filter;

import android.content.Context;
import android.opengl.GLES20;

import com.wind.ndk.opengles.R;
import com.wind.ndk.opengles.j.TextResoureReader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created By wind
 * on 2020-01-12
 */
public class ScreenFilter extends BaseFilter {
    int mPositionLocation;
    int mCoordLocation;
    int mTextureLocation;
    FloatBuffer mPositionBuffer;
    FloatBuffer mCoordBuffer;


    public ScreenFilter(Context context){
        super(TextResoureReader.readFromRaw(context, R.raw.screen_vertex),
                TextResoureReader.readFromRaw(context,R.raw.screen_fragment));


        //bitmap= BitmapFactory.decodeResource(context.getResources(),R.drawable.iwaka);


        //获取 着色器源码中属性的位置
        mPositionLocation=GLES20.glGetAttribLocation(mProgram,"a_Position");
        mCoordLocation=GLES20.glGetAttribLocation(mProgram,"a_Coord");


        mTextureLocation=GLES20.glGetUniformLocation(mProgram,"u_Texture");


        mPositionBuffer=ByteBuffer.allocateDirect(4*2*4).order(ByteOrder.nativeOrder())
            .asFloatBuffer();
        //opengl 世界坐标系中的顶点坐标
        float [] postion={
               -1f,1f,
                1f,1f,
                -1f,-1f,
                1f,-1f
        };
        mPositionBuffer.put(postion);
        mPositionBuffer.position(0);

        mCoordBuffer=ByteBuffer.allocateDirect(4*2*4).order(ByteOrder.nativeOrder())
                .asFloatBuffer();
        //opengl中的纹理坐标  直接使用此坐标绘制出的图片会上下颠倒
        float [] coord={
                0f,1f,
                1f,1f,
                0f,0f,
                1f,0f
        };
        /* 需要将纹理转化为手机屏幕坐标才能在手机上正确显示
           纹理坐标 转化为手机屏幕坐标只需要执行一次vflip即可，即上下翻转
         */
        //手机屏幕坐标
        float [] screenCoord={ //使用此坐标绘制出的图片没有上下颠倒，但是图片会被缩放，可以通过计算图片与屏幕比例，取相应的纹理坐标
                0f,0f,
                1f,0f,
                0f,1f,
                1f,1f
        };

        mCoordBuffer.put(screenCoord);
        mCoordBuffer.position(0);
    }


    @Override
    public int onDrawFrame(int textureId) {
        //使用着色器程序 务必执行
        GLES20.glUseProgram(mProgram);
        //设置视口
        //GLES20.glViewport(0,0,mWidth,mHeight);
        //设置顶点坐标 buffer定位到0，务必执行
        mPositionBuffer.position(0);
        GLES20.glVertexAttribPointer(mPositionLocation,2,GLES20.GL_FLOAT,
                false,0,mPositionBuffer);
        GLES20.glEnableVertexAttribArray(mPositionLocation);
        //设置纹理坐标
        mCoordBuffer.position(0);
        GLES20.glVertexAttribPointer(mCoordLocation,2,GLES20.GL_FLOAT,
                false,0,mCoordBuffer);
        GLES20.glEnableVertexAttribArray(mCoordLocation);

        //激活第1个纹理
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        //绑定纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,textureId);
        //将bitmap上传到纹理
       // GLESUtils.texImage2D(GLES20.GL_TEXTURE_2D,0,bitmap,0);
        //设置纹理
        GLES20.glUniform1i(mTextureLocation,0);
        //通过三角形带形式绘制，总共4个点->绘制出一个矩形
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP,0,4);

        //解除绑定
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,0);
       /* GLES20.glDisableVertexAttribArray(mPositionLocation);
        GLES20.glDisableVertexAttribArray(mCoordLocation);*/


        return textureId;
    }
}
