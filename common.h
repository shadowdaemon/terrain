#include <GLFW/glfw3.h>

#define RESX                                1366
#define RESY                                768
#define PI                                  3.14159265358979323846f
#define PIx180                              180.0f*PI
#define VIEW_DISTANCE                       1500
#define VIEW_DISTANCE_HALF                  750
#define WORLD_SIZE                          100000
#define WORLD_GRAVITY                       0.4f
#define TERRAIN_GRID_SIZE                   120
#define TERRAIN_GRID_SIZE_HALF              60
#define TERRAIN_GRID_SIZE_QUARTER           30
#define TERRAIN_SQUARE_SIZE                 500
#define TERRAIN_SQUARE_SIZE_HALF            250
#define TERRAIN_STEP_SIZE                   4
#define TERRAIN_WATER_LEVEL                 0
#define TERRAIN_SCALE_HEIGHT                9500
#define LOWER_CLOUD_HEIGHT                  4500
#define T_TYPE_NULL                         0
#define T_TYPE_CRATER                       1
#define T_TYPE_VILLAGE                      2
#define T_TYPE_GRASS1                       3
#define T_TYPE_GRASS2                       4
#define T_TYPE_GRASS3                       5
#define T_TYPE_DIRT                         6
#define T_TYPE_ROCK                         7
#define T_TYPE_FOREST1                      8
#define T_TYPE_SNOW                         9
#define KEY_NONE                            0
#define KEY_LMB                             5
#define KEY_LMB_RELEASE                     6
#define KEY_MMB                             7
#define KEY_RMB                             9
#define INPUT_NONE                          0
#define INPUT_UP                            1
#define INPUT_DOWN                          2
#define INPUT_LEFT                          3
#define INPUT_RIGHT                         4
#define INPUT_UP_RIGHT                      5
#define INPUT_UP_LEFT                       6
#define INPUT_DOWN_RIGHT                    7
#define INPUT_DOWN_LEFT                     8
#define INPUT_VERT_UP                       9
#define INPUT_VERT_DOWN                     10
#define INPUT_SPACE                         16
#define INPUT_LEFT_SHIFT                    32
#define UNIT_AIRFIGHTER                     0

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
#define glUniform2fARB _Uniform2fARB
#define glUniform2fvARB _Uniform2fvARB
#define glUniform4fvARB _Uniform4fvARB
#define glUniformMatrix4fvARB _UniformMatrix4fvARB
#define glGetUniformLocationARB _GetUniformLocationARB
#define glGetAttribLocationARB _GetAttribLocationARB
#define glBindAttribLocationARB _BindAttribLocationARB
#define glVertexAttrib3fvARB _VertexAttrib3fvARB
#define glMultiTexCoordPointerEXT _MultiTexCoordPointerEXT
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
PFNGLUNIFORM2FARBPROC _Uniform2fARB;
PFNGLUNIFORM2FVARBPROC _Uniform2fvARB;
PFNGLUNIFORM4FVARBPROC _Uniform4fvARB;
PFNGLUNIFORMMATRIX4FVARBPROC _UniformMatrix4fvARB;
PFNGLGETUNIFORMLOCATIONARBPROC _GetUniformLocationARB;
PFNGLGETATTRIBLOCATIONARBPROC _GetAttribLocationARB;
PFNGLBINDATTRIBLOCATIONARBPROC _BindAttribLocationARB;
PFNGLVERTEXATTRIB3FVARBPROC _VertexAttrib3fvARB;
PFNGLMULTITEXCOORDPOINTEREXTPROC _MultiTexCoordPointerEXT;


struct v2i {
  int x;
  int y;
};
struct v2f {
  float x;
  float y;
};
struct v2d {
  double x;
  double y;
};
struct v3i {
  int x;
  int y;
  int z;
};
struct v3f {
  float x;
  float y;
  float z;
};

struct terrain {
  float height;
  unsigned char type;
};

struct fx {
  struct v3f pos;
  unsigned char type;
  unsigned char life;
};

struct airunit {
  int type;
  struct v3f pos;
  struct v3f rot;
  struct v3f vec;
  float thrust;
  float vtol_thrust;
  float speed;
  float height;
};

struct terrain algorithmicTerrain(float x, float z);
float readTerrainHeight(float x, float y);
float readTerrainHeightPlane(float x, float z, struct v3f *normal, int t_size);
unsigned char readTerrainType(float x, float z);
void drawTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *t_size, char *swapb);
const struct aiScene *loadModel(const char *file);
const struct aiScene *loadTextQuad(const char *file);
void drawModel(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha);
void render(GLFWwindow *window, struct aiScene *scene, struct aiScene *textquads, GLuint *textures, GLuint *shaders,
  struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *t_size, float *fps, struct airunit *airunits);
