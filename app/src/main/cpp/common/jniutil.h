//
// Created by wind on 2023/3/28.
//

#ifndef NDK_OPENGLES_JNIUTIL_H
#define NDK_OPENGLES_JNIUTIL_H

#include <jni.h>
#include "../com_wind_ndk_opengles_n_NativeGLRenderer.h"


jbyteArray charToJByteArray(JNIEnv *env, unsigned char *buf, int len) {
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, reinterpret_cast<jbyte *>(buf));
    return array;
}



char *jbyteArrayToChar(JNIEnv *env, jbyteArray buf) {
    char *chars = NULL;
    jbyte *bytes = env->GetByteArrayElements(buf, 0);
    int chars_len = env->GetArrayLength(buf);
    chars = new char[chars_len + 1];
    memset(chars, 0, chars_len + 1);
    memcpy(chars, bytes, chars_len);
    chars[chars_len] = 0;
    env->ReleaseByteArrayElements(buf, bytes, 0);
    return chars;
}
#endif //NDK_OPENGLES_JNIUTIL_H
