//
// Created by wind on 2023/3/28.
//

#ifndef NDK_OPENGLES_ASSET_PNG_DECODER_H
#define NDK_OPENGLES_ASSET_PNG_DECODER_H


#include "../libpng/png_decoder.h"
#include <android/asset_manager.h>
#include "../log.h"
class AssetPngDecoder: public PngDecoder{
public:
    AssetPngDecoder(AAssetManager* mgr ,char *fName);
    virtual ~AssetPngDecoder();
    virtual void pngReadCallback(png_bytep data, png_size_t length);
protected:
   virtual void fClose();
   virtual png_bytep readHead();
private:
    AAsset* asset;

};


#endif //NDK_OPENGLES_ASSET_PNG_DECODER_H
