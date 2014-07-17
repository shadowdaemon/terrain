#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeImage.h>
#include "maths.h"


#define glActiveTexureARB _ActiveTexureARB
#define glCreateProgramARB _CreateProgramARB
#define glCreateShaderARB _CreateShaderARB
#define glShaderSourceARB _ShaderSourceARB
#define glGetShaderSourceARB _GetShaderSourceARB
#define glCompileShaderARB _CompileShaderARB
#define glGetShaderInfoLogARB _GetShaderInfoLogARB
#define glGetProgramInfoLogARB _GetProgramInfoLogARB
#define glGetProgramivARB _GetProgramivARB
#define glAttachShaderARB _AttachShaderARB
#define glLinkProgramARB _LinkProgramARB
#define glUseProgramARB _UseProgramARB
#define glUniform1iARB _Uniform1iARB
#define glUniform1fARB _Uniform1fARB
#define glUniform2fvARB _Uniform2fvARB
#define glUniformMatrix4fvARB _UniformMatrix4fvARB
#define glGetUniformLocationARB _GetUniformLocationARB
#define glGetAttribLocationARB _GetAttribLocationARB
#define glBindAttribLocationARB _BindAttribLocationARB
#define glVertexAttrib3fvARB _VertexAttrib3fvARB
PFNGLACTIVETEXTUREARBPROC _ActiveTexureARB;
PFNGLCREATEPROGRAMPROC _CreateProgramARB;
PFNGLCREATESHADERPROC _CreateShaderARB;
PFNGLSHADERSOURCEARBPROC _ShaderSourceARB;
PFNGLGETSHADERSOURCEARBPROC _GetShaderSourceARB;
PFNGLCOMPILESHADERARBPROC _CompileShaderARB;
PFNGLGETSHADERINFOLOGPROC _GetShaderInfoLogARB;
PFNGLGETPROGRAMINFOLOGPROC _GetProgramInfoLogARB;
PFNGLGETPROGRAMIVARBPROC _GetProgramivARB;
PFNGLATTACHSHADERPROC _AttachShaderARB;
PFNGLLINKPROGRAMARBPROC _LinkProgramARB;
PFNGLUSEPROGRAMPROC _UseProgramARB;
PFNGLUNIFORM1IARBPROC _Uniform1iARB;
PFNGLUNIFORM1FARBPROC _Uniform1fARB;
PFNGLUNIFORM2FVARBPROC _Uniform2fvARB;
PFNGLUNIFORMMATRIX4FVARBPROC _UniformMatrix4fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC _GetUniformLocationARB;
PFNGLGETATTRIBLOCATIONARBPROC _GetAttribLocationARB;
PFNGLBINDATTRIBLOCATIONARBPROC _BindAttribLocationARB;
PFNGLVERTEXATTRIB3FVARBPROC _VertexAttrib3fvARB;


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


long fileLength(const char *file)
{
  FILE *fp = NULL;
  long len = 0;

  if ((fp = fopen(file, "r"))) {
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fclose(fp);
    return len;
  }
  return len;
}


void loadGLSL(GLchar *src, long len, const char *file)
{
  FILE *fp = NULL;

  if ((fp = fopen(file, "r"))) {
    fread(src, 1, len, fp);
    src[len++] = '\0';
    fclose(fp);
  }
}


