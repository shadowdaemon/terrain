#include <GLFW/glfw3.h>
#include "maths.h"


float algorithmicTerrainHeight(float x, float z)
{
  float height, x1, z1, a1, a2, h1, h2, g1, g2;

  while (x > WORLD_SIZE)
    x -= WORLD_SIZE * 2;
  while (x < -WORLD_SIZE)
    x += WORLD_SIZE * 2;
  while (z > WORLD_SIZE)
    z -= WORLD_SIZE * 2;
  while (z < -WORLD_SIZE)
    z += WORLD_SIZE * 2;
  x1 = 0.00011f * sqrt(x*x+z*z);
  z1 = 0.00017f * z;
  a1 = sinf(x1) * 5300;
  a2 = sinf(z1) * 1700;
  h1 = sinf(x * 0.00087f) * 670;
  h2 = sinf(z * 0.00114f) * 630;
  g1 = sinf(x * 0.0051f) * 50;
  g2 = sinf(z * 0.0043f) * 70;
  height = (float) (fabs(a1) - fabs(a2) - fabs(h1) - fabs(h2) - fabs(g1) - fabs(g2));
  x1 *= 2.317f;
  z1 *= 2.127f;
  if (height > TERRAIN_WATER_LEVEL)
    height += (float) ((height - 100.0f) * (height - 140.0f) * 0.0002f * sinf(x1 - z1));
  x1 *= 0.47f;
  z1 *= 1.37f;
  if (height > TERRAIN_WATER_LEVEL)
    height -= (float) ((height + 20.0f) * (height - 50.0f) * 0.0002f) * (1 - sinf(x1 + z1));
  if (height > TERRAIN_WATER_LEVEL)
    height += (450 - height) * 0.5f;
  else
    height += (-700 - height) * 0.02f;
  x1 = 0.000223f * x;
  z1 = 0.000712f * z;
  h1 = (int) (x1*x1+z1*z1) % 20000;
  height += fabs(h1 - 10000) - 9000;

  return height;
}


char calculateTerrainType(float height, float sample)
{
  char type = T_TYPE_DIRT;
  float diff = fabs(height - sample);

  if (height <= TERRAIN_WATER_LEVEL)
    type = T_TYPE_ROCK;
  else if (height < TERRAIN_WATER_LEVEL + 100)
    type = T_TYPE_DIRT;
  else if (height < 500)
    type = T_TYPE_GRASS1;
  else if (diff > TERRAIN_SQUARE_SIZE / 5.0f)
    type = T_TYPE_DIRT;
  else if (height < 1250 || (diff < TERRAIN_SQUARE_SIZE / 9.0f && height < 2500))
    type = T_TYPE_GRASS2;
  else if (height < 2500)
    type = T_TYPE_GRASS3;
  else if (height < 3000)
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
  float h = algorithmicTerrainHeight(x + TERRAIN_SQUARE_SIZE / 3.0f, z + TERRAIN_SQUARE_SIZE / 3.0f);

  temp.height = algorithmicTerrainHeight(x, z);
  temp.type = calculateTerrainType(temp.height, h);

  return temp;
}


void moveTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *swapb, int squaresize)
{
  float rot;
  struct v3f center;

  rot = camerarot.x < 75.0f ? camerarot.x : 75.0f;
  center = degreestovector2d (camerapos, camerarot.y, 0.0f, TERRAIN_CENTRE_DISTANCE * cosf (rot / PIx180));
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
  for (alt = 1; alt < 50; alt++) {
    if (camheight < TERRAIN_SQUARE_SIZE * 15) {
      *squaresize = TERRAIN_SQUARE_SIZE;
      break;
    }
    else if (camheight < alt * TERRAIN_SQUARE_SIZE * 15) {
      *squaresize = (int) (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE * ((alt * 2) - 3) / 2);
      break;
    }
    else
      *squaresize = (int) (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE * ((alt * 2) - 3) / 2);
  }
  if (alt2 != alt) {
    alt2 = alt;
    *swapb = 0;
  }
  // *squaresize = TERRAIN_SQUARE_SIZE;
  x = (int) (sector->x / -(*squaresize));
  z = (int) (sector->y / -(*squaresize));
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(0.01f, 0.01f, 0.01f);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    x3 = fabs(TERRAIN_GRID_SIZE_HALF - xgrid) - 24; x3 = x3 < 0 ? 0 : (x3 + 24) * 5;
    z3 = fabs(TERRAIN_GRID_SIZE_HALF - zgrid) - 24; z3 = z3 < 0 ? 0 : (z3 + 24) * 5;
    xpos = (xgrid - TERRAIN_GRID_SIZE_HALF) * (x3 + *squaresize) + x * *squaresize;
    zpos = (zgrid - TERRAIN_GRID_SIZE_HALF) * (z3 + *squaresize) + z * *squaresize;
    dist = distance2d(camerapos, mv3f(-xpos, 0.0f, -zpos));
    if (xgrid > TERRAIN_GRID_SIZE_HALF + 23) {
      x1 = xpos + x3 - 2759.0f + *squaresize / 2;
      x2 = xpos - x3 - 2759.0f - *squaresize / 2;
    }
    else if (xgrid < TERRAIN_GRID_SIZE_HALF - 23) {
      x1 = xpos + x3 + 2759.0f + *squaresize / 2;
      x2 = xpos - x3 + 2759.0f - *squaresize / 2;
    }
    else {
      x1 = xpos + x3 + *squaresize / 2;
      x2 = xpos - x3 - *squaresize / 2;
    }
    if (zgrid > TERRAIN_GRID_SIZE_HALF + 23) {
      z1 = zpos + z3 - 2759.0f + *squaresize / 2;
      z2 = zpos - z3 - 2759.0f - *squaresize / 2;
    }
    else if (zgrid < TERRAIN_GRID_SIZE_HALF - 23) {
      z1 = zpos + z3 + 2759.0f + *squaresize / 2;
      z2 = zpos - z3 + 2759.0f - *squaresize / 2;
    }
    else {
      z1 = zpos + z3 + *squaresize / 2;
      z2 = zpos - z3 - *squaresize / 2;
    }
    x1 = snap(x1, 32); x2 = snap(x2, 32); z1 = snap(z1, 32); z2 = snap(z2, 32);
    cull = fabs((int) (camerarot.y - vectorstodegree2d(camerapos, mv3f(-xpos, 0, -zpos))));
    while (cull >= 360)
      cull -= 360;
    if (camerarot.x > 27.0f || cull <= 75 || cull >= 285 || dist < *squaresize * 1.5f) {
      NEx[xgrid][zgrid] = x1;
      NEz[xgrid][zgrid] = z2;
      temp1 = readTerrain (NEx[xgrid][zgrid], NEz[xgrid][zgrid]); // color read here
      NEy[xgrid][zgrid] = (int) temp1.height;
      NWx[xgrid][zgrid] = x2;
      NWz[xgrid][zgrid] = z2;
      NWy[xgrid][zgrid] = (int) (readTerrainHeight (NWx[xgrid][zgrid], NWz[xgrid][zgrid]));
      SEx[xgrid][zgrid] = x1;
      SEz[xgrid][zgrid] = z1;
      temp2 = readTerrain (SEx[xgrid][zgrid], SEz[xgrid][zgrid]); // color read here too..
      SEy[xgrid][zgrid] = (int) temp2.height;
      SWx[xgrid][zgrid] = x2;
      SWz[xgrid][zgrid] = z1;
      SWy[xgrid][zgrid] = (int) (readTerrainHeight (SWx[xgrid][zgrid], SWz[xgrid][zgrid]));
      switch (temp2.type) {
      case T_TYPE_GRASS1:
        SEcolorR[xgrid][zgrid] = 30;
        SEcolorG[xgrid][zgrid] = 98;
        SEcolorB[xgrid][zgrid] = 5;
        break;
      case T_TYPE_GRASS2:
        SEcolorR[xgrid][zgrid] = 55;
        SEcolorG[xgrid][zgrid] = 105;
        SEcolorB[xgrid][zgrid] = 5;
        break;
      case T_TYPE_GRASS3:
        SEcolorR[xgrid][zgrid] = 48;
        SEcolorG[xgrid][zgrid] = 90;
        SEcolorB[xgrid][zgrid] = 42;
        break;
      case T_TYPE_ROCK:
        SEcolorR[xgrid][zgrid] = 91;
        SEcolorG[xgrid][zgrid] = 96;
        SEcolorB[xgrid][zgrid] = 78;
        break;
      case T_TYPE_DIRT:
        SEcolorR[xgrid][zgrid] = 101;
        SEcolorG[xgrid][zgrid] = 98;
        SEcolorB[xgrid][zgrid] = 53;
        break;
      case T_TYPE_CRATER:
        SEcolorR[xgrid][zgrid] = 53;
        SEcolorG[xgrid][zgrid] = 56;
        SEcolorB[xgrid][zgrid] = 33;
        break;
      default:
        SEcolorR[xgrid][zgrid] = 83;
        SEcolorG[xgrid][zgrid] = 122;
        SEcolorB[xgrid][zgrid] = 45;
        break;
      }
      switch (temp1.type) {
      case T_TYPE_GRASS1:
        NEcolorR[xgrid][zgrid] = 30;
        NEcolorG[xgrid][zgrid] = 98;
        NEcolorB[xgrid][zgrid] = 5;
        break;
      case T_TYPE_GRASS2:
        NEcolorR[xgrid][zgrid] = 55;
        NEcolorG[xgrid][zgrid] = 105;
        NEcolorB[xgrid][zgrid] = 5;
        break;
      case T_TYPE_GRASS3:
        NEcolorR[xgrid][zgrid] = 48;
        NEcolorG[xgrid][zgrid] = 90;
        NEcolorB[xgrid][zgrid] = 42;
        break;
      case T_TYPE_ROCK:
        NEcolorR[xgrid][zgrid] = 91;
        NEcolorG[xgrid][zgrid] = 96;
        NEcolorB[xgrid][zgrid] = 78;
        break;
      case T_TYPE_DIRT:
        NEcolorR[xgrid][zgrid] = 93;
        NEcolorG[xgrid][zgrid] = 101;
        NEcolorB[xgrid][zgrid] = 53;
        break;
      case T_TYPE_CRATER:
        NEcolorR[xgrid][zgrid] = 53;
        NEcolorG[xgrid][zgrid] = 56;
        NEcolorB[xgrid][zgrid] = 33;
        break;
      default:
        NEcolorR[xgrid][zgrid] = 83;
        NEcolorG[xgrid][zgrid] = 122;
        NEcolorB[xgrid][zgrid] = 45;
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
        //glBegin(GL_LINES);
        glColor3ub(SEcolorR[xgrid-1][zgrid], SEcolorG[xgrid-1][zgrid], SEcolorB[xgrid-1][zgrid]);
        glNormal3f(SWnormx[xgrid][zgrid], SWnormy[xgrid][zgrid], SWnormz[xgrid][zgrid]);
        glTexCoord2i((SWx[xgrid][zgrid]), (SWz[xgrid][zgrid]));
        glVertex3i(SWx[xgrid][zgrid], SWy[xgrid][zgrid], SWz[xgrid][zgrid]);
        glColor3ub(SEcolorR[xgrid][zgrid], SEcolorG[xgrid][zgrid], SEcolorB[xgrid][zgrid]);
        glNormal3f(SEnormx[xgrid][zgrid], SEnormy[xgrid][zgrid], SEnormz[xgrid][zgrid]);
        glTexCoord2i((SEx[xgrid][zgrid]), (SEz[xgrid][zgrid]));
        glVertex3i(SEx[xgrid][zgrid], SEy[xgrid][zgrid], SEz[xgrid][zgrid]);
        glColor3ub(NEcolorR[xgrid-1][zgrid], NEcolorG[xgrid-1][zgrid], NEcolorB[xgrid-1][zgrid]);
        glNormal3f(NWnormx[xgrid][zgrid], NWnormy[xgrid][zgrid], NWnormz[xgrid][zgrid]);
        glTexCoord2i((NWx[xgrid][zgrid]), (NWz[xgrid][zgrid]));
        glVertex3i(NWx[xgrid][zgrid], NWy[xgrid][zgrid], NWz[xgrid][zgrid]);
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
