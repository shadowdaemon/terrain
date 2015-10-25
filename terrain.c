#include <GLFW/glfw3.h>
#include "maths.h"


float algorithmicTerrainHeight1(float x, float z)
{
     float height, x1, z1, a1, a2, h1, h2, g1, g2;
     x1 = 0.00011f * sqrt(x*x+z*z);
     z1 = 0.00017f * z;
     a1 = sinf(x1 * 0.19f) * 9700;
     a2 = sinf(z1 - 0.07f * x1) * 4700;
     g1 = sinf(0.000223f * x) * 3742;
     g2 = sinf(0.000212f * z) * 3721;
     x += 3337.2f;
     z += 14213.5f;
     h1 = 4000 - fabs(4000 - (int)(x*0.0008f) % 8000);
     h2 = 5000 - fabs(5000 - (int)(z*0.00007f) % 10000);
     height = 11000 - fabs(a1) - pos(a2) - fabs(g1) + neg(g2) + h1 * 2.2f + h2 * 2.7f;
     x += 2331.2f;
     z += 5213.3f;
     if (height > 2000 || height < 1500) {
          a1 = height < 1500 ? (1500 - height) * 0.001f : 0;
          a1 = a1 > 1 ? 1 : a1;
          a2 = height > 2000 ? (height - 2000) * 0.0005f : 0;
          a2 = a2 > 1 ? 1 : a2;
          h1 = sinf(x * 0.00077f) * 710 * (a1 + a2);
          h2 = sinf(z * 0.00072f) * 716 * (a1 + a2);
          height += - neg(h1) - pos(h2);
     }
     height += (500 - height) * 0.31f + (11000 - height) * 0.17f + (height - 3500) * (3500 - height) * 0.0001f;
     if (height > 3000) {
          a2 = (height - 3000) * 0.0005f;
          a2 = a2 > 2 ? 2 : a2;
          g1 = sinf(x * 0.0051f) * 50 * a2;
          g2 = sinf(z * 0.0043f) * 52 * a2;
          height += - fabs(g1) - fabs(g2);
     }
     else if (height < 2300) {
          a1 = (2300 - height) * 0.0005f;
          a1 = a1 < 0 ? 0 : a1;
          g1 = cosf(x * 0.000267f) * 500 * a1;
          g2 = cosf(z * 0.000279f) * 590 * a1;
          height += g1 - g2;
     }
     return height;
}


float algorithmicTerrainHeight2(float x, float z)
{
     float height, x1, z1, a1, a2, h1, h2, g1, g2;
     x1 = 0.0014f * x;
     z1 = 0.0021f * z;
     a1 = sinf(x1 * 0.37f) * 6700;
     a2 = sinf(z1 - 0.07f * x1) * 2950;
     g1 = sinf(0.00223f * x) * 1742;
     g2 = sinf(0.00212f * z) * 2721;
     h1 = 6000 - fabs(6000 - (int)(x*0.0092f) % 12000);
     h2 = 5000 - fabs(5000 - (int)(z*0.0009f) % 10000);
     height = 8000 - fabs(a1) - fabs(a2) - fabs(g1) - fabs(g2) + h1 * 2.2f + h2 * 2.9f;
     if (height > 2000 || height < 1500) {
          a1 = height < 1500 ? (1500 - height) * 0.001f : 0;
          a1 = a1 > 1 ? 1 : a1;
          a2 = height > 2000 ? (height - 2000) * 0.0005f : 0;
          a2 = a2 > 1 ? 1 : a2;
          h1 = sinf(x * 0.0077f) * 910 * (a1 + a2);
          h2 = sinf(z * 0.0072f) * 916 * (a1 + a2);
          height += - fabs(h1) - fabs(h2);
     }
     if (height > 3000) {
          a2 = (height - 3000) * 0.0005f;
          a2 = a2 > 2 ? 2 : a2;
          g1 = sinf(x * 0.061f) * 60 * a2;
          g2 = sinf(z * 0.063f) * 62 * a2;
          height += - fabs(g1) - fabs(g2);
     }
     else if (height < 2300) {
          a1 = (2300 - height) * 0.0005f;
          a1 = a1 < 0 ? 0 : a1;
          g1 = cosf(x * 0.00267f) * 500 * a1;
          g2 = cosf(z * 0.00279f) * 590 * a1;
          height += g1 - g2;
     }
     return height;
}


float algorithmicTerrainHeight3(float x, float z, float height)
{
     float x1, z1, a1, a2, h1, h2, g1, g2;
     x1 = 0.00005f * x;
     z1 = 0.00011f * z;
     a1 = sinf(x1) * 3700;
     a2 = sinf(z1) * 4450;
     g1 = sinf(0.000223f * x) * 1722;
     g2 = sinf(0.000212f * z) * 1601;
     h1 = 3000 - fabs(3000 - (int)(x*0.0011f) % 6000);
     h2 = 4000 - fabs(4000 - (int)(z*0.00012f) % 8000);
     height += 8000 - fabs(a1) - fabs(a2) - fabs(g1) - fabs(g2) + h1 * 6.2f + h2 * 6.9f;
     if (height > 2500 || height < 1000) {
          a1 = height < 1000 ? (1000 - height) * 0.001f : 0;
          a1 = a1 > 1 ? 1 : a1;
          a2 = height > 2500 ? (height - 2500) * 0.0007f : 0;
          a2 = a2 > 1 ? 1 : a2;
          h1 = sinf(x * 0.00077f) * 310 * (a1 + a2);
          h2 = sinf(z * 0.00072f) * 316 * (a1 + a2);
          height += - fabs(h1) - fabs(h2);
     }
     if (height > 2000) {
          a2 = (height - 2000) * 0.0005f;
          a2 = a2 > 2 ? 2 : a2;
          g1 = sinf(x * 0.0017f) * 53 * a2;
          g2 = sinf(z * 0.0014f) * 60 * a2;
          height += - fabs(g1) - fabs(g2);
     }
     return height;
}


