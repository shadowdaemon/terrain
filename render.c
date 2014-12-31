#include <assimp/scene.h>
#include "maths.h"


void updateFogAndFrustum(GLfloat *clear, struct v3f camerapos, int t_size)
{
  struct v3f pos;
  float fog_end = t_size * TERRAIN_GRID_SIZE * 0.45f;
  static float fog_start;
  float ground = readTerrainHeightPlane(camerapos.x, camerapos.z, &pos, t_size);
  float fstart;
  float temp = camerapos.y - TERRAIN_SCALE_HEIGHT;

  if (temp < 1250.0f && camerapos.y > TERRAIN_SCALE_HEIGHT)
    fog_end -= (1 - temp / 1250.0f) * (fog_end * 0.66667f);
  if (camerapos.y < LOWER_CLOUD_HEIGHT)
    temp = fog_end * 0.3f;
  else if (camerapos.y < TERRAIN_SCALE_HEIGHT)
    temp = fog_end * 0.5f;
  else
    temp = fog_end * 0.7f;
  if (fog_start > fog_end * 0.7f)
    fog_start = fog_end * 0.7f;
  else
    fog_start += (temp - fog_start) * 0.1f;
  ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
  fstart = (camerapos.y - ground) * 0.1f;
  if (fstart > 10.0f)
    fstart = 10.0f;
  else if (fstart < 1.0f)
    fstart = 1.0f;
  glFogfv(GL_FOG_COLOR, clear);
  glFogf(GL_FOG_START, fog_start);
  glFogf(GL_FOG_END, fog_end);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.54f * fstart, 0.54f * fstart, -0.4f * fstart, 0.4f * fstart,
            0.8f * fstart, fog_end * 1.1f);
  glMatrixMode(GL_MODELVIEW);
}


void renderGroundScenery(struct aiScene *scene, GLuint *textures, struct v3f camerapos,
                         struct v3f camerarot, struct v2f sector, int t_size, char swapb)
{
  int xgrid, zgrid, x, z, x1, z1, cull, density, i;
  const int size = t_size * 0.2f; /* Size of generation sector, also affects density. */
  float xpos, zpos, dist;
  static struct v3f normal[SCENERY_SIZE];
  static float height[SCENERY_SIZE];
  static unsigned char type[SCENERY_SIZE];
  GLubyte alpha;
  GLuint color[3];

