#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeImage.h>
#include <assimp/scene.h>
#include "maths.h"


float pgrad[PERLIN_SIZE][PERLIN_SIZE][2];

static void keyInputGLFW(GLFWwindow* window, int key,
                         int scancode, int action, int mods);


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
     int i;
     FIBITMAP *img = FreeImage_Load(FreeImage_GetFileType(file, 0), file, 0);
     img = FreeImage_ConvertTo32Bits(img);
     GLsizei width = FreeImage_GetWidth(img);
     GLsizei height = FreeImage_GetHeight(img);
     GLubyte *bits = (GLubyte*) FreeImage_GetBits(img);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                  GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
     /* Set up mipmapping. */
     for (i = 1; i < 10; i++) {
          width /= 2;
          height /= 2;
          bits = (GLubyte*) FreeImage_GetBits
               (FreeImage_Rescale(img, width, height, FILTER_BICUBIC));
          glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, width, height,
                       0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
          if (width == 1 || height == 1)
               break;
     }
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i);
     FreeImage_Unload(img);
}


void createPerlinTexture(int size, int tex, GLubyte *bits)
{
     typedef struct {
          GLubyte r;
          GLubyte g;
          GLubyte b;
          GLubyte a;
     } col;
     int i, k, x, y, x1, y1;
     float r, s, t;
     col *b = (col *) malloc(sizeof(col) * size * size);
     for(x = 0; x < size; x++)
     {
          for(y = 0; y < size; y++)
          {
               k = x * size + y;
               /* Need to provide non-integer values. */
               x1 = x * 0.97f + 1.12f;
               y1 = y * 0.98f + 0.95f;
               switch (tex) {
               case TEX_TERRAIN_1:
                    /* Large scale terrain texture. */
                    r = 121 + 41 * fabs(perlin(x1, y1, 0.3f));
                    s = 25 * fabs(perlin(x1 + 1, y1 + 1, 0.3f));
                    t = 36 * fabs(perlin(x1 + 1, y1 - 1, 0.3f));
                    r = r + s - t;
                    b[k].r = r + 3 * s;
                    b[k].g = r + t;
                    b[k].b = r;
                    b[k].a = 255;
                    break;
               case TEX_TERRAIN_2:
                    /* Creates color neutral texture with depth. */
                    /* This is used for terrain detail. */
                    r = 123 + 37 * fabs(perlin(x1, y1, 0.3f));
                    s = 31 * fabs(perlin(x1 + 1, y1 + 1, 0.3f));
                    t = 23 * fabs(perlin(x1 + 1, y1 - 1, 0.3f));
                    r = r + s - t;
                    b[k].r = r;
                    b[k].g = r;
                    b[k].b = r;
                    b[k].a = 255;
                    break;
               case TEX_CLOUD:
                    /* Color neutral with alpha for cloud and water. */
                    r = 123 + 37 * fabs(perlin(x1, y1, 0.3f));
                    s = 23 * fabs(perlin(x1, y1, 0.3f));
                    t = 17 * fabs(perlin(x1, y1, 0.3f));
                    r = r + s - t;
                    b[k].r = r;
                    b[k].g = r;
                    b[k].b = r;
                    b[k].a = r;
                    break;
               default:
                    /* Something went wrong. */
                    b[k].r = 0;
                    b[k].g = 0;
                    b[k].b = 0;
                    b[k].a = 255;
                    break;
               }
          }
     }
     FIBITMAP *img = FreeImage_ConvertFromRawBits
          ((BYTE*) b, size, size, size, 32, FI_RGBA_RED_MASK,
           FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
     /* Mipmaps. */
     for (i = 0; i < 10; i++){
          bits = (GLubyte *) FreeImage_GetBits
            (FreeImage_Rescale(img, size, size, FILTER_BICUBIC));
          glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, size, size,
                       0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
          size /= 2;
          if (size == 1)
               break;
     }
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, i);
     FreeImage_Unload(img);
     free(b);
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
     GLubyte *bits = NULL;
     /* Set up GLFW. */
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
     window = glfwCreateWindow(width, height, "terrain",
                               glfwGetPrimaryMonitor(), NULL);
     if (window == NULL)
          return NULL;
     glfwMakeContextCurrent(window);
     /* Clear the screen immediately. */
     glClear(GL_COLOR_BUFFER_BIT |
             GL_DEPTH_BUFFER_BIT |
             GL_STENCIL_BUFFER_BIT);
     glfwSetKeyCallback(window, keyInputGLFW);
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
     glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
     glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
     glfwSwapInterval(1);
     /* Set up OpenGL state. */
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
     glMatrixMode(GL_MODELVIEW);
     glfwGetFramebufferSize(window, &width, &height);
     glViewport(0, 0, width, height);
     glEnableClientState(GL_VERTEX_ARRAY);
     glEnableClientState(GL_NORMAL_ARRAY);
     glEnableClientState(GL_TEXTURE_COORD_ARRAY);
     /* Set up FreeImage and textures. */
     FreeImage_Initialise(GL_FALSE);
     FreeImage_SetOutputMessage(errorFreeImage);
     bits = (GLubyte *) malloc(sizeof(GLubyte) * 4 * 4096);
     glEnable(GL_TEXTURE_2D);
     glGenTextures(10, textures);
     /* Render to texture. */
     glActiveTextureARB(GL_TEXTURE4_ARB);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_RENDER]);
     /* Terrain textures. */
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN_1]);
     createPerlinTexture(PERLIN_SIZE, TEX_TERRAIN_1, bits);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
     glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
     glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_TERRAIN_2]);
     createPerlinTexture(PERLIN_SIZE, TEX_TERRAIN_2, bits);
     /* Other textures. */
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE]);
     loadTexture2D("data/textures/foliage.tga");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_CLOUD]);
     createPerlinTexture(PERLIN_SIZE, TEX_CLOUD, bits);
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FONT]);
     loadTexture2D("data/textures/font_alpha.tga");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_BUILDING]);
     loadTexture2D("data/textures/building1.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE_GRASS]);
     loadTexture2D("data/textures/foliage_grass.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_AIR_FIGHTER_1]);
     loadTexture2D("data/textures/fighter.png");
     /* Six texture functions may be specified:
        GL_ADD, GL_MODULATE, GL_DECAL, GL_BLEND,
        GL_REPLACE, or GL_COMBINE. */
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     free(bits);
     /* Some debug info. */
     printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
     printf("GL_SHADING_LANGUAGE_VERSION: %s\n",
            glGetString(GL_SHADING_LANGUAGE_VERSION));
     int maxt = 0;
     glGetIntegerv(GL_MAX_TEXTURE_COORDS, &maxt);
     printf("GL_MAX_TEXTURE_COORDS: %d\n", maxt);
     /* Set up ARB function pointers. */
     /* glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
        glfwGetProcAddress("glActiveTextureARB"); */
     glCreateProgramARB = (PFNGLCREATEPROGRAMPROC)
          glfwGetProcAddress("glCreateProgram");
     glCreateShaderARB = (PFNGLCREATESHADERPROC)
          glfwGetProcAddress("glCreateShader");
     glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)
          glfwGetProcAddress("glShaderSourceARB");
     glGetShaderSourceARB = (PFNGLGETSHADERSOURCEARBPROC)
          glfwGetProcAddress("glGetShaderSourceARB");
     glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)
          glfwGetProcAddress("glCompileShaderARB");
     glGetShaderInfoLogARB = (PFNGLGETSHADERINFOLOGPROC)
          glfwGetProcAddress("glGetShaderInfoLog");
     glGetProgramInfoLogARB = (PFNGLGETPROGRAMINFOLOGPROC)
          glfwGetProcAddress("glGetProgramInfoLog");
     glGetProgramivARB = (PFNGLGETPROGRAMIVARBPROC)
          glfwGetProcAddress("glGetProgramivARB");
     glAttachShaderARB = (PFNGLATTACHSHADERPROC)
          glfwGetProcAddress("glAttachShader");
     glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)
          glfwGetProcAddress("glLinkProgramARB");
     glUseProgramARB = (PFNGLUSEPROGRAMPROC)
          glfwGetProcAddress("glUseProgram");
     glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
          glfwGetProcAddress("glUniform1iARB");
     glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
          glfwGetProcAddress("glUniform1fARB");
     glUniform2fARB = (PFNGLUNIFORM2FARBPROC)
          glfwGetProcAddress("glUniform2fARB");
     glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)
          glfwGetProcAddress("glUniform2fvARB");
     glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)
          glfwGetProcAddress("glUniform4fvARB");
     glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)
          glfwGetProcAddress("glUniformMatrix4fvARB");
     glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)
          glfwGetProcAddress("glGetUniformLocationARB");
     glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)
          glfwGetProcAddress("glGetAttribLocationARB");
     glBindAttribLocationARB = (PFNGLBINDATTRIBLOCATIONARBPROC)
          glfwGetProcAddress("glBindAttribLocationARB");
     glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARBPROC)
          glfwGetProcAddress("glVertexAttrib3fARB");
     glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARBPROC)
          glfwGetProcAddress("glVertexAttrib3fvARB");
     glMultiTexCoordPointerEXT = (PFNGLMULTITEXCOORDPOINTEREXTPROC)
          glfwGetProcAddress("glMultiTexCoordPointerEXT");
     /* Compile shaders. */
     linkShader(&shaders[0], "data/shaders/main.vsh",
                "data/shaders/main.fsh");
     linkShader(&shaders[2], "data/shaders/sprite.vsh",
                "data/shaders/sprite.fsh");
     linkShader(&shaders[4], "data/shaders/sun.vsh",
                "data/shaders/sun.fsh");
     return window;
}


