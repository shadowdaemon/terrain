#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FreeImage.h>
#include <assimp/scene.h>
#include "maths.h"


int tSize = TERRAIN_SQUARE_SIZE;
struct terrainMod *tMods;
float pgrad[PERLIN_SIZE][PERLIN_SIZE][2];
struct v3f nullv3f;

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
     GLsizei width  = FreeImage_GetWidth(img);
     GLsizei height = FreeImage_GetHeight(img);
     GLubyte *bits  = (GLubyte*) FreeImage_GetBits(img);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                  GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
     /* Set up mipmapping. */
     for (i = 1; i < 40; i++) {
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


void loadTexSub(int left, int top, int right, int bottom, const char *file)
{
     int i;
     FIBITMAP *imgN = FreeImage_Load(FreeImage_GetFileType(file, 0), file, 0);
     FIBITMAP *img  = FreeImage_Copy(imgN, left, top, right, bottom);
     img = FreeImage_ConvertTo32Bits(img);
     GLsizei width  = FreeImage_GetWidth(img);
     GLsizei height = FreeImage_GetHeight(img);
     GLubyte *bits  = (GLubyte*) FreeImage_GetBits(img);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                  GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid *) bits);
     /* Set up mipmapping. */
     for (i = 1; i < 40; i++) {
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
     FreeImage_Unload(imgN);
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
          if (fread(src, 1, len, fp))
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
     if (strlen(v_file)) {
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
     glGenTextures(MAX_TEXTURES, textures);
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
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FOLIAGE_GRASS]);
     loadTexture2D("data/textures/foliage_grass.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_BODY_1]);
     loadTexture2D("data/textures/warzone/page-14-droid-hubs.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_PROP_1]);
     loadTexture2D("data/textures/warzone/page-16-droid-drives.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_BARB_1]);
     loadTexture2D("data/textures/warzone/page-7-barbarians-arizona.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_BUILDING_1]);
     loadTexture2D("data/textures/building1.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_BUILDING_2]);
     loadTexture2D("data/textures/warzone/page-13-player-buildings.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FLOOR_1]);
     loadTexture2D("data/textures/warzone/page-9-player-buildings-bases-rockies.png");
     glBindTexture(GL_TEXTURE_2D, textures[TEX_FX_1]);
     //loadTexSub(40, 104, 59, 97, "data/textures/warzone/page-18-fx.png");
     loadTexSub(40, 256-104, 59, 256-97, "data/textures/warzone/page-18-fx.png");
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
                         struct unit unit)
{
     const float dist = 40.0f;
     struct v3f temppos = unit.pos;
     float temp = 0.0f;
     if (unit.p.airv.speed > 80.0f) {
          temp = 0.37f + (unit.p.airv.speed - 80.0f) * 0.01f;
          temp = temp > 1.0f ? 1.0f : temp;
     }
     else
          temp = 0.37f;
     degreestovector3d(&temppos, unit.rot, mv3f(0, 180, 0), dist);
     cpos->x -= (cpos->x - temppos.x) * temp;
     cpos->y -= (cpos->y - temppos.y) * temp;
     cpos->z -= (cpos->z - temppos.z) * temp;
     crot->x = unit.rot.x;
     crot->y = vectorstodegree2d(unit.pos, *cpos);
     temp = readTerrainHeightPlane(cpos->x, cpos->z, &temppos);
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


void updateAirUnits(struct unit **units, struct v3f cpos)
{
     struct unit *p = *units;
     struct v3f pos = cpos;// mv3f(11000.0f, 0.0f, 5500.0f); /* Destination. */
     while ((*units)->type != UNIT_END_LIST) {
          airUnitMove(*units, pos);
          *units = (*units)->next;
     }
     *units = p;
}


char loadModels(struct aiScene *scene)
{
     const struct aiScene *stemp;
     if ((stemp = loadModel("data/models/tree1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_TREE_POPLAR] = *stemp; /* Poplar. */
     if ((stemp = loadModel("data/models/tree2.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_TREE_OAK] = *stemp; /* Oak. */
     if ((stemp = loadModel("data/models/tree3.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_TREE_FIR] = *stemp; /* Fir. */
     if ((stemp = loadModel("data/models/tree4.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_TREE_BUSH] = *stemp; /* Bush. */
     if ((stemp = loadModel("data/models/mtree1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_MTREE_SMALL] = *stemp; /* Small multi-trees. */
     if ((stemp = loadModel("data/models/stump1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_TREE_STUMP] = *stemp; /* Stump. */
     if ((stemp = loadModel("data/models/rock1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_ROCK1] = *stemp; /* Rock. */
     /* These must not be loaded near start of array because of
        trees appearing above slopes.  See renderGroundScenery(). */
     if ((stemp = loadModel("data/models/mtree2.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_MTREE_BIG] = *stemp; /* Sparse multi-trees. */
     if ((stemp = loadModel("data/models/mtree3.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_MTREE_SPARSE] = *stemp; /* Ditto. */
     if ((stemp = loadModel("data/models/mtree4.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_MTREE_FIR] = *stemp; /* Sparse firs. */
     if ((stemp = loadModel("data/models/house1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_BUILDING_HOUSE1] = *stemp;
     if ((stemp = loadModel("data/models/house2.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_BUILDING_HOUSE2] = *stemp;
     if ((stemp = loadModel("data/models/warzone/body1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_BODY_1] = *stemp;
     if ((stemp = loadModel("data/models/warzone/prop1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_PROP_1] = *stemp;
     if ((stemp = loadModel("data/models/warzone/exjeep.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_JEEP_1] = *stemp;
     if ((stemp = loadModel("data/models/warzone/blvfact1.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_BUILDING_VTOL_FAC] = *stemp;
     if ((stemp = loadModel("data/models/warzone/blbfact.obj")) == NULL)
          return GL_FALSE;
     else
          scene[MODEL_BUILDING_FAC_FLOOR] = *stemp;
     return GL_TRUE;
}


char loadTextQuads(struct aiScene *quads)
{
     const struct aiScene *stemp;
     if ((stemp = loadTextQuad("data/models/quads/0.obj")) == NULL)
          return GL_FALSE;
     else
          quads[0] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/1.obj")) == NULL)
          return GL_FALSE;
     else
          quads[1] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/2.obj")) == NULL)
          return GL_FALSE;
     else
          quads[2] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/3.obj")) == NULL)
          return GL_FALSE;
     else
          quads[3] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/4.obj")) == NULL)
          return GL_FALSE;
     else
          quads[4] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/5.obj")) == NULL)
          return GL_FALSE;
     else
          quads[5] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/6.obj")) == NULL)
          return GL_FALSE;
     else
          quads[6] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/7.obj")) == NULL)
          return GL_FALSE;
     else
          quads[7] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/8.obj")) == NULL)
          return GL_FALSE;
     else
          quads[8] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/9.obj")) == NULL)
          return GL_FALSE;
     else
          quads[9] = *stemp;
     if ((stemp = loadTextQuad("data/models/quads/minus.obj")) == NULL)
          return GL_FALSE;
     else
          quads[10] = *stemp;
     return GL_TRUE;
}


int main(int argc, char *argv[])
{
     GLuint textures[MAX_TEXTURES], shaders[5];
     GLFWwindow *window = NULL;
     int i, st = 0, numTeams = 2;
     char direction, state = 0;
     float fps = 0.0f;
     tMods = (struct terrainMod *) malloc(sizeof(struct terrainMod) * 2);
     tMods[0].pos.x  = 500;
     tMods[0].pos.y  = 500;
     tMods[0].radius = 750;
     tMods[0].height = 0;
     struct v2f sector = {0.0f, 0.0f};          /* Terrain update sector. */
     struct v3f crot = {0.0f, 0.0f, 0.0f};            /* Camera rotation. */
     struct v3f cpos = {0.0f, readTerrainHeightPlane2 /* Camera position. */
                        (0.0f, 0.0f) + 1.8f, 0.0f};
     struct aiScene *scene     = malloc(sizeof(struct aiScene) * 32);
     struct aiScene *textquads = malloc(sizeof(struct aiScene) * 36);
     struct team    *teams     = malloc(sizeof(struct team) * 2);
     struct spriteA sprites;
     struct projA projectiles;
     struct unitA tUnit;
     projectiles.p = malloc(sizeof(struct proj) * MAX_PROJ);
     projectiles.a = 0;
     for (i = 0; i < MAX_PROJ; i++) {
          projectiles.p[i].type = PROJ_NULL;
          projectiles.p[i].life = 0;
     }
     sprites.p = malloc(sizeof(struct sprite) * MAX_SPRITE);
     sprites.a = 0;
     for (i = 0; i < MAX_SPRITE; i++) {
          sprites.p[i].type = SPRITE_NULL;
          sprites.p[i].life = 0;
     }
     for (i = 0; i < numTeams; i++) {
          initUnitList(&teams[i].air);
          initUnitList(&teams[i].ground);
          initUnitList(&teams[i].building);
     }
     nullv3f = mv3f(0.0f, 0.0f, 0.0f);
     createGradient();
     if ((window = startGraphics(textures, shaders)) != NULL &&
         loadModels(scene) && loadTextQuads(textquads)) {
          /* Position some units around for testing. */
          addUnitAir(&teams[0].air, UNIT_AIR_FIGHTER_1, mv3f
                     (50, readTerrainHeightPlane2(50, 0), 0));
          addUnitAir(&teams[0].air, UNIT_AIR_FIGHTER_1, mv3f
                     (150, readTerrainHeightPlane2(150, 0), 0));
          addUnitAir(&teams[0].air, UNIT_AIR_FIGHTER_1, mv3f
                     (0, readTerrainHeightPlane2(0, 200), 200));
          addUnitGround(&teams[0].ground, UNIT_GROUND_JEEP_1, mv3f
                        (20, readTerrainHeightPlane2(20, 100), 100));
          addUnitGround(&teams[0].ground, UNIT_GROUND_JEEP_1, mv3f
                        (520, readTerrainHeightPlane2(520, 500), 500));
          addUnitBuilding(&teams[0].building, UNIT_BUILDING_VTOL_FAC, mv3f
                          (500, readTerrainHeightPlane2
                           (500, 500), 500));
          /* Main loop. */
          while (!glfwWindowShouldClose(window)) {
               keyboardInput(window, &direction);
               mouseLook(window, &crot);
               movement(&cpos, &crot, direction, 1.0f,
                        INPUT_TYPE_PEDESTRIAN);
               /* This mess is just for testing. */
               if (state == 0) {
                    mouseLook(window, &crot);
                    movement(&cpos, &crot, direction, 0.7f,
                             INPUT_TYPE_PEDESTRIAN);
                    tUnit = closestUnit(teams[0].ground, 0, cpos);
                    if (tUnit.a < 10.0f
                        && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
                         state = 1;
                         st = 10;
                    }
               }
               /* else if (state == 1) { */
               /*      if (teams[0].air[0].p.airv.height > 3.0f) */
               /*           mouseLook(window, &teams[0].air[0].rot); */
               /*      flyMovement(&teams[0].air[0], direction); */
               /*      cameraTrailMovement(&cpos, &crot, teams[0].air[0]); */
               /*      if (teams[0].air[0].p.airv.thrust == 0 && */
               /*          teams[0].air[0].p.airv.height */
               /*          < 3.0f && teams[0].air[0].p.airv.speed < 2.0f */
               /*          && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS */
               /*          && st < 1) { */
               /*           state = 0; */
               /*           teams[0].air[0].p.airv.vtolThrust = 0; */
               /*      } */
               /* } */
               else if (state == 1) {
                    movement(&tUnit.p->pos, &tUnit.p->rot,
                             direction, 10.0f, INPUT_TYPE_VEHICLE);
                    cameraTrailMovement(&cpos, &crot, *(tUnit.p));
                    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
                         addProjectile(&projectiles, &sprites, PROJ_BULLET,
                                       tUnit.p->pos, tUnit.p->rot);
                    if (glfwGetKey(window, GLFW_KEY_TAB)
                        == GLFW_PRESS && st < 1)
                         state = 0;
               }
               st--;
               st = st > 0 ? st : 0;
               /* Update various things, order is important. */
               for (i = 0; i < numTeams; i++) {
                    updateAirUnits(&teams[i].air, cpos);
               // updateGroundUnits(ground);
               }
               updateProjectiles(&projectiles);
               updateSprites(&sprites);
               updateCamera(crot);
               glTranslatef(-cpos.x, -cpos.y, -cpos.z);
               render(window, scene, textquads, textures, shaders,
                      cpos, crot, &sector, &fps, &sprites, teams);
          }
          free(scene);
          free(textquads);
          for (i = 0; i < numTeams; i++) {
               freeUnitList(&teams[i].air);
               freeUnitList(&teams[i].ground);
               freeUnitList(&teams[i].building);
          }
          glfwDestroyWindow(window);
          glfwTerminate();
          return EXIT_SUCCESS;
     }
     else /* Awww. */
          return EXIT_FAILURE;
}
