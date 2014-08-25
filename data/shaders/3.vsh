#version 120
uniform mat4 modelview;
uniform mat4 projection;
//uniform vec2 screenSize;
//uniform float spriteSize;
varying float depth;
void main()
{
  vec2 screensize = vec2(1366, 768);
  float spritesize = 10.0;
  vec4 eyepos = modelview * gl_Vertex;
  vec4 projvoxel = projection * vec4(spritesize, spritesize, eyepos.z, eyepos.w);
  vec2 projsize = screensize * projvoxel.xy / projvoxel.w;
  gl_PointSize = 0.25 * (projsize.x + projsize.y);
  gl_Position = projection * eyepos;
  depth = gl_Position.z;
}


