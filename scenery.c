#include <assimp/scene.h>
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
                                            textures[TEX_BUILDING]);
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
                                       textures[TEX_BUILDING]);
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
     for (xgrid = 0, zgrid = 0, i = 0; i < SCENERY_SIZE && xgrid
               < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
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