void axisMultiplier(float *quat1, float *quat2, float *quat3)
{
     quat1[0] = quat2[0] * quat3[0] - quat2[0] * quat3[1]
          - quat2[2] * quat3[2] - quat2[3] * quat3[3];
     quat1[1] = quat2[0] * quat3[1] + quat2[1] * quat3[0]
          + quat2[2] * quat3[3] - quat2[3] * quat3[2];
     quat1[2] = quat2[0] * quat3[2] + quat2[2] * quat3[0]
          + quat2[3] * quat3[1] - quat2[1] * quat3[3];
     quat1[3] = quat2[0] * quat3[3] + quat2[3] * quat3[0]
          + quat2[1] * quat3[2] - quat2[2] * quat3[1];
}


void createQuats(float *quat, char axis, float degrees)
{
     float radians = (degrees / 180.0f) * PI;
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


void createMatrix(float *quat, float *vmat)
{
     vmat[0] = 1.0f - 2.0f * (quat[2] * quat[2] + quat[3] * quat[3]);
     vmat[1] = 2.0f * (quat[1] * quat[2] + quat[3] * quat[0]);
     vmat[2] = 2.0f * (quat[1] * quat[3] - quat[2] * quat[0]);
     vmat[3] = 0.0f;
     vmat[4] = 2.0f * (quat[1]* quat[2] - quat[3] * quat[0]);
     vmat[5] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[3] * quat[3]);
     vmat[6] = 2.0f * (quat[3] * quat[2] + quat[1] * quat[0]);
     vmat[7] = 0.0f;
     vmat[8] = 2.0f * (quat[1] * quat[3] + quat[2] * quat[0]);
     vmat[9] = 2.0f * (quat[2]* quat[3] - quat[1] * quat[0]);
     vmat[10] = 1.0f - 2.0f * (quat[1] * quat[1] + quat[2] * quat[2]);
     vmat[11] = 0.0f;
     vmat[12] = 0;
     vmat[13] = 0;
     vmat[14] = 0;
     vmat[15] = 1.0f;
}


