#include <jni.h>
#include <android/asset_manager_jni.h>
#include <string.h>
#include "libpng/png_decoder.h"
#include "pngdecoder/file_png_decoder.h"
#include "pngdecoder/asset_png_decoder.h"
#include "common/jniutil.h"
#include "global.h"
#include "GLLooper.h"

//
// Created by wind on 2023/3/28.
//

extern "C"
JNIEXPORT jobject JNICALL
Java_com_wind_ndk_opengles_n_PngDecoder_native_1decode_1from_1file(JNIEnv *env, jobject thiz,
                                                                   jstring jfilename) {

    char* cFileName= const_cast<char *>(env->GetStringUTFChars(jfilename, 0));
    PngDecoder* decoder=new FilePngDecoder(cFileName);
    env->ReleaseStringUTFChars(jfilename,cFileName);

    png_bytep bytes =decoder->decode();
    int w=decoder->getWidth();
    int h=decoder->getHeight();
    size_t len = strlen(reinterpret_cast<const char *>(bytes));
    jbyteArray byteArray=charToJByteArray(env,bytes,len);
    jclass rawClass=env->FindClass("com/wind/ndk/opengles/n/PngDecoder");
    jmethodID  metId=env->GetStaticMethodID(rawClass,"buildRawFromNative",
                                            "([BII)Lcom/wind/ndk/opengles/n/PngDecoder$Raw;");
    jobject rawObj=env->CallStaticObjectMethod(rawClass,metId,byteArray,w,h);

    return rawObj;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_wind_ndk_opengles_n_PngDecoder_native_1decode_1from_1asset(JNIEnv *env, jobject thiz,
                                                                    jstring jfilename,
                                                                    jobject jmgr) {

    AAssetManager* mgr=AAssetManager_fromJava(env,jmgr);
    char* cFileName= const_cast<char *>(env->GetStringUTFChars(jfilename, 0));
    PngDecoder* decoder=new AssetPngDecoder(mgr,cFileName);
    env->ReleaseStringUTFChars(jfilename,cFileName);

    png_bytep  bytes=decoder->decode();
    int w=decoder->getWidth();
    int h=decoder->getHeight();
    glLooper->postMessage(kMsgUpdateTexImage,w,h, bytes);
    ALOGE("native_1decode_1from_1asset %d",bytes);
   /* if (Pixles!=NULL){
        int len=w*h;
        jbyteArray byteArray=charToJByteArray(env,Pixles,len);

        //反射创建java层raw对象

        jclass rawClass=env->FindClass("com/wind/ndk/opengles/n/PngDecoder");
        jmethodID  metId=env->GetStaticMethodID(rawClass,"buildRawFromNative",
                                                "([BII)Lcom/wind/ndk/opengles/n/PngDecoder$Raw;");
        jobject rawObj=env->CallStaticObjectMethod(rawClass,metId,byteArray,w,h);
        return rawObj;
    }*/

    return nullptr;

}

