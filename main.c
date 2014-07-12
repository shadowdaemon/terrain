#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeImage.h>
#include "maths.h"


static void keyInputGLFW(GLFWwindow* window, int key, int scancode, int action, int mods);


static void errorGLFW(int error, const char* err)
{
  printf("GLFW: %s\n", err);
}


static void errorFreeImage(FREE_IMAGE_FORMAT fif, const char *err)
{
  printf("FreeImage: %s\n", err);
}


void loadTexture2D(const char *file)
{
  FIBITMAP *img = FreeImage_Load(FreeImage_GetFileType(file, 0), file, 0);
  img = FreeImage_ConvertTo32Bits(img);
  GLsizei width = FreeImage_GetWidth(img);
  GLsizei height = FreeImage_GetHeight(img);
  GLubyte *bits = (GLubyte*) FreeImage_GetBits(img);
  int i;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
  for (i = 1; i < 10; i++){
    width /= 2;
    height /= 2;
    bits = (GLubyte*) FreeImage_GetBits(FreeImage_Rescale(img, width, height, FILTER_BICUBIC));
    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
    if (width == 1 || height == 1)
      break;
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i);
  FreeImage_Unload(img);
}


GLFWwindow *startGraphics(GLFWwindow *window, GLuint *textures)
{
  int lightpos[]  = {1, 0, 500, 0};
  int width, height;
  // float lightdir[]  = {0.0f, -1.0f, 0.0f};
  float lightspec[] = {0.5f, 0.5f, 0.5f, 1.0f};
  float lightamb[]  = {0.1f, 0.1f, 0.1f, 1.0f};
  float lightdiff[] = {0.8f, 0.8f, 0.8f, 1.0f};
  GLuint prog1, fragShader1, vertShader1;
  GLsizei logSize;
  GLchar log[499];

  glfwSetErrorCallback(errorGLFW);
  if (glfwInit() == GL_FALSE)
    return NULL;
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 32);
  glfwWindowHint(GLFW_ACCUM_RED_BITS, 8);
  glfwWindowHint(GLFW_ACCUM_GREEN_BITS, 8);
  glfwWindowHint(GLFW_ACCUM_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ACCUM_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  width = 1366;
  height = 768;
  window = glfwCreateWindow(width, height, "test", glfwGetPrimaryMonitor(), NULL);
  if (window == NULL)
    return NULL;
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, keyInputGLFW);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
  glfwSwapInterval(1);
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
  glMatrixMode(GL_PROJECTION);
  glFrustum(-0.54f, 0.54f, -0.4f, 0.4f, 0.8f, 50000.0f);
  glMatrixMode(GL_MODELVIEW);
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  /*glSelectBuffer(SIM_SELECT_BUFFER_SIZE, select_buf);
  glInitNames();
  glPushName(0);*/

  FreeImage_Initialise(GL_FALSE);
  FreeImage_SetOutputMessage(errorFreeImage);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(5, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  loadTexture2D("data/textures/terrain.tga");
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  loadTexture2D("data/textures/foliage.tga");
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glEnable(GL_LIGHT0);
  glLightiv(GL_LIGHT0, GL_POSITION, lightpos);
  //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 77.0f);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 10.0f);
  //glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100.0f);
  //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightdir);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);

  //printf("%s\n", glGetString(GL_VERSION));
  //printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  /*
  #include "shaders.a"
  prog1 = glCreateProgramARB();
  fragShader1 = glCreateShaderARB(GL_FRAGMENT_SHADER);
  glShaderSourceARB(fragShader1, 1, fragShaderSource1, NULL);
  glCompileShaderARB(fragShader1);
  glGetShaderInfoLogARB(fragShader1, 500, &logSize, log);
  if (logSize)
    printf("%s\n", log);
  glAttachShaderARB(prog1, fragShader1);
  vertShader1 = glCreateShaderARB(GL_VERTEX_SHADER);
  glShaderSourceARB(vertShader1, 1, vertShaderSource1, NULL);
  glCompileShaderARB(vertShader1);
  glGetShaderInfoLogARB(vertShader1, 500, &logSize, log);
  if (logSize)
    printf("%s\n", log);
  //glAttachShaderARB(prog1, vertShader1);
  glLinkProgramARB(prog1);
  glGetAttribLocationARB(prog1, "Position");
  glGetProgramInfoLogARB(prog1, 500, &logSize, log);
  if (logSize)
    printf("%s\n", log);
  const GLfloat mat[] = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};
  glUniformMatrix4fvARB(glGetUniformLocationARB(prog1, "ProjMat"), 1, GL_TRUE, mat);
  GLint param;
  glGetProgramivARB(prog1, GL_LINK_STATUS, &param);
  if (param)
    glUseProgramARB(prog1);
  */

  return window;
}


