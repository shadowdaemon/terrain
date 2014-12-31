#version 120
uniform vec2 steps;
varying vec2 coords;
varying vec2 offset;
void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  offset = abs((0.5 - coords.s) / steps) + abs((0.5 - coords.t) / steps);
  coords = gl_MultiTexCoord4.st; // stpq
}


