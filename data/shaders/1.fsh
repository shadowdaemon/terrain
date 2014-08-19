#version 120
uniform sampler2D scene;
uniform float gamma;
uniform float numcolors;
uniform vec4 clear;
varying vec2 coords;
varying float mod;
void main() 
{
  float mod2 = mod;
  vec3 c = texture2D(scene, coords).rgb;
  if (all(greaterThan(texture2D(scene, coords), clear + 0.3)))
    mod2 = mod + 16;
  if (mod2 > numcolors - 8)
    mod2 = numcolors - 8;
  c = pow(c, vec3(gamma, gamma, gamma));
  c = c * (numcolors - mod2);
  c = floor(c);
  c = c / (numcolors - mod2);
  c = pow(c, vec3(1.0 / gamma));
  gl_FragColor = vec4(c, 1.0);
}


