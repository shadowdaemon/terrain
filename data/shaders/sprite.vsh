#version 120
uniform float size;
uniform vec2 screensize;
varying float depth;
void main()
{
  vec4 pos = gl_ModelViewMatrix * gl_Vertex;
  vec4 projvoxel = gl_ProjectionMatrix * vec4(size, size, pos.z, pos.w);
  vec2 projsize = screensize * projvoxel.xy / projvoxel.w;
  if (projsize.x < 0.0)
    projsize.x = 0.0;
  if (projsize.y < 0.0)
    projsize.y = 0.0;
  gl_PointSize = 0.25 * (projsize.x + projsize.y);
  gl_Position = gl_ProjectionMatrix * pos;
  depth = gl_Position.z;
}


