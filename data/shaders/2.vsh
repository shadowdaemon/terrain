#version 120
uniform vec2 steps;
varying vec2 coords;
varying vec2 offset;
void main()
{
  //gl_Position = ftransform();
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  //offset = 1.0 / steps;
  //offset = (gl_Position.x + gl_Position.y) / steps;
  //offset = (coords.s + coords.t) / steps;
  offset = abs((0.5 - coords.s) / steps) + abs((0.5 - coords.t) / steps);
  coords = gl_MultiTexCoord4.st; // stpq
}


