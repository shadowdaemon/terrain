#version 120
varying vec2 myTexCoord;
void main()
{
  // gl_Position = ftransform();
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  myTexCoord = vec2(gl_MultiTexCoord0);
}
