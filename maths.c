#include "maths.h"


int snap(float in, int snap)
{
  int i = 0, j;

  j = in > 0 ? 1 : -1;
  in *= j;
  while ((i * snap) < in)
    i++;
  if (in - i * snap < (i + 1) * snap - in)
    return i * snap * j;
  else
    return (i + 1) * snap * j;
}


struct v3f mv3f(float x, float y, float z)
{
  struct v3f temp;
  temp.x = x;
  temp.y = y;
  temp.z = z;
  return temp;
}


float distance2d(struct v3f pos_a, struct v3f pos_b)
{
  float dist = ((pos_a.x - pos_b.x) * (pos_a.x - pos_b.x)) + ((pos_a.z - pos_b.z) * (pos_a.z - pos_b.z));
  return sqrt(dist);
}


float distance3d(struct v3f pos_a, struct v3f pos_b)
{
  float dist = ((pos_a.x - pos_b.x) * (pos_a.x - pos_b.x)) + ((pos_a.z - pos_b.z) * (pos_a.z - pos_b.z)) + ((pos_a.y - pos_b.y) * (pos_a.y - pos_b.y));
  return sqrt(dist);
}


struct v3f degreestovector2d(struct v3f pos_self, float rot_self, float rot, float dist)
{
  float temp1;
  float temp2;

  rot = (rot_self + rot) / PIx180;
  temp1 = pos_self.x - dist * sinf(rot);
  temp2 = pos_self.z + dist * cosf(rot);
  pos_self.x = temp1;
  pos_self.y = 0.0f;
  pos_self.z = temp2;

  return pos_self;
}


void degreestovector3d(struct v3f *pos_self, struct v3f rot_self, struct v3f rot, float dist)
{
  rot.y = (rot_self.y + rot.y) / PIx180;
  rot.x = (rot_self.x + rot.x) / PIx180;
  pos_self->x -= dist * sinf(rot.y) * cosf(rot.x);
  pos_self->y += dist * sinf(rot.x);
  pos_self->z += dist * cosf(rot.y) * cosf(rot.x);
}


float vectorstodegree2d(struct v3f pos_self, struct v3f pos_b)
{
  float temp;

  pos_b.x = pos_b.x - pos_self.x;
  pos_b.y = pos_b.z - pos_self.z;
  temp = atan2(pos_b.y, pos_b.x);
  temp = temp * 180 / PI;
  temp -= 90.0f;
  while (temp >= 360.0f)
    temp -= 360.0f;
  while (temp < 0.0f)
    temp += 360.0f;

  return temp;
}


struct v3f calcNormal(float in_v1[3], float in_v2[3], float in_v3[3])
{
  float v1[3], v2[3], v3[3];
  float length;
  struct v3f temp;

  v1[0] = in_v1[0] - in_v2[0];
  v1[1] = in_v1[1] - in_v2[1];
  v1[2] = in_v1[2] - in_v2[2];
  v2[0] = in_v2[0] - in_v3[0];
  v2[1] = in_v2[1] - in_v3[1];
  v2[2] = in_v2[2] - in_v3[2];
  v3[0] = v1[1] * v2[2] - v1[2] * v2[1];
  v3[1] = v1[2] * v2[0] - v1[0] * v2[2];
  v3[2] = v1[0] * v2[1] - v1[1] * v2[0];
  length = sqrt((v3[0] * v3[0]) + (v3[1] * v3[1]) + (v3[2] * v3[2]));
  v1[0] = v3[0] / length;
  v1[1] = v3[1] / length;
  v1[2] = v3[2] / length;
  temp.x = v1[0];
  temp.y = v1[1];
  temp.z = v1[2];

  return temp;
}


float plane1(float in_p[2], float in_v1[3], float in_v2[3], float in_v3[3])
{
  struct v3f temp = calcNormal(in_v1, in_v2, in_v3);
  float p;

  p = temp.x * in_v1[0] + temp.x * in_v2[0] + temp.x * in_v3[0] +
      temp.y * in_v1[1] + temp.y * in_v2[1] + temp.y * in_v3[1] +
      temp.z * in_v1[2] + temp.z * in_v2[2] + temp.z * in_v3[2];
  return (p - temp.x * in_v1[0] - temp.x * in_v2[0] - temp.x * in_p[0] -
              temp.y * in_v1[1] - temp.y * in_v2[1] - temp.z * in_v1[2] -
              temp.z * in_v2[2] - temp.z * in_p[1]) / temp.y;
}