GLFWwindow *startGraphics(GLFWwindow *window, GLuint *textures, GLuint *shaders)
{
  int lightpos[]  = {1, 0, 500, 0};
  int width, height;
  // float lightdir[]  = {0.0f, -1.0f, 0.0f};
  float lightspec[] = {0.15f, 0.18f, 0.17f, 1.0f};
  float lightamb[]  = {0.1f, 0.07f, 0.08f, 1.0f};
  float lightdiff[] = {0.82f, 0.77f, 0.75f, 1.0f};
  GLuint fragShader, vertShader;
  GLchar *fragSrc = NULL, *vertSrc = NULL;
  long filelen;
  GLsizei logSize;
  GLchar log[5000];

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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
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
  glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_PROJECTION);
  glFrustum(-0.54f, 0.54f, -0.4f, 0.4f, 0.8f, 50000.0f);
  glMatrixMode(GL_MODELVIEW);
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  FreeImage_Initialise(GL_FALSE);
  FreeImage_SetOutputMessage(errorFreeImage);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(5, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  loadTexture2D("data/textures/terrain.tga");
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  loadTexture2D("data/textures/foliage.tga");
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  loadTexture2D("data/textures/cloud_alpha.tga");
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
  glEnable(GL_LIGHT1);
  lightpos[1]  = 5000;
  glLightiv(GL_LIGHT1, GL_POSITION, lightpos);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 10.0f);
  lightspec[0] = 0.12f;
  lightspec[1] = 0.1f;
  lightspec[2] = 0.11f;
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightspec);
  lightamb[0] = 0.0f;
  lightamb[1] = 0.0f;
  lightamb[2] = 0.0f;
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightamb);
  lightdiff[0] = 0.15f;
  lightdiff[1] = 0.17f;
  lightdiff[2] = 0.19f;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightdiff);

  printf("%s\n", glGetString(GL_VERSION));
  printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glActiveTexureARB = (PFNGLACTIVETEXTUREARBPROC) glfwGetProcAddress("glActiveTexureARB");
  glCreateProgramARB = (PFNGLCREATEPROGRAMPROC) glfwGetProcAddress("glCreateProgram");
  glCreateShaderARB = (PFNGLCREATESHADERPROC) glfwGetProcAddress("glCreateShader");
  glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) glfwGetProcAddress("glShaderSourceARB");
  glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC) glfwGetProcAddress("glGetShaderSourceARB");
  glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) glfwGetProcAddress("glCompileShaderARB");
  glGetShaderInfoLogARB = (PFNGLGETSHADERINFOLOGPROC) glfwGetProcAddress("glGetShaderInfoLog");
  glGetProgramInfoLogARB = (PFNGLGETPROGRAMINFOLOGPROC) glfwGetProcAddress("glGetProgramInfoLog");
  glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC) glfwGetProcAddress("glGetProgramivARB");
  glAttachShaderARB = (PFNGLATTACHSHADERPROC) glfwGetProcAddress("glAttachShader");
  glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) glfwGetProcAddress("glLinkProgramARB");
  glUseProgramARB = (PFNGLUSEPROGRAMPROC) glfwGetProcAddress("glUseProgram");
  glUniform1iARB = (PFNGLUNIFORM1IARBPROC) glfwGetProcAddress("glUniform1iARB");
  glUniform1fARB = (PFNGLUNIFORM1FARBPROC) glfwGetProcAddress("glUniform1fARB");
  glUniform2fvARB = (PFNGLUNIFORM3FVARBPROC) glfwGetProcAddress("glUniform1fARB");
  glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) glfwGetProcAddress("glUniformMatrix4fvARB");
  glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) glfwGetProcAddress("glGetUniformLocationARB");
  glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC) glfwGetProcAddress("glGetAttribLocationARB");
  glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC) glfwGetProcAddress("glBindAttribLocationARB");
  glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC) glfwGetProcAddress("glVertexAttrib3fvARB");

  shaders[0] = glCreateProgramARB();
  vertShader = glCreateShaderARB(GL_VERTEX_SHADER);
  filelen = fileLength("data/shaders/1.vsh");
  vertSrc = (GLchar *) malloc(sizeof(GLchar) * filelen);
  loadGLSL(vertSrc, filelen, "data/shaders/1.vsh");
  printf("%s\n", vertSrc);
  glShaderSourceARB(vertShader, 1, (const GLchar **) &vertSrc, NULL);
  free(vertSrc);
  glCompileShaderARB(vertShader);
  glGetShaderInfoLogARB(vertShader, 500, &logSize, log);
  if (logSize)
    printf("GLSL vertex: %s\n", log);
  glAttachShaderARB(shaders[0], vertShader);
  fragShader = glCreateShaderARB(GL_FRAGMENT_SHADER);
  filelen = fileLength("data/shaders/2.fsh");
  fragSrc = (GLchar *) malloc(sizeof(GLchar) * filelen);
  loadGLSL(fragSrc, filelen, "data/shaders/2.fsh");
  printf("%s\n", fragSrc);
  glShaderSourceARB(fragShader, 1, (const GLchar **) &fragSrc, NULL);
  free(fragSrc);
  glCompileShaderARB(fragShader);
  glGetShaderInfoLogARB(fragShader, 500, &logSize, log);
  if (logSize)
    printf("GLSL fragment: %s\n", log);
  glAttachShaderARB(shaders[0], fragShader);
  glLinkProgramARB(shaders[0]);
  glGetProgramInfoLogARB(shaders[0], 500, &logSize, log);
  if (logSize)
    printf("%s\n", log);

  return window;
}


