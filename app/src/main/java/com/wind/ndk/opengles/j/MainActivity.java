package com.wind.ndk.opengles.j;

import android.Manifest;
import android.app.ActivityManager;
import android.content.pm.ConfigurationInfo;
import android.content.pm.PackageManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.RadioGroup;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.wind.ndk.opengles.R;

public class MainActivity extends AppCompatActivity {


    MyGLSurfaceView glSurfaceView;
    boolean rendererSet;
    CameraRenderer mCameraRenderer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        glSurfaceView=findViewById(R.id.gl_sruface_view);
        //检查是否支持opengles 2.0
        ActivityManager activityManager= (ActivityManager) getSystemService(ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo=activityManager.getDeviceConfigurationInfo();
        boolean supports=configurationInfo.reqGlEsVersion>=0x20000;

        boolean granted=ActivityCompat.checkSelfPermission(this, Manifest.permission.CAMERA)
                == PackageManager.PERMISSION_GRANTED;
       if (!granted){
           ActivityCompat.requestPermissions(this,new String[]{ Manifest.permission.CAMERA},1111);
       }
        if (supports && granted){
            //1 设置egl版本为2
            glSurfaceView.setEGLContextClientVersion(2);
            //2 设置渲染器renderer
           // glSurfaceView.setRenderer(new MyGLRenderer(glSurfaceView));
            glSurfaceView.setRenderer(mCameraRenderer=new CameraRenderer(glSurfaceView));
            /*
             *  3 设置渲染模式
             * @see #RENDERMODE_CONTINUOUSLY  不断的渲染
             * @see #RENDERMODE_WHEN_DIRTY   需要时渲染
             */
            glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            rendererSet=true;


            ((MyRecordButton) findViewById(R.id.btn_record)).setOnRecordListener(
                    new MyRecordButton.OnRecordListener() {
                        /**
                         * 开始录制
                         */
                        @Override
                        public void onStartRecording() {
                            mCameraRenderer.startRecording();
                        }

                        /**
                         * 停止录制
                         */
                        @Override
                        public void onStopRecording() {
                            mCameraRenderer.stopRecording();
                            Toast.makeText(MainActivity.this, "录制完成！", Toast.LENGTH_SHORT).show();
                        }
                    });

            ((RadioGroup) findViewById(R.id.group_record_speed)).setOnCheckedChangeListener(
                    new RadioGroup.OnCheckedChangeListener() {
                        /**
                         * 选择录制模式
                         * @param group
                         * @param checkedId
                         */
                        @Override
                        public void onCheckedChanged(RadioGroup group, int checkedId) {
                            switch (checkedId) {
                                case R.id.rbtn_record_speed_extra_slow: //极慢
                                    mCameraRenderer.setSpeed(CameraRenderer.Speed.MODE_EXTRA_SLOW);
                                    break;
                                case R.id.rbtn_record_speed_slow:   //慢
                                    mCameraRenderer.setSpeed(CameraRenderer.Speed.MODE_SLOW);
                                    break;
                                case R.id.rbtn_record_speed_normal: //正常
                                    mCameraRenderer.setSpeed(CameraRenderer.Speed.MODE_NORMAL);
                                    break;
                                case R.id.rbtn_record_speed_fast:   //快
                                    mCameraRenderer.setSpeed(CameraRenderer.Speed.MODE_FAST);
                                    break;
                                case R.id.rbtn_record_speed_extra_fast: //极快
                                    mCameraRenderer.setSpeed(CameraRenderer.Speed.MODE_EXTRA_FAST);
                                    break;
                            }
                        }
                    });
        }


    }


    /**
     * 处理activity生命周期
     * 当activity onPause 后glsurfaceview停止绘制
     */
    @Override
    protected void onPause() {
        super.onPause();
        if (rendererSet){
            glSurfaceView.onPause();
        }
    }
    /**
     * 处理activity生命周期
     * 当activity onResume 后glsurfaceview恢复绘制
     */
    @Override
    protected void onResume() {
        super.onResume();
        if (rendererSet){
            glSurfaceView.onResume();
        }
    }
}
