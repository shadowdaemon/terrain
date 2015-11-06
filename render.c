#include <assimp/scene.h>
#include "maths.h"


void renderClear(GLfloat *color, struct v3f cpos, int tsize)
{
     struct v3f pos;
     float fog_end = tsize * TERRAIN_GRID_SIZE * 0.95f;
     static float fog_start = 0.0f;
     float ground = readTerrainHeightPlane
          (cpos.x, cpos.z, &pos, tsize);
     float fstart;
     float temp = cpos.y - TERRAIN_SCALE_HEIGHT;
     if (temp < 1250.0f && cpos.y > TERRAIN_SCALE_HEIGHT)
          fog_end -= (1 - temp / 1250.0f) * (fog_end * 0.66667f);
     if (cpos.y < LOWER_CLOUD_HEIGHT)
          temp = fog_end * 0.3f;
     else if (cpos.y < TERRAIN_SCALE_HEIGHT)
          temp = fog_end * 0.5f;
     else
          temp = fog_end * 0.7f;
     if (fog_start > fog_end * 0.7f)
          fog_start = fog_end * 0.7f;
     else
          fog_start += (temp - fog_start) * 0.1f;
     ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
     fstart = (cpos.y - ground) * 0.1f;
     if (fstart > 10.0f)
          fstart = 10.0f;
     else if (fstart < 1.0f)
          fstart = 1.0f;
     glFogfv(GL_FOG_COLOR, color);
     glFogf(GL_FOG_START, fog_start);
     glFogf(GL_FOG_END, fog_end);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glFrustum(-0.54f * fstart, 0.54f * fstart,
               -0.4f * fstart, 0.4f * fstart,
               0.8f * fstart, fog_end * 1.1f);
     glMatrixMode(GL_MODELVIEW);
     glClearColor(color[0], color[1], color[2], color[3]);
     glClear(GL_COLOR_BUFFER_BIT |
             GL_DEPTH_BUFFER_BIT |
             GL_STENCIL_BUFFER_BIT);
     glShadeModel(GL_SMOOTH);
}


void sceneQuad(void)
{
     glBegin(GL_QUADS);
     glMultiTexCoord2i(GL_TEXTURE4_ARB, 0, 0);
     glVertex3i(0, 0, 0);
     glMultiTexCoord2i(GL_TEXTURE4_ARB, 1, 0);
     glVertex3i(RESX, 0, 0);
     glMultiTexCoord2i(GL_TEXTURE4_ARB, 1, 1);
     glVertex3i(RESX, RESY, 0);
     glMultiTexCoord2i(GL_TEXTURE4_ARB, 0, 1);
     glVertex3i(0, RESY, 0);
     glEnd();
}