void updateFogLights(GLfloat *clear, GLfloat *ambient, float camheight, int squaresize, float *fogend)
{
  static float fogstart = 10;

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
  glClearColor(clear[0], clear[1], clear[2], clear[3]);
  fogstart -= (fogstart - squaresize * TERRAIN_GRID_SIZE * 0.35f) * 0.1f;
  *fogend -= (*fogend - squaresize * TERRAIN_GRID_SIZE * 0.45f) * 0.1f;
  glFogfv(GL_FOG_COLOR, clear);
  glFogf(GL_FOG_START, fogstart < 35000.0f ? fogstart : 35000.0f);
  glFogf(GL_FOG_END, *fogend < 40000.f ? *fogend : 40000.f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.54f, 0.54f, -0.4f, 0.4f, 0.8f, *fogend * 1.1f);
  glMatrixMode(GL_MODELVIEW);
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


void mouseLook(GLFWwindow *window, struct v3f *cameraRot)
{
  const float mouse_sensitivity = 0.1f;
  struct v2d mouse_pos;

  glfwGetCursorPos (window, &mouse_pos.x, &mouse_pos.y);
  if (cameraRot->x > 90.0f)
    glfwSetCursorPos (window, mouse_pos.x, (int) (90 / mouse_sensitivity));
  if (cameraRot->x < -90.0f)
    glfwSetCursorPos (window, mouse_pos.x, (int) (-90 / mouse_sensitivity));
  while (cameraRot->y >= 360.0f)
    cameraRot->y -= 360.0f;
  while (cameraRot->y < 0.0f)
    cameraRot->y += 360.0f;
  // mouse x, y / view x, y swapped here...
  cameraRot->y = (float) (mouse_pos.x * mouse_sensitivity);
  cameraRot->x = (float) (mouse_pos.y * mouse_sensitivity);
}


static void keyInputGLFW(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}


void keyboardInput(GLFWwindow *window, char *direction)
{
  char x = 0, z = 0;

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, 'W') == GLFW_PRESS)
    z++;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, 'S') == GLFW_PRESS)
    z--;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, 'D') == GLFW_PRESS)
    x++;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, 'A') == GLFW_PRESS)
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
  int xgrid, zgrid, x1, z1, cull;
  const int squaresize = TERRAIN_SQUARE_SIZE * 5;
  float x, z, xpos = 0.0f, zpos = 0.0f, dist;
  struct terrain temp;
  GLubyte alpha;

  x = (int) (sector.x / -squaresize);
  z = (int) (sector.y / -squaresize);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    xpos = (-TERRAIN_GRID_SIZE_HALF + xgrid + x) * squaresize;
    zpos = (-TERRAIN_GRID_SIZE_HALF + zgrid + z) * squaresize;
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
    if (temp.height > TERRAIN_WATER_LEVEL + 50 && temp.height < 3750 && (cull <= 85 || cull >= 275 || fabs(camerarot.x) > 27.0f) && dist < VIEW_DISTANCE && (x1 % 117 < 50) && temp.type != T_TYPE_DIRT) {
      if (dist < VIEW_DISTANCE_HALF)
        alpha = 255;
      else if (dist < VIEW_DISTANCE)
        alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
      else
        alpha = 0;
      drawModel(models[x1 % 4], mv3f(-xpos, temp.height, -zpos), mv3f(0, x1 % 359, 0), 5, alpha);
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
  glTranslatef(-camerapos.x, -camerapos.y, -camerapos.z);
  glRotatef((GLfloat) (-camerarot.y), 0.0f, 1.0f, 0.0f);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
  glColor3ub(122, 122, 255);
  glVertex3f(35000.0f, 3000.0f, 35000.0f);
  glVertex3f(-35000.0f, 3000.0f, 35000.0f);
  glColor3fv(clear);
  glVertex3f(-35000.0f, 3000.0f, -fogend);
  glVertex3f(35000.0f, 3000.0f, -fogend);
  glEnd();
  glPopMatrix();
}


