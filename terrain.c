#include <GLFW/glfw3.h>
#include "maths.h"


float algorithmicTerrainHeight1(float x, float z, float height)
{
  float x1, z1, a1, a2, h1, h2, g1, g2;

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
  height += 20000 - fabs(a1) - fabs(a2) - fabs(g1) - fabs(g2) + h1 * 5.2f + h2 * 6.7f;
  x += 2331.2f;
  z += 5213.3f;
  if (height > 2000 || height < 1500) {
    a1 = height < 1500 ? (1500 - height) * 0.001f : 0;
    a1 = a1 > 1 ? 1 : a1;
    a2 = height > 2000 ? (height - 2000) * 0.0005f : 0;
    a2 = a2 > 1 ? 1 : a2;
    h1 = sinf(x * 0.00077f) * 710 * (a1 + a2);
    h2 = sinf(z * 0.00072f) * 716 * (a1 + a2);
    height += - fabs(h1) - fabs(h2);
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


float algorithmicTerrainHeight2(float x, float z, float height)
{
  float x1, z1, a1, a2, h1, h2, g1, g2;

  x1 = 0.00014f * x;
  z1 = 0.00021f * z;
  a1 = sinf(x1 * 0.37f) * 12700;
  a2 = sinf(z1 - 0.07f * x1) * 4950;
  g1 = sinf(0.000223f * x) * 2742;
  g2 = sinf(0.000212f * z) * 3721;
  h1 = 6000 - fabs(6000 - (int)(x*0.00092f) % 12000);
  h2 = 5000 - fabs(5000 - (int)(z*0.00009f) % 10000);
  height += 21000 - fabs(a1) - fabs(a2) - fabs(g1) - fabs(g2) + h1 * 6.2f + h2 * 6.9f;
  if (height > 2000 || height < 1500) {
    a1 = height < 1500 ? (1500 - height) * 0.001f : 0;
    a1 = a1 > 1 ? 1 : a1;
    a2 = height > 2000 ? (height - 2000) * 0.0005f : 0;
    a2 = a2 > 1 ? 1 : a2;
    h1 = sinf(x * 0.00077f) * 910 * (a1 + a2);
    h2 = sinf(z * 0.00072f) * 916 * (a1 + a2);
    height += - fabs(h1) - fabs(h2);
  }
  height += (1500 - height) * 0.26f + (height - 1000) * (1000 - height) * 0.00017f;
  if (height > 3000) {
    a2 = (height - 3000) * 0.0005f;
    a2 = a2 > 2 ? 2 : a2;
    g1 = sinf(x * 0.0061f) * 60 * a2;
    g2 = sinf(z * 0.0063f) * 62 * a2;
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
    g1 = sinf(x * 0.0057f) * 53 * a2;
    g2 = sinf(z * 0.0054f) * 60 * a2;
    height += - fabs(g1) - fabs(g2);
  }

  return height;
}


float algorithmicTerrainHeight(float x, float z)
{
  float height = 0.0f, dist = 0.0f;

  height = algorithmicTerrainHeight3(z*0.67, x*0.67, height) * 1.1f - 1500;
  dist = distance2d(mv3f(-102000, 0, 131200), mv3f(x, 0, z));
  if (dist < 186000) {
    dist = (186000 - dist) / 186000;
    dist = dist < 0 ? 0 : dist;
    height += (algorithmicTerrainHeight3(x*0.11, z*0.11, height) - 2250) * dist * 2.2f;
  }
  dist = distance2d(mv3f(5000, 0, -102000), mv3f(x, 0, z));
  if (dist < 120000) {
    dist = (120000 - dist) / 120000;
    dist = dist < 0 ? 0 : dist;
    height += (algorithmicTerrainHeight1(x*0.3, z*0.3, height) - 1500) * dist * 0.71f;
  }
  dist = distance2d(mv3f(94000, 0, 17500), mv3f(x, 0, z));
  if (dist < 107000) {
    dist = (107000 - dist) / 107000;
    dist = dist < 0 ? 0 : dist;
    height += (algorithmicTerrainHeight2(x*0.4, z*0.4, height) - 750) * dist * 0.75f;
  }
  dist = distance2d(mv3f(0, 0, 0), mv3f(x, 0, z));
  if (dist < 15000) {
    dist = (15000 - dist) / 10000;
    dist = dist < 0 ? 0 : dist > 1 ? 1 : dist;
    height += (1070.0f - height) * dist;
  }
  height -= height < 1500 ? (height - 1500) * 0.46f : 0;
  height -= height > 8000 ? (height - 8000) * 0.61f : 0;

  return height;
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
  else if (height < 5000)
    type = T_TYPE_GRASS3;
  else if (height < 7000)
    type = T_TYPE_DIRT;
  else
    type = T_TYPE_ROCK;

  return type;
}


float readTerrainHeight(float x, float y) {
  struct terrain temp = readTerrain(x, y);
  return temp.height;
}


struct terrain readTerrain(float x, float z)
{
  struct terrain temp;

  temp.height = algorithmicTerrainHeight(x, z);
  temp.type = calculateTerrainType(temp.height);

  return temp;
}


void moveTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *swapb, int squaresize)
{
  //float rot;
  struct v3f center;

  //rot = camerarot.x < 75.0f ? camerarot.x : 75.0f;
  //center = degreestovector2d (camerapos, camerarot.y, 0.0f, TERRAIN_CENTRE_DISTANCE * cosf (rot / PIx180));
  center = camerapos;
  if (center.x > (sector->x + TERRAIN_STEP_SIZE * squaresize) ||
       center.x < (sector->x - TERRAIN_STEP_SIZE * squaresize)) {
    sector->x = center.x;
    *swapb = 0;
  }
  if (center.z > (sector->y + TERRAIN_STEP_SIZE * squaresize) ||
       center.z < (sector->y - TERRAIN_STEP_SIZE * squaresize)) {
    sector->y = center.z;
    *swapb = 0;
  }
}


void selectPosition(void)
{}


void drawTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *swapb, int *squaresize)
{
  struct terrain temp1, temp2;
  struct v3f temp3f;
  int xgrid, zgrid, x1, z1, x2, z2, x3, z3, alt, cull;
  static int alt2 = 0;
  const int altstep = 11;
  float x, z, xpos = 0.0f, zpos = 0.0f, dist;
  float v1[3], v2[3], v3[3];
  unsigned char NEcolorR [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  unsigned char NEcolorG [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  unsigned char NEcolorB [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  unsigned char SEcolorR [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  unsigned char SEcolorG [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  unsigned char SEcolorB [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NEx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NEy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NEz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SEx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SEy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SEz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SWx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SWy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int SWz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NWx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NWy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  int NWz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Nnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Nnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Nnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Snormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Snormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float Snormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NEnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NEnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NEnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NWnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NWnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float NWnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SEnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SEnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SEnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SWnormx [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SWnormy [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];
  float SWnormz [TERRAIN_GRID_SIZE][TERRAIN_GRID_SIZE];

  glMateriali(GL_FRONT, GL_SHININESS, 97);
  moveTerrain(camerapos, camerarot, sector, swapb, *squaresize);
  //selectPosition();

  // scaling terrain
  for (alt = 0; alt < 25000; alt++) {
    if (camheight < TERRAIN_SQUARE_SIZE * 3) {
      *squaresize = TERRAIN_SQUARE_SIZE;
      break;
    }
    else if (camheight < alt * TERRAIN_SQUARE_SIZE * altstep) {
      *squaresize = (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE * ((alt * 2) - 3) / 2);
      break;
    }
    else
      *squaresize = (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE * ((alt * 2) - 3) / 2);
  }
  if (alt2 != alt) {
    alt2 = alt;
    *swapb = 0;
  }
  // *squaresize = TERRAIN_SQUARE_SIZE;
  x = (int) (sector->x / (*squaresize));
  z = (int) (sector->y / (*squaresize));
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  //glScalef(0.05f, 0.05f, 0.05f);
  glScalef(0.0005f, 0.0005f, 0.0005f);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    if (alt < 5) {
      x3 = fabs(TERRAIN_GRID_SIZE_HALF - xgrid) - 20; x3 = x3 < 0 ? 0 : (x3 + 20) * 8;
      z3 = fabs(TERRAIN_GRID_SIZE_HALF - zgrid) - 20; z3 = z3 < 0 ? 0 : (z3 + 20) * 8;
      xpos = (xgrid - TERRAIN_GRID_SIZE_HALF) * (x3 + *squaresize) + x * *squaresize;
      zpos = (zgrid - TERRAIN_GRID_SIZE_HALF) * (z3 + *squaresize) + z * *squaresize;
      dist = distance2d(camerapos, mv3f(xpos, 0.0f, zpos));
      if (xgrid > TERRAIN_GRID_SIZE_HALF + 19) {
        x1 = xpos + x3 - 3040.0f + *squaresize / 2;
        x2 = xpos - x3 - 3040.0f - *squaresize / 2;
      }
      else if (xgrid < TERRAIN_GRID_SIZE_HALF - 19) {
        x1 = xpos + x3 + 3040.0f + *squaresize / 2;
        x2 = xpos - x3 + 3040.0f - *squaresize / 2;
      }
      else {
        x1 = xpos + x3 + *squaresize / 2;
        x2 = xpos - x3 - *squaresize / 2;
      }
      if (zgrid > TERRAIN_GRID_SIZE_HALF + 19) {
        z1 = zpos + z3 - 3040.0f + *squaresize / 2;
        z2 = zpos - z3 - 3040.0f - *squaresize / 2;
      }
      else if (zgrid < TERRAIN_GRID_SIZE_HALF - 19) {
        z1 = zpos + z3 + 3040.0f + *squaresize / 2;
        z2 = zpos - z3 + 3040.0f - *squaresize / 2;
      }
      else {
        z1 = zpos + z3 + *squaresize / 2;
        z2 = zpos - z3 - *squaresize / 2;
      }
    }
    else {
      x3 = fabs(TERRAIN_GRID_SIZE_HALF - xgrid) * 8;
      z3 = fabs(TERRAIN_GRID_SIZE_HALF - zgrid) * 8;
      xpos = (xgrid - TERRAIN_GRID_SIZE_HALF) * (x3 + *squaresize) + x * *squaresize;
      zpos = (zgrid - TERRAIN_GRID_SIZE_HALF) * (z3 + *squaresize) + z * *squaresize;
      dist = distance2d(camerapos, mv3f(xpos, 0.0f, zpos));
      x1 = xpos + x3 + *squaresize / 2;
      x2 = xpos - x3 - *squaresize / 2;
      z1 = zpos + z3 + *squaresize / 2;
      z2 = zpos - z3 - *squaresize / 2;
    }
    cull = fabs((int) (camerarot.y - 180 - vectorstodegree2d(camerapos, mv3f(xpos, 0, zpos))));
    while (cull >= 360)
      cull -= 360;
    if (camerarot.x > 27.0f || cull <= 75 || cull >= 285 || dist < *squaresize * 3.5f) {
      NEx[xgrid][zgrid] = x1;
      NEz[xgrid][zgrid] = z2;
      temp1 = readTerrain (NEx[xgrid][zgrid], NEz[xgrid][zgrid]); // color read here
      NEy[xgrid][zgrid] = (int) temp1.height;
      NWx[xgrid][zgrid] = x2;
      NWz[xgrid][zgrid] = z2;
      NWy[xgrid][zgrid] = (int) readTerrainHeight (NWx[xgrid][zgrid], NWz[xgrid][zgrid]);
      SEx[xgrid][zgrid] = x1;
      SEz[xgrid][zgrid] = z1;
      temp2 = readTerrain (SEx[xgrid][zgrid], SEz[xgrid][zgrid]); // color read here too..
      SEy[xgrid][zgrid] = (int) temp2.height;
      SWx[xgrid][zgrid] = x2;
      SWz[xgrid][zgrid] = z1;
      SWy[xgrid][zgrid] = (int) readTerrainHeight (SWx[xgrid][zgrid], SWz[xgrid][zgrid]);
      switch (temp2.type) {
      case T_TYPE_GRASS1:
        x2 = ((1000 - SEy[xgrid][zgrid]) / 900.0f) * 70.0f;
        SEcolorR[xgrid][zgrid] = 40 + x2;
        SEcolorG[xgrid][zgrid] = 108;
        SEcolorB[xgrid][zgrid] = 15;
        break;
      case T_TYPE_GRASS2:
        z1 = ((2000 - SEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
        SEcolorR[xgrid][zgrid] = 65 - z1;
        SEcolorG[xgrid][zgrid] = 100;
        SEcolorB[xgrid][zgrid] = 15;
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
        x1 = x1 < -77 ? -77 : x1;
        SEcolorR[xgrid][zgrid] = 101 + x1;
        SEcolorG[xgrid][zgrid] = 106 + x1;
        SEcolorB[xgrid][zgrid] = 88 + x1;
        break;
      case T_TYPE_DIRT:
        SEcolorR[xgrid][zgrid] = 111;
        SEcolorG[xgrid][zgrid] = 108;
        SEcolorB[xgrid][zgrid] = 63;
        break;
      case T_TYPE_CRATER:
        SEcolorR[xgrid][zgrid] = 63;
        SEcolorG[xgrid][zgrid] = 66;
        SEcolorB[xgrid][zgrid] = 43;
        break;
      default:
        SEcolorR[xgrid][zgrid] = 93;
        SEcolorG[xgrid][zgrid] = 87;
        SEcolorB[xgrid][zgrid] = 55;
        break;
      }
      switch (temp1.type) {
      case T_TYPE_GRASS1:
        x2 = ((1000 - NEy[xgrid][zgrid]) / 900.0f) * 70.0f;
        NEcolorR[xgrid][zgrid] = 40 + x2;
        NEcolorG[xgrid][zgrid] = 108;
        NEcolorB[xgrid][zgrid] = 15;
        break;
      case T_TYPE_GRASS2:
        z1 = ((2000 - NEy[xgrid][zgrid]) / 1000.0f) * 25.0f;
        NEcolorR[xgrid][zgrid] = 65 - z1;
        NEcolorG[xgrid][zgrid] = 100;
        NEcolorB[xgrid][zgrid] = 15;
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
        x1 = x1 < -77 ? -77 : x1;
        NEcolorR[xgrid][zgrid] = 101 + x1;
        NEcolorG[xgrid][zgrid] = 106 + x1;
        NEcolorB[xgrid][zgrid] = 88 + x1;
        break;
      case T_TYPE_DIRT:
        NEcolorR[xgrid][zgrid] = 103;
        NEcolorG[xgrid][zgrid] = 111;
        NEcolorB[xgrid][zgrid] = 63;
        break;
      case T_TYPE_CRATER:
        NEcolorR[xgrid][zgrid] = 63;
        NEcolorG[xgrid][zgrid] = 66;
        NEcolorB[xgrid][zgrid] = 43;
        break;
      default:
        NEcolorR[xgrid][zgrid] = 93;
        NEcolorG[xgrid][zgrid] = 87;
        NEcolorB[xgrid][zgrid] = 55;
        break;
      }
      //  calcnormal for top segment
      v1[0] = (GLfloat) SEx[xgrid][zgrid];
      v1[1] = (GLfloat) SEy[xgrid][zgrid];
      v1[2] = (GLfloat) SEz[xgrid][zgrid];  //  se vertex
      v2[0] = (GLfloat) NEx[xgrid][zgrid];
      v2[1] = (GLfloat) NEy[xgrid][zgrid];
      v2[2] = (GLfloat) NEz[xgrid][zgrid];  //  ne vertex
      v3[0] = (GLfloat) NWx[xgrid][zgrid];
      v3[1] = (GLfloat) NWy[xgrid][zgrid];
      v3[2] = (GLfloat) NWz[xgrid][zgrid];  //  nw vertex
      temp3f = calcNormal(v1, v2, v3);  //  ccw - SE, NE, NW
      Nnormx[xgrid][zgrid] = temp3f.x;
      Nnormy[xgrid][zgrid] = temp3f.y;
      Nnormz[xgrid][zgrid] = temp3f.z;
      //  calcnormal for bottom segment
      //  se in array already...
      v2[0] = (GLfloat) SWx[xgrid][zgrid];
      v2[1] = (GLfloat) SWy[xgrid][zgrid];
      v2[2] = (GLfloat) SWz[xgrid][zgrid];  //  sw vertex
      //  nw in array already...
      temp3f = calcNormal(v1, v3, v2);  //  ccw - SE, NW, SW  //  swapped order
      Snormx[xgrid][zgrid] = temp3f.x;
      Snormy[xgrid][zgrid] = temp3f.y;
      Snormz[xgrid][zgrid] = temp3f.z;
      //  calcnormal for NW vertex
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
      //  calcnormal for SE vertex
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
      //  calcnormal for SW vertex
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
      //  calcnormal for NE vertex
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
          glColor3ub(SEcolorR[xgrid-1][zgrid], SEcolorG[xgrid-1][zgrid], SEcolorB[xgrid-1][zgrid]);
        glNormal3f(SWnormx[xgrid][zgrid], SWnormy[xgrid][zgrid], SWnormz[xgrid][zgrid]);
        glTexCoord2i((SWx[xgrid][zgrid]), (SWz[xgrid][zgrid]));
        glVertex3i(SWx[xgrid][zgrid], SWy[xgrid][zgrid], SWz[xgrid][zgrid]);
        if (xgrid >= TERRAIN_GRID_SIZE - 1)
          glColor3ub(122, 122, 122);
        else
          glColor3ub(SEcolorR[xgrid][zgrid], SEcolorG[xgrid][zgrid], SEcolorB[xgrid][zgrid]);
        glNormal3f(SEnormx[xgrid][zgrid], SEnormy[xgrid][zgrid], SEnormz[xgrid][zgrid]);
        glTexCoord2i((SEx[xgrid][zgrid]), (SEz[xgrid][zgrid]));
        glVertex3i(SEx[xgrid][zgrid], SEy[xgrid][zgrid], SEz[xgrid][zgrid]);
        if (xgrid <= 0)
          glColor3ub(122, 122, 122);
        else
          glColor3ub(NEcolorR[xgrid-1][zgrid], NEcolorG[xgrid-1][zgrid], NEcolorB[xgrid-1][zgrid]);
        glNormal3f(NWnormx[xgrid][zgrid], NWnormy[xgrid][zgrid], NWnormz[xgrid][zgrid]);
        glTexCoord2i((NWx[xgrid][zgrid]), (NWz[xgrid][zgrid]));
        glVertex3i(NWx[xgrid][zgrid], NWy[xgrid][zgrid], NWz[xgrid][zgrid]);
        if (xgrid >= TERRAIN_GRID_SIZE - 1)
          glColor3ub(122, 122, 122);
        else
          glColor3ub(NEcolorR[xgrid][zgrid], NEcolorG[xgrid][zgrid], NEcolorB[xgrid][zgrid]);
        glNormal3f(NEnormx[xgrid][zgrid], NEnormy[xgrid][zgrid], NEnormz[xgrid][zgrid]);
        glTexCoord2i((NEx[xgrid][zgrid]), (NEz[xgrid][zgrid]));
        glVertex3i(NEx[xgrid][zgrid], NEy[xgrid][zgrid], NEz[xgrid][zgrid]);
        glEnd();
      }
    }
    if (xgrid >= TERRAIN_GRID_SIZE - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}
