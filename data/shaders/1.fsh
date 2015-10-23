#version 120
uniform sampler2D scene;
uniform float gamma;
uniform float numcolors;
uniform vec4 clear;
varying vec2 coords;
varying float mod;
varying vec2 offset;
void main()
{
  float alpha = 0.5;
  float mod2 = mod;
  vec4 u1, u2, d1, d2, l1, l2, r1, r2;
  vec4 v1, v2, v3, v4, v5, v6, o1, o2, co;
  vec3 c;
  vec2 mod3 = offset;
  if (all(greaterThan(texture2D(scene, coords), clear + 0.1))) {
    mod2 = mod + 16;
    mod3 = offset * 2.3;
    alpha = 0.9;
  }
  if (mod2 > numcolors - 8)
    mod2 = numcolors - 8;
  u1 = texture2D(scene, coords + vec2(0.0, -mod3.y));
  u2 = texture2D(scene, coords + vec2(0.0, -mod3.y * 2.0));
  d1 = texture2D(scene, coords + vec2(0.0, mod3.y));
  d2 = texture2D(scene, coords + vec2(0.0, mod3.y * 2.0));
  l1 = texture2D(scene, coords + vec2(-mod3.x, 0.0));
  l2 = texture2D(scene, coords + vec2(-mod3.x * 2.0, 0.0));
  r1 = texture2D(scene, coords + vec2(mod3.x, 0.0));
  r2 = texture2D(scene, coords + vec2(mod3.x * 2.0, 0.0));
  v1 = mix(l1, l2, 0.2);
  v2 = mix(r1, r2, 0.2);
  v3 = mix(u1, u2, 0.2);
  v4 = mix(d1, d2, 0.2);
  o1 = max(l2, r2);
  o2 = max(d2, u2);
  v5 = mix(v1, v2, 0.5);
  o1 = mix(o1, v5, 0.5);
  v6 = mix(v3, v4, 0.5);
  o2 = mix(o2, v6, 0.3);
  co = mix(o1, o2, 0.3);
  c = pow(co.rgb, vec3(gamma, gamma, gamma));
  c = c * (numcolors - mod2);
  c = floor(c);
  c = c / (numcolors - mod2);
  c = pow(c, vec3(1.0 / gamma));
  gl_FragColor = vec4(c.rgb, alpha);
}


