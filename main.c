#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeImage.h>
// #include <assimp/cimport.h>
#include <assimp/scene.h>
// #include <assimp/vector3.h>
// #include <assimp/postprocess.h>
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


void loadTexture2D(const char *file, const char contrast)
{
  FIBITMAP *img = FreeImage_Load(FreeImage_GetFileType(file, 0), file, 0);
  FIBITMAP *foo = FreeImage_Load(FreeImage_GetFileType(file, 0), file, 0);
  img = FreeImage_ConvertTo32Bits(img);
  foo = FreeImage_ConvertTo32Bits(foo);
  GLsizei width = FreeImage_GetWidth(foo);
  GLsizei height = FreeImage_GetHeight(foo);
  if (contrast == 'y') {
    FreeImage_AdjustContrast(img, -70);
    FreeImage_AdjustContrast(foo, -90);
  }
  GLubyte *bits = (GLubyte*) FreeImage_GetBits(foo);
  int i;
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
  for (i = 1; i < 10; i++){
    width /= 2;
    height /= 2;
    if (contrast == 'y')
      FreeImage_AdjustContrast(img, 7 * i);
    bits = (GLubyte*) FreeImage_GetBits(FreeImage_Rescale(img, width, height, FILTER_BICUBIC));
    glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
    if (width == 1 || height == 1)
      break;
  }
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i);
  FreeImage_Unload(img);
  FreeImage_Unload(foo);
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


void linkShader(GLuint *shader, const char *v_file, const char *f_file)
{
  GLuint fragShader, vertShader;
  GLchar *fragSrc = NULL, *vertSrc = NULL;
  long filelen;
  GLsizei logSize;
  GLchar log[5000];

  *shader = glCreateProgramARB();
  if (strlen(v_file) > 0) {
  vertShader = glCreateShaderARB(GL_VERTEX_SHADER);
  filelen = fileLength(v_file);
  vertSrc = (GLchar *) malloc(sizeof(GLchar) * filelen);
  loadGLSL(vertSrc, filelen, v_file);
  // printf("%s\n", vertSrc);
  glShaderSourceARB(vertShader, 1, (const GLchar **) &vertSrc, NULL);
  glCompileShaderARB(vertShader);
  free(vertSrc);
  glGetShaderInfoLogARB(vertShader, 500, &logSize, log);
  if (logSize)
    printf("GLSL vertex: %s\n", log);
  glAttachShaderARB(*shader, vertShader);
  }
  if (strlen(f_file)) {
  fragShader = glCreateShaderARB(GL_FRAGMENT_SHADER);
  filelen = fileLength(f_file);
  fragSrc = (GLchar *) malloc(sizeof(GLchar) * filelen);
  loadGLSL(fragSrc, filelen, f_file);
  // printf("%s\n", fragSrc);
  glShaderSourceARB(fragShader, 1, (const GLchar **) &fragSrc, NULL);
  glCompileShaderARB(fragShader);
  free(fragSrc);
  glGetShaderInfoLogARB(fragShader, 500, &logSize, log);
  if (logSize)
    printf("GLSL fragment: %s\n", log);
  glAttachShaderARB(*shader, fragShader);
  }
  glLinkProgramARB(*shader);
  glGetProgramInfoLogARB(*shader, 500, &logSize, log);
  if (logSize)
    printf("%s\n", log);
}


