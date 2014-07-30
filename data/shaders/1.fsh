#version 120
uniform sampler2D scene;
uniform float gamma;
uniform float numColors;
varying vec2 coords;
void main() 
{
  vec3 c = texture2D(scene, coords).rgb;
  c = pow(c, vec3(gamma, gamma, gamma));
  c = c * numColors;
  c = floor(c);
  c = c / numColors;
  c = pow(c, vec3(1.0/gamma));
  gl_FragColor = vec4(c, 1.0);
  //gl_FragColor = texture2D(scene, coords);
  //gl_FragColor = vec4(texture2D(scene, coords).rgb, 1);
}


