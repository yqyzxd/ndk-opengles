//
// Created by wind on 2023/3/28.
//

#include <errno.h>
#include <string.h>
#include "file_png_decoder.h"

FilePngDecoder::FilePngDecoder(char *fName) : PngDecoder(fName){
    fp= fopen(fileName,"rb");
    if (fp== nullptr){
        ALOGE("open fail errno = %d reason = %s \n", errno, strerror(errno));
    }

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