float algorithmicTerrainHeight4(float x, float z, float height)
{
     float h1, h2;
     h1 = 1000 - fabs(1000 - (int)(fabs(x*0.3f)) % 2000);
     h2 = 1000 - fabs(1000 - (int)(fabs(z*0.3f)) % 2000);
     return h1 + h2;
}


float algorithmicTerrainHeight5(float x, float z)
{
     float height = 0.0f, x1, z1;
     height = -500;
     x1 = 1.5f - sinf(x * 0.001f);
     if (x1 < 0)
          x1 = 0;
     else if (x1 > 1)
          x1 = 1;
     z1 = 1.5f - sinf(z * 0.001f);
     if (z1 < 0)
          z1 = 0;
     else if (z1 > 1)
          z1 = 1;
     height += (500.0f - height) * x1 * z1;
     return height;
}


float algorithmicTerrainHeight6(float x, float z)
{
     float hlevel = sinf(x * 0.00011f) + sinf(z * 0.00013f);
     float height = hlevel * 67 + 137;
     float temp1 = sinf(z * 0.00083f) + sinf(x * 0.00097f) + hlevel;
     float temp2 = cosf((x * 0.67f + z - 1423.0f) * 0.0027f);
     float temp3 = sin(z * 0.001172f - temp1) + sinf(x * 0.001021f + temp1);
     float temp4 = sinf(z * 0.00212f + temp2) + cosf(x * 0.00171f - temp2);
     float temp5;
     float grain1 = -fabs(temp3 * 11 - 2);
     float grain2 = -fabs(temp4 * 11 + 5);
     temp1 = (temp1 + cosf(hlevel)) * 0.5f;
     if (grain2 < -30.0f)
          grain2 -= (grain2 + 30.0f) * 0.93f;
     if (grain2 > -30.0f)
          grain2 += (grain2 + 30.0f) * 0.53f;
     temp3 = cosf((temp3 * hlevel - temp4) * (temp1 * 0.17f));
     temp5 = temp3 * (temp1 + temp2) * (temp1 - temp3) * 11;
     if (temp5 < 120.0f)
          temp5 = temp5 + temp3 * (1 - (temp5 / 120.0f)) * hlevel * 12;
     else
          temp5 = temp5 - temp4 * (1 - (temp5 / 120.0f)) * 6;
     height += hlevel + grain1 + grain2 + temp5;
     height += height * (height - 10.0f) * 0.04f;
     if (height < TERRAIN_WATER_LEVEL)
          height += height * 1.46f;
     else
          height += 25.0f;
     return height;
}


float algorithmicTerrainHeight7(float x, float z)
{
     float height = 0;
     float temp1 = sinf(z * 0.00083f) + sinf(x * 0.00097f) * 0.5f;
     float temp2 = cosf((x * 0.67f + z - 1423.0f) * 0.0027f);
     float temp3 = sin(z * 0.001172f - temp1) + sinf(x * 0.001021f + temp1);
     float temp4 = sinf(z * 0.00212f + temp2) + cosf(x * 0.00171f - temp2);
     float temp5;
     float grain1 = -fabs(temp3 * 11 - 2);
     float grain2 = -fabs(temp4 * 11 + 5);
     if (grain2 < -30.0f)
          grain2 -= (grain2 + 30.0f) * 0.93f;
     if (grain2 > -30.0f)
          grain2 += (grain2 + 30.0f) * 0.53f;
     temp3 = cosf((temp3 - temp4) * (temp1 * 0.17f));
     temp5 = temp3 * (temp1 + temp2) * (temp1 - temp3) * 11;
     if (temp5 < 120.0f)
          temp5 = temp5 + temp3 * (1 - (temp5 / 120.0f)) * 12;
     else
          temp5 = temp5 - temp4 * (1 - (temp5 / 120.0f)) * 6;
     height += grain1 + grain2 + temp5;
     height += height * (height - 10.0f) * 0.04f;
     if (height < TERRAIN_WATER_LEVEL)
          height += height * 1.46f;
     else
          height += 25.0f;
     return height;
}


float algorithmicTerrainHeight8a(float x, float z)
{
     float height = 0;
     int x1, z1, h, g1, g2;
     g1 = fabs(sinf(x * 0.00013f) + sinf(z * 0.00015f));
     g2 = g1 * 127 + 2213;
     x1 = x / g2;
     z1 = z / g2;
     h  = floor(fabs(x1 * z1));
     height = (h % 5) + 5.2f;
     h      = floor(height * fabs((0.09f + x1) * 0.71f - (0.12f + z1) * 0.77f));
     height = (height * 0.8f + (h % 3) * 1.23f) * 343 - g1 * 671 - 1200;
     return height;
}


