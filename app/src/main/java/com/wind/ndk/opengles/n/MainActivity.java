package com.wind.ndk.opengles.n;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.wind.ndk.opengles.R;

/**
 * Created By wind
 * on 2020-01-13
 */
public class MainActivity extends AppCompatActivity implements SurfaceHolder.Callback {


    NativeGLRenderer renderer;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_native_main);

        SurfaceView surfaceView=findViewById(R.id.surfaceview);
        surfaceView.getHolder().addCallback(this);

        renderer=new NativeGLRenderer();
        //png,jpg等还需借助 libpng lpbjpg等库解码才能显示
        BitmapFactory.Options options=new BitmapFactory.Options();
        options.inPreferredConfig=Bitmap.Config.ARGB_8888;
        final Bitmap bitmap= BitmapFactory.decodeResource(getResources(),R.drawable.iwaka,options);


       /* new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                //无效代码
                renderer.updateTexImage(bitmap);
            }
        },1000);*/
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        renderer.onSurfaceCreated(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        renderer.onSurfaceChanged(holder.getSurface(),width,height);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        renderer.onSurfaceDestroy();
    }
}
