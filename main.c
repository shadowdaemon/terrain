#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maths.h"


int startGraphics(GLuint *textures)
{
  int check = 0;
  int lightpos[]  = {1, 0, 500, 0};
  // float lightdir[]  = {0.0f, -1.0f, 0.0f};
  float lightspec[] = {0.5f, 0.5f, 0.5f, 1.0f};
  float lightamb[]  = {0.1f, 0.1f, 0.1f, 1.0f};
  float lightdiff[] = {0.8f, 0.8f, 0.8f, 1.0f};

  glfwInit();
  check = glfwOpenWindow(1366, 768, 24, 24, 24, 8, 8, 8, GLFW_FULLSCREEN);
  glfwSetWindowTitle("Fractal Worlds");
  glEnable(GL_DEPTH_TEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glClearDepth(1.0f);
  glDepthRange(0.0f, 1.0f);
  glDepthFunc(GL_LESS);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glHint(GL_FOG_HINT, GL_NICEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
  glShadeModel(GL_SMOOTH);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glMatrixMode(GL_PROJECTION);
  glFrustum(-0.027f, 0.027f, -0.020f, 0.020f, 0.04f, 50000.0f);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, 1366, 768);
  /*glSelectBuffer(SIM_SELECT_BUFFER_SIZE, select_buf);
  glInitNames();
  glPushName(0);*/
  glGenTextures(5, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glfwLoadTexture2D("data/textures/terrain.tga", GLFW_BUILD_MIPMAPS_BIT);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glfwLoadTexture2D("data/textures/foliage.tga", GLFW_BUILD_MIPMAPS_BIT);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glfwLoadTexture2D("data/textures/font_alpha.tga", GLFW_BUILD_MIPMAPS_BIT);
  glEnable(GL_LIGHT0);
  glLightiv(GL_LIGHT0, GL_POSITION, lightpos);
  //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 77.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 10.0f);
  //glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100.0f);
  //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);

  return check;
}


void updateFogLights(GLfloat *clear, GLfloat *ambient, float camheight, int squaresize)
{
  static float fogstart = 10;
  static float fogend = 20;

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  glClearColor(clear[0], clear[1], clear[2], clear[3]);
  fogstart -= (fogstart - squaresize * TERRAIN_GRID_SIZE * 0.4f) * 0.1f;
  fogend -= (fogend - squaresize * TERRAIN_GRID_SIZE * 0.5f - 7500.0f) * 0.1f;
  glFogfv(GL_FOG_COLOR, clear);
  glFogf(GL_FOG_START, fogstart < 35000.0f ? fogstart : 35000.0f);
  glFogf(GL_FOG_END, fogend < 40000.f ? fogend : 40000.f);
}


void axisMultiplier(float *quat1_in, float *quat2_in, float *quat3_in)
{
  quat1_in[0] = quat2_in[0] * quat3_in[0] - quat2_in[0] * quat3_in[1] - quat2_in[2] * quat3_in[2] - quat2_in[3] * quat3_in[3];
  quat1_in[1] = quat2_in[0] * quat3_in[1] + quat2_in[1] * quat3_in[0] + quat2_in[2] * quat3_in[3] - quat2_in[3] * quat3_in[2];
  quat1_in[2] = quat2_in[0] * quat3_in[2] + quat2_in[2] * quat3_in[0] + quat2_in[3] * quat3_in[1] - quat2_in[1] * quat3_in[3];
  quat1_in[3] = quat2_in[0] * quat3_in[3] + quat2_in[3] * quat3_in[0] + quat2_in[1] * quat3_in[2] - quat2_in[2] * quat3_in[1];
}


void createQuats(float *quat, char axis, float degrees)
{
  float radians = ((degrees / 180.0f) * PI);
  float result = sinf(radians / 2.0f);

  quat[3] = 0.0f;
  if (axis == 'x') {
    quat[0] = cosf(radians / 2.0f);
    quat[1] = result;
    quat[2] = 0.0f;
  }
  else /*if (axis == "y")*/ {
    quat[0] = cosf(radians / 2.0f);
    quat[2] = result;
    quat[1] = 0.0f;
  }
}


void createMatrix(float *quat, float *viewmatrix)
{
  viewmatrix[0] = 1.0f - 2.0f * (quat[2] * quat[2] + quat[3] * quat[3]);
  viewmatrix[1] = 2.0f * (quat[1] * quat[2] + quat[3] * quat[0]);
  viewmatrix[2] = 2.0f * (quat[1] * quat[3] - quat[2] * quat[0]);
  viewmatrix[3] = 0.0f;
  viewmatrix[4] = 2.0f * (quat[1]* quat[2] - quat[3] * quat[0]);
  viewmatrix[5] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[3] * quat[3]);
  viewmatrix[6] = 2.0f * (quat[3] * quat[2] + quat[1] * quat[0]);
  viewmatrix[7] = 0.0f;
  viewmatrix[8] = 2.0f * (quat[1] * quat[3] + quat[2] * quat[0]);
  viewmatrix[9] = 2.0f * (quat[2]* quat[3] - quat[1] * quat[0]);
  viewmatrix[10] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[2] * quat[2]);
  viewmatrix[11] = 0.0f;
  viewmatrix[12] = 0;
  viewmatrix[13] = 0;
  viewmatrix[14] = 0;
  viewmatrix[15] = 1.0f;
}


