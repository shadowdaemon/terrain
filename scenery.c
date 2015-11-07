#include <assimp/scene.h>
#include <stdlib.h>
#include "maths.h"


void gScenery(struct aiScene *scene, GLuint *textures,
              float *height, unsigned char *type, struct v3f *normal,
              float dist, float vdist, float vdisthalf, int density,
              int x1, int z1, int xpos, int zpos, int i)
{
     int a;
     float slope;
     GLubyte alpha;
     GLuint color[3];
     const float msize = 0.655f;
     if (height[i] > TERRAIN_WATER_LEVEL + 50) {
          if (dist < vdisthalf)
               alpha = 255;
          else if (dist < vdist)
               alpha = (GLubyte) (255 - ((dist - vdisthalf)
                                         / vdisthalf) * 255);
          else
               alpha = 0;
          if (x1 < density) {
               slope = distance3d(mv3f(0.0f, -1.0f, 0.0f), normal[i]);
               glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE]);
               if (type[i] == T_TYPE_GRASS_1 || type[i] == T_TYPE_GRASS_2) {
                    a = x1 % 15;
                    if (slope < 0.1f && height[i] > 250)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_SPARSE],
                                   mv3f(xpos, height[i] - 1.5f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else if (a < 5)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_TREE_POPLAR],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else if (a == 6) {
                         color[0] = 95; color[1] = 95; color[2] = 95;
                         glBindTexture(GL_TEXTURE_2D,
                                       textures[TEX_TERRAIN_1]);
                         drawModel2((const struct aiScene *)
                                    &scene[MODEL_ROCK1],
                                    mv3f(xpos, height[i], zpos),
                                    mv3f(x1, z1, 0),
                                    1.8f + (z1 % 10) * 0.23f, color, alpha);
                    }
                    else if (x1 % 15 == 7 && z1 % 7 == 0) {
                         if (slope < 0.3f) {
                              glBindTexture(GL_TEXTURE_2D,
                                            textures[TEX_BUILDING_1]);
                              drawModel((const struct aiScene *)
                                        &scene[MODEL_BUILDING_HOUSE1],
                                        mv3f(xpos, height[i], zpos),
                                        mv3f(0, x1 % 90, 0), msize, alpha);
                         }
                    }
                    else
                         drawModel((const struct aiScene *)
                                   &scene[x1 % 6],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), msize, alpha);
               }
               else if (type[i] == T_TYPE_FOREST1) {
                    if (slope < 0.1f)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_SPARSE],
                                   mv3f(xpos, height[i] - 1.5f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else if (slope < 0.14f)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_BIG],
                                   mv3f(xpos, height[i] - 1.8f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else
                         drawModel((const struct aiScene *)
                                   &scene[x1 % 6],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), msize, alpha);
               }
               else if (type[i] == T_TYPE_FOREST2) {
                    if (slope < 0.1f)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_FIR],
                                   mv3f(xpos, height[i] - 1.5f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else if (slope < 0.14f)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_BIG],
                                   mv3f(xpos, height[i] - 1.8f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_TREE_FIR],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), msize, alpha);
               }
               else if (type[i] == T_TYPE_DIRT || type[i] == T_TYPE_DESERT) {
                    a = x1 % 3;
                    if (a == 0)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_TREE_STUMP],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), 0.333f, alpha);
                    else if (a == 1 && height[i] < 3700) {
                         color[0] = 255; color[1] = 230; color[2] = 240;
                         drawModel2((const struct aiScene *)
                                    &scene[MODEL_TREE_BUSH],
                                    mv3f(xpos, height[i], zpos),
                                    mv3f(0, x1, 0), 0.333f, color, alpha);
                    }
                    else {
                         color[0] = 103; color[1] = 111; color[2] = 63;
                         glBindTexture(GL_TEXTURE_2D,
                                       textures[TEX_TERRAIN_1]);
                         drawModel2((const struct aiScene *)
                                    &scene[MODEL_ROCK1],
                                    mv3f(xpos, height[i], zpos),
                                    mv3f(x1, z1, 0),
                                    2.3f + (z1 % 10) * 0.23f, color, alpha);
                    }
               }
               else if (type[i] == T_TYPE_VILLAGE) {
                    a = (x1 + z1) % 15;
                    if (slope < 0.1f && a < 2)
                         drawModel((const struct aiScene *)
                                   &scene[MODEL_MTREE_SPARSE],
                                   mv3f(xpos, height[i] - 1.5f, zpos),
                                   mv3f(0, x1, 0), msize, alpha);
                    else if (slope < 0.3f) {
                         glBindTexture(GL_TEXTURE_2D,
                                       textures[TEX_BUILDING_1]);
                         if (a < 5)
                              drawModel((const struct aiScene *)
                                        &scene[MODEL_BUILDING_HOUSE2],
                                        mv3f(xpos, height[i], zpos),
                                        mv3f(0, x1 % 90, 0), msize, alpha);
                         else if (a < 10)
                              drawModel((const struct aiScene *)
                                        &scene[MODEL_BUILDING_HOUSE1],
                                        mv3f(xpos, height[i], zpos),
                                        mv3f(0, x1 % 90, 0), msize, alpha);
                         else {
                              glBindTexture(GL_TEXTURE_2D,
                                            textures[TEX_FOLIAGE]);
                              drawModel((const struct aiScene *)
                                        &scene[MODEL_TREE_POPLAR],
                                        mv3f(xpos, height[i], zpos),
                                        mv3f(0, x1, 0), msize, alpha);
                         }
                    }
                    else
                         drawModel((const struct aiScene *)
                                   &scene[x1 % 6],
                                   mv3f(xpos, height[i], zpos),
                                   mv3f(0, x1, 0), msize, alpha);
               }
               else if (type[i] == T_TYPE_SNOW) {
                    if (slope < 0.45f) {
                         color[0] = 155; color[1] = 155; color[2] = 155;
                         glBindTexture(GL_TEXTURE_2D,
                                       textures[TEX_TERRAIN_1]);
                         drawModel2((const struct aiScene *)
                                    &scene[MODEL_ROCK1],
                                    mv3f(xpos, height[i], zpos),
                                    mv3f(x1, z1, 0),
                                    2.35f + (z1 % 10) * 0.71f, color, alpha);
                    }
               }
               else if (height[i] < 3100)
                    drawModel((const struct aiScene *)
                              &scene[x1 % 6],
                              mv3f(xpos, height[i], zpos),
                              mv3f(0, x1, 0), msize, alpha);
          }
     }
}


