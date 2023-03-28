//
// Created by 史浩 on 2020-01-12.
//

#ifndef NDK_OPENGLES_LOG_H
#define NDK_OPENGLES_LOG_H

#define JNI_DEBUG 1
#define JNI_TAG "JNI_TAG"
#include <android/log.h>
#define ALOGE(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_ERROR, JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGI(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_INFO,  JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGD(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG, format, ##__VA_ARGS__); }
#define ALOGW(format, ...) if (JNI_DEBUG) { __android_log_print(ANDROID_LOG_WARN,  JNI_TAG, format, ##__VA_ARGS__); }


#endif //NDK_OPENGLES_LOG_H
