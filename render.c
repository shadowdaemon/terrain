#include <assimp/scene.h>
#include "maths.h"


void updateFog(GLfloat *clear, int squaresize, float *fogend, float height)
{
  static float fogstart = 10.0f;
  float fstart = 0.08f;
  float temp = 0.0f;

  temp = (250 - fabs(CLOUD_HEIGHT - height)) * (height > CLOUD_HEIGHT ? -250.0f : -50.0f);
  temp = temp > 0 ? 0 : temp;
  temp += squaresize * TERRAIN_GRID_SIZE * 0.9f;
  if (*fogend > temp)
    *fogend -= (*fogend - temp) * 0.31f;
  else
    *fogend -= (*fogend - temp) * 0.04f;
  temp = *fogend * 0.75f;
  if (fogstart > temp)
    fogstart -= (fogstart - temp) * 0.17f;
  else
    fogstart -= (fogstart - temp) * 0.02f;
  *fogend = *fogend < squaresize * 5 ? squaresize * 5 : *fogend;
  fogstart = fogstart > 3000 ? 3000 : fogstart;
  fstart = *fogend * 0.00002f > 4.0f ? 4.0f : *fogend * 0.00002f;
  glFogfv(GL_FOG_COLOR, clear);
  glFogf(GL_FOG_START, fogstart);
  glFogf(GL_FOG_END, *fogend);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.4f * fstart, 5.4f * fstart, -4.0f * fstart, 4.0f * fstart, 8.0f * fstart, *fogend * 1.1f);
  glMatrixMode(GL_MODELVIEW);
}