void renderGroundScenery(struct aiScene *scene, GLuint *textures,
                         struct v3f cpos, struct v3f crot,
                         int tsize, float fps)
{
     int xgrid, zgrid, x, z, x1, z1, cull, density, i;
     static char update = 1;
     static float vdist = VIEW_DISTANCE * 0.3f;
     const int size = 100;
     float xpos, zpos, dist, vdisthalf;
     static struct v3f sector;
     static struct v3f normal[SCENERY_SIZE];
     static float height[SCENERY_SIZE];
     static unsigned char type[SCENERY_SIZE];
     glMateriali(GL_FRONT, GL_SHININESS, 92);
     if (distance2d(cpos, sector) > 2 * tsize) {
          sector = cpos;
          update = 1;
     }
     x = (int) (sector.x / size);
     z = (int) (sector.z / size);
     if (fps < 16.0f && vdist > 300.0f) {
          if (fps < 12.0f)
               vdist -= 25.0f;
          else
               vdist -= 2.5f;
     }
     else if (vdist < VIEW_DISTANCE)
          vdist++;
     vdisthalf = vdist / 2.0f;
     for (xgrid = 0, zgrid = 0, i = 0; i < SCENERY_SIZE &&
               xgrid < TERRAIN_GRID_SIZE &&
               zgrid < TERRAIN_GRID_SIZE; xgrid++) {
          xpos = (xgrid - TERRAIN_GRID_SIZE_HALF + x) * size;
          zpos = (zgrid - TERRAIN_GRID_SIZE_HALF + z) * size;
          x1 = sqrt(fabs(xpos * zpos + 107)) * 123;
          x1 = (int) (x1 + xpos) % 88;
          z1 = (int) (x1 + zpos) % 85;
          xpos += z1;
          zpos += x1;
          dist = distance3d(cpos, mv3f(xpos, height[i], zpos));
          cull = fabs((int) (crot.y - 180 - vectorstodegree2d
                             (cpos, mv3f(xpos, 0, zpos))));
          while (cull >= 360)
               cull -= 360;
          if (cull <= 85 || cull >= 275 || crot.x > 27
              || update == 1 || dist < tsize * 2) {
               if (update) {
                    height[i] = readTerrainHeightPlane
                         (xpos, zpos, &normal[i], tsize) - 0.5f;
                    type[i]   = readTerrainType(xpos, zpos);
               }
               x1 = x1 * x1 + z1 * z1;
               x1 = x1 % SCENERY_DENSITY;
               switch (type[i]) {
               case T_TYPE_GRASS_1:
                    density = 210;
                    break;
               case T_TYPE_GRASS_2:
                    density = 327;
                    break;
               case T_TYPE_GRASS_3:
                    density = 201;
                    break;
               case T_TYPE_VILLAGE:
                    density = 720;
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
               case T_TYPE_DESERT:
                    density = 52;
                    break;
               case T_TYPE_SNOW:
                    density = 103;
                    break;
               default:
                    density = 170;
               }
               if (dist < vdist)
                    gScenery(scene, textures, height, type, normal,
                             dist, vdist, vdisthalf, density, x1, z1,
                             xpos, zpos, i);
          }
          i++;
          if (xgrid >= TERRAIN_GRID_SIZE - 1) {
               zgrid++;
               xgrid = -1;
          }
     }
     update = 0;
}


