#version 120
// attribute vec4 pos;
uniform mat4 ProjMat;
uniform vec2 InSize;
varying vec2 coords;
varying vec2 texCoord;
varying vec2 oneTexel;
void main()
{
  vec4 pos = gl_ModelViewProjectionMatrix * gl_Vertex;
  coords = gl_MultiTexCoord4.st;
  // vec4 pos = gl_Vertex;
  vec4 outPos = ProjMat * vec4(pos.xy, 0.0, 1.0);
  gl_Position = vec4(outPos.xy, 0.2, 1.0);
  oneTexel = 1.0 / InSize;
  texCoord = outPos.xy * 0.5 + 0.5;
  //texCoord = coords;// * 0.5 + 0.5;
}


