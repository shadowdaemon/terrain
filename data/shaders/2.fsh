#version 120
uniform sampler2D scene;
uniform vec4 clear;
varying vec2 coords;
varying vec2 offset;
void main()
{
  vec2 mod = offset;
  float alpha = 0.3;
  if (all(greaterThan(texture2D(scene, coords), clear + 0.1))) {
    alpha = 1.0;
    mod = offset * 1.5;
  }
  vec4 c  = texture2D(scene, coords);
  vec4 u  = texture2D(scene, coords + vec2(0.0, -mod.y));
  vec4 d  = texture2D(scene, coords + vec2(0.0, mod.y));
  vec4 l  = texture2D(scene, coords + vec2(-mod.x, 0.0));
  vec4 r  = texture2D(scene, coords + vec2(mod.x, 0.0));
  vec4 v1 = mix(l, r, 0.5);
  vec4 v2 = mix(u, d, 0.5);
  vec4 color = mix(v1, v2, 0.5);
  gl_FragColor = vec4(color.rgb, alpha);
}