void updateCamera(struct v3f crot)
{
     static float q1[4], q2[4], q3[4];
     static float vmat[16];
     glLoadIdentity();
     createQuats(q3, 'x', crot.x);
     axisMultiplier(q1, q2, q3);
     q2[0] = q3[0];
     q2[1] = q3[1];
     createQuats(q3, 'y', crot.y);
     axisMultiplier(q1, q2, q3);
     createMatrix(q1, vmat);
     glMultMatrixf(vmat);
}


void cameraTrailMovement(struct v3f *cpos, struct v3f *crot,
                         struct airunit unit, int tsize)
{
     struct v3f temppos = unit.pos;
     float temp = 0.0f;
     if (unit.speed > 80.0f) {
          temp = 0.37f + (unit.speed - 80.0f) * 0.01f;
          temp = temp > 1.0f ? 1.0f : temp;
     }
     else
          temp = 0.37f;
     degreestovector3d(&temppos, unit.rot, mv3f(0, 180, 0), 25.0f);
     cpos->x -= (cpos->x - temppos.x) * temp;
     cpos->y -= (cpos->y - temppos.y) * temp;
     cpos->z -= (cpos->z - temppos.z) * temp;
     crot->x = unit.rot.x;
     crot->y = vectorstodegree2d(unit.pos, *cpos);
     temp = readTerrainHeightPlane(cpos->x, cpos->z, &temppos, tsize);
     temp += 5.0f;
     temp = temp < TERRAIN_WATER_LEVEL + 10 ?
          TERRAIN_WATER_LEVEL + 10 : temp;
     cpos->y = cpos->y < temp ? temp : cpos->y;
}


