#include <GL/glfw.h>
#include "maths.h"


float algorithmicTerrainHeight(float x, float z)
{
  float height, x1, z1, x2, z2, a1, a2, h1, h2, g1, g2;

  x1 = 0.00011f * sqrt(x*x+z*z);
  z1 = 0.00017f * z;
  a1 = sinf(x1) * 5300;
  a2 = sinf(z1) * 1700;
  x2 = x * 0.087f;
  z2 = z * 0.114f;
  h1 = sinf(x2 / 100) * 670;
  h2 = sinf(z2 / 100) * 630;
  x *= 0.1f;
  z *= 0.5f;
  g1 = sinf(x / 100) * 250;
  g2 = sinf(z / 100) * 70;
  height = (float) (fabs(a1) - fabs(a2) - fabs(h1) - fabs(h2) - fabs(g1) - fabs(g2));
  x1 *= 2.3f;
  z1 *= 2.1f;
  height += (float) ((height - 100.0f) * (height - 140.0f) * 0.0002f * sinf(x1 - z1));
  x1 *= 0.4f;
  z1 *= 1.3f;
  height -= (float) ((height + 20.0f) * (height - 50.0f) * 0.0002f) * (1 - sinf(x1 + z1));
  height += (450 - height) * 0.5f;
  x1 = 0.00123f * x;
  z1 = 0.00071f * z;
  h1 = (int) (x1*x1+z1*z1) % 20000;
  height += fabs(h1 - 10000) - 6000;
  if (height < TERRAIN_WATER_LEVEL - 100)
    height = TERRAIN_WATER_LEVEL;
  else if (height < TERRAIN_WATER_LEVEL + 30)
    height = TERRAIN_WATER_LEVEL + 30;

  return height;
}


