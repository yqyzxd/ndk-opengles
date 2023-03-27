//
// Created by wind on 2023/3/27.
//

#include "png_reader.h"

void png_read_callback(png_structp png_ptr, png_bytep data, size_t length){

    auto reader= static_cast<PngReader *>(png_get_io_ptr(png_ptr));
    reader->readFromFile(data,length);

}
PngReader::PngReader(char *fileName) {
    fp = fopen(fileName, "rb");
}

Raw* PngReader::Read() {
    if (fp== nullptr){
        return nullptr;
    }

    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;

    png_ptr = png_create_read_struct
            (PNG_LIBPNG_VER_STRING, nullptr,
             nullptr, nullptr);

    if (!png_ptr)
        return nullptr;

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr,
                                (png_infopp)NULL, (png_infopp)NULL);
        return nullptr;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        /* Free all of the memory associated with the png_ptr and info_ptr. */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        //fclose(fp);
        /* If we get here, we had a problem reading the file. */
        return nullptr;
    }
    //png_init_io(png_ptr,fp);
    png_set_read_fn(png_ptr, this, png_read_callback );

    png_read_info(png_ptr, info_ptr);

    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);

    /* Expand paletted colors into true RGB triplets. */
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    /* Expand grayscale images to the full 8 bits from 1, 2 or 4 bits/pixel. */
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    /* Expand paletted or RGB images with transparency to full alpha channels
     * so the data will be available as RGBA quartets.
     */
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0)
        png_set_tRNS_to_alpha(png_ptr);

    png_read_update_info(png_ptr,info_ptr);


    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // Allocate the image_data as a big block
    png_byte *image_data = new png_byte[rowbytes * height];


    //row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep *row_pointers = new png_bytep[height];

    // set the individual row_pointers to point at the correct offsets of image_data
    for (int i = 0; i < height; ++i) {
        row_pointers[i] = image_data + i * rowbytes;
    }

    //read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    Raw* raw = new Raw(image_data, width, height);

    //clean up memory and close stuff
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    delete[] row_pointers;

    return raw;

}

void PngReader::readFromFile(png_bytep data, size_t length) {
    fread(data,1,length,fp);
}

PngReader::~PngReader() {
    fclose(fp);
}