float algorithmicTerrainHeight8b(float x, float z, float o)
{
     float h1, h2;
     h1 = algorithmicTerrainHeight8a(x + o, z + o);
     h2 = algorithmicTerrainHeight8a(x - o, z - o);
     return (h1 + h2) * 0.5f;
}


float algorithmicTerrainHeight8(float x, float z)
{
     float o = (sinf(x * 0.00021f) + sinf(z * 0.00022f)) * 857;
     return (algorithmicTerrainHeight8b(x, z, 2312 + o) + algorithmicTerrainHeight8b(x, z, 1721 + o)) * 0.5f;
}


char calculateTerrainType(float height)
{
     char type = T_TYPE_DIRT;
     if (height <= TERRAIN_WATER_LEVEL)
          type = T_TYPE_ROCK;
     else if (height < TERRAIN_WATER_LEVEL + 100)
          type = T_TYPE_DIRT;
     else if (height < 1000)
          type = T_TYPE_GRASS1;
     else if (height < 2000)
          type = T_TYPE_GRASS2;
     else if (height < 3500)
          type = T_TYPE_GRASS3;
     else if (height < 4200)
          type = T_TYPE_DIRT;
     else
          type = T_TYPE_SNOW;
     return type;
}


struct terrain algorithmicTerrain2(float x, float z)
{
     struct terrain temp;
     temp.height = algorithmicTerrainHeight7(x, z);
     temp.type   = calculateTerrainType(temp.height);
     return temp;
}


struct terrain algorithmicTerrain(float xi, float zi)
{
     struct terrain temp;
     float a, b, c, x1, z1;
     float x = xi * 1.3f, z = zi * 1.3f;
     temp.height = 0.0f;
     temp.mod    = 0.0f;
     temp.type   = T_TYPE_NULL;
     x1 = 1.0f - fabs(sinf(x * 0.000037f));
     z1 = 1.0f - fabs(cosf(z * 0.000043f + x1 * 0.01f));
     a = (x1 + 0.13f) * z1 - 0.1f;
     b = 0.7f - x1 * (z1 + 0.71f);
     if (a > 1) {
          a = 1;
          temp.height += algorithmicTerrainHeight1(z * 0.53f, x * 0.59f);
     }
     else if (a < 0)
          a = 0;
     else
          temp.height += algorithmicTerrainHeight1(z * 0.53f, x * 0.59f) * a;
     if (b > 1) {
          b = 1;
          temp.height += algorithmicTerrainHeight6(z * 0.2f, x * 0.2f) * 1.5f;
     }
     else if (b < 0)
          b = 0;
     else
          temp.height += algorithmicTerrainHeight6(z * 0.2f, x * 0.2f) * 1.5f * b;
     if (a + b < 0.4f)
          temp.height += algorithmicTerrainHeight8(x, z) * (0.4f - a - b) * 1.37f;
     if (temp.height > 2500) {
          c = (temp.height - 2500) * 0.0005f;
          if (c > 0.2f)
               c = 0.2f;
          temp.height += c * fabs(algorithmicTerrainHeight2(z * 0.3f, x * 0.3f));
     }
     if (temp.height > TERRAIN_WATER_LEVEL)
          temp.height += 35.0f;
     if (temp.height < 600) {
          x1 = 0.96f - fabs(sinf(x * 0.00007f + a));
          if (x1 < 0)
               x1 = 0;
          else if (x1 > 1)
               x1 = 1;
          z1 = 0.93f - fabs(cosf(z * 0.00007f + b));
          if (z1 < 0)
               z1 = 0;
          else if (z1 > 1)
               z1 = 1;
          temp.height += algorithmicTerrainHeight7(z * 0.4f, x * 0.4f) * x1 * z1;
          temp.mod = x1 * z1;
          if (temp.mod > 0.21f)
               temp.type = T_TYPE_DESERT;
     }
     else if (temp.height > 600 && temp.height < 1000) {
          x1 = 1.0f - fabs(sinf(x * 0.00041f + a * 2.3f));
          if (x1 < 0)
               x1 = 0;
          else if (x1 > 1)
               x1 = 1;
          z1 = 0.93f - fabs(cosf(z * 0.00037f + b));
          if (z1 < 0)
               z1 = 0;
          else if (z1 > 1)
               z1 = 1;
          temp.mod = x1 * z1;
          if (x1 * z1 > 0.19f)
               temp.type = T_TYPE_FOREST1;
     }
     else if (temp.height > 1000 && temp.height < 2000) {
          x1 = 1.0f - fabs(cosf(x * 0.00036f));
          if (x1 < 0)
               x1 = 0;
          else if (x1 > 1)
               x1 = 1;
          z1 = 1.0f - fabs(sinf(z * 0.00031f));
          if (z1 < 0)
               z1 = 0;
          else if (z1 > 1)
               z1 = 1;
          temp.mod = x1 * z1;
          if (x1 * z1 > 0.21f)
               temp.type = T_TYPE_FOREST2;
     }
     if (temp.height > 0 && temp.height < 1200) {
          x1 = 0.871f - fabs(sinf(x * 0.00018f - b));
          if (x1 < 0)
               x1 = 0;
          else if (x1 > 1)
               x1 = 1;
          z1 = 0.787f - fabs(cosf(z * 0.00019f));
          if (z1 < 0)
               z1 = 0;
          else if (z1 > 1)
               z1 = 1;
          temp.height += (600 - temp.height) * x1 * z1 * 0.43f;
          if (temp.height > 150 && x1 * z1 > 0.55f)
               temp.type = T_TYPE_VILLAGE;
     }
     if (temp.type == T_TYPE_NULL)
          temp.type = calculateTerrainType(temp.height);
     return temp;
}


