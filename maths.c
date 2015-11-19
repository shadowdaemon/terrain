#include <stdlib.h>
#include "maths.h"


extern struct v3f nullv3f;

float pos(float num)
{
     return num > 0.0f ? num : 0.0f;
}


float neg(float num)
{
     return num < 0.0f ? num : 0.0f;
}


int snap(float num, int snap)
{
     int temp = num - (int) num % snap;

     if (fabs(temp - num) < fabs(temp + snap -num))
          return temp;
     else
          return temp + snap;
}


float clamp(float *in, float min, float max)
{
     if (*in > max)
          *in = max;
     else if (*in < min)
          *in = min;
     return *in;
}


struct v2f mv2f(float x, float y)
{
     struct v2f temp;
     temp.x = x;
     temp.y = y;
     return temp;
}


struct v3f mv3f(float x, float y, float z)
{
     struct v3f temp;
     temp.x = x;
     temp.y = y;
     temp.z = z;
     return temp;
}


float distance2d(struct v3f a, struct v3f b)
{
     float dist = ((a.x - b.x) * (a.x - b.x)) +
          ((a.z - b.z) * (a.z - b.z));
     return sqrt(dist);
}


float distance3d(struct v3f a, struct v3f b)
{
     float dist = ((a.x - b.x) * (a.x - b.x)) +
          ((a.z - b.z) * (a.z - b.z)) +
          ((a.y - b.y) * (a.y - b.y));
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


void degreestovector3d(struct v3f *pos_self, struct v3f rot_self,
                       struct v3f rot, float dist)
{
     rot.y = (rot_self.y + rot.y) / PIx180;
     rot.x = (rot_self.x + rot.x) / PIx180;
     pos_self->x -= dist * sinf(rot.y) * cosf(rot.x);
     pos_self->y += dist * sinf(rot.x);
     pos_self->z += dist * cosf(rot.y) * cosf(rot.x);
}


float vectorstodegree2d(struct v3f a, struct v3f b)
{
     float temp;
     b.x = b.x - a.x;
     b.y = b.z - a.z;
     temp = atan2(b.y, b.x);
     temp = temp * 180 / PI;
     temp -= 90.0f;
     while (temp >= 360.0f)
          temp -= 360.0f;
     while (temp < 0.0f)
          temp += 360.0f;
     return temp;
}


struct v3f normalize3d(struct v3f pos)
{
     struct v3f temp = nullv3f;
     float d = distance3d(temp, pos);
     return mv3f(pos.x / d, pos.y / d, pos.z / d);
}


struct v3f calcNormal(float iv1[3], float iv2[3], float iv3[3])
{
     float v1[3], v2[3], v3[3];
     float length;
     struct v3f temp;
     v1[0] = iv1[0] - iv2[0];
     v1[1] = iv1[1] - iv2[1];
     v1[2] = iv1[2] - iv2[2];
     v2[0] = iv2[0] - iv3[0];
     v2[1] = iv2[1] - iv3[1];
     v2[2] = iv2[2] - iv3[2];
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


void createMatrix(float *quat, float *vmat)
{
     vmat[0] = 1.0f - 2.0f * (quat[2] * quat[2] + quat[3] * quat[3]);
     vmat[1] = 2.0f * (quat[1] * quat[2] + quat[3] * quat[0]);
     vmat[2] = 2.0f * (quat[1] * quat[3] - quat[2] * quat[0]);
     vmat[3] = 0.0f;
     vmat[4] = 2.0f * (quat[1]* quat[2] - quat[3] * quat[0]);
     vmat[5] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[3] * quat[3]);
     vmat[6] = 2.0f * (quat[3] * quat[2] + quat[1] * quat[0]);
     vmat[7] = 0.0f;
     vmat[8] = 2.0f * (quat[1] * quat[3] + quat[2] * quat[0]);
     vmat[9] = 2.0f * (quat[2]* quat[3] - quat[1] * quat[0]);
     vmat[10] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[2] * quat[2]);
     vmat[11] = 0.0f;
     vmat[12] = 0;
     vmat[13] = 0;
     vmat[14] = 0;
     vmat[15] = 1.0f;
}


