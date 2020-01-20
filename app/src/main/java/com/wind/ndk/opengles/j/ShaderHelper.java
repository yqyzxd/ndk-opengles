package com.wind.ndk.opengles.j;

import android.opengl.GLES20;

/**
 * Created By wind
 * on 2020-01-12
 */
public class ShaderHelper {

    /**
     * 创建着色器程序
     * @param vertexShaderId  顶点着色器id
     * @param fragmentShaderId 片元着色器id
     * @return
     */
    public static int linkProgram(int vertexShaderId,int fragmentShaderId){
        //2.1 创建着色器程序
        int program=GLES20.glCreateProgram();
        if (program==0){//error
            return 0;
        }
        //2.2 让着色器程序关联shaderId
        GLES20.glAttachShader(program,vertexShaderId);
        GLES20.glAttachShader(program,fragmentShaderId);
        //2.3 链接程序
        GLES20.glLinkProgram(program);
        //2.4 获取链接状态
        int[] linkStatus=new int[1];
        GLES20.glGetProgramiv(program,GLES20.GL_LINK_STATUS,linkStatus,0);
        if (linkStatus[0]==0){
            //链接失败
            //->获取失败详情信息
            String infoLog=GLES20.glGetProgramInfoLog(program);
            System.out.println("linkProgram error:"+infoLog);
            //->删除创建的program
            GLES20.glDeleteProgram(program);
            return 0;

        }
        return program;
    }
    /**
     * 编译着色器源码返回shaderId
     * @param type         值为GL_VERTEX_SHADER或GL_FRAGMENT_SHADER
     * @param shaderSource 着色器源码
     * @return
     */
    public static int compileShader(int type,String shaderSource){
        //1.1 创建着色器对象
        int shaderId= GLES20.glCreateShader(type);
        if (shaderId==0){
            //error
            return 0;
        }
        //1.2 上传着色器源代码
        GLES20.glShaderSource(shaderId,shaderSource);
        //1.3 编译着色器源代码
        GLES20.glCompileShader(shaderId);
        //1.4获取编译状态
        int [] compileStatus=new int[1];
        GLES20.glGetShaderiv(shaderId,GLES20.GL_COMPILE_STATUS,compileStatus,0);
        if (compileStatus[0]==0){//编译失败
            //获取错误原因
            String infoLog=GLES20.glGetShaderInfoLog(shaderId);
            //log输出
            System.out.println("compileShader error:"+infoLog);
            GLES20.glDeleteShader(shaderId);
        }

        return shaderId;
    }
}