GLFWwindow *startGraphics(GLuint *textures, GLuint *shaders)
{
  GLFWwindow *window = NULL;
  int width, height;

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
  width = RESX;
  height = RESY;
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
  glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glHint(GL_FOG_HINT, GL_NICEST);
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glMatrixMode(GL_MODELVIEW);
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  FreeImage_Initialise(GL_FALSE);
  FreeImage_SetOutputMessage(errorFreeImage);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(5, textures);
  glActiveTextureARB(GL_TEXTURE0_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  loadTexture2D("data/textures/terrain.tga", 'y');
  glActiveTextureARB(GL_TEXTURE1_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  loadTexture2D("data/textures/foliage.tga", 'n');
  glActiveTextureARB(GL_TEXTURE2_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[2]);
  loadTexture2D("data/textures/cloud_alpha.tga", 'n');
  glActiveTextureARB(GL_TEXTURE3_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[3]);
  loadTexture2D("data/textures/fighter.png", 'n');
  glActiveTextureARB(GL_TEXTURE4_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[4]);
  glActiveTextureARB(GL_TEXTURE5_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[5]);
  loadTexture2D("data/textures/building1.png", 'n');
  glActiveTextureARB(GL_TEXTURE6_ARB);
  glBindTexture(GL_TEXTURE_2D, textures[6]);
  loadTexture2D("data/textures/font_alpha.tga", 'n');
  glActiveTextureARB(GL_TEXTURE0_ARB);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
  glUniform2fARB = (PFNGLUNIFORM2FARBPROC) glfwGetProcAddress("glUniform2fARB");
  glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) glfwGetProcAddress("glUniform2fvARB");
  glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) glfwGetProcAddress("glUniform4fvARB");
  glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) glfwGetProcAddress("glUniformMatrix4fvARB");
  glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) glfwGetProcAddress("glGetUniformLocationARB");
  glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC) glfwGetProcAddress("glGetAttribLocationARB");
  glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC) glfwGetProcAddress("glBindAttribLocationARB");
  glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC) glfwGetProcAddress("glVertexAttrib3fvARB");
  glMultiTexCoordPointerEXT = (PFNGLMULTITEXCOORDPOINTEREXTPROC) glfwGetProcAddress("glMultiTexCoordPointerEXT");

  linkShader(&shaders[0], "data/shaders/1.vsh", "data/shaders/1.fsh");
  linkShader(&shaders[1], "data/shaders/2.vsh", "data/shaders/2.fsh");

  return window;
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

  ground = readTerrainHeight(camerapos.x, camerapos.z);
  ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
  height = camerapos.y - ground;

  return height;
}


void cameraTrailMovement(struct v3f *camerapos, struct v3f *camerarot, struct v3f modelpos, struct v3f modelrot)
{
  struct v3f temppos = modelpos;
  float ground = 0.0f;

  degreestovector3d(&temppos, modelrot, mv3f(0, 180, 0), 20);
  camerapos->x += (temppos.x - camerapos->x) * 0.37f;
  camerapos->y += (temppos.y - camerapos->y) * 0.37f;
  camerapos->z += (temppos.z - camerapos->z) * 0.37f;
  camerarot->x = modelrot.x;
  //camerarot->y = vectorstodegree2d(mv3f(modelpos.x, 0, modelpos.z), *camerapos) - 180;
  camerarot->y = vectorstodegree2d(modelpos, *camerapos);
  ground = readTerrainHeightPlane(camerapos->x, camerapos->z, TERRAIN_SQUARE_SIZE, &temppos);
  ground += TERRAIN_SQUARE_SIZE * 0.07f;
  ground = ground < TERRAIN_WATER_LEVEL + 10 ? TERRAIN_WATER_LEVEL + 10 : ground;
  camerapos->y = camerapos->y < ground ? ground : camerapos->y;
}


void mouseLook(GLFWwindow *window, struct v3f *camerarot)
{
  const float mouse_sensitivity = 0.1f;
  struct v2d mouse_pos;

  glfwGetCursorPos (window, &mouse_pos.x, &mouse_pos.y);
  if (camerarot->x > 90.0f)
    glfwSetCursorPos (window, mouse_pos.x, (int) (90 / mouse_sensitivity));
  if (camerarot->x < -90.0f)
    glfwSetCursorPos (window, mouse_pos.x, (int) (-90 / mouse_sensitivity));
  while (camerarot->y >= 360.0f)
    camerarot->y -= 360.0f;
  while (camerarot->y < 0.0f)
    camerarot->y += 360.0f;
  // mouse x, y / view x, y swapped here...
  camerarot->y = (float) (mouse_pos.x * mouse_sensitivity);
  camerarot->x = (float) (mouse_pos.y * mouse_sensitivity);
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
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, ' ') == GLFW_PRESS)
    *direction += INPUT_SPACE;
}


