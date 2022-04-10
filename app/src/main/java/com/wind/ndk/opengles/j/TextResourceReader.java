package com.wind.ndk.opengles.j;

import android.content.Context;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created By wind
 * on 2020-01-12
 */
public class TextResourceReader {


    public static String readFromRaw(Context context,int rawId){
        StringBuilder sBuilder=new StringBuilder();
        try {
            InputStream ips=context.getResources().openRawResource(rawId);
            BufferedReader bufferedReader=new BufferedReader(new InputStreamReader(ips));
            String line;
            while((line=bufferedReader.readLine())!=null){
                sBuilder.append(line).append("\n");

            }

        }catch (Exception e){
            e.printStackTrace();
        }

        return sBuilder.toString();
    }
}
