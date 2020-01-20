//
// Created by 史浩 on 2019-12-03.
//

#ifndef NDK_OPENGLES_GLSHADERS_H
#define NDK_OPENGLES_GLSHADERS_H

#ifdef __cplusplus
extern "C" {;
#endif

#define SHADER_STRING(s) #s

typedef enum {
    VERTEX_DEFAULT,
    VERTEX_REVERSE,

    FRAGMENT_SOLID,
    FRAGMENT_ABGR,
    FRAGMENT_ARGB,
    FRAGMENT_BGR,
    FRAGMENT_RGB,
    FRAGMENT_I420,
    FRAGMENT_NV12,
    FRAGMENT_NV21
} ShaderType;

const char* GlShader_GetShader(ShaderType type);





#ifdef __cplusplus
};
#endif

#endif //NDK_OPENGLES_GLSHADERS_H
