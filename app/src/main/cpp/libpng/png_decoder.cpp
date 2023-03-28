//
// Created by wind on 2023/3/28.
//

#include <string.h>
#include "png_decoder.h"


PngDecoder::PngDecoder(char *fName) {
    //deep copy
    char* name=new char[strlen(fName)+1];
    strcpy(name,fName);
    this->fileName=name;
}

void png_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    PngDecoder* decoder= static_cast<PngDecoder *>(png_get_io_ptr(png_ptr));
    decoder->pngReadCallback(data,  length);
}
png_bytep PngDecoder::decode() {

    /*if (!isPng()){
        fClose();
        return NULL;
    }*/

    //create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 NULL, NULL);
    if (!png_ptr) {
        fClose();
        return NULL;
    }

    //create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
        fClose();
        return NULL;
    }
    //create png end info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fClose();
        return NULL;
    }
    //png error stuff, not sure libpng man suggests this.
    if (setjmp(png_jmpbuf(png_ptr))) {
        fClose();
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return NULL;
    }

    //init png reading
    //png_init_io(png_ptr, fp);
    ALOGE("PngDecoder before png_set_read_fn");
    png_set_read_fn(png_ptr, this, png_read_callback);
    ALOGE("PngDecoder after png_set_read_fn");
    //let libpng know you already read the first 8 bytes
   // png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);


    //variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 width, height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 NULL, NULL, NULL);
    // Convert transparency to full alpha
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    // Convert grayscale, if needed.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    // Convert paletted images, if needed.
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // Add alpha channel, if there is none (rationale: GL_RGBA is faster than GL_RGB on many GPUs)
    if (color_type == PNG_COLOR_TYPE_PALETTE || color_type == PNG_COLOR_TYPE_RGB)
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);

    // Ensure 8-bit packing
    if (bit_depth < 8)
        png_set_packing(png_ptr);
    else if (bit_depth == 16)
        png_set_scale_16(png_ptr);

    png_read_update_info(png_ptr, info_ptr);


    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    ALOGE("PngDecoder decode rowbytes:%d",rowbytes);
    // Allocate the image_data as a big block
    png_byte *image_data = new png_byte[rowbytes * height];

    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[height];

    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < height; i++) {
        row_pointers[i] = image_data + i * rowbytes;
    }
    //read the png into image_data through row_pointers
    ALOGE("PngDecoder before png_read_image");
    png_read_image(png_ptr, &row_pointers[0]);

    ALOGE("PngDecoder after png_read_image");

    png_read_end(png_ptr, end_info);
    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete[] row_pointers;

    fClose();
    this->width=width;
    this->height=height;
    return image_data;
}


bool PngDecoder::isPng() {
    //header for testing if it is a png
   // png_byte header[8];
    png_bytep head= readHead();
    //test if png
    int isPng = !png_sig_cmp(head, 0, 8);

    return isPng;
}

int PngDecoder::getWidth() {
    return width;
}

int PngDecoder::getHeight() {
    return height;
}
PngDecoder::~PngDecoder() {
    if (fileName){
        delete fileName;
    }
}


