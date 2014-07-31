#version 120
varying vec2 coords;
varying float mod;
void main()
{
  //gl_Position = ftransform();
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  coords = gl_MultiTexCoord4.st;
  mod = abs((0.5 - coords.s) * 25) + abs((0.5 - coords.t) * 25);
}