void updateCamera(struct v3f cameraRot)
{
  static float q1[4], q2[4], q3[4];
  static float viewmatrix[16];

  glLoadIdentity();
  createQuats(q3, 'x', cameraRot.x);
  axisMultiplier(q1, q2, q3);
  q2[0] = q3[0];
  q2[1] = q3[1];
  createQuats(q3, 'y', cameraRot.y);
  axisMultiplier(q1, q2, q3);
  createMatrix(q1, viewmatrix);
  glMultMatrixf(viewmatrix);
}


float cameraHeight(struct v3f camerapos)
{
  struct v3f temp;
  static float height;

  temp.x = fabs((int) (-camerapos.y - readTerrainHeight(camerapos.x, camerapos.z)));
  if (height > temp.x)
    height -= (height - temp.x) * 0.1f; // adjust quickly if going down
  else
    height -= (height - temp.x) * 0.05f; // adjust slowly if going up

  return height;
}


void mouseLook(struct v3f *cameraRot)
{
  const float mouse_sensitivity = 0.1f;
  struct v2i mouse_pos;

  glfwGetMousePos (&mouse_pos.x, &mouse_pos.y);
  if (cameraRot->x > 90.0f)
    glfwSetMousePos (mouse_pos.x, (int) (90 / mouse_sensitivity));
  if (cameraRot->x < -90.0f)
    glfwSetMousePos (mouse_pos.x, (int) (-90 / mouse_sensitivity));
  while (cameraRot->y >= 360.0f)
    cameraRot->y -= 360.0f;
  while (cameraRot->y < 0.0f)
    cameraRot->y += 360.0f;
  // mouse x, y / view x, y swapped here...
  cameraRot->y = (float) (mouse_pos.x * mouse_sensitivity);
  cameraRot->x = (float) (mouse_pos.y * mouse_sensitivity);
}


void keyboardInput(char *direction, int *key)
{
  static char *skey = (char *) "KEY_NONE";
  char x = 0, z = 0;

  glfwPollEvents();
  *key = KEY_NONE;
  if (glfwGetKey(GLFW_KEY_ESC))
    skey = (char *) "GLFW_KEY_ESC";
  else if (glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_ESC")) {
    *key = GLFW_KEY_ESC;
    skey = (char *) "KEY_NONE";
    //engine.userEvent = TRUE;
  }
  if (glfwGetKey(GLFW_KEY_SPACE))
    skey = (char *) "GLFW_KEY_SPACE";
  else if (glfwGetKey(GLFW_KEY_SPACE) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_SPACE")) {
    *key = GLFW_KEY_SPACE;
    skey = (char *) "KEY_NONE";
  }
  if (glfwGetKey(GLFW_KEY_LCTRL))
    skey = (char *) "GLFW_KEY_LCTRL";
  else if (glfwGetKey(GLFW_KEY_LCTRL) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_LCTRL")) {
    *key = GLFW_KEY_LCTRL;
    skey = (char *) "KEY_NONE";
  }
  if (glfwGetKey(GLFW_KEY_F1))
    skey = (char *) "GLFW_KEY_F1";
  else if (glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_F1")) {
    *key = GLFW_KEY_F1;
    skey = (char *) "KEY_NONE";
  }
  if (glfwGetKey(GLFW_KEY_F2))
    skey = (char *) "GLFW_KEY_F2";
  else if (glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_F2")) {
    *key = GLFW_KEY_F2;
    skey = (char *) "KEY_NONE";
  }
  if (glfwGetKey(GLFW_KEY_F3))
    skey = (char *) "GLFW_KEY_F3";
  else if (glfwGetKey(GLFW_KEY_ESC) == GLFW_RELEASE && 0 == strcmp(skey, "GLFW_KEY_F3")) {
    *key = GLFW_KEY_F3;
    skey = (char *) "KEY_NONE";
  }
  if (glfwGetKey(GLFW_KEY_UP) || glfwGetKey('W'))
    z++;
  if (glfwGetKey(GLFW_KEY_DOWN) || glfwGetKey('S'))
    z--;
  if (glfwGetKey(GLFW_KEY_RIGHT) || glfwGetKey('D'))
    x++;
  if (glfwGetKey(GLFW_KEY_LEFT) || glfwGetKey('A'))
    x--;
  if (x == 0 && z == 0)
    *direction = INPUT_NONE;
  else if (x == 0 && z == 1)
    *direction = INPUT_UP;
  else if (x == 0 && z == -1)
    *direction = INPUT_DOWN;
  else if (x == 1 && z == 0)
    *direction = INPUT_RIGHT;
  else if (x == -1 && z == 0)
    *direction = INPUT_LEFT;
  else if (x == 1 && z == 1)
    *direction = INPUT_UP_RIGHT;
  else if (x == -1 && z == 1)
    *direction = INPUT_UP_LEFT;
  else if (x == 1 && z == -1)
    *direction = INPUT_DOWN_RIGHT;
  else if (x == -1 && z == -1)
    *direction = INPUT_DOWN_LEFT;
}


