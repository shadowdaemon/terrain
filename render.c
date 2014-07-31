#include <assimp/scene.h>
#include "maths.h"


void updateFogLights(GLfloat *clear, GLfloat *ambient, float camheight, int squaresize, float *fogend)
{
  static float fogstart = 10.0f;
  float fstart = 0.5;
  float temp = 0.0f;
  float lightspec[4];
  float lightamb[4];
  float lightdiff[4];

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  glClearColor(clear[0], clear[1], clear[2], clear[3]);
  temp = squaresize * TERRAIN_GRID_SIZE * 0.9f;
  if (*fogend > temp)
    *fogend -= (*fogend - temp) * 0.1f;
  else
    *fogend -= (*fogend - temp) * 0.04f;
  temp = *fogend * 0.9f;
  if (fogstart > temp)
    fogstart -= (fogstart - temp) * 0.1f;
  else
    fogstart -= (fogstart - temp) * 0.02f;
  //fogstart = fogstart > 37000 ? 37000 : fogstart;
  glFogfv(GL_FOG_COLOR, clear);
  glFogf(GL_FOG_START, fogstart);
  glFogf(GL_FOG_END, *fogend);
  lightspec[0] = 0.12f;
  lightspec[1] = 0.1f;
  lightspec[2] = 0.11f;
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightspec);
  lightamb[0] = 0.0f;
  lightamb[1] = 0.0f;
  lightamb[2] = 0.0f;
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightamb);
  lightdiff[0] = 0.25f + sinf(camheight*0.00017f) * 0.15f;
  lightdiff[1] = 0.17f;
  lightdiff[2] = 0.19f;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightdiff);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.4f * fstart, 5.4f * fstart, -4.0f * fstart, 4.0f * fstart, 8.0f * fstart, *fogend * 1.1f);
  glMatrixMode(GL_MODELVIEW);
}