void movement(struct v3f *camerapos, struct v3f camerarot, char direction, float speed)
{
  struct v3f normal;
  float ground;

  switch (direction) {
  case INPUT_UP:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 0.0f, 0.0f), -speed);
    break;
  case INPUT_DOWN:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 0.0f, 0.0f), speed);
    break;
  case INPUT_LEFT:
    degreestovector3d(camerapos, mv3f(0.0f, camerarot.y, 0.0f),
                                   mv3f(0.0f, 90.0f, 0.0f), speed);
    break;
  case INPUT_RIGHT:
    degreestovector3d(camerapos, mv3f(0.0f, camerarot.y, 0.0f),
                                   mv3f(0.0f, 270.0f, 0.0f), speed);
    break;
  case INPUT_UP_RIGHT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 45.0f, 0.0f), -speed);
    break;
  case INPUT_UP_LEFT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, -45.0f, 0.0f), -speed);
    break;
  case INPUT_DOWN_RIGHT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, -45.0f, 0.0f), speed);
    break;
  case INPUT_DOWN_LEFT:
    degreestovector3d(camerapos, camerarot, mv3f(0.0f, 45.0f, 0.0f), speed);
    break;
  default: break;
  }
  ground = readTerrainHeightPlane(camerapos->x, camerapos->z, TERRAIN_SQUARE_SIZE, &normal);
  ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
  ground += TERRAIN_SQUARE_SIZE * 0.02f;
  camerapos->y = ground;
}


void flyMovement(struct airunit *unit, char input)
{
  struct v3f temppos;
  float ground, speed, temp1, maxthrust, drag, lift;

  switch (input & ~INPUT_SPACE) {
  case INPUT_UP:
    unit->thrust = unit->thrust + 0.01f;
    break;
  case INPUT_DOWN:
    unit->thrust = unit->thrust - 0.01f;
    break;
  case INPUT_LEFT:
    unit->rot.z -= 5.0f;
    break;
  case INPUT_RIGHT:
    unit->rot.z += 5.0f;
    break;
  case INPUT_UP_RIGHT:
    unit->thrust = unit->thrust + 0.01f;
    unit->rot.z += 5.0f;
    break;
  case INPUT_UP_LEFT:
    unit->thrust = unit->thrust + 0.01f;
    unit->rot.z -= 5.0f;
    break;
  case INPUT_DOWN_RIGHT:
    unit->thrust = unit->thrust - 0.01f;
    unit->rot.z += 5.0f;
    break;
  case INPUT_DOWN_LEFT:
    unit->thrust = unit->thrust - 0.01f;
    unit->rot.z -= 5.0f;
    break;
  default: break;
  }
  switch (unit->type) {
  case UNIT_AIRFIGHTER:
    maxthrust = 1.0f;
    drag = 0.0015f;
    lift = 0.0171f;
  }
  ground = readTerrainHeightPlane(unit->pos.x, unit->pos.z, TERRAIN_SQUARE_SIZE, &temppos);
  ground = ground < TERRAIN_WATER_LEVEL + 7 ? TERRAIN_WATER_LEVEL + 7 : ground;
  //temp1 = 1 - fabs(unit->pos.y - 9700) / 19000.0f;
  //temp1 = temp1 < 0.0f ? 0.0f : temp1;
  temppos = unit->pos;
  if ((input | INPUT_SPACE) == input)
    unit->vec.y += 0.65f;
  unit->thrust = unit->thrust > maxthrust ? maxthrust : unit->thrust < 0 ? 0 : unit->thrust;
  degreestovector3d(&temppos, unit->rot, mv3f(180.0f, 180.0f, 0.0f), unit->thrust);
  unit->vec.x += temppos.x - unit->pos.x;
  unit->vec.y += temppos.y - unit->pos.y;
  unit->vec.z += temppos.z - unit->pos.z;
  unit->speed = distance2d(mv3f(0, 0, 0), unit->vec);
  speed = unit->speed * 0.5f;
  speed = speed < 300 ? speed : 300;
  temppos = mv3f(0, 0, 0);
  degreestovector3d(&temppos, unit->rot, mv3f(180.0f, 180.0f, 0.0f), speed * 0.7f);
  temp1 = (vectorstodegree2d(temppos, mv3f(0, 0, 0)) - vectorstodegree2d(unit->vec, mv3f(0, 0, 0))) * 0.09f;
  if (vectorstodegree2d(temppos, mv3f(0, 0, 0)) > vectorstodegree2d(unit->vec, mv3f(0, 0, 0)) + 0.8f)
    unit->rot.z += temp1 > 7 ? 0 : temp1;
  else if (vectorstodegree2d(unit->vec, mv3f(0, 0, 0)) > vectorstodegree2d(temppos, mv3f(0, 0, 0)) + 0.8f)
    unit->rot.z += temp1 < -7 ? 0 : temp1;
  unit->rot.z -= unit->rot.z * 0.12f;
  unit->rot.z = unit->rot.z > 70 ? 70 : unit->rot.z < -70 ? -70 : unit->rot.z;
  unit->pos.x += temppos.x + unit->vec.x;
  unit->pos.y += temppos.y + unit->vec.y;
  unit->pos.z += temppos.z + unit->vec.z;
  unit->vec.y += speed * lift;
  if (unit->vec.y > -WORLD_GRAVITY * 20)
    unit->vec.y -= WORLD_GRAVITY;
  temp1 = (1.5f - unit->vec.y) * 0.1f;
  temp1 = temp1 < 50 ? temp1 - 3 > 0 ? temp1 - 3 : 0 : 50;
  degreestovector3d(&unit->vec, unit->rot, mv3f(180.0f, 180.0f, 0.0f), temp1 > 0 ? temp1 : 0);
  unit->vec.x -= unit->vec.x * drag * speed;
  unit->vec.y -= unit->vec.y * drag * speed;
  unit->vec.z -= unit->vec.z * drag * speed;
  unit->height = fabs(unit->pos.y - ground);
  /*if (unit->height < 500)
    unit->vec.y += 0.2f * (500 - unit->height) / 500.0f;
  if (unit->height < 1250)
    unit->vec.y += 0.5f * (1250 - unit->height) / 1250.0f;*/
  if (unit->pos.y < ground) {
    unit->vec.y = 0.0f;
    unit->pos.y = ground;
    unit->vec.x -= unit->vec.x * 0.4f;
    unit->vec.z -= unit->vec.z * 0.4f;
    unit->thrust -= unit->thrust * 0.08f;
  }
}