char calculateTerrainType(float height, float sample)
{
  char type = T_TYPE_DIRT;
  float diff = fabs(height - sample);

  if (height <= TERRAIN_WATER_LEVEL)
    type = T_TYPE_WATER;
  else if (height < TERRAIN_WATER_LEVEL + 45)
    type = T_TYPE_WATER_EDGE;
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
  return algorithmicTerrainHeight(x, y);
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
  int x_shift, z_shift, x_grid, z_grid, x2, z2, alt;
  static int alt2 = 0;
  float x, z, xpos = 0.0f, zpos = 0.0f;
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

  moveTerrain(camerapos, camerarot, sector, swapb, *squaresize);
  //selectPosition();

  // scaling terrain
  for (alt = 1; alt < 15; alt++) {
    if (camheight < TERRAIN_SQUARE_SIZE * 25) {
      *squaresize = TERRAIN_SQUARE_SIZE;
      break;
    }
    else if (camheight < alt * TERRAIN_SQUARE_SIZE * 25) {
      *squaresize = (int) (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE * ((alt * 2) - 3) * 0.5f);
      break;
    }
    else {
      *squaresize = (int) (TERRAIN_SQUARE_SIZE + TERRAIN_SQUARE_SIZE
                          * ((alt * 2) - 3) * 0.5f);
    }
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
  for (x_grid = 0, z_grid = 0; x_grid < TERRAIN_GRID_SIZE && z_grid < TERRAIN_GRID_SIZE; x_grid++) {
    x_shift = *squaresize + fabs(TERRAIN_GRID_SIZE_HALF - x_grid) * 8;
    //x_shift = *squaresize;
    z_shift = *squaresize + fabs(TERRAIN_GRID_SIZE_HALF - z_grid) * 8;
    //z_shift = *squaresize;
    xpos = (-TERRAIN_GRID_SIZE_HALF + x_grid) * x_shift + x * *squaresize;
    zpos = (-TERRAIN_GRID_SIZE_HALF + z_grid) * z_shift + z * *squaresize;
    x_shift = *squaresize * 0.5f + fabs(TERRAIN_GRID_SIZE_HALF - x_grid) * 8;
    //x_shift = *squaresize * 0.5f;
    z_shift = *squaresize * 0.5f + fabs(TERRAIN_GRID_SIZE_HALF - z_grid) * 8;
    //z_shift = *squaresize * 0.5f;
    NEx[x_grid][z_grid] = (int) (xpos + x_shift);
    NEz[x_grid][z_grid] = (int) (zpos - z_shift);
    temp1 = readTerrain (NEx[x_grid][z_grid], NEz[x_grid][z_grid]); // color read here
    NEy[x_grid][z_grid] = (int) temp1.height;
    NWx[x_grid][z_grid] = (int) (xpos - x_shift);
    NWz[x_grid][z_grid] = (int) (zpos - z_shift);
    NWy[x_grid][z_grid] = (int) (readTerrainHeight (NWx[x_grid][z_grid], NWz[x_grid][z_grid]));
    SEx[x_grid][z_grid] = (int) (xpos + x_shift);
    SEz[x_grid][z_grid] = (int) (zpos + z_shift);
    temp2 = readTerrain (SEx[x_grid][z_grid], SEz[x_grid][z_grid]); // color read here too..
    SEy[x_grid][z_grid] = (int) temp2.height;
    SWx[x_grid][z_grid] = (int) (xpos - x_shift);
    SWz[x_grid][z_grid] = (int) (zpos + z_shift);
    SWy[x_grid][z_grid] = (int) (readTerrainHeight (SWx[x_grid][z_grid], SWz[x_grid][z_grid]));
    switch (temp2.type) {
    case T_TYPE_WATER:
      SEcolorR[x_grid][z_grid] = 30;
      SEcolorG[x_grid][z_grid] = 97;
      SEcolorB[x_grid][z_grid] = 157;
      break;
    case T_TYPE_WATER_EDGE:
      SEcolorR[x_grid][z_grid] = 50;
      SEcolorG[x_grid][z_grid] = 97;
      SEcolorB[x_grid][z_grid] = 75;
      break;
    case T_TYPE_GRASS1:
      SEcolorR[x_grid][z_grid] = 30;
      SEcolorG[x_grid][z_grid] = 98;
      SEcolorB[x_grid][z_grid] = 5;
      break;
    case T_TYPE_GRASS2:
      SEcolorR[x_grid][z_grid] = 55;
      SEcolorG[x_grid][z_grid] = 105;
      SEcolorB[x_grid][z_grid] = 5;
      break;
    case T_TYPE_GRASS3:
      SEcolorR[x_grid][z_grid] = 48;
      SEcolorG[x_grid][z_grid] = 90;
      SEcolorB[x_grid][z_grid] = 42;
      break;
    case T_TYPE_ROCK:
      SEcolorR[x_grid][z_grid] = 91;
      SEcolorG[x_grid][z_grid] = 96;
      SEcolorB[x_grid][z_grid] = 78;
      break;
    case T_TYPE_DIRT:
      SEcolorR[x_grid][z_grid] = 101;
      SEcolorG[x_grid][z_grid] = 98;
      SEcolorB[x_grid][z_grid] = 53;
      break;
    case T_TYPE_CRATER:
      SEcolorR[x_grid][z_grid] = 53;
      SEcolorG[x_grid][z_grid] = 56;
      SEcolorB[x_grid][z_grid] = 33;
      break;
    default:
      SEcolorR[x_grid][z_grid] = 83;
      SEcolorG[x_grid][z_grid] = 122;
      SEcolorB[x_grid][z_grid] = 45;
      break;
    }
    switch (temp1.type) {
    case T_TYPE_WATER:
      NEcolorR[x_grid][z_grid] = 30;
      NEcolorG[x_grid][z_grid] = 97;
      NEcolorB[x_grid][z_grid] = 157;
      break;
    case T_TYPE_WATER_EDGE:
      NEcolorR[x_grid][z_grid] = 50;
      NEcolorG[x_grid][z_grid] = 97;
      NEcolorB[x_grid][z_grid] = 75;
      break;
    case T_TYPE_GRASS1:
      NEcolorR[x_grid][z_grid] = 30;
      NEcolorG[x_grid][z_grid] = 98;
      NEcolorB[x_grid][z_grid] = 5;
      break;
    case T_TYPE_GRASS2:
      NEcolorR[x_grid][z_grid] = 55;
      NEcolorG[x_grid][z_grid] = 105;
      NEcolorB[x_grid][z_grid] = 5;
      break;
    case T_TYPE_GRASS3:
      NEcolorR[x_grid][z_grid] = 48;
      NEcolorG[x_grid][z_grid] = 90;
      NEcolorB[x_grid][z_grid] = 42;
      break;
    case T_TYPE_ROCK:
      NEcolorR[x_grid][z_grid] = 91;
      NEcolorG[x_grid][z_grid] = 96;
      NEcolorB[x_grid][z_grid] = 78;
      break;
    case T_TYPE_DIRT:
      NEcolorR[x_grid][z_grid] = 93;
      NEcolorG[x_grid][z_grid] = 101;
      NEcolorB[x_grid][z_grid] = 53;
      break;
    case T_TYPE_CRATER:
      NEcolorR[x_grid][z_grid] = 53;
      NEcolorG[x_grid][z_grid] = 56;
      NEcolorB[x_grid][z_grid] = 33;
      break;
    default:
      NEcolorR[x_grid][z_grid] = 83;
      NEcolorG[x_grid][z_grid] = 122;
      NEcolorB[x_grid][z_grid] = 45;
      break;
    }
    //  calcnormal for top segment
    v1[0] = (GLfloat) SEx[x_grid][z_grid];
    v1[1] = (GLfloat) SEy[x_grid][z_grid];
    v1[2] = (GLfloat) SEz[x_grid][z_grid];  //  se vertex
    v2[0] = (GLfloat) NEx[x_grid][z_grid];
    v2[1] = (GLfloat) NEy[x_grid][z_grid];
    v2[2] = (GLfloat) NEz[x_grid][z_grid];  //  ne vertex
    v3[0] = (GLfloat) NWx[x_grid][z_grid];
    v3[1] = (GLfloat) NWy[x_grid][z_grid];
    v3[2] = (GLfloat) NWz[x_grid][z_grid];  //  nw vertex
    temp3f = calcNormal(v1, v2, v3);  //  ccw - SE, NE, NW
    Nnormx[x_grid][z_grid] = temp3f.x;
    Nnormy[x_grid][z_grid] = temp3f.y;
    Nnormz[x_grid][z_grid] = temp3f.z;
    //  calcnormal for bottom segment
    //  se in array already...
    v2[0] = (GLfloat) SWx[x_grid][z_grid];
    v2[1] = (GLfloat) SWy[x_grid][z_grid];
    v2[2] = (GLfloat) SWz[x_grid][z_grid];  //  sw vertex
    //  nw in array already...
    temp3f = calcNormal(v1, v3, v2);  //  ccw - SE, NW, SW  //  swapped order
    Snormx[x_grid][z_grid] = temp3f.x;
    Snormy[x_grid][z_grid] = temp3f.y;
    Snormz[x_grid][z_grid] = temp3f.z;
    //  calcnormal for NW vertex
    x2 = (x_grid - 1);
    z2 = (z_grid - 1);
    NWnormx[x_grid][z_grid] = (Nnormx[x_grid][z_grid] + Snormx[x_grid][z2]
                               + Nnormx[x2][z2] + Snormx[x2][z2]
                               + Nnormx[x2][z_grid] + Snormx[x_grid][z_grid]) / 6;
    NWnormy[x_grid][z_grid] = (Nnormy[x_grid][z_grid] + Snormy[x_grid][z2]
                               + Nnormy[x2][z2] + Snormy[x2][z2]
                               + Nnormy[x2][z_grid] + Snormy[x_grid][z_grid]) / 6;
    NWnormz[x_grid][z_grid] = (Nnormz[x_grid][z_grid] + Snormz[x_grid][z2]
                               + Nnormz[x2][z2] + Snormz[x2][z2]
                               + Nnormz[x2][z_grid] + Snormz[x_grid][z_grid]) / 6;
    //  calcnormal for SE vertex
    x2 = (x_grid + 1);
    z2 = (z_grid + 1);
    SEnormx[x_grid][z_grid] = (Nnormx[x_grid][z_grid] + Snormx[x_grid][z_grid]
                               + Nnormx[x_grid][z2] + Snormx[x2][z2]
                               + Nnormx[x2][z2] + Snormx[x2][z_grid]) / 6;
    SEnormy[x_grid][z_grid] = (Nnormy[x_grid][z_grid] + Snormy[x_grid][z_grid]
                               + Nnormy[x_grid][z2] + Snormy[x2][z2]
                               + Nnormy[x2][z2] + Snormy[x2][z_grid]) / 6;
    SEnormz[x_grid][z_grid] = (Nnormz[x_grid][z_grid] + Snormz[x_grid][z_grid]
                               + Nnormz[x_grid][z2] + Snormz[x2][z2]
                               + Nnormz[x2][z2] + Snormz[x2][z_grid]) / 6;
    //  calcnormal for SW vertex
    x2 = (x_grid - 1);
    z2 = (z_grid + 1);
    SWnormx[x_grid][z_grid] = (Snormx[x_grid][z_grid] + Nnormx[x2][z_grid]
                               + Snormx[x2][z_grid] + Nnormx[x2][z2]
                               + Snormx[x_grid][z2] + Nnormx[x_grid][z2]) / 6;
    SWnormy[x_grid][z_grid] = (Snormy[x_grid][z_grid] + Nnormy[x2][z_grid]
                               + Snormy[x2][z_grid] + Nnormy[x2][z2]
                               + Snormy[x_grid][z2] + Nnormy[x_grid][z2]) / 6;
    SWnormz[x_grid][z_grid] = (Snormz[x_grid][z_grid] + Nnormz[x2][z_grid]
                               + Snormz[x2][z_grid] + Nnormz[x2][z2]
                               + Snormz[x_grid][z2] + Nnormz[x_grid][z2]) / 6;
    //  calcnormal for NE vertex
    x2 = (x_grid + 1);
    z2 = (z_grid - 1);
    NEnormx[x_grid][z_grid] = (Nnormx[x_grid][z_grid] + Snormx[x2][z_grid]
                               + Nnormx[x2][z_grid] + Snormx[x2][z2]
                               + Nnormx[x_grid][z2] + Snormx[x_grid][z2]) / 6;
    NEnormy[x_grid][z_grid] = (Nnormy[x_grid][z_grid] + Snormy[x2][z_grid]
                               + Nnormy[x2][z_grid] + Snormy[x2][z2]
                               + Nnormy[x_grid][z2] + Snormy[x_grid][z2]) / 6;
    NEnormz[x_grid][z_grid] = (Nnormz[x_grid][z_grid] + Snormz[x2][z_grid]
                               + Nnormz[x2][z_grid] + Snormz[x2][z2]
                               + Nnormz[x_grid][z2] + Snormz[x_grid][z2]) / 6;
    if (*swapb) {
      if (readTerrainHeight(xpos, zpos) > TERRAIN_WATER_LEVEL) {
        if (glfwGetKey(GLFW_KEY_F10)) {
          glLineWidth(1.0f);
          glBegin(GL_LINE_STRIP); //test
        }
        else
          glBegin(GL_TRIANGLE_STRIP);
        glColor3ub(SEcolorR[x_grid-1][z_grid], SEcolorG[x_grid-1][z_grid], SEcolorB[x_grid-1][z_grid]);
        glNormal3f(SWnormx[x_grid][z_grid], SWnormy[x_grid][z_grid], SWnormz[x_grid][z_grid]);
        glTexCoord2i((SWx[x_grid][z_grid]), (SWz[x_grid][z_grid]));
        glVertex3i(SWx[x_grid][z_grid], SWy[x_grid][z_grid], SWz[x_grid][z_grid]);
        glColor3ub(SEcolorR[x_grid][z_grid], SEcolorG[x_grid][z_grid], SEcolorB[x_grid][z_grid]);
        glNormal3f(SEnormx[x_grid][z_grid], SEnormy[x_grid][z_grid], SEnormz[x_grid][z_grid]);
        glTexCoord2i((SEx[x_grid][z_grid]), (SEz[x_grid][z_grid]));
        glVertex3i(SEx[x_grid][z_grid], SEy[x_grid][z_grid], SEz[x_grid][z_grid]);
        glColor3ub(NEcolorR[x_grid-1][z_grid], NEcolorG[x_grid-1][z_grid], NEcolorB[x_grid-1][z_grid]);
        glNormal3f(NWnormx[x_grid][z_grid], NWnormy[x_grid][z_grid], NWnormz[x_grid][z_grid]);
        glTexCoord2i((NWx[x_grid][z_grid]), (NWz[x_grid][z_grid]));
        glVertex3i(NWx[x_grid][z_grid], NWy[x_grid][z_grid], NWz[x_grid][z_grid]);
        glColor3ub(NEcolorR[x_grid][z_grid], NEcolorG[x_grid][z_grid], NEcolorB[x_grid][z_grid]);
        glNormal3f(NEnormx[x_grid][z_grid], NEnormy[x_grid][z_grid], NEnormz[x_grid][z_grid]);
        glTexCoord2i((NEx[x_grid][z_grid]), (NEz[x_grid][z_grid]));
        glVertex3i(NEx[x_grid][z_grid], NEy[x_grid][z_grid], NEz[x_grid][z_grid]);
        glEnd();
      }
      else {
        if (glfwGetKey(GLFW_KEY_F10)) {
          glLineWidth(1.0f);
          glBegin(GL_LINE_STRIP); //test
        }
        else
          glBegin(GL_QUADS);
        glColor3ub(NEcolorR[x_grid-1][z_grid], NEcolorG[x_grid-1][z_grid], NEcolorB[x_grid-1][z_grid]);
        glNormal3f(NWnormx[x_grid][z_grid], NWnormy[x_grid][z_grid], NWnormz[x_grid][z_grid]);
        glTexCoord2i((NWx[x_grid][z_grid]), (NWz[x_grid][z_grid]));
        glVertex3i(NWx[x_grid][z_grid], NWy[x_grid][z_grid], NWz[x_grid][z_grid]);
        glColor3ub(SEcolorR[x_grid-1][z_grid], SEcolorG[x_grid-1][z_grid], SEcolorB[x_grid-1][z_grid]);
        glNormal3f(SWnormx[x_grid][z_grid], SWnormy[x_grid][z_grid], SWnormz[x_grid][z_grid]);
        glTexCoord2i((SWx[x_grid][z_grid]), (SWz[x_grid][z_grid]));
        glVertex3i(SWx[x_grid][z_grid], SWy[x_grid][z_grid], SWz[x_grid][z_grid]);
        glColor3ub(SEcolorR[x_grid][z_grid], SEcolorG[x_grid][z_grid], SEcolorB[x_grid][z_grid]);
        glNormal3f(SEnormx[x_grid][z_grid], SEnormy[x_grid][z_grid], SEnormz[x_grid][z_grid]);
        glTexCoord2i((SEx[x_grid][z_grid]), (SEz[x_grid][z_grid]));
        glVertex3i(SEx[x_grid][z_grid], SEy[x_grid][z_grid], SEz[x_grid][z_grid]);
        glColor3ub(NEcolorR[x_grid][z_grid], NEcolorG[x_grid][z_grid], NEcolorB[x_grid][z_grid]);
        glNormal3f(NEnormx[x_grid][z_grid], NEnormy[x_grid][z_grid], NEnormz[x_grid][z_grid]);
        glTexCoord2i((NEx[x_grid][z_grid]), (NEz[x_grid][z_grid]));
        glVertex3i(NEx[x_grid][z_grid], NEy[x_grid][z_grid], NEz[x_grid][z_grid]);
        glEnd();
      }
    }
    if (x_grid >= TERRAIN_GRID_SIZE - 1) {
      z_grid++;
      x_grid = -1;
    }
  }
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}
