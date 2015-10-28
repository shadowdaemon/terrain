#version 120
uniform sampler2D texture;
varying float depth;
void main()
{
  gl_FragColor = texture2D(texture, gl_PointCoord / depth);
}


