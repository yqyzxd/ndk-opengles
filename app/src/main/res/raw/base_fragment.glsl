//float数据是什么精度的
precision mediump float;

//采样点的坐标
varying vec2 v_Coord;

//采样器
uniform sampler2D u_Texture;

void main(){
    //变量 接收像素值
    // texture2D：采样器 采集 v_Coord的像素
    //赋值给 gl_FragColor 就可以了
    gl_FragColor = texture2D(u_Texture, v_Coord);
}