void updateFogLights(GLfloat *clear, GLfloat *ambient, float camheight, int squaresize, float *fogend)
{
  static float fogstart = 10.0f;
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
  temp = *fogend * 0.35f;
  if (fogstart > temp)
    fogstart -= (fogstart - temp) * 0.1f;
  else
    fogstart -= (fogstart - temp) * 0.02f;
  fogstart = fogstart > 37000 ? 37000 : fogstart;
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
  lightdiff[0] = 0.25f + sinf(camheight*0.00057f) * 0.15f;
  lightdiff[1] = 0.17f;
  lightdiff[2] = 0.19f;
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightdiff);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.4f, 5.4f, -4.0f, 4.0f, 8.0f, *fogend * 1.1f);
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
  static float height;
  float ground;

  ground = fabs((int) (-camerapos.y - readTerrainHeight(-camerapos.x, -camerapos.z)));
  ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
  height -= (height - ground) * 0.11f;

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


void drawFoliage(struct model *models, struct v3f camerapos, struct v3f camerarot, struct v2f sector, float camheight)
{
  int xgrid, zgrid, x1, z1, cull;
  int squaresize = TERRAIN_SQUARE_SIZE * 0.8f;
  float x, z, xpos = 0.0f, zpos = 0.0f, dist;
  struct terrain temp;
  GLubyte alpha;

  glMateriali(GL_FRONT, GL_SHININESS, 37);
  x = (int) (sector.x / -squaresize);
  z = (int) (sector.y / -squaresize);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE && zgrid < TERRAIN_GRID_SIZE; xgrid++) {
    xpos = (-TERRAIN_GRID_SIZE_HALF + xgrid + x) * squaresize;
    zpos = (-TERRAIN_GRID_SIZE_HALF + zgrid + z) * squaresize;
    x1 = (int) xpos % 297;
    z1 = (int) zpos % 153;
    xpos += z1;
    zpos += x1;
    cull = fabs((int) (camerarot.y - vectorstodegree2d(camerapos, mv3f(-xpos, 0, -zpos))));
    while (cull >= 360)
      cull -= 360;
    temp = readTerrain(xpos, zpos);
    dist = distance3d(camerapos, mv3f(-xpos, -camheight, -zpos));
    x1 = x1 * x1 + z1 * z1;
    if (temp.height > TERRAIN_WATER_LEVEL + 50 && temp.height < 3750 && (cull <= 85 || cull >= 275 || fabs(camerarot.x) > 27.0f) && dist < VIEW_DISTANCE && (x1 % 1176 < 137) && temp.type != T_TYPE_DIRT) {
      if (dist < VIEW_DISTANCE_HALF)
        alpha = 255;
      else if (dist < VIEW_DISTANCE)
        alpha = (GLubyte) (255 - ((dist - VIEW_DISTANCE_HALF) / (float) VIEW_DISTANCE_HALF) * 255);
      else
        alpha = 0;
      drawModel(models[x1 % 4], mv3f(-xpos, temp.height, -zpos), mv3f(0, x1 % 359, 0), 3, alpha);
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
  glColor3fv(clear);
  glVertex3f(150000.0f, 3000.0f, 150000.0f);
  glVertex3f(-150000.0f, 3000.0f, 150000.0f);
  glColor3ub(117, 132, 215);
  glVertex3f(-150000.0f, 3000.0f, -fogend);
  glVertex3f(150000.0f, 3000.0f, -fogend);
  glVertex3f(150000.0f, 3000.0f, -fogend);
  glVertex3f(-150000.0f, 3000.0f, -fogend);
  glColor3fv(clear);
  glVertex3f(-150000.0f, -5000.0f, -fogend);
  glVertex3f(150000.0f, -5000.0f, -fogend);
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
  glTranslatef(-camerapos.x, 0.0f, -camerapos.z);
  glRotatef((GLfloat) (-camerarot.y), 0.0f, 1.0f, 0.0f);
  glBegin(GL_QUADS);
  glColor4ub(32, 112, 255, 110);
  glNormal3i(0, -1, 0);
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
  int xshift, zshift, xgrid, zgrid, size = *squaresize * 8;
  float xpos, zpos, height = 11500.0f, scale = 0.00005f;

  glMateriali(GL_FRONT, GL_SHININESS, 167);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  glTranslatef(-camerapos.x, 0.0f, -camerapos.z);
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glTranslatef(-camerapos.x*scale, 0.0f, -camerapos.z*scale);
  glScalef(scale, scale, scale);
  glBegin(GL_QUADS);
  glColor4ub(128, 128, 128, 60);
  glNormal3i(0, -1, 0);
  for (xgrid = 0, zgrid = 0; xgrid < TERRAIN_GRID_SIZE_HALF && zgrid < TERRAIN_GRID_SIZE_HALF; xgrid++) {
    xshift = zshift = size;
    xpos = (-TERRAIN_GRID_SIZE_QUARTER + xgrid) * xshift;
    zpos = (-TERRAIN_GRID_SIZE_QUARTER + zgrid) * zshift;
    xshift = zshift = size / 2;
    glTexCoord2i(xpos + xshift, zpos + zshift);
    glVertex3f(xpos + xshift, height, zpos + zshift);
    glTexCoord2i(xpos - xshift, zpos + zshift);
    glVertex3f(xpos - xshift, height, zpos + zshift);
    glTexCoord2i(xpos - xshift, zpos - zshift);
    glVertex3f(xpos - xshift, height, zpos - zshift);
    glTexCoord2i(xpos + xshift, zpos - zshift);
    glVertex3f(xpos + xshift, height, zpos - zshift);
    if (xgrid >= TERRAIN_GRID_SIZE_HALF - 1) {
      zgrid++;
      xgrid = -1;
    }
  }
  glEnd();
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  //glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
}


void render(GLFWwindow *window, struct model *models, GLuint *textures, GLuint *shaders, int *swapb, struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *squaresize, float *fogend)
{
  GLfloat materialColor[4];
  GLfloat clear[4]   = {0.5f, 0.5f, 0.5f, 1.0f};
  GLfloat ambient[4] = {0.49f, 0.45f, 0.47f, 1.0f};

  materialColor[3] = 1.0f;
  materialColor[0] = materialColor[1] = materialColor[2] = 0.8f;
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialColor);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glUseProgramARB(0);
  renderSky(camerapos, camerarot, clear, *fogend);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_FOG);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  //glUseProgramARB(shaders[0]);
  //glActiveTextureARB(GL_TEXTURE0_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  //glUniform1iARB(glGetUniformLocationARB(shaders[0], "scene"), 0);
  drawTerrain(camerapos, camerarot, sector, camheight, swapb, squaresize);
  renderWater(camerapos, camerarot, squaresize);
  updateFogLights(clear, ambient, camheight, *squaresize, fogend);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  //glEnableClientState(GL_TEXTURE_COORD_ARRAY); /* this does not currently work */
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  drawFoliage(models, camerapos, camerarot, *sector, camheight);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  renderCloud(camerapos, camerarot, squaresize);
  if (*swapb)
    glfwSwapBuffers(window);
  *swapb = 1;
  glfwPollEvents();
}