void drawFoliage(struct model *models, struct v3f camerapos, struct v3f camerarot, struct v2f sector)
{
  int x_grid, z_grid, x1, z1, cull;
  float x, z, xpos = 0.0f, zpos = 0.0f, dist;
  struct terrain temp;
  GLubyte alpha;

  x = (int) (sector.x / -TERRAIN_SQUARE_SIZE);
  z = (int) (sector.y / -TERRAIN_SQUARE_SIZE);
  for (x_grid = 0, z_grid = 0; x_grid < TERRAIN_GRID_SIZE && z_grid < TERRAIN_GRID_SIZE; x_grid++) {
    xpos = (-TERRAIN_GRID_SIZE_HALF + x_grid + x) * TERRAIN_SQUARE_SIZE;
    zpos = (-TERRAIN_GRID_SIZE_HALF + z_grid + z) * TERRAIN_SQUARE_SIZE;
    x1 = (int) xpos % 97;
    z1 = (int) zpos % 53;
    xpos += z1;
    zpos += x1;
    cull = fabs((int) (camerarot.y - vectorstodegree2d(camerapos, mv3f(-xpos, 0, -zpos))));
    while (cull >= 360)
    cull -= 360;
    temp = readTerrain(xpos, zpos);
    dist = distance2d(camerapos, mv3f(-xpos, 0.0f, -zpos));
    x1 = x1 * x1 + z1 * z1;
    if (temp.height > TERRAIN_WATER_LEVEL + 50 && temp.height < 2750 && (cull <= 85 || cull >= 275 || fabs(camerarot.x) > 27.0f) && dist < VIEW_DISTANCE && (x1 % 117 < 3) && temp.type != T_TYPE_DIRT) {
      if (dist < VIEW_DISTANCE_HALF)
        alpha = 255;
      else if (dist < VIEW_DISTANCE)
        alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
      else
        alpha = 0;
      drawModel(models[x1 % 4], mv3f(-xpos, temp.height, -zpos), mv3f(0, x1 % 359, 0), 5, alpha);
    }
    if (x_grid >= TERRAIN_GRID_SIZE - 1) {
      z_grid++;
      x_grid = -1;
    }
  }
}


void skyPlane(struct v3f camerapos, struct v3f camerarot, GLfloat *clear)
{
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_FOG);
  glTranslatef(-camerapos.x, -camerapos.y, -camerapos.z);
  glRotatef((GLfloat) (-camerarot.y), 0.0f, 1.0f, 0.0f);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  glColor3ub(122, 122, 255);
  glVertex3f(20000.0f, 3000.0f, 20000.0f);
  glVertex3f(-20000.0f, 3000.0f, 20000.0f);
  glColor3fv(clear);
  glVertex3f(-20000.0f, 3000.0f, -20000.0f);
  glVertex3f(20000.0f, 3000.0f, -20000.0f);
  glEnd();
  glPopMatrix();
}