void renderSky(struct v3f cpos, struct v3f crot, GLfloat *clear, int tsize)
{
     float fog_end = tsize * TERRAIN_GRID_SIZE * 0.45f;
     glPushMatrix();
     glDisable(GL_DEPTH_TEST);
     glDisable(GL_FOG);
     glTranslatef(cpos.x, cpos.y, cpos.z);
     glRotatef(-crot.y, 0.0f, 1.0f, 0.0f);
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


void renderWater(struct v3f cpos, struct v3f crot, GLfloat color[4],
                 int tsize)
{
     int xshift, zshift, xgrid, zgrid, size = tsize * 8;
     float xpos, zpos;
     const float scale = 0.0001f;
     glMateriali(GL_FRONT, GL_SHININESS, 97);
     glDisable(GL_CULL_FACE);
     glPushMatrix();
     glTranslatef(cpos.x, 0.0f, cpos.z);
     glMatrixMode(GL_TEXTURE);
     glPushMatrix();
     glScalef(scale, scale, scale);
     glTranslatef(cpos.x, cpos.z, 0.0f);
     glBegin(GL_QUADS);
     color[3] = 0.7f;
     glColor4fv(color);
     glNormal3i(0, -1, 0);
     for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE_HALF
            && zgrid < TERRAIN_GRID_SIZE_HALF; xgrid++) {
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


void renderCloud(struct v3f cpos, struct v3f crot, GLubyte alpha,
                 float height, float scale, int tsize)
{
     int i, size;
     float rot, x, z;
     int temp;
     size = TERRAIN_GRID_SIZE * tsize * 0.5f;
     temp = fabs(height - cpos.y) * 0.1f;
     temp = temp > alpha ? alpha : temp;
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);
     glMateriali(GL_FRONT, GL_SHININESS, 31);
     glDisable(GL_CULL_FACE);
     glEnable(GL_TEXTURE_2D);
     glPushMatrix();
     glTranslatef(cpos.x, 0.0f, cpos.z);
     glMatrixMode(GL_TEXTURE);
     glPushMatrix();
     glScalef(scale, scale, scale);
     glTranslatef(cpos.x, cpos.z, 0.0f);
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
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
}


void renderSun(GLuint *shaders, GLuint *textures,
               struct v3f cpos, GLint sunpos[4])
{
     int i;
     float r, x, z;
     glPushMatrix();
     glTranslatef(cpos.x + sunpos[0],
                  cpos.y + sunpos[1],
                  cpos.z + sunpos[2]);
     r = atan2(sunpos[0], sunpos[1]) * -180 / PI;
     glRotatef(r, 0.0f, 0.0f, 1.0f);
     glBegin(GL_TRIANGLE_FAN);
     glColor4ub(235, 215, 30, 10 + (GLubyte) fabs(sunpos[1] * 0.244f));
     glVertex3f(0.0f, 0.0f, 0.0f);
     glColor4ub(235, 200, 60, 3);
     for (i = 0; i <= 360; i += 15) {
          r = i / PIx180;
          x = -500 * sinf(r);
          z = 500 * cosf(r);
          glVertex3f(x, 0.0f, z);
     }
     glEnd();
     glBegin(GL_TRIANGLE_FAN);
     glColor4ub(245, 235, 137, 10 + (GLubyte) fabs(sunpos[1] * 0.244f));
     glVertex3f(0.0f, 0.0f, 0.0f);
     glColor4ub(235, 223, 91, 153);
     for (i = 0; i <= 360; i += 15) {
          r = i / PIx180;
          x = -100 * sinf(r);
          z = 100 * cosf(r);
          glVertex3f(x, 0.0f, z);
     }
     glEnd();
     glPopMatrix();
     if (USE_SHADERS) {
          glMatrixMode(GL_PROJECTION);
          glPushMatrix();
          glLoadIdentity();
          glOrtho(0, RESX, 0, RESY, -1, 1);
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
          glLoadIdentity();
          glDisable(GL_DEPTH_TEST);
          glDisable(GL_FOG);
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textures[TEX_RENDER]);
          glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, RESX, RESY, 0);
          glUseProgramARB(shaders[4]);
          glUniform1iARB(glGetUniformLocationARB
                         (shaders[4], "scene"), 4);
          glUniform2fARB(glGetUniformLocationARB
                         (shaders[4], "steps"), RESX, RESY);
          sceneQuad();
          glUseProgramARB(0);
          glEnable(GL_DEPTH_TEST);
          glEnable(GL_FOG);
          glDisable(GL_TEXTURE_2D);
          glMatrixMode(GL_PROJECTION);
          glPopMatrix();
          glMatrixMode(GL_MODELVIEW);
          glPopMatrix();
     }
}