void grassQuad(struct v3f pos, float rot, float size, char type, GLuint alpha)
{
     int h;
     float u, v, vv;
     switch (type) {
     case GRASS_GRASS1:
          h = 1;
          u = 0.0f;
          v = 0.0f;
          break;
     case GRASS_GRASS2:
          h = 1;
          u = 0.0f;
          v = 0.25f;
          break;
     case GRASS_BUSH1:
          h = 1;
          u = 0.505f;
          v = 0.0f;
          break;
     case GRASS_BUSH2:
          h = 1;
          u = 0.505f;
          v = 0.25f;
          break;
     case GRASS_BAMBOO:
          h = 2;
          u = 0.0f;
          v = 0.5f;
          break;
     case GRASS_DEAD:
          h = 1;
          u = 0.505f;
          v = 0.5f;
          break;
     case GRASS_FLOWERS:
          h = 1;
          u = 0.505f;
          v = 0.75f;
          break;
     default:
          h = 1;
          u = 0.0f;
          v = 0.0f;
     }
     if (h == 1)
          vv = 0.24f;
     else
          vv = 0.495f;
     glMateriali(GL_FRONT, GL_SHININESS, 170);
     glPushMatrix();
     glTranslatef(pos.x, pos.y, pos.z);
     glRotatef(rot, 0.0f, 1.0f, 0.0f);
     glScalef(size, size, size);
     glBegin(GL_QUADS);
     glColor4ub(255, 255, 255, alpha);
     glNormal3i(0, 0, 1);
     glTexCoord2f(u, v);
     glVertex3i(0, 0, 0);
     glTexCoord2f(u + 0.495f, v);
     glVertex3i(1, 0, 0);
     glTexCoord2f(u + 0.495f, v + vv);
     glVertex3i(1, h, 0);
     glTexCoord2f(u, v + vv);
     glVertex3i(0, h, 0);
     glEnd();
     glPopMatrix();
}


int compGrass(const void *b, const void *a)
{
     const struct grass *ga = (const struct grass *) a;
     const struct grass *gb = (const struct grass *) b;
     return (ga->d > gb->d) - (ga->d < gb->d);
}