void mouseLook(GLFWwindow *window, struct v3f *crot)
{
     const float sens = 0.1f; /* Mouse sensitivity. */
     struct v2d pos;          /* Mouse position. */
     glfwGetCursorPos (window, &pos.x, &pos.y);
     if (crot->x > 90.0f)
          glfwSetCursorPos (window, pos.x,
                            (int) (90 / sens));
     if (crot->x < -90.0f)
          glfwSetCursorPos (window, pos.x,
                            (int) (-90 / sens));
     while (crot->y >= 360.0f)
          crot->y -= 360.0f;
     while (crot->y < 0.0f)
          crot->y += 360.0f;
     /* Mouse x, y and view x, y swapped here. */
     crot->y = pos.x * sens;
     crot->x = pos.y * sens;
}


static void keyInputGLFW(GLFWwindow *window, int key,
                         int scancode, int action, int mods)
{
     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
          glfwSetWindowShouldClose(window, GL_TRUE);
}


void keyboardInput(GLFWwindow *window, char *direction)
{
     char x = 0, z = 0;
     if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS
         || glfwGetKey(window, 'W') == GLFW_PRESS)
          z++;
     if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS
         || glfwGetKey(window, 'S') == GLFW_PRESS)
          z--;
     if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS
         || glfwGetKey(window, 'D') == GLFW_PRESS)
          x++;
     if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS
         || glfwGetKey(window, 'A') == GLFW_PRESS)
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
     if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
          *direction += INPUT_LEFT_SHIFT;
     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS
         || glfwGetKey(window, ' ') == GLFW_PRESS)
          *direction += INPUT_SPACE;
}


void movement(struct v3f *cpos, struct v3f crot, char direction,
              float speed, int tsize)
{
     struct v3f normal, pos;
     float ground, temp, dir = 0.0f, moar;
     char a = 0;
     pos = *cpos;
     if ((direction | INPUT_LEFT_SHIFT) == direction)
          speed *= 2.3f;
     switch (direction & ~(INPUT_LEFT_SHIFT | INPUT_SPACE)) {
     case INPUT_UP:
          dir = 0.0f;
          a = -1;
          break;
     case INPUT_DOWN:
          dir = 0.0f;
          a = 1;
          break;
     case INPUT_LEFT:
          dir = 90.0f;
          a = 1;
          break;
     case INPUT_RIGHT:
          dir = 270.0f;
          a = 1;
          break;
     case INPUT_UP_RIGHT:
          dir = 45.0f;
          a = -1;
          break;
     case INPUT_UP_LEFT:
          dir = -45.0f;
          a = -1;
          break;
     case INPUT_DOWN_RIGHT:
          dir = -45.0f;
          a = 1;
          break;
     case INPUT_DOWN_LEFT:
          dir = 45.0f;
          a = 1;
          break;
     default: break;
     }
     degreestovector3d(&pos, crot, mv3f(0.0f, dir, 0.0f), a * speed);
     temp   = readTerrainHeightPlane(pos.x, pos.z, &normal, tsize);
     ground = readTerrainHeightPlane(cpos->x, cpos->z, &normal, tsize);
     if (temp > ground + 1.3f)
          speed = 0.041f;
     else if (temp > ground)
          speed *= 1 - ((temp - ground) / 1.3f);
     moar = cpos->y;
     degreestovector3d(cpos, crot, mv3f(0.0f, dir, 0.0f), a * speed);
     ground = readTerrainHeightPlane(cpos->x, cpos->z, &normal, tsize);
     ground = ground < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : ground;
     ground += 1.8f;
     cpos->y = (moar + temp + 1.8f) / 2.0f;
     cpos->y = cpos->y < ground ? ground : cpos->y;
}


