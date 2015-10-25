#version 120
uniform sampler2D scene;
varying vec2 coords;
varying vec2 offset;
void main()
{
  vec4 s0, s1, s2, s3, o0, o1, maxv, tmax;
  s0 = texture2D(scene, coords + vec2(0, -offset.y * 1.3));
  s1 = texture2D(scene, coords + vec2(0, offset.y * 1.3));
  s2 = texture2D(scene, coords + vec2(-offset.x * 1.3, 0));
  s3 = texture2D(scene, coords + vec2(offset.x * 1.3, 0));
  if (s0.r + s0.g > s1.b * 2)
    o0 = mix(s0, s1, 0.2);
  else
    o0 = mix(s1, s0, 0.2);
  if (s2.r + s2.g > s3.b * 2)
    o1 = mix(s2, s3, 0.2);
  else
    o1 = mix(s3, s2, 0.2);
  maxv = max(o0, o1);
  gl_FragColor = vec4(maxv.rgb, 0.67);
}