void renderGrass(GLuint *textures, struct v3f cpos, struct v3f crot,
                 int tsize, float fps)
{
     int xgrid, zgrid, x, z, x1, z1, cull, density, i = 0, j = 0, a;
     static char update = 1;
     static float vdist = VIEW_DISTANCE_HALF * 0.3f;
     const int size = 30;
     float xpos, zpos, dist, rot, rot2, vdisthalf;
     static struct v3f sector;
     static struct v3f normal[SCENERY_SIZE_GRASS];
     static float height[SCENERY_SIZE_GRASS];
     static unsigned char type[SCENERY_SIZE_GRASS];
     GLubyte alpha;
     static struct grass grass[SCENERY_SIZE_GRASS];
     glMateriali(GL_FRONT, GL_SHININESS, 92);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE_GRASS]);
     glDisable(GL_CULL_FACE);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);
     if (distance2d(cpos, sector) > 0.5f * tsize) {
          sector = cpos;
          update = 1;
     }
     x = (int) (sector.x / size);
     z = (int) (sector.z / size);
     if (fps < 19.0f && vdist > 150.0f) {
          if (fps < 14.0f)
               vdist -= 25.0f;
          else
               vdist -= 2.5f;
     }
     else if (vdist < VIEW_DISTANCE_HALF)
          vdist++;
     vdisthalf = vdist / 2.0f;
     for (xgrid = 0, zgrid = 0;
          i < SCENERY_SIZE_GRASS && xgrid < TERRAIN_GRID_SIZE_HALF
               && zgrid < TERRAIN_GRID_SIZE_HALF;
          xgrid++) {
          xpos = (xgrid - TERRAIN_GRID_SIZE_QUARTER + x) * size;
          zpos = (zgrid - TERRAIN_GRID_SIZE_QUARTER + z) * size;
          x1 = sqrt(fabs(xpos * zpos + 83)) * 213;
          x1 = (int) (x1 + xpos) % 77;
          z1 = (int) (x1 + zpos) % 73;
          xpos += z1;
          zpos += x1;
          dist = distance3d(cpos, mv3f(xpos, height[i], zpos));
          rot  = 180 - vectorstodegree2d(cpos, mv3f(xpos, 0, zpos));
          cull = fabs((int) (crot.y - rot));
          while (cull >= 360)
               cull -= 360;
          if (cull <= 85 || cull >= 275 || crot.x > 27 || update == 1
              || dist < tsize * 2) {
               if (update) {
                    height[i] = readTerrainHeightPlane
                         (xpos, zpos, &normal[i], tsize) - 1.2f;
                    /* h2 = fabs(distance3d(mv3f(0.0f, -1.0f, 0.0f), normal[i])); */
                    /* height[i] -= h2 > 0.25f ? 1.7f : h2 > 0.4f ? 2.5f : 0; */
                    type[i] = readTerrainType(xpos, zpos);
               }
               x1 = x1 * x1 + z1 * z1;
               x1 = x1 % SCENERY_DENSITY_GRASS;
               switch (type[i]) {
               case T_TYPE_GRASS_1:
                    density = 710;
                    break;
               case T_TYPE_GRASS_2:
                    density = 747;
                    break;
               case T_TYPE_GRASS_3:
                    density = 701;
                    break;
               case T_TYPE_VILLAGE:
                    density = 880;
                    break;
               case T_TYPE_FOREST1:
                    density = 970;
                    break;
               case T_TYPE_FOREST2:
                    density = 920;
                    break;
               case T_TYPE_DIRT:
                    density = 376;
                    break;
               case T_TYPE_DESERT:
                    density = 261;
                    break;
               case T_TYPE_SNOW:
                    density = 0;
                    break;
               default:
                    density = 670;
               }
               if (dist < vdist) {
                    rot2 = height[i] * 20;
                    if (height[i] > TERRAIN_WATER_LEVEL + 50) {
                         if (dist < vdisthalf)
                              alpha = 255;
                         else if (dist < vdist)
                              alpha = (GLubyte) (255 -
                                  ((dist - vdisthalf) / vdisthalf) * 255);
                         else
                              alpha = 0;
                         if (x1 < density) {
                              grass[j].p = mv3f(xpos, height[i], zpos);
                              grass[j].r = rot2;
                              grass[j].s = 4.3f;
                              grass[j].d = dist;
                              grass[j].a = alpha;
                              if (type[i] == T_TYPE_DIRT ||
                                  type[i] == T_TYPE_DESERT)
                                   grass[j].t = GRASS_DEAD;
                              else if (type[i] == T_TYPE_GRASS_1) {
                                   a = x1 % 6;
                                   if (a < 2)
                                        grass[j].t = GRASS_GRASS1;
                                   else if (a < 4)
                                        grass[j].t = GRASS_GRASS2;
                                   else
                                        grass[j].t = GRASS_FLOWERS;
                              }
                              else
                                   grass[j].t = x1 % 8;
                              j++;
                         }
                    }
               }
          }
          i++;
          if (xgrid >= TERRAIN_GRID_SIZE_HALF - 1) {
               zgrid++;
               xgrid = -1;
          }
     }
     qsort(grass, j, sizeof(struct grass), compGrass);
     for (i = 0; i <= j; i++)
          grassQuad(grass[i].p, grass[i].r, grass[i].s,
                    grass[i].t, grass[i].a);
     update = 0;
     glEnable(GL_CULL_FACE);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
}