void flyMovement(struct airunit *unit, char input, int tsize)
{
     struct v3f pos = mv3f(0.0f, 0.0f, 0.0f);
     float ground, max_thrust, max_vtol_thrust, thrust_step,
          thrust_ceiling, tlapse, aero, drag, lift,
          glide, pressure, temp;
     switch (unit->type) {
     case UNIT_AIR_FIGHTER_1:
          max_thrust = WORLD_GRAVITY + 0.1f;
          max_vtol_thrust = WORLD_GRAVITY + 0.03f;
          thrust_step = 0.01f;
          thrust_ceiling = 21000.0f;
          aero = 0.04f; /* This needs to be less than drag. */
          drag = 0.053f;
          lift = 0.095f;
          glide = 0.035f;
     }
     switch (input & ~INPUT_SPACE) {
     case INPUT_UP:
          unit->thrust = unit->thrust + thrust_step;
          break;
     case INPUT_DOWN:
          unit->thrust = unit->thrust - thrust_step;
          break;
     case INPUT_LEFT:
          unit->rot.z -= 5.0f;
          break;
     case INPUT_RIGHT:
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_RIGHT:
          unit->thrust = unit->thrust + thrust_step;
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_LEFT:
          unit->thrust = unit->thrust + thrust_step;
          unit->rot.z -= 5.0f;
          break;
     case INPUT_DOWN_RIGHT:
          unit->thrust = unit->thrust - thrust_step;
          unit->rot.z += 5.0f;
          break;
     case INPUT_DOWN_LEFT:
          unit->thrust = unit->thrust - thrust_step;
          unit->rot.z -= 5.0f;
          break;
     default: break;
     }
     ground = readTerrainHeightPlane(unit->pos.x, unit->pos.z, &pos, tsize);
     ground = ground < TERRAIN_WATER_LEVEL + 7 ?
          TERRAIN_WATER_LEVEL + 7 : ground;
     /* Airspeed.  The speed of sound is 340.29 metres per second. */
     unit->speed = distance3d(mv3f(0.0f, 0.0f, 0.0f), unit->vec);
     pressure = unit->pos.y < 30000.0f ?
          (30000.0f - unit->pos.y) / 30000.0f : 0.0f;
     /* Thrust lapse, atmospheric density.  See
        https://en.wikipedia.org/wiki/Jet_engine_performance */
     tlapse = unit->pos.y < thrust_ceiling ?
          (thrust_ceiling - unit->pos.y) / (thrust_ceiling * 0.7f) : 0.0f;
     tlapse = tlapse > 1.0f ? 1.0f : tlapse;
     pos = mv3f(0.0f, 0.0f, 0.0f);
     /* VTOL thrust. */
     if ((input | INPUT_SPACE) == input) {
          unit->vtol_thrust = unit->vtol_thrust + 0.05f >
               max_vtol_thrust ? max_vtol_thrust :
               unit->vtol_thrust + 0.05f;
          degreestovector3d(&pos, unit->rot, mv3f(90.0f, 180.0f, 0.0f),
                            unit->vtol_thrust * tlapse);
     }
     else
          unit->vtol_thrust = unit->vtol_thrust - 0.025f >
               0.0f ? unit->vtol_thrust - 0.025f : 0.0f;
     /* Normal thrust. */
     unit->thrust = unit->thrust > max_thrust ?
          max_thrust : unit->thrust < 0.0f ? 0.0f : unit->thrust;
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->thrust * tlapse +
                       unit->speed * aero * pressure);
     /* Update position and vector. */
     unit->vec.x += pos.x;
     unit->vec.y += pos.y - WORLD_GRAVITY;
     unit->vec.z += pos.z;
     unit->pos.x += unit->vec.x;
     unit->pos.y += unit->vec.y;
     unit->pos.z += unit->vec.z;
     /* Drag. */
     pos = mv3f(0.0f, 0.0f, 0.0f);
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f), 1.0f);
     temp = 1 - distance3d(pos, normalize3d(unit->vec)) / 2.0f;
     drag = drag * (pressure + 0.01f) * temp;
     /* if (drag < 0.0f) */
     /*   drag = 0.0f; */
     /* else if (drag > 0.9f) */
     /*   drag = 0.9f; */
     unit->vec.x -= unit->vec.x * drag;
     unit->vec.y -= unit->vec.y * drag;
     unit->vec.z -= unit->vec.z * drag;
     /* Lift and glide. */
     pos = mv3f(0.0f, 0.0f, 0.0f);
     lift *= fabs(unit->rot.x) < 50.0f ?
          (50.0f - fabs(unit->rot.x)) / 50.0f : 0.0f;
     degreestovector3d(&pos, unit->rot, mv3f(90.0f, 180.0f, 0.0f),
                       sqrt(unit->speed) * temp * pressure * lift);
     temp = unit->vec.y + WORLD_GRAVITY;
     if (temp < 0.0f && unit->speed < 40)
          degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                            -temp * pressure * glide);
     unit->vec.x += pos.x;
     unit->vec.y += pos.y;
     unit->vec.z += pos.z;
     /* Aircraft banking. */
     pos = mv3f(0.0f, 0.0f, 0.0f);
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->speed);
     temp = (vectorstodegree2d(pos, mv3f(0.0f, 0.0f, 0.0f)) -
             vectorstodegree2d(unit->vec, mv3f(0.0f, 0.0f, 0.0f))) * 0.09f;
     if (vectorstodegree2d(pos, mv3f(0.0f, 0.0f, 0.0f))
         > vectorstodegree2d(unit->vec, mv3f(0.0f, 0.0f, 0.0f)) + 0.8f)
          unit->rot.z += temp > 7 ? 0 : temp;
     else if (vectorstodegree2d(unit->vec, mv3f(0.0f, 0.0f, 0.0f))
              > vectorstodegree2d(pos, mv3f(0.0f, 0.0f, 0.0f)) + 0.8f)
          unit->rot.z += temp < -7 ? 0 : temp;
     unit->rot.z -= unit->rot.z * 0.12f;
     unit->rot.z = unit->rot.z > 70 ? 70 :
          unit->rot.z < -70 ? -70 : unit->rot.z;
     if (unit->rot.y > 360.0f)
          unit->rot.y -= 360.0f;
     else if (unit->rot.y < 0.0f)
          unit->rot.y += 360.0f;
     /* Ground detection and collision handling. */
     unit->height = fabs(unit->pos.y - ground);
     if (unit->pos.y < ground) {
          unit->pos.y = ground;
          unit->rot.z = 0.0f;
          unit->vec.y *= 0.2f;
          unit->vec.x -= unit->vec.x * 0.2f;
          unit->vec.z -= unit->vec.z * 0.2f;
          if (unit->vec.y < -WORLD_GRAVITY * 0.7f || unit->speed > 11) {
               unit->vec.x -= unit->vec.x * 0.5f;
               unit->vec.y = 0.0f;
               unit->vec.z -= unit->vec.z * 0.5f;
               unit->thrust = 0.0f;
          }
     }
}