void movement(struct v3f *camerapos, struct v3f camerarot, char direction, float speed)
{
  float ground, temp;
  const int offset = TERRAIN_SQUARE_SIZE / 4;

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
  temp = -readTerrainHeight(-camerapos->x + offset, -camerapos->z + offset);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x + offset, -camerapos->z - offset);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x - offset, -camerapos->z + offset);
  ground = ground < temp ? ground : temp;
  temp = -readTerrainHeight(-camerapos->x - offset, -camerapos->z - offset);
  ground = ground < temp ? ground : temp;
  ground += -TERRAIN_SQUARE_SIZE * 0.02f;
  ground = ground > TERRAIN_WATER_LEVEL - 70 ? TERRAIN_WATER_LEVEL - 70 : ground;
  camerapos->y = camerapos->y > ground ? ground : camerapos->y;
  //camerapos->y = ground;
}


int main(int argc, char *argv[])
{
  GLuint textures[5], shaders[5];
  GLFWwindow *window = NULL;
  int swapb = 1, squaresize = 0;
  char direction;
  float camheight = 0.0f, fogend = 20.0f, speed = 0.0f;
  struct v2f sector    = {0.0f, 0.0f};
  struct v3f camerarot = {0.0f, 0.0f, 0.0f};
  struct v3f camerapos = {0.0f, 0.0f, 0.0f};
  struct model *models = malloc(sizeof(struct model) * 5);

  if ((window = startGraphics(window, textures, shaders)) != NULL) {
    glfwSwapBuffers(window);
    loadFromOBJFile("data/models/tree1.obj", &models[0]);
    loadFromOBJFile("data/models/tree2.obj", &models[1]);
    loadFromOBJFile("data/models/tree3.obj", &models[2]);
    loadFromOBJFile("data/models/tree4.obj", &models[3]);
    while (!glfwWindowShouldClose(window)) {
      camheight = cameraHeight(camerapos);
      keyboardInput(window, &direction);
      mouseLook(window, &camerarot);
      render(window, models, textures, shaders, &swapb, camerapos, camerarot, &sector, camheight, &squaresize, &fogend);
      speed = 150;
      movement(&camerapos, camerarot, direction, speed);
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
