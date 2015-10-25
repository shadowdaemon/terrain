#version 120
uniform vec2 steps;
// uniform mat4 projmat;
varying vec2 coords;
varying vec2 offset;
void main()
{
  vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
  coords = gl_MultiTexCoord4.st; //(projmat * vec4(pos.xy, 0.0, 1.0)).xy * 0.5 + 0.5;
  offset = abs((0.5 - coords.s) / steps) + abs((0.5 - coords.t) / steps);
  gl_Position = pos;// * (offset.x + offset.y);
}


