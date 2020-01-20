package com.wind.ndk.opengles.n;

import android.view.Surface;

/**
 * Created By wind
 * on 2020-01-12
 */
public interface SurfaceCallback {


    void onSurfaceCreated(Surface surface);


    void onSurfaceChanged(Surface surface,int width, int height);


    void onSurfaceDestroy();
}
