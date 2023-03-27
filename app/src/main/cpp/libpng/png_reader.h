//
// Created by wind on 2023/3/27.
//

#ifndef NDK_PNG_PNG_READER_H
#define NDK_PNG_PNG_READER_H

#include "raw.h"

extern "C"{
#include "png.h"
#include "pngconf.h"
}

class PngReader {
public:
    PngReader(char *fileName);
    ~PngReader();

    Raw* Read();

    void readFromFile(png_bytep string, size_t i);

private:
    FILE *fp;

};


#endif //NDK_PNG_PNG_READER_H