void renderWater(struct v3f camerapos, struct v3f camerarot, int *squaresize)
{
  int xshift, zshift, xgrid, zgrid, size = *squaresize * 2;
  float xpos, zpos;

  glDisable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(-camerapos.x, 0.0f, -camerapos.z);
  glRotatef((GLfloat) (-camerarot.y), 0.0f, 1.0f, 0.0f);
  glBegin(GL_QUADS);
  glColor4ub(32, 112, 255, 170);
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


void render(GLFWwindow *window, struct model *models, GLuint *textures, int *swapb, struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *squaresize, float *fogend)
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
  renderSky(camerapos, camerarot, clear, *fogend);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  updateFogLights(clear, ambient, camheight, *squaresize, fogend);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  drawTerrain(camerapos, camerarot, sector, camheight, swapb, squaresize);
  renderWater(camerapos, camerarot, squaresize);
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
    glfwSwapBuffers(window);
  *swapb = 1;
  glfwPollEvents();
}


void movement(struct v3f *camerapos, struct v3f camerarot, char direction, float speed)
{
  float ground, temp;

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
  temp = -readTerrainHeight(-camerapos->x + TERRAIN_SQUARE_SIZE, -camerapos->z + TERRAIN_SQUARE_SIZE);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x + TERRAIN_SQUARE_SIZE, -camerapos->z - TERRAIN_SQUARE_SIZE);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x - TERRAIN_SQUARE_SIZE, -camerapos->z + TERRAIN_SQUARE_SIZE);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x - TERRAIN_SQUARE_SIZE, -camerapos->z - TERRAIN_SQUARE_SIZE);
  ground = ground < temp ? ground : temp;
  ground += -TERRAIN_SQUARE_SIZE * 0.3f;
  camerapos->y = camerapos->y > ground ? ground : camerapos->y;
  //camerapos->y = ground;
}


int main(int argc, char *argv[])
{
  GLuint textures[5];
  GLFWwindow *window = NULL;
  int swapb = 1, squaresize = 0;
  char direction;
  float camheight = 0.0f, fogend = 20.0f;
  struct v2f sector    = {0.0f, 0.0f};
  struct v3f camerarot = {0.0f, 0.0f, 0.0f};
  struct v3f camerapos = {0.0f, 0.0f, 0.0f};
  struct model *models = malloc(sizeof(struct model) * 5);

  if ((window = startGraphics(window, textures)) != NULL) {
    loadFromOBJFile("data/models/tree1.obj", &models[0]);
    loadFromOBJFile("data/models/tree2.obj", &models[1]);
    loadFromOBJFile("data/models/tree3.obj", &models[2]);
    loadFromOBJFile("data/models/tree4.obj", &models[3]);
    while (!glfwWindowShouldClose(window)) {
      camheight = cameraHeight(camerapos);
      keyboardInput(window, &direction);
      mouseLook(window, &camerarot);
      render(window, models, textures, &swapb, camerapos, camerarot, &sector, camheight, &squaresize, &fogend);
      movement(&camerapos, camerarot, direction, 100.0f);
      updateCamera(camerarot);
      glTranslatef(camerapos.x, camerapos.y, camerapos.z);
    }
    free(models);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
  }
  else
    return EXIT_FAILURE;
}
