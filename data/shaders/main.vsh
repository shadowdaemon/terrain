#version 120
uniform vec2 steps;
varying vec2 coords;
varying float mod;
varying vec2 offset;
void main()
{
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  coords = gl_MultiTexCoord4.st;
  mod    = abs((0.5 - coords.s) * 25) + abs((0.5 - coords.t) * 25);
  offset = abs((0.5 - coords.s) / steps) + abs((0.5 - coords.t) / steps);
}