void renderFoliage(struct aiScene *scene, struct v3f camerapos, struct v3f camerarot, struct v2f sector, float camheight)
{
  int xgrid, zgrid, x1, z1, cull;
  int squaresize = TERRAIN_SQUARE_SIZE * 0.4f;
  float x, z, xpos = 0.0f, zpos = 0.0f, dist;
  struct terrain temp;
  GLubyte alpha;

  glMateriali(GL_FRONT, GL_SHININESS, 37);
  x = (int) (sector.x / squaresize);
  z = (int) (sector.y / squaresize);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    xpos = (xgrid - TERRAIN_GRID_SIZE_HALF + x) * squaresize;
    zpos = (zgrid - TERRAIN_GRID_SIZE_HALF + z) * squaresize;
    x1 = (int) xpos % 297;
    z1 = (int) zpos % 153;
    xpos += z1;
    zpos += x1;
    cull = fabs((int) (camerarot.y - 180 - vectorstodegree2d(camerapos, mv3f(xpos, 0, zpos))));
    while (cull >= 360)
      cull -= 360;
    if (cull <= 85 || cull >= 275 || fabs(camerarot.x) > 27.0f) {
      temp = readTerrainB(xpos, zpos);
      dist = distance3d(camerapos, mv3f(xpos, temp.height, zpos));
      x1 = x1 * x1 + z1 * z1;
      x1 = x1 % 3176;
      if (((dist < VIEW_DISTANCE) || dist < TERRAIN_SQUARE_SIZE * 10) && (x1 < 587)) {
        if (temp.height > TERRAIN_WATER_LEVEL + 50 && temp.height < 3750 && temp.diff < 200.0f && temp.type != T_TYPE_DIRT) {
          if (dist < VIEW_DISTANCE_HALF)
            alpha = 255;
          else if (dist < VIEW_DISTANCE)
            alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
          else
            alpha = 0;
          drawModel((const struct aiScene *) &scene[x1 % 6], mv3f(xpos, temp.height, zpos), mv3f(0, x1, 0), 1, alpha);
        }
      }
    }
    if (xgrid >= TERRAIN_GRID_SIZE - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void renderSky(struct v3f camerapos, struct v3f camerarot, GLfloat *clear, GLubyte *skyColor, float fogend)
{
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glTranslatef(camerapos.x, camerapos.y, camerapos.z);
  glRotatef(-camerarot.y, 0.0f, 1.0f, 0.0f);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  //glColor3fv(clear);
  glColor3ubv(skyColor);
  glVertex3f(7000.0f, 3000.0f, 50000.0f);
  glVertex3f(-7000.0f, 3000.0f, 50000.0f);
  //glColor3ubv(skyColor);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glColor3fv(clear);
  glVertex3f(-7000.0f, -5000.0f, -fogend);
  glVertex3f(7000.0f, -5000.0f, -fogend);
  glEnd();
  glBegin(GL_TRIANGLES);
  glColor3ubv(skyColor);
  glVertex3f(7000.0f, 3000.0f, 50000.0f);
  glVertex3f(7000.0f, 3000.0f, -fogend);
  glColor3fv(clear);
  glVertex3f(7000.0f, -5000.0f, -fogend);
  glColor3ubv(skyColor);
  glVertex3f(-7000.0f, 3000.0f, -fogend);
  glVertex3f(-7000.0f, 3000.0f, 50000.0f);
  glColor3fv(clear);
  glVertex3f(-7000.0f, -5000.0f, -fogend);
  glEnd();
  glPopMatrix();
}


void renderWater(struct v3f camerapos, struct v3f camerarot, int *squaresize)
{
  int xshift, zshift, xgrid, zgrid, size = *squaresize * 8;
  float xpos, zpos;

  glMateriali(GL_FRONT, GL_SHININESS, 17);
  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(camerapos.x, 0.0f, camerapos.z);
  glRotatef(-camerarot.y, 0.0f, 1.0f, 0.0f);
  glBegin(GL_QUADS);
  glColor4ub(32, 112, 255, 210);
  glNormal3i(0, 1, 0);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE_HALF && zgrid < TERRAIN_GRID_SIZE_HALF; xgrid++) {
    xshift = zshift = size;
    xpos = (-TERRAIN_GRID_SIZE_QUARTER + xgrid) * xshift;
    zpos = (-TERRAIN_GRID_SIZE_QUARTER + zgrid) * zshift;
    xshift = zshift = size / 2;
    glVertex3f(xpos + xshift, TERRAIN_WATER_LEVEL, zpos + zshift);
    glVertex3f(xpos - xshift, TERRAIN_WATER_LEVEL, zpos + zshift);
    glVertex3f(xpos - xshift, TERRAIN_WATER_LEVEL, zpos - zshift);
    glVertex3f(xpos + xshift, TERRAIN_WATER_LEVEL, zpos - zshift);
    if (xgrid >= TERRAIN_GRID_SIZE_HALF - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
  glEnd();
  glPopMatrix();
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
}


void renderCloud(struct v3f camerapos, struct v3f camerarot, int *squaresize)
{
  int i, size;
  float rot, x, z;
  const int height = 4500;
  const float scale = 0.00005f;

  size = TERRAIN_GRID_SIZE * *squaresize;
  glMateriali(GL_FRONT, GL_SHININESS, 111);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(camerapos.x, 0.0f, camerapos.z);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glTranslatef(camerapos.x*scale, 0.0f, camerapos.z*scale);
  glScalef(scale, scale, scale);
  glColor4ub(128, 128, 128, 120);
  glNormal3i(0, -1, 0);
  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(0.0f, height, 0.0f);
  glColor4ub(128, 128, 128, 0);
  for (i = 0; i <= 360; i += 30) {
    rot = i / PIx180;
    x = -size * sinf(rot);
    z = size * cosf(rot);
    glTexCoord2f(x, z);
    glVertex3f(x, height, z);
  }
  glEnd();
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_CULL_FACE);
}


void beam(struct v3f start, struct v3f end)
{
  glBegin(GL_LINES);
  glColor4ub(255, 25, 5, 175);
  glVertex3f(-start.x, start.y, -start.z);
  glColor4ub(205, 25, 5, 0);
  glVertex3f(-end.x, end.y, -end.z);
  glEnd();
}


void sceneQuad(void)
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0, RESX, 0, RESY, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
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
  //glEnable(GL_DEPTH_TEST);
  //glEnable(GL_FOG);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}


void render(GLFWwindow *window, struct aiScene *scene, GLuint *textures, GLuint *shaders,
            int *swapb, struct v3f camerapos, struct v3f camerarot, struct v2f *sector,
            float camheight, int *squaresize, float *fogend, struct airunit *airunits)
{
  GLfloat materialColor[4];
  GLfloat clear[4]   = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat ambient[4] = {0.49f, 0.45f, 0.47f, 1.0f};
  GLubyte skyColor[3] = {117, 132, 215};

  materialColor[3] = 1.0f;
  materialColor[0] = materialColor[1] = materialColor[2] = 0.8f;
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  renderSky(camerapos, camerarot, clear, skyColor, *fogend);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  drawTerrain(camerapos, camerarot, sector, camheight, swapb, squaresize);
  renderWater(camerapos, camerarot, squaresize);
  clear[2] += fabs(sinf(camheight*0.00067f)) * 0.23f;
  updateFogLights(clear, ambient, camheight, *squaresize, fogend);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  renderFoliage(scene, camerapos, camerarot, *sector, camheight);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  //struct v3f pos = airunits[0].pos;
  //degreestovector3d(&pos, airunits[0].rot, mv3f(180, 180, 0), 15000);
  //glUseProgramARB(shaders[0]);
  //beam(airunits[0].pos, pos);
  //glUseProgramARB(0);
  //glDisable(GL_TEXTURE_2D);
  //glShadeModel(GL_FLAT);
  /*int i;
  drawModel((const struct aiScene *) &scene[6], airunits[0].pos, mv3f(airunits[0].rot.x, -airunits[0].rot.y, airunits[0].rot.z), 1, 255);
  for (i = 1; i < 15; i++)
  drawModel((const struct aiScene *) &scene[6], airunits[i].pos, mv3f(airunits[i].rot.x, -airunits[i].rot.y, airunits[i].rot.z), 1, 255);*/
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  renderCloud(camerapos, camerarot, squaresize);

  glReadBuffer(GL_BACK);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, RESX, RESY, 0);
  glUseProgramARB(shaders[1]);
  glUniform1iARB(glGetUniformLocationARB(shaders[1], "scene"), 4);
  glUniform2fARB(glGetUniformLocationARB(shaders[1], "steps"), 2000.0f, 2000.0f);
  glUniform4fvARB(glGetUniformLocationARB(shaders[1], "clear"), 0, clear);
  sceneQuad();
  glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, RESX, RESY, 0);
  glUseProgramARB(shaders[0]);
  glUniform1iARB(glGetUniformLocationARB(shaders[0], "scene"), 4);
  glUniform1fARB(glGetUniformLocationARB(shaders[0], "gamma"), 0.6f);
  glUniform1fARB(glGetUniformLocationARB(shaders[0], "numColors"), 64.0f);
  glUniform4fvARB(glGetUniformLocationARB(shaders[0], "clear"), 0, clear);
  sceneQuad();
  glUseProgramARB(0);

  if (*swapb)
    glfwSwapBuffers(window);
  *swapb = 1;
  glfwPollEvents();
}
