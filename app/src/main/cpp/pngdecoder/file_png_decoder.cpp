//
// Created by wind on 2023/3/28.
//

#include "file_png_decoder.h"

FilePngDecoder::FilePngDecoder(char *fName) : PngDecoder(fName){
    fp= fopen(fileName,"rb");
}

void FilePngDecoder::pngReadCallback(png_bytep data, png_size_t length) {
    fread(data,1,length,fp);
}

png_bytep FilePngDecoder::readHead() {
    png_byte* head=new png_byte[8];
    fread(head,1,8,fp);
}
FilePngDecoder::~FilePngDecoder() {
    fClose();
}

void FilePngDecoder::fClose() {
    if (fp){
        fclose(fp);
        fp=0;
    }
}