/*void updateAirPositions(struct airunit *airunits)
{
  int i;
  float dist;

  for (i = 1; i < 15; i++) {
    //airunits[i].rot.y += 3 + i * 0.1f;
    if (airunits[i].height < 100)
      flyMovement(&airunits[i], INPUT_SPACE);
    else
      flyMovement(&airunits[i], INPUT_UP);
    dist = distance2d(airunits[i].pos, airunits[i-1].pos);
    if (airunits[i].height < 1200) {
      airunits[i].rot.y += (vectorstodegree2d(airunits[i].pos, airunits[i-1].pos) - airunits[i].rot.y) * 0.05f;
      if (dist < 1200) {
        airunits[i].rot.x -= airunits[i].rot.x * 0.1f;
        flyMovement(&airunits[i], INPUT_DOWN);
      }
      else {
        airunits[i].rot.x += (-27 - airunits[i].rot.x) * 0.06f;
        flyMovement(&airunits[i], INPUT_SPACE);
      }
    }
    else if (airunits[i].height < 2800) {
      if (airunits[i].speed < 20)
        airunits[i].rot.x += (-7 - airunits[i].rot.x) * 0.06f;
      else
        airunits[i].rot.x += (-23 - airunits[i].rot.x) * 0.04f;
      if (airunits[i].vec.y < -0.75f)
        flyMovement(&airunits[i], INPUT_SPACE);
      else if (dist > 5000)
        flyMovement(&airunits[i], INPUT_UP + INPUT_SPACE);
      else
        flyMovement(&airunits[i], INPUT_DOWN);
    }
    else if (dist < 2000) {
      airunits[i].rot.y += (vectorstodegree2d(airunits[i].pos, airunits[i-1].pos) - airunits[i].rot.y) * 0.05f;
      if (airunits[i].height < 4800)
        airunits[i].rot.x -= airunits[i].rot.x * 0.1f;
      else
        airunits[i].rot.x += (11 - airunits[i].rot.x) * 0.08f;
      if (airunits[i].speed < 15 && airunits[i].height < 2200)
        flyMovement(&airunits[i], INPUT_UP);
      else
        flyMovement(&airunits[i], INPUT_DOWN);
    }
    else {
      airunits[i].rot.y += (vectorstodegree2d(airunits[i].pos, airunits[i-1].pos) - airunits[i].rot.y) * 0.05f;
      if (airunits[i].speed < 50 && airunits[i].height < 3200)
        flyMovement(&airunits[i], INPUT_UP);
      else
        flyMovement(&airunits[i], INPUT_DOWN);
    }
  }
}*/


