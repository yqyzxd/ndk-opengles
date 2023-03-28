package com.wind.ndk.opengles.n;

import android.content.res.AssetManager;

/**
 * Copyright (C), 2015-2022, 杭州迈优文化创意有限公司
 * FileName: PngDecoder
 * Author: wind
 * Date: 2023/3/28 09:27
 * Description: 描述该类的作用
 * Path: 路径
 * History:
 * <author> <time> <version> <desc>
 */
public class PngDecoder {

    public static class Raw{
        byte[] bytes;
        int width;
        int height;

        public Raw(byte[] bytes,int width,int height){
            this.bytes=bytes;
            this.width=width;
            this.height=height;
        }
    }
    public Raw decodeFromFile(String fileName){
        return native_decode_from_file(fileName);
    }

    public Raw decodeFromAsset(AssetManager mgr,String fileName){
        return native_decode_from_asset(fileName,mgr);
    }


    private static Raw buildRawFromNative(byte[] bytes,int width,int height){
        Raw raw=new Raw(bytes,width,height);
        return raw;
    }

    private native Raw native_decode_from_file(String fileName);
    private native Raw native_decode_from_asset(String fileName, AssetManager mgr);


}