float readTerrainHeight(float x, float z)
{
     struct terrain temp = algorithmicTerrain(x, z);
     return temp.height;
}


unsigned char readTerrainType(float x, float z)
{
     struct terrain temp = algorithmicTerrain(x, z);
     return temp.type;
}


float readTerrainHeightPlane(float x, float z, struct v3f *normal, int t_size)
{
     int xgrid, zgrid;
     float x1, z1, x2, z2, x3 = 0, z3 = 0;
     float height, p[2] = {x, z}, v1[3], v2[3], v3[3];
     for (xgrid = 0, x2 = 5000000; xgrid < 4; xgrid++) {
          x1 = (xgrid - 2) * t_size + x - (int) x % t_size;
          x2 = fabs(x - x1) < x2 ? fabs(x - x1) : x2;
          if (fabs(x - x1) > x2)
               break;
          else
               x3 = x1;
     }
     for (zgrid = 0, z2 = 5000000; zgrid < 4; zgrid++) {
          z1 = (zgrid - 2) * t_size + z - (int) z % t_size;
          z2 = fabs(z - z1) < z2 ? fabs(z - z1) : z2;
          if (fabs(z - z1) > z2)
               break;
          else
               z3 = z1;
     }
     x1 = x3 + t_size / 2;
     x2 = x3 - t_size / 2;
     z1 = z3 - t_size / 2;
     z2 = z3 + t_size / 2;
     if (distance2d(mv3f(x, 0, z), mv3f(x1, 0, z1)) < distance2d(mv3f(x, 0, z), mv3f(x2, 0, z2))) {
          v1[0] = x2; v1[1] = readTerrainHeight(x2, z1); v1[2] = z1;
          v2[0] = x1; v2[1] = readTerrainHeight(x1, z1); v2[2] = z1;
          v3[0] = x1; v3[1] = readTerrainHeight(x1, z2); v3[2] = z2;
          height = planeHeight(p, v1, v2, v3, normal);
     }
     else {
          v1[0] = x2; v1[1] = readTerrainHeight(x2, z2); v1[2] = z2;
          v2[0] = x1; v2[1] = readTerrainHeight(x1, z2); v2[2] = z2;
          v3[0] = x2; v3[1] = readTerrainHeight(x2, z1); v3[2] = z1;
          height = planeHeight(p, v1, v2, v3, normal);
     }
     return height;
}


float readTerrainHeightPlane2(float x, float z, int t_size)
{
     struct v3f normal;
     return readTerrainHeightPlane(x, z, &normal, t_size);
}


void moveTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int t_size, char *swapb)
{
     if (distance2d(camerapos, mv3f(sector->x, 0.0f, sector->y)) > TERRAIN_STEP_SIZE * t_size) {
          sector->x = camerapos.x;
          sector->y = camerapos.z;
          *swapb = 0;
     }
}


