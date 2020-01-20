package com.wind.ndk.opengles.j;

import android.opengl.GLES20;

/**
 * Created By wind
 * on 2020-01-19
 */
public class GLESUtils {


    public static void genTextures(int[] textures){


        GLES20.glGenTextures(textures.length,textures,0);

        for (int i=0;i<textures.length;i++){

            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,textures[i]);


            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_MAG_FILTER,GLES20.GL_LINEAR);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_MIN_FILTER,GLES20.GL_NEAREST);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_WRAP_S,GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,GLES20.GL_TEXTURE_WRAP_T,GLES20.GL_CLAMP_TO_EDGE);

            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D,0);
        }

    }
}
