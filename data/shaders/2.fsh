#version 120
uniform sampler2D scene;
uniform vec4 clear;
varying vec2 coords;
varying vec2 offset;
void main()
{
  vec2 mod = offset;
  if (all(greaterThan(texture2D(scene, coords), clear + 0.1)))
    mod = offset * 1.5;
  vec4 c  = texture2D(scene, coords);
  vec4 u1 = texture2D(scene, coords + vec2(0.0, -mod.y));
  vec4 u2 = texture2D(scene, coords + vec2(0.0, -mod.y * 2.0));
  vec4 d1 = texture2D(scene, coords + vec2(0.0, mod.y));
  vec4 d2 = texture2D(scene, coords + vec2(0.0, mod.y * 2.0));
  vec4 l1 = texture2D(scene, coords + vec2(-mod.x, 0.0));
  vec4 l2 = texture2D(scene, coords + vec2(-mod.x * 2.0, 0.0));
  vec4 r1 = texture2D(scene, coords + vec2(mod.x, 0.0));
  vec4 r2 = texture2D(scene, coords + vec2(mod.x * 2.0, 0.0));
  vec4 v1 = mix(l1, l2, 0.8);
  vec4 v2 = mix(r1, r2, 0.8);
  vec4 v3 = mix(u1, u2, 0.8);
  vec4 v4 = mix(d1, d2, 0.8);
  vec4 v5 = mix(v1, v2, 0.5);
  vec4 v6 = mix(v3, v4, 0.5);
  vec4 color = mix(v5, v6, 0.5);
  gl_FragColor = vec4(color.rgb, 1);
}