void airUnitMove(struct airunit *unit, struct v3f pos, int tsize)
{
     struct v3f temp1 = unit->pos;
     struct v3f temp2 = unit->pos;
     struct v3f temp3 = unit->pos;
     struct v3f temp4 = unit->pos;
     struct v3f temp5 = unit->pos;
     float dist = distance2d(unit->pos, pos);
     float speed = dist < 7000.0f ? 35.0f : 55.0f;
     int thrust;
     unit->rot.y += (vectorstodegree2d(unit->pos, pos) - unit->rot.y) * 0.1f;
     degreestovector3d(&temp1, unit->rot, mv3f(0.0f, 0.0f, 0.0f), 7500.0f);
     degreestovector3d(&temp2, unit->rot, mv3f(0.0f, 0.0f, 0.0f), 6000.0f);
     degreestovector3d(&temp3, unit->rot, mv3f(0.0f, 0.0f, 0.0f), 4500.0f);
     degreestovector3d(&temp4, unit->rot, mv3f(0.0f, 0.0f, 0.0f), 3000.0f);
     degreestovector3d(&temp5, unit->rot, mv3f(0.0f, 0.0f, 0.0f), 1500.0f);
     if (unit->speed > speed + 20.0f)
          thrust = INPUT_DOWN;
     else if (unit->speed > speed)
          thrust = INPUT_NONE;
     else
          thrust = INPUT_UP;
     if (readTerrainHeight(temp1.x, temp1.z) > unit->pos.y ||
         readTerrainHeight(temp2.x, temp2.z) > unit->pos.y ||
         readTerrainHeight(temp3.x, temp3.z) > unit->pos.y ||
         readTerrainHeight(temp4.x, temp4.z) > unit->pos.y ||
         readTerrainHeight(temp5.x, temp5.z) > unit->pos.y) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (-40.0f - unit->rot.x) * 0.1f;
          unit->rot.y += 2.5f;
     }
     else if (unit->vec.y < -WORLD_GRAVITY * 2.5f
              && unit->height < 2500.0f) {
          flyMovement(unit, INPUT_SPACE + thrust, tsize);
          unit->rot.x += (-25.0f - unit->rot.x) * 0.05f;
     }
     else if (dist < 5000.0f) {
          flyMovement(unit, INPUT_NONE, tsize);
          unit->rot.x += (-15.0f - unit->rot.x) * 0.1f;
     }
     else if (unit->height < 1200.0f) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (-10.0f - unit->rot.x) * 0.1f;
     }
     else if (unit->height > 10000.0f) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (3.5f - unit->rot.x) * 0.02f;
     }
     else {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (0.0f - unit->rot.x) * 0.03f;
     }
}