void renderFoliage(struct aiScene *scene, struct v3f camerapos, struct v3f camerarot, struct v2f sector, int squaresize)
{
  struct v3f normal;
  int xgrid, zgrid, x1, z1, cull, density;
  const int size = TERRAIN_SQUARE_SIZE * 0.4f;
  float x, z, xpos = 0.0f, zpos = 0.0f, height, dist;
  unsigned char type;
  GLubyte alpha;

  glMateriali(GL_FRONT, GL_SHININESS, 92);
  x = (int) (sector.x / size);
  z = (int) (sector.y / size);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    xpos = (xgrid - TERRAIN_GRID_SIZE_HALF + x) * size;
    zpos = (zgrid - TERRAIN_GRID_SIZE_HALF + z) * size;
    x1 = sqrt(fabs(xpos * zpos + 107)) * 123;
    x1 = (int) (x1 + xpos) % 88;
    z1 = (int) (x1 + zpos) % 85;
    xpos += z1;
    zpos += x1;
    cull = fabs((int) (camerarot.y - 180 - vectorstodegree2d(camerapos, mv3f(xpos, 0, zpos))));
    while (cull >= 360)
      cull -= 360;
    if (cull <= 85 || cull >= 275 || camerarot.x > 27.0f) {
      height = readTerrainHeightPlane(xpos, zpos, squaresize, &normal);
      type = readTerrainType(xpos, zpos);
      dist = distance3d(camerapos, mv3f(xpos, height, zpos));
      x1 = x1 * x1 + z1 * z1;
      x1 = x1 % 1076;
      switch (type) {
      case T_TYPE_GRASS1:
        density = 110;
        break;
      case T_TYPE_GRASS2:
        density = 127;
        break;
      case T_TYPE_GRASS3:
        density = 101;
        break;
      case T_TYPE_VILLAGE:
        density = 103;
        break;
      case T_TYPE_FOREST1:
        density = 470;
        break;
      default:
        density = 120;
      }
      if ((dist < VIEW_DISTANCE || dist < TERRAIN_SQUARE_SIZE * 10) && x1 < density) {
        if (height > TERRAIN_WATER_LEVEL + 50 && height < 3750 && type != T_TYPE_DIRT) {
          if (dist < VIEW_DISTANCE_HALF)
            alpha = 255;
          else if (dist < VIEW_DISTANCE)
            alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
          else
            alpha = 0;
          drawModel((const struct aiScene *) &scene[x1 % 6], mv3f(xpos, height, zpos), mv3f(0, x1, 0), 0.333f, alpha);
          if (type == T_TYPE_FOREST1)
            drawModel((const struct aiScene *) &scene[(x1 + 2) % 6], mv3f(xpos - 20, readTerrainHeightPlane(xpos - 20, zpos - 23, squaresize, &normal), zpos - 23), mv3f(0, x1, 0), 0.333f, alpha);
        }
      }
    }
    if (xgrid >= TERRAIN_GRID_SIZE - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void renderBuildings(struct aiScene *scene, struct v3f camerapos, struct v3f camerarot, struct v2f sector, int squaresize)
{
  struct v3f normal;
  int xgrid, zgrid, x1, z1, cull;
  const int size = TERRAIN_SQUARE_SIZE * 0.4f;
  float x, z, xpos = 0.0f, zpos = 0.0f, height, dist;
  unsigned char type;
  GLubyte alpha;

  glMateriali(GL_FRONT, GL_SHININESS, 67);
  x = (int) (sector.x / size);
  z = (int) (sector.y / size);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    xpos = (xgrid - TERRAIN_GRID_SIZE_HALF + x) * size;
    zpos = (zgrid - TERRAIN_GRID_SIZE_HALF + z) * size;
    x1 = (int) xpos % 71;
    z1 = (int) zpos % 63;
    xpos += z1;
    zpos += x1;
    cull = fabs((int) (camerarot.y - 180 - vectorstodegree2d(camerapos, mv3f(xpos, 0, zpos))));
    while (cull >= 360)
      cull -= 360;
    if (cull <= 85 || cull >= 275 || camerarot.x > 27.0f) {
      height = readTerrainHeightPlane(xpos, zpos, squaresize, &normal);
      type = readTerrainType(xpos, zpos);
      dist = distance3d(camerapos, mv3f(xpos, height, zpos));
      x1 = x1 * x1 + z1 * z1;
      x1 = x1 % 3176;
      if ((dist < VIEW_DISTANCE || dist < TERRAIN_SQUARE_SIZE * 10) && x1 < 77/* && distance3d(mv3f(normal.x, fabs(normal.y), normal.z), mv3f(0, 1, 0)) < 1.0f*/) {
        if (type == T_TYPE_VILLAGE) {
          if (dist < VIEW_DISTANCE_HALF)
            alpha = 255;
          else if (dist < VIEW_DISTANCE)
            alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
          else
            alpha = 0;
          drawModel((const struct aiScene *) &scene[7], mv3f(xpos, height, zpos), mv3f(0, x1 % 90, 0), 0.35f, alpha);
        }
      }
    }
    if (xgrid >= TERRAIN_GRID_SIZE - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void renderSky(struct v3f camerapos, struct v3f camerarot, GLfloat *clear, float fogend)
{
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glTranslatef(camerapos.x, camerapos.y, camerapos.z);
  glRotatef(-camerarot.y, 0.0f, 1.0f, 0.0f);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  glColor3fv(clear);
  glVertex3f(7000.0f, 3000.0f, 50000.0f);
  glVertex3f(-7000.0f, 3000.0f, 50000.0f);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glVertex3f(-7000.0f, -5000.0f, -fogend);
  glVertex3f(7000.0f, -5000.0f, -fogend);
  glEnd();
  glBegin(GL_TRIANGLES);
  glVertex3f(7000.0f, 3000.0f, 50000.0f);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glVertex3f(7000.0f, -5000.0f, -fogend);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glVertex3f(-7000.0f, 3000.0f, 50000.0f);
  glVertex3f(-7000.0f, -5000.0f, -fogend);
  glEnd();
  glPopMatrix();
}


void renderWater(struct v3f camerapos, struct v3f camerarot, int *squaresize, GLfloat color[4])
{
  int xshift, zshift, xgrid, zgrid, size = *squaresize * 8;
  float xpos, zpos;
  const float scale = 0.0001f;

  glMateriali(GL_FRONT, GL_SHININESS, 97);
  glDisable(GL_CULL_FACE);
  glPushMatrix();
  glTranslatef(camerapos.x, 0.0f, camerapos.z);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(scale, scale, scale);
  glTranslatef(camerapos.x, camerapos.z, 0.0f);
  glBegin(GL_QUADS);
  color[3] = 0.7f;
  glColor4fv(color);
  glNormal3i(0, -1, 0);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE_HALF && zgrid < TERRAIN_GRID_SIZE_HALF; xgrid++) {
    xshift = zshift = size;
    xpos = (-TERRAIN_GRID_SIZE_QUARTER + xgrid) * xshift;
    zpos = (-TERRAIN_GRID_SIZE_QUARTER + zgrid) * zshift;
    xshift = zshift = size / 2;
    glTexCoord2f(xpos + xshift, zpos + zshift);
    glVertex3f(xpos + xshift, TERRAIN_WATER_LEVEL, zpos + zshift);
    glTexCoord2f(xpos - xshift, zpos + zshift);
    glVertex3f(xpos - xshift, TERRAIN_WATER_LEVEL, zpos + zshift);
    glTexCoord2f(xpos - xshift, zpos - zshift);
    glVertex3f(xpos - xshift, TERRAIN_WATER_LEVEL, zpos - zshift);
    glTexCoord2f(xpos + xshift, zpos - zshift);
    glVertex3f(xpos + xshift, TERRAIN_WATER_LEVEL, zpos - zshift);
    if (xgrid >= TERRAIN_GRID_SIZE_HALF - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
  glEnd();
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_CULL_FACE);
}


void renderCloud(struct v3f camerapos, struct v3f camerarot, int *squaresize)
{
  int i, size;
  float rot, x, z;
  int alpha;
  const float scale = 0.00005f;

  size = TERRAIN_GRID_SIZE * *squaresize;
  alpha = fabs(CLOUD_HEIGHT - camerapos.y) * 0.5f;
  alpha = alpha > 180 ? 180 : alpha;
  glMateriali(GL_FRONT, GL_SHININESS, 31);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(camerapos.x, 0.0f, camerapos.z);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(scale, scale, scale);
  glTranslatef(camerapos.x, camerapos.z, 0.0f);
  glColor4ub(128, 128, 128, (GLubyte) alpha);
  glNormal3i(0, -1, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0f, CLOUD_HEIGHT, 0.0f);
  glColor4ub(128, 128, 128, 0);
  for (i = 0; i <= 360; i += 30) {
    rot = i / PIx180;
    x = -size * sinf(rot);
    z = size * cosf(rot);
    glTexCoord2f(x, z);
    glVertex3f(x, CLOUD_HEIGHT, z);
  }
  glEnd();
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_CULL_FACE);
}


void renderSun(struct v3f camerapos, GLint sunpos[4], float size)
{
  int i;
  float r, x, z;

  glPushMatrix();
  glTranslatef(camerapos.x + sunpos[0], camerapos.y + sunpos[1], camerapos.z + sunpos[2]);
  r = atan2(sunpos[0], sunpos[1]) * -180 / PI;
  glRotatef(r, 0.0f, 0.0f, 1.0f);
  glColor4ub(235, 215, 30, 10 + (GLubyte) fabs(sunpos[1] * 0.244f));
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glColor4ub(235, 200, 60, 3);
  for (i = 0; i <= 360; i += 15) {
    r = i / PIx180;
    x = -size * sinf(r);
    z = size * cosf(r);
    glVertex3f(x, 0.0f, z);
  }
  glEnd();
  glPopMatrix();
}


void renderMoon(struct v3f camerapos, GLint pos[4], float size)
{
  int i;
  float r, x, z;

  glPushMatrix();
  glTranslatef(camerapos.x + pos[0], camerapos.y + pos[1], camerapos.z + pos[2]);
  r = atan2(pos[0], pos[1]) * -180 / PI;
  glRotatef(r, 0.0f, 0.0f, 1.0f);
  glColor3ub(210, 210, 212);
  glBegin(GL_TRIANGLE_FAN);
  //glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(size*0.5f, 0.0f, size*0.4f);
  glColor4ub(203, 203, 206, 200);
  for (i = 0; i <= 360; i += 15) {
    r = i / PIx180;
    x = -size * sinf(r);
    z = size * cosf(r);
    glVertex3f(x, 0.0f, z);
  }
  glEnd();
  glPopMatrix();
}


void renderExhaust(struct v3f pos, struct v3f rot, float size)
{
  int i;
  float r, x, y;

  glMateriali(GL_FRONT, GL_SHININESS, 110);
  //glMateriali(GL_FRONT, GL_EMISSION, 78);
  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
  glBegin(GL_TRIANGLE_FAN);
  glColor4ub(195, 110, 30, 185);
  glVertex3f(0.5f, 0.5f, -size * 10.0f - 5.2f);
  glColor4ub(255, 225, 90, 30);
  for (i = 0; i <= 360; i += 120) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x + 0.5f, y + 0.5f, -1.7f);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glColor4ub(195, 110, 30, 185);
  glVertex3f(-0.5f, 0.5f, -size * 10.0f - 5.2f);
  glColor4ub(255, 225, 90, 30);
  for (i = 0; i <= 360; i += 120) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x - 0.5f, y + 0.5f, -1.7f);
  }
  glEnd();
  glPopMatrix();
  //glMateriali(GL_FRONT, GL_EMISSION, 0);
}


