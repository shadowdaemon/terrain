#version 120
uniform sampler2D scene;
varying vec2 coords;
void main()
{
  vec4 c  = texture2D(scene, coords);
  gl_FragColor = c;
}