void drawTerrain(GLuint *textures, struct v3f camerapos, struct v3f camerarot, struct v2f *sector,
                 int *t_size, char *swapb)
{
     struct terrain temp1, temp2;
     struct v3f temp3f;
     static int size = 0;
     int xgrid, zgrid, x1, z1, x2, z2, x3, z3, cull;
     float x, z, xpos = 0.0f, zpos = 0.0f, dist;
     float v1[3], v2[3], v3[3];
     static unsigned char NEcolorR [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static unsigned char NEcolorG [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static unsigned char NEcolorB [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static unsigned char SEcolorR [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static unsigned char SEcolorG [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static unsigned char SEcolorB [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NEx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NEy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NEz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SEx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SEy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SEz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SWx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SWy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double SWz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NWx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NWy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static double NWz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Nnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Nnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Nnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Snormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Snormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float Snormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NEnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NEnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NEnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NWnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NWnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float NWnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SEnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SEnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SEnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SWnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SWnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     static float SWnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
     glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LESS);
     glEnable(GL_FOG);
     glEnable(GL_TEXTURE_2D);
     glEnable(GL_LIGHTING);
     glEnable(GL_NORMALIZE);
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN_2]);
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN_1]);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
     glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
     glMateriali(GL_FRONT, GL_SHININESS, 11);
     if (camerapos.y < TERRAIN_SCALE_HEIGHT)
          *t_size = TERRAIN_SQUARE_SIZE;
     else
          *t_size = TERRAIN_SQUARE_SIZE * 3;
     if (size != *t_size) {
          size = *t_size;
          *swapb = 0;
     }
     moveTerrain(camerapos, camerarot, sector, *t_size, swapb);
     x = (int) (sector->x / *t_size);
     z = (int) (sector->y / *t_size);
     for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
          x3 = fabs(TERRAIN_GRID_SIZE_HALF - xgrid) - 20; x3 = x3 < 0 ? 0 : (x3 + 20) * 8;
          z3 = fabs(TERRAIN_GRID_SIZE_HALF - zgrid) - 20; z3 = z3 < 0 ? 0 : (z3 + 20) * 8;
          xpos = (xgrid - TERRAIN_GRID_SIZE_HALF) * (x3 + *t_size) + x * *t_size;
          zpos = (zgrid - TERRAIN_GRID_SIZE_HALF) * (z3 + *t_size) + z * *t_size;
          dist = distance2d(camerapos, mv3f(xpos, 0.0f, zpos));
          if (xgrid > TERRAIN_GRID_SIZE_HALF + 19) {
               x1 = xpos + x3 - 3040.0f + *t_size / 2;
               x2 = xpos - x3 - 3040.0f - *t_size / 2;
          }
          else if (xgrid < TERRAIN_GRID_SIZE_HALF - 19) {
               x1 = xpos + x3 + 3040.0f + *t_size / 2;
               x2 = xpos - x3 + 3040.0f - *t_size / 2;
          }
          else {
               x1 = xpos + x3 + *t_size / 2;
               x2 = xpos - x3 - *t_size / 2;
          }
          if (zgrid > TERRAIN_GRID_SIZE_HALF + 19) {
               z1 = zpos + z3 - 3040.0f + *t_size / 2;
               z2 = zpos - z3 - 3040.0f - *t_size / 2;
          }
          else if (zgrid < TERRAIN_GRID_SIZE_HALF - 19) {
               z1 = zpos + z3 + 3040.0f + *t_size / 2;
               z2 = zpos - z3 + 3040.0f - *t_size / 2;
          }
          else {
               z1 = zpos + z3 + *t_size / 2;
               z2 = zpos - z3 - *t_size / 2;
          }
          cull = fabs((int) (camerarot.y - 180 - vectorstodegree2d(camerapos, mv3f(xpos, 0, zpos))));
          while (cull >= 360)
               cull -= 360;
          if (camerarot.x > 47.0f || cull <= 75 || cull >= 285 || dist < *t_size * 3.5f || *swapb == 0) {
               if (*swapb == 0) {
                    NEx[xgrid][zgrid] = x1;
                    NEz[xgrid][zgrid] = z2;
                    temp1 = algorithmicTerrain (NEx[xgrid][zgrid], NEz[xgrid][zgrid]);
                    NEy[xgrid][zgrid] = (int) temp1.height;
                    NWx[xgrid][zgrid] = x2;
                    NWz[xgrid][zgrid] = z2;
                    NWy[xgrid][zgrid] = (int) readTerrainHeight (NWx[xgrid][zgrid], NWz[xgrid][zgrid]);
                    SEx[xgrid][zgrid] = x1;
                    SEz[xgrid][zgrid] = z1;
                    temp2 = algorithmicTerrain (SEx[xgrid][zgrid], SEz[xgrid][zgrid]);
                    SEy[xgrid][zgrid] = (int) temp2.height;
                    SWx[xgrid][zgrid] = x2;
                    SWz[xgrid][zgrid] = z1;
                    SWy[xgrid][zgrid] = (int) readTerrainHeight (SWx[xgrid][zgrid], SWz[xgrid][zgrid]);
                    switch (temp2.type) {
                    case T_TYPE_GRASS1:
                         x1 = temp2.mod > 0.15f ? temp2.height < 300.0f ? 11 : -29 : 0;
                         x2 = ((1000 - SEy[xgrid][zgrid]) / 900.0f) * 70.0f;
                         SEcolorR[xgrid][zgrid] = 40 + x1 + x2;
                         SEcolorG[xgrid][zgrid] = 108;
                         SEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_GRASS2:
                         z1 = temp2.mod > 0.11f ? 5 : 0;
                         z2 = ((2000 - SEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
                         SEcolorR[xgrid][zgrid] = 65 - z1 - z2;
                         SEcolorG[xgrid][zgrid] = 100 - z1;
                         SEcolorB[xgrid][zgrid] = 15 + z1;
                         break;
                    case T_TYPE_GRASS3:
                         x1 = ((5000 - SEy[xgrid][zgrid]) / 3000.0f) * 45.0f;
                         SEcolorR[xgrid][zgrid] = 103 - x1;
                         SEcolorG[xgrid][zgrid] = 100;
                         z1 = ((5000 - SEy[xgrid][zgrid]) / 3000.0f) * 37.0f;
                         SEcolorB[xgrid][zgrid] = 52 - z1;
                         break;
                    case T_TYPE_ROCK:
                         x1 = SEy[xgrid][zgrid] < TERRAIN_WATER_LEVEL ? SEy[xgrid][zgrid] * 0.041f : 0;
                         if (x1 < -77)
                              x1 = -77;
                         else if (x1 > 0)
                              x1 = 0;
                         SEcolorR[xgrid][zgrid] = 101 + x1;
                         SEcolorG[xgrid][zgrid] = 106 + x1;
                         SEcolorB[xgrid][zgrid] = 88 + x1;
                         break;
                    case T_TYPE_DIRT:
                         SEcolorR[xgrid][zgrid] = 103;
                         SEcolorG[xgrid][zgrid] = 111;
                         SEcolorB[xgrid][zgrid] = 63;
                         break;
                    case T_TYPE_DESERT:
                         SEcolorR[xgrid][zgrid] = 90;
                         SEcolorG[xgrid][zgrid] = 100;
                         SEcolorB[xgrid][zgrid] = 35;
                         break;
                    case T_TYPE_CRATER:
                         SEcolorR[xgrid][zgrid] = 63;
                         SEcolorG[xgrid][zgrid] = 66;
                         SEcolorB[xgrid][zgrid] = 43;
                         break;
                    case T_TYPE_VILLAGE:
                         SEcolorR[xgrid][zgrid] = 65;
                         SEcolorG[xgrid][zgrid] = 95;
                         SEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_FOREST1:
                         z1 = ((2000 - SEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
                         SEcolorR[xgrid][zgrid] = 65 - z1;
                         SEcolorG[xgrid][zgrid] = 100;
                         SEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_FOREST2:
                         SEcolorR[xgrid][zgrid] = 48;
                         SEcolorG[xgrid][zgrid] = 90;
                         SEcolorB[xgrid][zgrid] = 27;
                         break;
                    case T_TYPE_SNOW:
                         SEcolorR[xgrid][zgrid] = 250;
                         SEcolorG[xgrid][zgrid] = 250;
                         SEcolorB[xgrid][zgrid] = 250;
                         break;
                    default:
                         SEcolorR[xgrid][zgrid] = 93;
                         SEcolorG[xgrid][zgrid] = 87;
                         SEcolorB[xgrid][zgrid] = 55;
                         break;
                    }
                    switch (temp1.type) {
                    case T_TYPE_GRASS1:
                         x1 = temp1.mod > 0.15f ? temp1.height < 300.0f ? 11 : -29 : 0;
                         x2 = ((1000 - NEy[xgrid][zgrid]) / 900.0f) * 70.0f;
                         NEcolorR[xgrid][zgrid] = 40 + x1 + x2;
                         NEcolorG[xgrid][zgrid] = 108;
                         NEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_GRASS2:
                         z1 = temp1.mod > 0.11f ? 5 : 0;
                         z2 = ((2000 - NEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
                         NEcolorR[xgrid][zgrid] = 65 - z1 - z2;
                         NEcolorG[xgrid][zgrid] = 100 - z1;
                         NEcolorB[xgrid][zgrid] = 15 + z1;
                         break;
                    case T_TYPE_GRASS3:
                         x1 = ((5000 - NEy[xgrid][zgrid]) / 3000.0f) * 45.0f;
                         NEcolorR[xgrid][zgrid] = 103 - x1;
                         NEcolorG[xgrid][zgrid] = 100;
                         z1 = ((5000 - NEy[xgrid][zgrid]) / 3000.0f) * 37.0f;
                         NEcolorB[xgrid][zgrid] = 52 - z1;
                         break;
                    case T_TYPE_ROCK:
                         x1 = NEy[xgrid][zgrid] < TERRAIN_WATER_LEVEL ? NEy[xgrid][zgrid] * 0.041f : 0;
                         if (x1 < -77)
                              x1 = -77;
                         else if (x1 > 0)
                              x1 = 0;
                         NEcolorR[xgrid][zgrid] = 101 + x1;
                         NEcolorG[xgrid][zgrid] = 106 + x1;
                         NEcolorB[xgrid][zgrid] = 88 + x1;
                         break;
                    case T_TYPE_DIRT:
                         NEcolorR[xgrid][zgrid] = 103;
                         NEcolorG[xgrid][zgrid] = 111;
                         NEcolorB[xgrid][zgrid] = 63;
                         break;
                    case T_TYPE_DESERT:
                         NEcolorR[xgrid][zgrid] = 90;
                         NEcolorG[xgrid][zgrid] = 100;
                         NEcolorB[xgrid][zgrid] = 35;
                         break;
                    case T_TYPE_CRATER:
                         SEcolorR[xgrid][zgrid] = 63;
                         SEcolorG[xgrid][zgrid] = 66;
                         SEcolorB[xgrid][zgrid] = 43;
                         break;
                    case T_TYPE_VILLAGE:
                         NEcolorR[xgrid][zgrid] = 65;
                         NEcolorG[xgrid][zgrid] = 95;
                         NEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_FOREST1:
                         z1 = ((2000 - NEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
                         NEcolorR[xgrid][zgrid] = 65 - z1;
                         NEcolorG[xgrid][zgrid] = 100;
                         NEcolorB[xgrid][zgrid] = 15;
                         break;
                    case T_TYPE_FOREST2:
                         NEcolorR[xgrid][zgrid] = 48;
                         NEcolorG[xgrid][zgrid] = 90;
                         NEcolorB[xgrid][zgrid] = 27;
                         break;
                    case T_TYPE_SNOW:
                         NEcolorR[xgrid][zgrid] = 250;
                         NEcolorG[xgrid][zgrid] = 250;
                         NEcolorB[xgrid][zgrid] = 250;
                         break;
                    default:
                         NEcolorR[xgrid][zgrid] = 93;
                         NEcolorG[xgrid][zgrid] = 87;
                         NEcolorB[xgrid][zgrid] = 55;
                         break;
                    }
               }
               v1[0] = (GLfloat) SEx[xgrid][zgrid];
               v1[1] = (GLfloat) SEy[xgrid][zgrid];
               v1[2] = (GLfloat) SEz[xgrid][zgrid];
               v2[0] = (GLfloat) NEx[xgrid][zgrid];
               v2[1] = (GLfloat) NEy[xgrid][zgrid];
               v2[2] = (GLfloat) NEz[xgrid][zgrid];
               v3[0] = (GLfloat) NWx[xgrid][zgrid];
               v3[1] = (GLfloat) NWy[xgrid][zgrid];
               v3[2] = (GLfloat) NWz[xgrid][zgrid];
               temp3f = calcNormal(v1, v2, v3);
               Nnormx[xgrid][zgrid] = temp3f.x;
               Nnormy[xgrid][zgrid] = temp3f.y;
               Nnormz[xgrid][zgrid] = temp3f.z;
               v2[0] = (GLfloat) SWx[xgrid][zgrid];
               v2[1] = (GLfloat) SWy[xgrid][zgrid];
               v2[2] = (GLfloat) SWz[xgrid][zgrid];
               temp3f = calcNormal(v1, v3, v2);
               Snormx[xgrid][zgrid] = temp3f.x;
               Snormy[xgrid][zgrid] = temp3f.y;
               Snormz[xgrid][zgrid] = temp3f.z;
               x2 = xgrid - 1;
               z2 = zgrid - 1;
               NWnormx[xgrid][zgrid] = (Nnormx[xgrid][zgrid] + Snormx[xgrid][z2]
                                        + Nnormx[x2][z2] + Snormx[x2][z2]
                                        + Nnormx[x2][zgrid] + Snormx[xgrid][zgrid]) / 6;
               NWnormy[xgrid][zgrid] = (Nnormy[xgrid][zgrid] + Snormy[xgrid][z2]
                                        + Nnormy[x2][z2] + Snormy[x2][z2]
                                        + Nnormy[x2][zgrid] + Snormy[xgrid][zgrid]) / 6;
               NWnormz[xgrid][zgrid] = (Nnormz[xgrid][zgrid] + Snormz[xgrid][z2]
                                        + Nnormz[x2][z2] + Snormz[x2][z2]
                                        + Nnormz[x2][zgrid] + Snormz[xgrid][zgrid]) / 6;
               x2 = xgrid + 1;
               z2 = zgrid + 1;
               SEnormx[xgrid][zgrid] = (Nnormx[xgrid][zgrid] + Snormx[xgrid][zgrid]
                                        + Nnormx[xgrid][z2] + Snormx[x2][z2]
                                        + Nnormx[x2][z2] + Snormx[x2][zgrid]) / 6;
               SEnormy[xgrid][zgrid] = (Nnormy[xgrid][zgrid] + Snormy[xgrid][zgrid]
                                        + Nnormy[xgrid][z2] + Snormy[x2][z2]
                                        + Nnormy[x2][z2] + Snormy[x2][zgrid]) / 6;
               SEnormz[xgrid][zgrid] = (Nnormz[xgrid][zgrid] + Snormz[xgrid][zgrid]
                                        + Nnormz[xgrid][z2] + Snormz[x2][z2]
                                        + Nnormz[x2][z2] + Snormz[x2][zgrid]) / 6;
               x2 = xgrid - 1;
               z2 = zgrid + 1;
               SWnormx[xgrid][zgrid] = (Snormx[xgrid][zgrid] + Nnormx[x2][zgrid]
                                        + Snormx[x2][zgrid] + Nnormx[x2][z2]
                                        + Snormx[xgrid][z2] + Nnormx[xgrid][z2]) / 6;
               SWnormy[xgrid][zgrid] = (Snormy[xgrid][zgrid] + Nnormy[x2][zgrid]
                                        + Snormy[x2][zgrid] + Nnormy[x2][z2]
                                        + Snormy[xgrid][z2] + Nnormy[xgrid][z2]) / 6;
               SWnormz[xgrid][zgrid] = (Snormz[xgrid][zgrid] + Nnormz[x2][zgrid]
                                        + Snormz[x2][zgrid] + Nnormz[x2][z2]
                                        + Snormz[xgrid][z2] + Nnormz[xgrid][z2]) / 6;
               x2 = xgrid + 1;
               z2 = zgrid - 1;
               NEnormx[xgrid][zgrid] = (Nnormx[xgrid][zgrid] + Snormx[x2][zgrid]
                                        + Nnormx[x2][zgrid] + Snormx[x2][z2]
                                        + Nnormx[xgrid][z2] + Snormx[xgrid][z2]) / 6;
               NEnormy[xgrid][zgrid] = (Nnormy[xgrid][zgrid] + Snormy[x2][zgrid]
                                        + Nnormy[x2][zgrid] + Snormy[x2][z2]
                                        + Nnormy[xgrid][z2] + Snormy[xgrid][z2]) / 6;
               NEnormz[xgrid][zgrid] = (Nnormz[xgrid][zgrid] + Snormz[x2][zgrid]
                                        + Nnormz[x2][zgrid] + Snormz[x2][z2]
                                        + Nnormz[xgrid][z2] + Snormz[xgrid][z2]) / 6;
               if (*swapb) {
                    glBegin(GL_TRIANGLE_STRIP);
                    if (xgrid <= 0)
                         glColor3ub(122, 122, 122);
                    else
                         glColor3ub(SEcolorR[xgrid-1][zgrid],
                                    SEcolorG[xgrid-1][zgrid],
                                    SEcolorB[xgrid-1][zgrid]);
                    //glVertexAttrib3fARB(7, SWnormx[xgrid][zgrid], SWnormy[xgrid][zgrid], SWnormz[xgrid][zgrid]);
                    glNormal3f(SWnormx[xgrid][zgrid],
                               SWnormy[xgrid][zgrid],
                               SWnormz[xgrid][zgrid]);
                    glMultiTexCoord2f(GL_TEXTURE0_ARB,
                                      SWx[xgrid][zgrid] * 0.00012f,
                                      SWz[xgrid][zgrid] * 0.00012f);
                    glMultiTexCoord2f(GL_TEXTURE1_ARB,
                                      SWx[xgrid][zgrid] * 0.047f,
                                      SWz[xgrid][zgrid] * 0.047f);
                    glVertex3d(SWx[xgrid][zgrid],
                               SWy[xgrid][zgrid],
                               SWz[xgrid][zgrid]);
                    if (xgrid >= TERRAIN_GRID_SIZE - 1)
                         glColor3ub(122, 122, 122);
                    else
                         glColor3ub(SEcolorR[xgrid][zgrid],
                                    SEcolorG[xgrid][zgrid],
                                    SEcolorB[xgrid][zgrid]);
                    //glVertexAttrib3fARB(7, SEx[xgrid][zgrid], SEy[xgrid][zgrid], SEz[xgrid][zgrid]);
                    glNormal3f(SEnormx[xgrid][zgrid],
                               SEnormy[xgrid][zgrid],
                               SEnormz[xgrid][zgrid]);
                    glMultiTexCoord2f(GL_TEXTURE0_ARB,
                                      SEx[xgrid][zgrid] * 0.00012f,
                                      SEz[xgrid][zgrid] * 0.00012f);
                    glMultiTexCoord2f(GL_TEXTURE1_ARB,
                                      SEx[xgrid][zgrid] * 0.047f,
                                      SEz[xgrid][zgrid] * 0.047f);
                    glVertex3d(SEx[xgrid][zgrid],
                               SEy[xgrid][zgrid],
                               SEz[xgrid][zgrid]);
                    if (xgrid <= 0)
                         glColor3ub(122, 122, 122);
                    else
                         glColor3ub(NEcolorR[xgrid-1][zgrid],
                                    NEcolorG[xgrid-1][zgrid],
                                    NEcolorB[xgrid-1][zgrid]);
                    //glVertexAttrib3fARB(7, NWx[xgrid][zgrid], NWy[xgrid][zgrid], NWz[xgrid][zgrid]);
                    glNormal3f(NWnormx[xgrid][zgrid],
                               NWnormy[xgrid][zgrid],
                               NWnormz[xgrid][zgrid]);
                    glMultiTexCoord2f(GL_TEXTURE0_ARB,
                                      NWx[xgrid][zgrid] * 0.00012f,
                                      NWz[xgrid][zgrid] * 0.00012f);
                    glMultiTexCoord2f(GL_TEXTURE1_ARB,
                                      NWx[xgrid][zgrid] * 0.047f,
                                      NWz[xgrid][zgrid] * 0.047f);
                    glVertex3d(NWx[xgrid][zgrid],
                               NWy[xgrid][zgrid],
                               NWz[xgrid][zgrid]);
                    if (xgrid >= TERRAIN_GRID_SIZE - 1)
                         glColor3ub(122, 122, 122);
                    else
                         glColor3ub(NEcolorR[xgrid][zgrid],
                                    NEcolorG[xgrid][zgrid],
                                    NEcolorB[xgrid][zgrid]);
                    //glVertexAttrib3fARB(7, NEx[xgrid][zgrid], NEy[xgrid][zgrid], NEz[xgrid][zgrid]);
                    glNormal3f(NEnormx[xgrid][zgrid],
                               NEnormy[xgrid][zgrid],
                               NEnormz[xgrid][zgrid]);
                    glMultiTexCoord2f(GL_TEXTURE0_ARB,
                                      NEx[xgrid][zgrid] * 0.00012f,
                                      NEz[xgrid][zgrid] * 0.00012f);
                    glMultiTexCoord2f(GL_TEXTURE1_ARB,
                                      NEx[xgrid][zgrid] * 0.047f,
                                      NEz[xgrid][zgrid] * 0.047f);
                    glVertex3d(NEx[xgrid][zgrid],
                               NEy[xgrid][zgrid],
                               NEz[xgrid][zgrid]);
                    glEnd();
               }
          }
          if (xgrid >= TERRAIN_GRID_SIZE - 1) {
               zgrid++;
               xgrid = -1;
          }
     }
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glDisable(GL_TEXTURE_2D);
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