void renderMoon(struct v3f cpos, GLint pos[4], float size)
{
     int i;
     float r, x, z;
     glPushMatrix();
     glTranslatef(cpos.x + pos[0], cpos.y + pos[1], cpos.z + pos[2]);
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
     glVertex3f(1.4286f, 1.4286f, -powf(size, 1.5f)
                * 5.0f - 14.857f * size);
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
     glVertex3f(-1.4286f, 1.4286f, -powf(size, 1.5f) *
                5.0f - 14.857f * size);
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
     drawModel((const struct aiScene *) &textquads[num % 10],
               mv3f(pos.x, pos.y, 0), mv3f(0, 0, 0), 250, 255);
     while (num > 9) {
          num -= num % 10;
          num /= 10;
          drawModel((const struct aiScene *) &textquads[num % 10],
                    mv3f(pos.x - 50 * ++i, pos.y, 0), mv3f(0, 0, 0),
                    250, 255);
     }
     if (j < 0)
          drawModel((const struct aiScene *) &textquads[10],
                    mv3f(pos.x - 50 * ++i, pos.y, 0), mv3f(0, 0, 0),
                    250, 255);
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


void renderUnits(struct aiScene *scene, GLuint *textures,
                 struct unit *units)
{
     int i, tB, tP, tW, mB, mP, mW;
     for (i = 0; i < 5; i++) {
          switch (units[i].type) {
          case UNIT_AIR_FIGHTER_1:
               tB  = TEX_BODY_1;
               mB  = MODEL_BODY_1;
               tP  = TEX_PROP_1;
               mP  = MODEL_PROP_1;
               tW  = TEX_WEAPON_1;
               mW  = 0;//MODEL_WEAPON_1;
               break;
          case UNIT_GROUND_JEEP_1:
               tB  = TEX_BARB_1;
               mB  = MODEL_JEEP_1;
               tP  = 0;
               mP  = 0;
               tW  = 0;
               mW  = 0;
               break;
          default:
               tB  = 0;
               mB  = 0;
               tP  = 0;
               mP  = 0;
               tW  = 0;
               mW  = 0;
          }
          if (mB) {
          glBindTexture(GL_TEXTURE_2D, textures[tB]);
          drawModel((const struct aiScene *) &scene[mB], units[i].pos,
                    units[i].rot, 0.25f, 255);
          }
          if (mP) {
          glBindTexture(GL_TEXTURE_2D, textures[tP]);
          drawModel((const struct aiScene *) &scene[mP], units[i].pos,
                    units[i].rot, 0.25f, 255);
          }
          if (mW) {
          glBindTexture(GL_TEXTURE_2D, textures[tW]);
          drawModel((const struct aiScene *) &scene[mW], units[i].pos,
                    units[i].rot, 0.25f, 255);
          }
     }
}


void render(GLFWwindow *window, struct aiScene *scene,
            struct aiScene *textquads,
            GLuint *textures, GLuint *shaders,
            struct v3f cpos, /* Camera position. */
            struct v3f crot, /* Camera rotation. */
            struct v2f *sector, int *tsize, float *fps,
            struct team *teams) {
     GLfloat color[4], temp;
     GLint lpos[4], mpos[4];
     int i;
     struct v3f n;
     float camHeight = fabs(cpos.y - readTerrainHeightPlane
                            (cpos.x, cpos.z, &n, *tsize));
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
     if (lpos[1] < 0)
          temp = 0.35f * (1 + lpos[1] / 1000.0f);
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
     color[0] = 0.18f;
     color[1] = 0.18f;
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
     color[1] = 0.5176470588235295f *
          (lpos[1] > -400 ? (lpos[1] + 400) / 1400.0f : 0);
     color[2] = 0.8431372549019608f *
          (lpos[1] > -200 ? 0.05f + (lpos[1] + 200) / 1200.0f : 0.05f);
     renderClear(color, cpos, *tsize);
     renderSky(cpos, crot, color, *tsize);
     renderSun(shaders, textures, cpos, lpos);
     renderMoon(cpos, mpos, 60);
     drawTerrain(textures, cpos, crot, sector, tsize, &swapb);
     if (cpos.y < TERRAIN_SCALE_HEIGHT && camHeight < VIEW_DISTANCE)
          renderGroundScenery(scene, textures, cpos, crot, *tsize, fps2);
     if (swapb) {
          glBindTexture(GL_TEXTURE_2D, textures[TEX_CLOUD]);
          renderWater(cpos, crot, color, *tsize);
          char numTeams = 2;
          for (i = 0; i < numTeams; i++) {
               renderUnits(scene, textures, teams[i].airunits);
               renderUnits(scene, textures, teams[i].groundunits);
          }
          if (0) { /* Just disable for now. */
               glEnable(GL_POINT_SPRITE);
               glEnable(GL_PROGRAM_POINT_SIZE);
               glUseProgramARB(shaders[2]);
               glUniform1iARB(glGetUniformLocationARB
                              (shaders[2], "texture"), 1);
               glUniform1fARB(glGetUniformLocationARB
                              (shaders[2], "size"), 10.0f);
               glUniform2fARB(glGetUniformLocationARB
                              (shaders[2], "screensize"), RESX, RESY);
               renderFX();
               glUseProgramARB(0);
          }
          glEnable(GL_LIGHTING);
          glEnable(GL_TEXTURE_2D);
          glBindTexture(GL_TEXTURE_2D, textures[TEX_CLOUD]);
          if (cpos.y < TERRAIN_SCALE_HEIGHT) {
               renderCloud(cpos, crot, 180 * CLOUD_DENSITY,
                           TERRAIN_SCALE_HEIGHT, 0.00005f, *tsize);
               renderCloud(cpos, crot, 100 * CLOUD_DENSITY,
                           LOWER_CLOUD_HEIGHT, 0.00001f, *tsize);
               if (camHeight < VIEW_DISTANCE_HALF)
                    renderGrass(textures, cpos, crot, *tsize, fps2);
          }
          else {
               renderCloud(cpos, crot, 65 * CLOUD_DENSITY,
                           LOWER_CLOUD_HEIGHT, 0.00001f, *tsize);
               renderCloud(cpos, crot, 120 * CLOUD_DENSITY,
                           TERRAIN_SCALE_HEIGHT, 0.00005f, *tsize);
          }
          glMatrixMode(GL_PROJECTION);
          glPushMatrix();
          glLoadIdentity();
          glOrtho(0, RESX, 0, RESY, -1, 1);
          glMatrixMode(GL_MODELVIEW);
          glPushMatrix();
          glLoadIdentity();
          if (USE_SHADERS) {
               glDisable(GL_DEPTH_TEST);
               glDisable(GL_FOG);
               glBindTexture(GL_TEXTURE_2D, textures[TEX_RENDER]);
               glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                               GL_LINEAR);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                               GL_LINEAR);
               glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0,
                                RESX, RESY, 0);
               glUseProgramARB(shaders[0]);
               glUniform1iARB(glGetUniformLocationARB
                              (shaders[0], "scene"), 4);
               glUniform1fARB(glGetUniformLocationARB
                              (shaders[0], "gamma"), 0.6f);
               glUniform1fARB(glGetUniformLocationARB
                              (shaders[0], "numcolors"), 64.0f);
               glUniform2fARB(glGetUniformLocationARB
                              (shaders[0], "steps"), RESX * 2, RESY * 2);
               glUniform4fvARB(glGetUniformLocationARB
                               (shaders[0], "clear"), 0, color);
               sceneQuad();
          }
          glUseProgramARB(0);
          /* glBindTexture(GL_TEXTURE_2D, textures[TEX_FONT]); */
          /* renderNumber(cpos.x, textquads, mv2f(250, 120)); */
          /* renderNumber(cpos.z, textquads, mv2f(250, 70)); */
          /* renderNumber(fps2, textquads, mv2f(250, 20)); */
          /* renderNumber(airunits[0].p.airv.height, textquads, */
          /*              mv2f(RESX - 100, 120)); */
          /* renderNumber(airunits[0].p.airv.speed * 10.0f, textquads, */
          /*              mv2f(RESX - 100, 70)); */
          /* renderNumber(airunits[0].p.airv.thrust * 100, textquads, */
          /*              mv2f(RESX - 100, 20)); */
     }
     glMatrixMode(GL_PROJECTION);
     glPopMatrix();
     glMatrixMode(GL_MODELVIEW);
     glPopMatrix();
     if (swapb)
          glfwSwapBuffers(window);
     swapb = 1;
     glfwPollEvents();
}
