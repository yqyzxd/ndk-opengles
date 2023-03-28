#include "com_wind_ndk_opengles_n_NativeGLRenderer.h"
#include "GLLooper.h"

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "android/bitmap.h"
#include "global.h"
extern unsigned char* Pixles;
#ifdef __cplusplus
extern "C" {
#endif

GLLooper* glLooper=0;
ANativeWindow* window=0;
JNIEXPORT void JNICALL Java_com_wind_ndk_opengles_n_NativeGLRenderer_native_1init
        (JNIEnv *, jobject){
    //创建looper
     glLooper=new GLLooper();
}

JNIEXPORT void JNICALL Java_com_wind_ndk_opengles_n_NativeGLRenderer_native_1on_1surface_1created
        (JNIEnv *env, jobject, jobject jsurface) {
    if (window!=NULL){
        ANativeWindow_release(window);
        window=NULL;
    }


   window=ANativeWindow_fromSurface(env,jsurface);
    if(glLooper){
        glLooper->postMessage(kMsgSurfaceCreated,window);
    }

}

JNIEXPORT void JNICALL Java_com_wind_ndk_opengles_n_NativeGLRenderer_native_1on_1surface_1changed
        (JNIEnv *, jobject, jobject , jint width, jint height) {
    if(glLooper){
        glLooper->postMessage(kMsgSurfaceChanged,width,height);
    }
}

JNIEXPORT void JNICALL Java_com_wind_ndk_opengles_n_NativeGLRenderer_native_1on_1surface_1destroy
        (JNIEnv *, jobject) {
    if(glLooper){
        glLooper->postMessage(kMsgSurfaceDestroyed);
    }
}

JNIEXPORT void JNICALL Java_com_wind_ndk_opengles_n_NativeGLRenderer_native_1update_1tex_1image
        (JNIEnv *env, jobject, jbyteArray jbitmap,jint width,jint height){
    if(glLooper){
        ALOGE("post kMsgUpdateTexImage");

        glLooper->postMessage(kMsgUpdateTexImage,width,height, Pixles);
    }

    /*AndroidBitmapInfo bitmapInfo;
    AndroidBitmap_getInfo(env,jbitmap,&bitmapInfo);
    int width=bitmapInfo.width;
    int height=bitmapInfo.height;
    void* bytes;
    AndroidBitmap_lockPixels(env, jbitmap, &bytes);
    AndroidBitmap_unlockPixels(env,jbitmap);
    if(glLooper){
        ALOGE("post kMsgUpdateTexImage");
        glLooper->postMessage(kMsgUpdateTexImage,width,height,bytes);
    }*/

}


#ifdef __cplusplus
}
#endif