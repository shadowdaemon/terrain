#version 120
varying vec2 myTexCoord;
uniform sampler2D scene;
void main() 
{ 
  // gl_FragColor = vec4(texture2D(scene, gl_TexCoord[0].xy).rgb, 1.0);
  gl_FragColor = texture2D(scene, myTexCoord);
}
