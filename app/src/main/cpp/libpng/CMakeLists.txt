
cmake_minimum_required(VERSION 3.22.1)


project("png")

add_library(
        png

        STATIC

        png.c
        pngerror.c
        pngget.c
        pngmem.c
        pngpread.c
        pngread.c
        pngrio.c
        pngrtran.c
        pngrutil.c
        pngset.c
        pngtrans.c
        pngwio.c
        pngwrite.c
        pngwtran.c
        pngwutil.c
        arm/arm_init.c
        arm/palette_neon_intrinsics.c
        arm/filter_neon_intrinsics.c
        png_decoder.cpp
)


target_link_libraries(
        png

        z)