/* Find point on a plane. */
float planeHeight(float ip[2], float iv1[3], float iv2[3],
                  float iv3[3], struct v3f *norm)
{
     float p;
     *norm = calcNormal(iv1, iv2, iv3);
     p = norm->x * iv1[0] + norm->x * iv2[0] + norm->x * iv3[0] +
          norm->y * iv1[1] + norm->y * iv2[1] + norm->y * iv3[1] +
          norm->z * iv1[2] + norm->z * iv2[2] + norm->z * iv3[2];
     return (p - norm->x * iv1[0] - norm->x * iv2[0] - norm->x
             * ip[0] - norm->y * iv1[1] - norm->y
             * iv2[1] - norm->z * iv1[2] - norm->z
             * iv2[2] - norm->z * ip[1]) / norm->y;
}


/* Interpolate between two floats. */
float lerp(float a0, float a1, float w, float m)
{
     w += (0.5f - w) * m;
     return (1.0 - w) * a0 + w * a1;
}


extern float pgrad[PERLIN_SIZE][PERLIN_SIZE][2];

void createGradient(void)
{
     int r, x, y;
     for (x = 0; x < PERLIN_SIZE; x++) {
          for (y = 0; y < PERLIN_SIZE; y++) {
               r = rand();
               pgrad[x][y][0] = -sinf(r);
               pgrad[x][y][1] = cosf(r);
          }
     }
}


float dotGridGradient(int ix, int iy, float x, float y)
{
     float dx = x - (double) ix;
     float dy = y - (double) iy;
     while (ix >= PERLIN_SIZE) ix -= PERLIN_SIZE;
     while (ix <= -PERLIN_SIZE) ix += PERLIN_SIZE;
     while (iy >= PERLIN_SIZE) iy -= PERLIN_SIZE;
     while (iy <= -PERLIN_SIZE) iy += PERLIN_SIZE;
     float out = dx * pgrad[iy][ix][0] + dy * pgrad[iy][ix][1];
     /* int t = fpclassify(out); */
     /* switch (t) { */
     /* case FP_NAN: */
     /*      printf("dotGridGradient: FP_NAN\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* case FP_INFINITE: */
     /*      printf("dotGridGradient: FP_INFINITE\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* case FP_SUBNORMAL: */
     /*      printf("dotGridGradient: FP_SUBNORMAL\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* default: */
     /*      break; */
     /* } */
     return out;
}


/* Thanks Wikipedia.  https://en.wikipedia.org/wiki/Perlin_noise */
/* And thanks to Ken Perlin. */
float perlin(float x, float y, float m)
{
     int x0 = x > 0.0 ? (int) x : (int) x - 1;
     int x1 = x0 + 1;
     int y0 = y > 0.0 ? (int) y : (int) y - 1;
     int y1 = y0 + 1;
     float sx = x - (double) x0;
     float sy = y - (double) y0;
     float n0, n1, ix0, ix1, out;
     n0  = dotGridGradient(x0, y0, x, y);
     n1  = dotGridGradient(x1, y0, x, y);
     ix0 = lerp(n0, n1, sx, m);
     n0  = dotGridGradient(x0, y1, x, y);
     n1  = dotGridGradient(x1, y1, x, y);
     ix1 = lerp(n0, n1, sx, m);
     out = lerp(ix0, ix1, sy, m);
     /* int t = 0; */
     /* t = fpclassify(out); */
     /* switch (t) { */
     /* case FP_NAN: */
     /*      printf("perlin: FP_NAN\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* case FP_INFINITE: */
     /*      printf("perlin: FP_INFINITE\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* case FP_SUBNORMAL: */
     /*      printf("perlin: FP_SUBNORMAL\n"); */
     /*      out = 0.0f; */
     /*      break; */
     /* default: */
     /*      break; */
     /* } */
     return out;
}