void render(struct model *models, GLuint *textures, int *swapb, struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *squaresize)
{
  GLfloat materialColor[4];
  GLfloat clear[4]   = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat ambient[4] = {0.45f, 0.45f, 0.45f, 1.0f};

  materialColor[3] = 1.0f;
  materialColor[0] = materialColor[1] = materialColor[2] = 0.8f;
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
  glMateriali(GL_FRONT, GL_SHININESS, 37);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  skyPlane(camerapos, camerarot, clear);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  updateFogLights(clear, ambient, camheight, *squaresize);
  glEnable(GL_NORMALIZE);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  drawTerrain(camerapos, camerarot, sector, camheight, swapb, squaresize);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  //glEnableClientState(GL_TEXTURE_COORD_ARRAY); /* this does not currently work */
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  drawFoliage(models, camerapos, camerarot, *sector);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  //glDepthFunc(GL_ALWAYS);
  //glDisable(GL_LIGHTING);
  //glDisable(GL_FOG);
  //if (gui()) {
  //glBindTexture(GL_TEXTURE_2D, textures[4]);
  //engine.menus[engine.menu].drawMenu();
  //}
  if (*swapb)
    glfwSwapBuffers();
  *swapb = 1;
}


void movement(struct v3f *camerapos, struct v3f camerarot, char direction, float speed)
{
  float ground;
  switch (direction) {
  case INPUT_UP:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 0.0f, 0.0f), speed);
    break;
  case INPUT_DOWN:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 0.0f, 0.0f), -speed);
    break;
  case INPUT_LEFT:
    degreestovector3d(camerapos, mv3f(0.0f, camerarot.y, 0.0f),
                                   mv3f(0.0f, 270.0f, 0.0f), speed);
    break;
  case INPUT_RIGHT:
    degreestovector3d(camerapos, mv3f(0.0f, camerarot.y, 0.0f),
                                   mv3f(0.0f, 90.0f, 0.0f), speed);
    break;
  case INPUT_UP_RIGHT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 45.0f, 0.0f), speed);
    break;
  case INPUT_UP_LEFT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, -45.0f, 0.0f), speed);
    break;
  case INPUT_DOWN_RIGHT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, -45.0f, 0.0f), -speed);
    break;
  case INPUT_DOWN_LEFT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 45.0f, 0.0f), -speed);
    break;
  default: break;
  }
  ground = -readTerrainHeight(-camerapos->x, -camerapos->z);
  ground += -readTerrainHeight(-camerapos->x + TERRAIN_SQUARE_SIZE_HALF, -camerapos->z + TERRAIN_SQUARE_SIZE_HALF);
  ground += -readTerrainHeight(-camerapos->x + TERRAIN_SQUARE_SIZE_HALF, -camerapos->z - TERRAIN_SQUARE_SIZE_HALF);
  ground += -readTerrainHeight(-camerapos->x - TERRAIN_SQUARE_SIZE_HALF, -camerapos->z + TERRAIN_SQUARE_SIZE_HALF);
  ground += -readTerrainHeight(-camerapos->x - TERRAIN_SQUARE_SIZE_HALF, -camerapos->z - TERRAIN_SQUARE_SIZE_HALF);
  ground = -TERRAIN_SQUARE_SIZE * 0.7f + ground / 5.0f;
  //camerapos->y = camerapos->y > ground ? ground : camerapos->y;
  camerapos->y = ground;
}


int main(int argc, char *argv[])
{
  GLuint textures[5];
  int swapb = 1, key = 0, squaresize = 0;
  char direction;
  float camheight = 0.0f;
  struct v2f sector    = {0.0f, 0.0f};
  struct v3f camerarot = {0.0f, 0.0f, 0.0f};
  struct v3f camerapos = {0.0f, 0.0f, 0.0f};
  struct model *models = malloc(sizeof(struct model) * 5);

  if (startGraphics(textures)) {
    loadFromOBJFile("data/models/tree1.obj", &models[0]);
    loadFromOBJFile("data/models/tree2.obj", &models[1]);
    loadFromOBJFile("data/models/tree3.obj", &models[2]);
    loadFromOBJFile("data/models/tree4.obj", &models[3]);
    while (key != GLFW_KEY_ESC) {
      camheight = cameraHeight(camerapos);
      keyboardInput(&direction, &key);
      mouseLook(&camerarot);
      render(models, textures, &swapb, camerapos, camerarot, &sector, camheight, &squaresize);
      movement(&camerapos, camerarot, direction, 100.0f);
      updateCamera(camerarot);
      glTranslatef(camerapos.x, camerapos.y, camerapos.z);
    }
  }
  free(models);

  return 0;
}
