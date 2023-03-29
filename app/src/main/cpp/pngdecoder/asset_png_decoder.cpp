//
// Created by wind on 2023/3/28.
//

#include <string.h>
#include "asset_png_decoder.h"


AssetPngDecoder::AssetPngDecoder(AAssetManager* mgr ,char *fName): PngDecoder(fName) {
    asset = AAssetManager_open(mgr, fName, AASSET_MODE_STREAMING);
    if (asset==NULL){
        ALOGE("AssetPngDecoder open assert error");
    }else{
        ALOGE("AssetPngDecoder open assert success");
    }

}

void AssetPngDecoder::pngReadCallback(png_bytep data, png_size_t length) {
   AAsset_read(asset,data,length);
}

png_bytep AssetPngDecoder::readHead() {
    png_byte* head=new png_byte[8];
    AAsset_read(asset,head,8);
    return head;
}

AssetPngDecoder::~AssetPngDecoder() {
    fClose();
}

void AssetPngDecoder::fClose() {
    if (asset){
        AAsset_close(asset);
        asset=NULL;
    }
}