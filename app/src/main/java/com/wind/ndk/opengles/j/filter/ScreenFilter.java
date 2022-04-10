package com.wind.ndk.opengles.j.filter;

import android.content.Context;
import android.opengl.GLES20;

import com.wind.ndk.opengles.R;
import com.wind.ndk.opengles.j.TextResourceReader;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

/**
 * Created By wind
 * on 2020-01-12
 */
public class ScreenFilter extends BaseFilter {

    public ScreenFilter(Context context) {
        super(context, R.raw.base_vertex, R.raw.base_fragment);
    }
}