void airUnitMoveVTOL(struct airunit *unit, struct v3f pos, int tsize)
{
     float dist = distance2d(unit->pos, pos);
     float pitch = unit->speed > 20.0f ? -5.0f : 7.5f;
     int thrust = unit->thrust > 0.0f ? INPUT_DOWN : INPUT_NONE;
     if (dist > 200.0f)
          unit->rot.y += (vectorstodegree2d
                          (unit->pos, pos) - unit->rot.y) * 0.1f;
     else
          unit->rot.y += 2.0f;
     if (unit->height > 150.0f) {
          if (unit->vec.y < -WORLD_GRAVITY - 7.0f)
               flyMovement(unit, INPUT_SPACE + thrust, tsize);
          else
               flyMovement(unit, INPUT_NONE + thrust, tsize);
          unit->rot.x += (pitch - unit->rot.x) * 0.1f;
     }
     else if (unit->vec.y < -WORLD_GRAVITY || unit->height < 70.0f) {
          flyMovement(unit, INPUT_SPACE + thrust, tsize);
          unit->rot.x += (pitch - unit->rot.x) * 0.1f;
     }
     else {
          flyMovement(unit, INPUT_NONE + thrust, tsize);
          unit->rot.x += (0.0f - unit->rot.x) * 0.05f;
     }
}


void updateAirUnits(struct airunit *units, int tsize)
{
     int i;
     struct v3f pos = units[0].pos;
     /* struct v3f pos = mv3f(110000.0f, 0.0f, 55000.0f); */
     for (i = 1; i < 1; i++) {
          if (distance2d(units[i].pos, pos) < 2500.0f)
               airUnitMoveVTOL(&units[i], pos, tsize);
          else
               airUnitMove(&units[i], pos, tsize);
     }
}


