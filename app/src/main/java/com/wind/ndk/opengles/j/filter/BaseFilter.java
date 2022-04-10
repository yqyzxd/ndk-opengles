package com.wind.ndk.opengles.j.filter;

import static android.opengl.GLES20.GL_FLOAT;
import static android.opengl.GLES20.GL_FRAGMENT_SHADER;
import static android.opengl.GLES20.GL_TEXTURE0;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TRIANGLE_STRIP;
import static android.opengl.GLES20.GL_VERTEX_SHADER;
import static android.opengl.GLES20.glActiveTexture;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glDeleteProgram;
import static android.opengl.GLES20.glDeleteShader;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glEnableVertexAttribArray;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniform1i;
import static android.opengl.GLES20.glUseProgram;
import static android.opengl.GLES20.glVertexAttribPointer;
import static android.opengl.GLES20.glViewport;

import android.content.Context;
import android.opengl.GLES20;

import com.wind.ndk.opengles.j.ShaderHelper;
import com.wind.ndk.opengles.j.TextResourceReader;
import com.wind.ndk.opengles.j.util.BufferHelper;

import java.nio.FloatBuffer;

/**
 * Created By wind
 * on 2020-01-12
 */
public abstract class BaseFilter {

    protected int mProgram;
    protected int mWidth;
    protected int mHeight;

    protected final FloatBuffer vertexData;
    protected final FloatBuffer textureData;
    //顶点坐标
    protected int mPositionLocation;
    //纹理坐标
    protected int mCoordLocation;

    //纹理
    protected int mTextureLocation;

    public BaseFilter(Context context,int vertexSourceId, int fragmentSourceId){
        //顶点着色器代码
        String vertexSource = TextResourceReader.readFromRaw(context,
                vertexSourceId);
        //片元着色器代码
        String fragmentSource = TextResourceReader.readFromRaw(context,
                fragmentSourceId);

        mProgram=initProgram(vertexSource,fragmentSource);

        vertexData= getVertexBuffer();
        textureData =getTextureBuffer();


    }

    private int initProgram(String vertexSource, String fragmentSource) {
        int vertexShaderId = ShaderHelper.compileShader(GL_VERTEX_SHADER,vertexSource);
        int fragmentShaderId =  ShaderHelper.compileShader(GL_FRAGMENT_SHADER,fragmentSource);
        //创建着色器程序
        int program=ShaderHelper.linkProgram(vertexShaderId,fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);


        getLocation(program);


        return program;
    }

    protected void getLocation(int program) {
        //获取 着色器源码中属性的位置
        mPositionLocation=GLES20.glGetAttribLocation(program,"a_Position");
        mCoordLocation=GLES20.glGetAttribLocation(program,"a_Coord");
        mTextureLocation=GLES20.glGetUniformLocation(program,"u_Texture");
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

    protected FloatBuffer getVertexBuffer() {
        float[] vertices = {
                -1.0f, -1.0f,
                1.0f, -1.0f,
                -1.0f, 1.0f,
                1.0f, 1.0f,
        };
        return  BufferHelper.getFloatBuffer(vertices);
    }


    public  void onReady(int width, int height){
        mWidth=width;
        mHeight=height;
    }

    public int onDrawFrame(int textureId){
        //1， 设置视窗
        glViewport(0, 0, mWidth, mHeight);
        //2，使用着色器程序
        glUseProgram(mProgram);


        //1，顶点数据
        vertexData.position(0);

        glVertexAttribPointer(mPositionLocation, 2, GL_FLOAT, false, 0, vertexData);//传值
        //使能顶点缓冲区
        glEnableVertexAttribArray(mPositionLocation);

        //2，纹理坐标
        textureData.position(0);
        glVertexAttribPointer(mCoordLocation, 2, GL_FLOAT, false, 0, textureData);
        glEnableVertexAttribArray(mCoordLocation);

        inflateLocation();


        //激活图层
        glActiveTexture(GL_TEXTURE0);
        //绑定
        glBindTexture(getTextureTarget(), textureId);
        //传递参数
        glUniform1i(mTextureLocation, 0);

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        return textureId;
    }

    protected void inflateLocation() {

    }


    protected int getTextureTarget(){
        return GL_TEXTURE_2D;
    }


    public void release(){
        glDeleteProgram(mProgram);
    }
}
