
cmake_minimum_required(VERSION 3.4.1)


#add_subdirectory(libpng)
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

        BaseFilter.cpp
        ScreenFilter.cpp
        GLRender.cpp
        GLLooper.cpp
        Triangle.cpp
        com_wind_ndk_opengles_n_NativeGLRenderer.cpp)



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
        ${log-lib})