void renderNumber(int num, struct aiScene *textquads, struct v2f pos)
{
  int i = 0, j = num;

  num = fabs(num);
  drawModel((const struct aiScene *) &textquads[num % 10], mv3f(pos.x, pos.y, 0), mv3f(0, 0, 0), 250, 255);
  while (num > 9) {
    num -= num % 10;
    num /= 10;
    drawModel((const struct aiScene *) &textquads[num % 10], mv3f(pos.x - 50 * ++i, pos.y, 0), mv3f(0, 0, 0), 250, 255);
  }
  if (j < 0)
    drawModel((const struct aiScene *) &textquads[10], mv3f(pos.x - 50 * ++i, pos.y, 0), mv3f(0, 0, 0), 250, 255);
}


void renderFX(void)
{
  int xgrid, zgrid;

  glColor3ub(255, 255, 255);
  for (xgrid = 0, zgrid = 0; xgrid < 10 && zgrid < 10; xgrid++) {
    glBegin(GL_POINTS);
    glVertex3f(xgrid * 50, 3500, zgrid * 50);
    glEnd();
    if (xgrid >= 9) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void sceneQuad(void)
{
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glColor3ub(255, 255, 255);
  glBegin(GL_QUADS);
  glMultiTexCoord2i(4, 0, 0);
  glVertex3i(0, 0, 0);
  glMultiTexCoord2i(4, 1, 0);
  glVertex3i(RESX, 0, 0);
  glMultiTexCoord2i(4, 1, 1);
  glVertex3i(RESX, RESY, 0);
  glMultiTexCoord2i(4, 0, 1);
  glVertex3i(0, RESY, 0);
  glEnd();
}


void render(GLFWwindow *window, struct aiScene *scene, struct aiScene *textquads, GLuint *textures,
            GLuint *shaders, struct v3f camerapos, struct v3f camerarot, struct v2f *sector,
            int *squaresize, float *fogend, float *fps, struct airunit *airunits)
{
  GLfloat color[4], temp;
  GLint lpos[4], mpos[4];
  static double time = 0;
  static float fps2 = 0;
  static char swapb = 1;

  *fps = 1 / (glfwGetTime() - time);
  fps2 += (*fps - fps2) * 0.05f;
  time = glfwGetTime();
  glEnable(GL_LIGHT0); // sun
  glEnable(GL_LIGHT1); // moon
  temp = time * 0.003f;
  lpos[0] = -1000 * sinf(temp);
  lpos[1] = 1000 * cosf(temp);
  lpos[2] = 0;
  lpos[3] = 0;
  glLightiv(GL_LIGHT0, GL_POSITION, lpos);
  mpos[0] = 400;
  mpos[1] = 800;
  mpos[2] = 300;
  mpos[3] = 0;
  glLightiv(GL_LIGHT1, GL_POSITION, mpos);
  if (lpos[1] > 300)
    temp = 0.3f * (lpos[1] - 300) / 700.0f;
  else
    temp = 0.0f;
  color[0] = temp;
  color[1] = temp;
  color[2] = temp;
  color[3] = 1.0f;
  glLightfv(GL_LIGHT0, GL_SPECULAR, color);
  color[0] = 0.1f;
  color[1] = 0.1f;
  color[2] = 0.12f;
  glLightfv(GL_LIGHT1, GL_SPECULAR, color);
  if (lpos[1] < -300)
    temp = 0.35f * (1 - (-300 - lpos[1]) / 700.0f);
  else
    temp = 0.35f;
  color[0] = temp;
  color[1] = temp;
  color[2] = temp;
  glLightfv(GL_LIGHT0, GL_AMBIENT, color);
  color[0] = 0.03f;
  color[1] = 0.03f;
  color[2] = 0.04f;
  glLightfv(GL_LIGHT1, GL_AMBIENT, color);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
  if (lpos[1] > -600)
    temp = (lpos[1] + 600) / 1600.0f;
  else
    temp = 0.0f;
  color[0] = temp;
  color[1] = lpos[1] > -400 ? (lpos[1] + 400) / 1400.0f : 0;
  color[2] = lpos[1] > -200 ? (lpos[1] + 200) / 1200.0f : 0;
  glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
  color[0] = 0.2f;
  color[1] = 0.2f;
  color[2] = 0.22f;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, color);
  color[0] = 0.8f;
  color[1] = 0.8f;
  color[2] = 0.8f;
  glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
  glMaterialfv(GL_FRONT, GL_AMBIENT, color);
  glMaterialfv(GL_FRONT, GL_SPECULAR, color);
  //glMaterialfv(GL_FRONT, GL_EMISSION, color);
  color[0] = 0.4588235294117647f * temp;
  color[0] += lpos[0] > 800 ? (lpos[0] - 800) * 0.005f * 0.1f : 0;
  color[1] = 0.5176470588235295f * (lpos[1] > -400 ? (lpos[1] + 400) / 1400.0f : 0);
  color[2] = 0.8431372549019608f * (lpos[1] > -200 ? 0.05f + (lpos[1] + 200) / 1200.0f : 0.05f);
  updateFog(color, *squaresize, fogend, camerapos.y);
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  renderSky(camerapos, camerarot, color, *fogend);
  renderSun(camerapos, lpos, 160);
  renderMoon(camerapos, mpos, 60);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  drawTerrain(camerapos, camerarot, sector, &swapb, squaresize);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  renderWater(camerapos, camerarot, squaresize, color);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  renderFoliage(scene, camerapos, camerarot, *sector, *squaresize);
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  renderBuildings(scene, camerapos, camerarot, *sector, *squaresize);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  drawModel((const struct aiScene *) &scene[6], airunits[0].pos, mv3f(airunits[0].rot.x, -airunits[0].rot.y, airunits[0].rot.z), 0.35f, 255);
  //for (i = 1; i < 15; i++)
    //drawModel((const struct aiScene *) &scene[6], airunits[i].pos, mv3f(airunits[i].rot.x, -airunits[i].rot.y, airunits[i].rot.z), 1, 255);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  renderExhaust(airunits[0].pos, mv3f(airunits[0].rot.x, -airunits[0].rot.y, airunits[0].rot.z), airunits[0].thrust * 0.35f);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_PROGRAM_POINT_SIZE);
  glUseProgramARB(shaders[2]);
  glUniform1iARB(glGetUniformLocationARB(shaders[2], "texture"), 1);
  glUniform1fARB(glGetUniformLocationARB(shaders[2], "size"), 10.0f);
  glUniform2fARB(glGetUniformLocationARB(shaders[2], "screensize"), 1366, 768);
  renderFX();
  glUseProgramARB(0);
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  renderCloud(camerapos, camerarot, squaresize);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, RESX, 0, RESY, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glBindTexture(GL_TEXTURE_2D, textures[6]);
  //renderNumber(camerapos->x, textquads, mv2f(RESX - 100, 120));
  //renderNumber(camerapos->z, textquads, mv2f(RESX - 100, 70));
  //renderNumber(fps2, textquads, mv2f(RESX - 100, 20));
  renderNumber(airunits[0].speed, textquads, mv2f(RESX - 100, 70));
  renderNumber(airunits[0].thrust * 100, textquads, mv2f(RESX - 100, 20));
  glReadBuffer(GL_BACK);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, RESX, RESY, 0);
  glUseProgramARB(shaders[0]);
  glUniform1iARB(glGetUniformLocationARB(shaders[0], "scene"), 4);
  glUniform1fARB(glGetUniformLocationARB(shaders[0], "gamma"), 0.6f);
  glUniform1fARB(glGetUniformLocationARB(shaders[0], "numcolors"), 64.0f);
  glUniform4fvARB(glGetUniformLocationARB(shaders[0], "clear"), 0, color);
  sceneQuad();
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, RESX, RESY, 0);
  glUseProgramARB(shaders[1]);
  glUniform1iARB(glGetUniformLocationARB(shaders[1], "scene"), 4);
  glUniform2fARB(glGetUniformLocationARB(shaders[1], "steps"), 2000.0f, 2000.0f);
  glUniform4fvARB(glGetUniformLocationARB(shaders[1], "clear"), 0, color);
  sceneQuad();
  glUseProgramARB(0);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  if (swapb)
    glfwSwapBuffers(window);
  swapb = 1;
  glfwPollEvents();
}