int main(int argc, char *argv[])
{
     GLuint textures[10], shaders[5];
     GLFWwindow *window = NULL;
     int i, tsize = TERRAIN_SQUARE_SIZE, st = 0;
     char direction, state = 0;
     float fps = 0.0f;
     struct v2f sector = {0.0f, 0.0f}; /* Terrain update sector. */
     struct v3f crot = {0.0f, 0.0f, 0.0f};            /* Camera rotation. */
     struct v3f cpos = {0.0f, readTerrainHeightPlane2 /* Camera position. */
                        (0.0f, 0.0f, tsize) + 1.8f, 0.0f};
     const struct aiScene *stemp;
     struct aiScene *scene     = malloc(sizeof(struct aiScene) * 32);
     struct aiScene *textquads = malloc(sizeof(struct aiScene) * 36);
     struct airunit *airunits  = malloc(sizeof(struct airunit) * 1);

     createGradient();
     if ((window = startGraphics(textures, shaders)) != NULL) {
          /* Load model data. */
          if ((stemp = loadModel("data/models/tree1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_TREE_POPLAR] = *stemp; /* Poplar. */
          if ((stemp = loadModel("data/models/tree2.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_TREE_OAK] = *stemp; /* Oak. */
          if ((stemp = loadModel("data/models/tree3.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_TREE_FIR] = *stemp; /* Fir. */
          if ((stemp = loadModel("data/models/tree4.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_TREE_BUSH] = *stemp; /* Bush. */
          if ((stemp = loadModel("data/models/mtree1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_MTREE_SMALL] = *stemp; /* Small multi-trees. */
          if ((stemp = loadModel("data/models/stump1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_TREE_STUMP] = *stemp; /* Stump. */
          if ((stemp = loadModel("data/models/rock1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_ROCK1] = *stemp; /* Rock. */
          /* These must not be loaded near start of array because of
             trees appearing above slopes.  See renderGroundScenery(). */
          if ((stemp = loadModel("data/models/mtree2.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_MTREE_BIG] = *stemp; /* Sparse multi-trees. */
          if ((stemp = loadModel("data/models/mtree3.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_MTREE_SPARSE] = *stemp; /* Ditto. */
          if ((stemp = loadModel("data/models/mtree4.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_MTREE_FIR] = *stemp; /* Sparse firs. */
          if ((stemp = loadModel("data/models/house1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_BUILDING_HOUSE1] = *stemp;
          if ((stemp = loadModel("data/models/house2.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_BUILDING_HOUSE2] = *stemp;
          if ((stemp = loadModel("data/models/fighter1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_AIR_FIGHTER1] = *stemp; /* More models, etc. */
          if ((stemp = loadModel("data/models/fighter2.obj")) == NULL)
               return EXIT_FAILURE;
          else
               scene[MODEL_AIR_FIGHTER2] = *stemp;
          /* Load text quad data. */
          if ((stemp = loadTextQuad("data/models/quads/0.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[0] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/1.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[1] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/2.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[2] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/3.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[3] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/4.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[4] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/5.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[5] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/6.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[6] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/7.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[7] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/8.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[8] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/9.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[9] = *stemp;
          if ((stemp = loadTextQuad("data/models/quads/minus.obj")) == NULL)
               return EXIT_FAILURE;
          else
               textquads[10] = *stemp;
          /* Position some air units around. */
          for (i = 0; i < 1; i++) {
               airunits[i].type = UNIT_AIR_FIGHTER_1;
               airunits[i].pos.x = (i - 1) * 50;
               airunits[i].pos.z = (i - 1) * 23 + 50;
               airunits[i].pos.y = readTerrainHeightPlane2
                    (airunits[i].pos.x, airunits[i].pos.z, tsize);
          }
          /* Main loop. */
          while (!glfwWindowShouldClose(window)) {
               keyboardInput(window, &direction);
               if (state == 0) {
                    mouseLook(window, &crot);
                    movement(&cpos, crot, direction, 1.0f, tsize);
                    if (distance3d(cpos, airunits[0].pos) < 10.0f
                        && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
                         state = 1;
                         st = 10;
                    }
               }
               else {
                    if (airunits[0].height > 3.0f)
                         mouseLook(window, &airunits[0].rot);
                    flyMovement(&airunits[0], direction, tsize);
                    cameraTrailMovement(&cpos, &crot, airunits[0], tsize);
                    if (airunits[0].thrust == 0 && airunits[0].height
                        < 3.0f && airunits[0].speed < 2.0f
                        && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS
                        && st < 1) {
                         state = 0;
                         airunits[0].vtol_thrust = 0;
                    }
               }
               st--;
               updateAirUnits(airunits, tsize);
               updateCamera(crot);
               glTranslatef(-cpos.x, -cpos.y, -cpos.z);
               render(window, scene, textquads, textures, shaders,
                      cpos, crot, &sector, &tsize, &fps, airunits);
          }
          free(scene);
          free(airunits);
          free(textquads);
          glfwDestroyWindow(window);
          glfwTerminate();
          return EXIT_SUCCESS;
     }
     else /* Awww. */
          return EXIT_FAILURE;
}
