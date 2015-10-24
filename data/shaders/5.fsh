#version 120
uniform sampler2D scene;
uniform vec4 clear;
// uniform float radius;
varying vec2 coords;
varying vec2 offset;
void main()
{
  float alpha = (1 - clear.r + clear.g + clear.b) / 7.0;
  if (all(greaterThan(texture2D(scene, coords), clear + 0.23)))
    alpha = 0.17;
  vec4 maxv = texture2D(scene, coords);
  vec4 s0, s1, s2, s3, o0, o1, tmax;
  // float w, u, v;
  // for(u = 0.0; u <= radius; u += 1.0) {
  //   for(v = 0.0; v <= radius; v += 1.0) {
  //     w = (sqrt(u * u + v * v) / radius) > 1.0 ? 0.0 : 1.0;
  //     s0 = texture2D(scene, coords + vec2(-u, -v));
  //     s1 = texture2D(scene, coords + vec2(u, v));
  //     s2 = texture2D(scene, coords + vec2(-u, v));
  //     s3 = texture2D(scene, coords + vec2(u, -v));
  //     o0 = max(s0, s1);
  //     o1 = max(s2, s3);
  //     tmax = max(o0, o1);
  //     maxv = mix(maxv, max(maxv, tmax), w);
  //   }
  // }
  s0 = texture2D(scene, coords + vec2(0, -offset.y * 2.1));
  s1 = texture2D(scene, coords + vec2(0, offset.y * 2.1));
  s2 = texture2D(scene, coords + vec2(-offset.x * 2.1, 0));
  s3 = texture2D(scene, coords + vec2(offset.x * 2.1, 0));
  o0 = max(s0, s1);
  o1 = max(s2, s3);
  maxv = max(o0, o1);
  gl_FragColor = mix(vec4(maxv.rgb, alpha), vec4(0,0,0,1), 0.1);
}


