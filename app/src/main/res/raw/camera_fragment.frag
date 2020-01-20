#extension GL_OES_EGL_image_external : require

precision mediump float;

varying vec2 v_Coord;

uniform samplerExternalOES u_Texture;

void main(){
    gl_FragColor=texture2D(u_Texture,v_Coord);
}

