//
// Created by wind on 2023/3/28.
//

#ifndef NDK_OPENGLES_FILE_PNG_DECODER_H
#define NDK_OPENGLES_FILE_PNG_DECODER_H

#include "../libpng/png_decoder.h"

class FilePngDecoder :public PngDecoder{

public:
    FilePngDecoder(char* fName);
    virtual ~FilePngDecoder();
    virtual void pngReadCallback(png_bytep data, png_size_t length);

protected:
    virtual void fClose();
    virtual png_bytep readHead();
private:
    FILE* fp=0;

};


#endif //NDK_OPENGLES_FILE_PNG_DECODER_H