int main(int argc, char *argv[])
{
  GLuint textures[7], shaders[5];
  GLFWwindow *window = NULL;
  int swapb = 1, squaresize = 0, i;
  char direction;
  float fps = 0.0f, camheight = 0.0f, fogend = 20.0f;
  struct v2f sector    = {0.0f, 0.0f};
  struct v3f camerarot = {0.0f, 0.0f, 0.0f};
  struct v3f camerapos = {0.0f, 0.0f, 0.0f};
  struct aiScene *scene = malloc(sizeof(struct aiScene) * 12);
  struct aiScene *textquads = malloc(sizeof(struct aiScene) * 36);
  struct airunit *airunits = malloc(sizeof(struct airunit) * 16);

  if ((window = startGraphics(textures, shaders)) != NULL) {
    glfwSwapBuffers(window);
    for (i = 0; i < 15; i++) {
      airunits[i].type = UNIT_AIRFIGHTER;
      airunits[i].pos.x = (i - 5) * 50;
    }
    scene[0] = *loadModel("data/models/tree1.obj");
    scene[1] = *loadModel("data/models/tree2.obj");
    scene[2] = *loadModel("data/models/tree3.obj");
    scene[3] = *loadModel("data/models/tree4.obj");
    scene[4] = *loadModel("data/models/mtree1.obj");
    scene[5] = *loadModel("data/models/stump1.obj");
    scene[6] = *loadModel("data/models/fighter1.obj");
    scene[7] = *loadModel("data/models/house1.obj");
    //scene[8] = *loadModel("data/models/md3-rx78.pk3");
    textquads[0] = *loadTextQuad("data/models/quads/0.obj");
    textquads[1] = *loadTextQuad("data/models/quads/1.obj");
    textquads[2] = *loadTextQuad("data/models/quads/2.obj");
    textquads[3] = *loadTextQuad("data/models/quads/3.obj");
    textquads[4] = *loadTextQuad("data/models/quads/4.obj");
    textquads[5] = *loadTextQuad("data/models/quads/5.obj");
    textquads[6] = *loadTextQuad("data/models/quads/6.obj");
    textquads[7] = *loadTextQuad("data/models/quads/7.obj");
    textquads[8] = *loadTextQuad("data/models/quads/8.obj");
    textquads[9] = *loadTextQuad("data/models/quads/9.obj");
    textquads[10] = *loadTextQuad("data/models/quads/minus.obj");
    while (!glfwWindowShouldClose(window)) {
      camheight = cameraHeight(camerapos);
      keyboardInput(window, &direction);
      //mouseLook(window, &camerarot);
      mouseLook(window, &airunits[0].rot);
      render(window, scene, textquads, textures, shaders, &swapb, &camerapos, camerarot,
             &sector, camheight, &squaresize, &fogend, &fps, airunits);
      //movement(&camerapos, camerarot, direction, 10);
      flyMovement(&airunits[0], direction);
      //updateAirPositions(airunits);
      cameraTrailMovement(&camerapos, &camerarot, airunits[0].pos, airunits[0].rot);
      updateCamera(camerarot);
      glTranslatef(-camerapos.x, -camerapos.y, -camerapos.z);
    }
    free(scene);
    free(airunits);
    free(textquads);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
  }
  else
    return EXIT_FAILURE;
}
