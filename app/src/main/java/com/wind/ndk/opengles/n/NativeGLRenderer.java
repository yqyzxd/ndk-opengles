package com.wind.ndk.opengles.n;

import android.graphics.Bitmap;
import android.view.Surface;

/**
 * Created By wind
 * on 2020-01-12
 */
public class NativeGLRenderer implements SurfaceCallback {
    static {
        System.loadLibrary("native-lib");
    }
    public NativeGLRenderer(){
        native_init();
    }


    public void updateTexImage(Bitmap bitmap){
        native_update_tex_image(bitmap);
    }



    @Override
    public void onSurfaceCreated(Surface surface) {
        native_on_surface_created(surface);
    }

    @Override
    public void onSurfaceChanged(Surface surface, int width, int height) {
        native_on_surface_changed(surface,width,height);
    }

    @Override
    public void onSurfaceDestroy() {
        native_on_surface_destroy();
    }


    private native void native_on_surface_created(Surface surface);
    private native void native_on_surface_changed(Surface surface,int width, int height);
    private native void native_on_surface_destroy();
    private native void native_init();
    private native void  native_update_tex_image(Bitmap bitmap);

}
