precision mediump float;
varying vec2 v_Coord;
uniform sampler2D u_Texture;
void main(){
    gl_FragColor=texture2D(u_Texture,v_Coord);
    //处理成灰度图
   /* vec4 rgba=texture2D(u_Texture,v_Coord);
    //305911公式
    float gray=0.3*rgba.r+0.59*rgba.g+0.11*rgba.b;
    gl_FragColor=vec4(gray,gray,gray,1);*/
}