  glMateriali(GL_FRONT, GL_SHININESS, 92);
  x = (int) (sector.x / size);
  z = (int) (sector.y / size);
  for (xgrid = 0, zgrid = 0, i = 0; i < SCENERY_SIZE && xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
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
    if (cull <= 85 || cull >= 275 || camerarot.x > 27 || swapb == 0) {
      if (swapb == 0) {
        height[i] = readTerrainHeightPlane(xpos, zpos, &normal[i], t_size);
        type[i] = readTerrainType(xpos, zpos);
      }
      dist = distance3d(camerapos, mv3f(xpos, height[i], zpos));
      x1 = x1 * x1 + z1 * z1;
      x1 = x1 % SCENERY_DENSITY;
      switch (type[i]) {
      case T_TYPE_GRASS1:
        density = 210;
        break;
      case T_TYPE_GRASS2:
        density = 327;
        break;
      case T_TYPE_GRASS3:
        density = 201;
        break;
      case T_TYPE_VILLAGE:
        density = 103;
        break;
      case T_TYPE_FOREST1:
        density = 470;
        break;
      case T_TYPE_FOREST2:
        density = 420;
        break;
      case T_TYPE_DIRT:
        density = 76;
        break;
      default:
        density = 170;
      }
      if (dist < VIEW_DISTANCE || dist < t_size * 6) {
        if (height[i] > TERRAIN_WATER_LEVEL + 50 && height[i] < 4000) {
          if (dist < VIEW_DISTANCE_HALF)
            alpha = 255;
          else if (dist < VIEW_DISTANCE)
            alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
          else
            alpha = 0;
          if (x1 < density) {
            glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE]);
            if (type[i] == T_TYPE_GRASS1 || type[i] == T_TYPE_GRASS2) {
              if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.1f)
                drawModel((const struct aiScene *) &scene[MODEL_MTREE_SPARSE], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.333f, alpha);
              else if (x1 % 15 < 5)
                drawModel((const struct aiScene *) &scene[MODEL_TREE_POPLAR], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.333f, alpha);
              else if (x1 % 15 == 6) {
                color[0] = 95; color[1] = 95; color[2] = 95;
                glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN]);
                drawModel2((const struct aiScene *) &scene[MODEL_ROCK1], mv3f(xpos, height[i], zpos), mv3f(x1, z1, 0),
                  1.8f + (z1 % 10) * 0.23f, color, alpha);
              }
              else if (x1 % 15 == 7 && z1 % 7 == 0) {
                if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.3f) {
                  glBindTexture(GL_TEXTURE_2D, textures[5]);
                  drawModel((const struct aiScene *) &scene[MODEL_BUILDING_HOUSE1], mv3f(xpos, height[i], zpos), mv3f(0, x1 % 90, 0), 0.35f, alpha);
                }
              }
              else
                drawModel((const struct aiScene *) &scene[x1 % 6], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.33f, alpha);
            }
            else if (type[i] == T_TYPE_FOREST1) {
              if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.1f)
                drawModel((const struct aiScene *) &scene[MODEL_MTREE_SPARSE], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.34f, alpha);
              else if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.14f)
                drawModel((const struct aiScene *) &scene[MODEL_MTREE_BIG], mv3f(xpos, height[i] - 1, zpos), mv3f(0, x1, 0), 0.34f, alpha);
              else
                drawModel((const struct aiScene *) &scene[x1 % 6], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.34f, alpha);
            }
            else if (type[i] == T_TYPE_FOREST2) {
              if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.1f)
                drawModel((const struct aiScene *) &scene[MODEL_MTREE_FIR], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.34f, alpha);
              else if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.14f)
                drawModel((const struct aiScene *) &scene[MODEL_MTREE_BIG], mv3f(xpos, height[i] - 1, zpos), mv3f(0, x1, 0), 0.34f, alpha);
              else
                drawModel((const struct aiScene *) &scene[MODEL_TREE_FIR], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.35f, alpha);
            }
            else if (type[i] == T_TYPE_DIRT || type[i] == T_TYPE_DESERT) {
              if (x1 % 3 == 0)
                drawModel((const struct aiScene *) &scene[MODEL_TREE_STUMP], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.333f, alpha);
              else if (x1 % 3 == 1 && height[i] < 3700) {
                color[0] = 255; color[1] = 230; color[2] = 240;
                drawModel2((const struct aiScene *) &scene[MODEL_TREE_BUSH], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.32f, color, alpha);
              }
              else {
                color[0] = 103; color[1] = 111; color[2] = 63;
                glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN]);
                drawModel2((const struct aiScene *) &scene[MODEL_ROCK1], mv3f(xpos, height[i], zpos), mv3f(x1, z1, 0),
                  2.3f + (z1 % 10) * 0.23f, color, alpha);
              }
            }
            else
              drawModel((const struct aiScene *) &scene[x1 % 6], mv3f(xpos, height[i], zpos), mv3f(0, x1, 0), 0.333f, alpha);
          }
          if (type[i] == T_TYPE_VILLAGE && x1 < 477) {
            xpos += 31;
            zpos += 37;
            height[i] = readTerrainHeightPlane(xpos, zpos, &normal[i], t_size);
            if (distance3d(mv3f(0, 1, 0), normalize3d(normal[i])) < 0.3f) {
              glBindTexture(GL_TEXTURE_2D, textures[5]);
              drawModel((const struct aiScene *) &scene[MODEL_BUILDING_HOUSE1], mv3f(xpos, height[i], zpos), mv3f(0, x1 % 90, 0), 0.35f, alpha);
            }
          }
        }
      }
    }
    i++;
    if (xgrid >= TERRAIN_GRID_SIZE - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void renderSky(struct v3f camerapos, struct v3f camerarot, GLfloat *clear, int t_size)
{
  float fog_end = t_size * TERRAIN_GRID_SIZE * 0.45f;

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
  glVertex3f(-7000.0f, 3000.0f, -fog_end);
  glVertex3f(7000.0f, 3000.0f, -fog_end);
  glVertex3f(7000.0f, 3000.0f, -fog_end);
  glVertex3f(-7000.0f, 3000.0f, -fog_end);
  glVertex3f(-7000.0f, -5000.0f, -fog_end);
  glVertex3f(7000.0f, -5000.0f, -fog_end);
  glEnd();
  glBegin(GL_TRIANGLES);
  glVertex3f(7000.0f, 3000.0f, 50000.0f);
  glVertex3f(7000.0f, 3000.0f, -fog_end);
  glVertex3f(7000.0f, -5000.0f, -fog_end);
  glVertex3f(-7000.0f, 3000.0f, -fog_end);
  glVertex3f(-7000.0f, 3000.0f, 50000.0f);
  glVertex3f(-7000.0f, -5000.0f, -fog_end);
  glEnd();
  glPopMatrix();
}


void renderWater(struct v3f camerapos, struct v3f camerarot, GLfloat color[4], int t_size)
{
  int xshift, zshift, xgrid, zgrid, size = t_size * 8;
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


void renderCloud(struct v3f camerapos, struct v3f camerarot, GLubyte alpha, float height, float scale, int t_size)
{
  int i, size;
  float rot, x, z;
  int temp;

  size = TERRAIN_GRID_SIZE * t_size * 0.5f;
  temp = fabs(height - camerapos.y) * 0.1f;
  temp = temp > alpha ? alpha : temp;
  glMateriali(GL_FRONT, GL_SHININESS, 31);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(camerapos.x, 0.0f, camerapos.z);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glScalef(scale, scale, scale);
  glTranslatef(camerapos.x, camerapos.z, 0.0f);
  glColor4ub(128, 128, 128, (GLubyte) temp);
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


void renderExhaust(struct v3f pos, struct v3f rot, float scale, float size)
{
  int i;
  float r, x, y;

  glMateriali(GL_FRONT, GL_SHININESS, 110);
  /* glMateriali(GL_FRONT, GL_EMISSION, 78); */
  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glScalef(scale, scale, scale);
  glRotatef(-rot.y, 0.0f, 1.0f, 0.0f);
  glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
  glRotatef(rot.z, 0.0f, 0.0f, 1.0f);
  glBegin(GL_TRIANGLE_FAN);
  glColor4ub(195, 110, 30, 185);
  glVertex3f(1.4286f, 1.4286f, -powf(size, 1.5f) * 5.0f - 14.857f * size);
  glColor4ub(255, 225, 90, 30);
  for (i = 0; i <= 360; i += 60) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x + 1.4286f, y + 1.428f, -4.857f);
  }
  glEnd();
  glBegin(GL_POLYGON);
  glColor4ub(255, 225, 90, 70);
  for (i = 360; i >= 0; i -= 60) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x + 1.428f, y + 1.428f, -4.857f);
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glColor4ub(195, 110, 30, 185);
  glVertex3f(-1.4286f, 1.4286f, -powf(size, 1.5f) * 5.0f - 14.857f * size);
  glColor4ub(255, 225, 90, 30);
  for (i = 0; i <= 360; i += 60) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x - 1.4286f, y + 1.428f, -4.857f);
  }
  glEnd();
  glBegin(GL_POLYGON);
  glColor4ub(255, 225, 90, 70);
  for (i = 360; i >= 0; i -= 60) {
    r = i / PIx180;
    x = -size * sinf(r);
    y = size * cosf(r);
    glVertex3f(x - 1.428f, y + 1.428f, -4.857f);
  }
  glEnd();
  glPopMatrix();
  /* glMateriali(GL_FRONT, GL_EMISSION, 0); */
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
    glVertex3f(xgrid * 50, 5000, zgrid * 50);
    glEnd();
    if (xgrid >= 9) {
      zgrid++;
      xgrid = -1;
    }
  }
}


