
cmake_minimum_required(VERSION 3.22.1)

project("native-lib")
add_subdirectory(libpng)
add_library(
        native-lib

        SHARED

        egl/EGLCore.cpp
        egl/BaseEGLSurface.cpp
        egl/WindowSurface.cpp
        egl/OffscreenSurface.cpp
        gles/GlShaders.cpp
        gles/GlUtils.cpp

        looper/Looper.cpp
        log.h
        BaseFilter.cpp
        ScreenFilter.cpp
        GLRender.cpp
        GLLooper.cpp
        Triangle.cpp
        com_wind_ndk_opengles_n_NativeGLRenderer.cpp
        com_wind_ndk_opengl_es_n_PngDecoder.cpp
        pngdecoder/asset_png_decoder.cpp
        pngdecoder/file_png_decoder.cpp
)



find_library(
        log-lib
        log)



target_link_libraries(
        native-lib


        jnigraphics
        EGL
        GLESv2
        android
        z
        png
        ${log-lib})