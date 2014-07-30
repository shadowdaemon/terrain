#version 120
varying vec2 coords;
void main()
{
  //gl_Position = ftransform();
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  coords = gl_MultiTexCoord4.st;
}