void renderAircraft(struct aiScene *scene, GLuint *textures, struct v3f camerapos,
                    struct airunit *units)
{
  int i, texture, model;

  for (i = 0; i < 15; i++) {
    switch (units[i].type) {
    case UNIT_AIR_FIGHTER1:
      texture = TEX_AIR_FIGHTER1;
      model = MODEL_AIR_FIGHTER1;
      break;
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, textures[texture]);
    drawModel((const struct aiScene *) &scene[model], units[i].pos, units[i].rot, 0.7f, 255);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    renderExhaust(units[i].pos, units[i].rot, 0.7f, units[i].thrust * 1.5f);
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
            int *t_size, float *fps, struct airunit *airunits)
{
  GLfloat color[4], temp;
  GLint lpos[4], mpos[4];
  static double time = 0;
  static float fps2 = 0;
  static char swapb = 1;

  *fps = 1 / (glfwGetTime() - time);
  fps2 += (*fps - fps2) * 0.05f;
  time = glfwGetTime();
  glEnable(GL_LIGHT0); /* sun */
  glEnable(GL_LIGHT1); /* moon */
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
  /* glMaterialfv(GL_FRONT, GL_EMISSION, color); */
  color[0] = 0.4588235294117647f * temp;
  color[0] += lpos[0] > 800 ? (lpos[0] - 800) * 0.005f * 0.1f : 0;
  color[1] = 0.5176470588235295f * (lpos[1] > -400 ? (lpos[1] + 400) / 1400.0f : 0);
  color[2] = 0.8431372549019608f * (lpos[1] > -200 ? 0.05f + (lpos[1] + 200) / 1200.0f : 0.05f);
  updateFogAndFrustum(color, camerapos, *t_size);
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glShadeModel(GL_SMOOTH);
  renderSky(camerapos, camerarot, color, *t_size);
  renderSun(camerapos, lpos, 160);
  renderMoon(camerapos, mpos, 60);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN]);
  drawTerrain(camerapos, camerarot, sector, t_size, &swapb);
  renderGroundScenery(scene, textures, camerapos, camerarot, *sector, *t_size, swapb);
  if (swapb) {
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    renderWater(camerapos, camerarot, color, *t_size);
    renderAircraft(scene, textures, camerapos, airunits);
    /* glEnable(GL_POINT_SPRITE); */
    /* glEnable(GL_PROGRAM_POINT_SIZE); */
    /* glUseProgramARB(shaders[2]); */
    /* glUniform1iARB(glGetUniformLocationARB(shaders[2], "texture"), 1); */
    /* glUniform1fARB(glGetUniformLocationARB(shaders[2], "size"), 10.0f); */
    /* glUniform2fARB(glGetUniformLocationARB(shaders[2], "screensize"), 1366, 768); */
    /* renderFX(); */
    /* glUseProgramARB(0); */
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    if (camerapos.y < TERRAIN_SCALE_HEIGHT) {
      renderCloud(camerapos, camerarot, 180, TERRAIN_SCALE_HEIGHT, 0.00005f, *t_size);
      renderCloud(camerapos, camerarot, 100, LOWER_CLOUD_HEIGHT, 0.00001f, *t_size);
    }
    else {
      renderCloud(camerapos, camerarot, 65, LOWER_CLOUD_HEIGHT, 0.00001f, *t_size);
      renderCloud(camerapos, camerarot, 120, TERRAIN_SCALE_HEIGHT, 0.00005f, *t_size);
    }
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, RESX, 0, RESY, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    /* renderNumber(camerapos->x, textquads, mv2f(RESX - 100, 120)); */
    /* renderNumber(camerapos->z, textquads, mv2f(RESX - 100, 70)); */
    renderNumber(fps2, textquads, mv2f(100, 20));
    renderNumber(airunits[0].height, textquads, mv2f(RESX - 100, 120));
    renderNumber(airunits[0].speed * 10.0f, textquads, mv2f(RESX - 100, 70));
    renderNumber(airunits[0].thrust * 100, textquads, mv2f(RESX - 100, 20));
    if (USE_SHADERS) {
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
      glUniform2fARB(glGetUniformLocationARB(shaders[1], "steps"), RESX, RESY);
      glUniform4fvARB(glGetUniformLocationARB(shaders[1], "clear"), 0, color);
      sceneQuad();
    }
  }
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
