attribute vec4 a_Position;
attribute vec4 a_Coord;
uniform mat4 u_Matrix;

varying vec2 v_Coord;

void main(){
    gl_Position=a_Position;
    v_Coord=(u_Matrix*a_Coord).xy;
}