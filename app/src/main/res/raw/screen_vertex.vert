attribute vec4 a_Position;
attribute vec2 a_Coord;
varying vec2 v_Coord;

void main(){
    gl_Position=a_Position;
    v_Coord=a_Coord;
}