#version 120
uniform sampler2D texture;
varying float depth;
void main()
{
  // float Z = gl_ProjectionMatrix[3].z/(gl_FragCoord.z * -2.0 + 1.0 - gl_ProjectionMatrix[2].z);
  gl_FragColor = texture2D(texture, gl_PointCoord);
}


