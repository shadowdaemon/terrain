#include <GLFW/glfw3.h>

#define RESX                                1366
#define RESY                                768
#define USE_SHADERS                         0
#define PI                                  3.14159265358979323846f
#define PIx180                              180.0f*PI
#define WORLD_GRAVITY                       0.4f
#define TERRAIN_GRID_SIZE                   120
#define TERRAIN_GRID_SIZE_HALF              60
#define TERRAIN_GRID_SIZE_QUARTER           30
#define TERRAIN_SQUARE_SIZE                 500
#define TERRAIN_STEP_SIZE                   10
#define TERRAIN_WATER_LEVEL                 0
#define TERRAIN_SCALE_HEIGHT                9500 /* Also upper cloud height. */
#define LOWER_CLOUD_HEIGHT                  4500
#define VIEW_DISTANCE                       3000
#define VIEW_DISTANCE_HALF                  1500
#define VIEW_DISTANCE_QUARTER               750
#define SCENERY_DENSITY                     1200 /* Lower number is more dense, don't go below 1000. */
#define SCENERY_SIZE                        14400 /* TERRAIN_GRID_SIZE^2. */
#define SCENERY_DENSITY_GRASS               1000
#define SCENERY_SIZE_GRASS                  3600 /* TERRAIN_GRID_SIZE_HALF^2. */
#define SCENERY_STEP_SIZE                   6
#define T_TYPE_NULL                         0
#define T_TYPE_CRATER                       1
#define T_TYPE_VILLAGE                      2
#define T_TYPE_GRASS1                       3
#define T_TYPE_GRASS2                       4
#define T_TYPE_GRASS3                       5
#define T_TYPE_DIRT                         6
#define T_TYPE_ROCK                         7
#define T_TYPE_FOREST1                      8
#define T_TYPE_FOREST2                      9
#define T_TYPE_SNOW                         10
#define T_TYPE_DESERT                       11
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

#define UNIT_AIR_FIGHTER1                   0
#define TEX_TERRAIN                         0
#define TEX_FOLIAGE                         1
#define TEX_CLOUD                           2
#define TEX_FONT                            3
#define TEX_RENDER                          4
#define TEX_BUILDING                        5
#define TEX_AIR_FIGHTER1                    6
#define TEX_FOLIAGE_GRASS                   7
#define MODEL_TREE_POPLAR                   0
#define MODEL_TREE_OAK                      1
#define MODEL_TREE_FIR                      2
#define MODEL_TREE_BUSH                     3
#define MODEL_MTREE_SMALL                   4
#define MODEL_TREE_STUMP                    5
#define MODEL_ROCK1                         6
#define MODEL_MTREE_BIG                     8
#define MODEL_MTREE_SPARSE                  9
#define MODEL_MTREE_FIR                     10
#define MODEL_BUILDING_HOUSE1               12
#define MODEL_BUILDING_HOUSE2               13
#define MODEL_AIR_FIGHTER1                  14
#define MODEL_AIR_FIGHTER2                  15
#define GRASS_GRASS1                        0
#define GRASS_GRASS2                        1
#define GRASS_BUSH1                         2
#define GRASS_BUSH2                         3
#define GRASS_BAMBOO                        4
#define GRASS_DEAD                          5
#define GRASS_FLOWERS                       6

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
float readTerrainHeightPlane2(float x, float z, int t_size);
unsigned char readTerrainType(float x, float z);
void drawTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *t_size, char *swapb);
const struct aiScene *loadModel(const char *file);
const struct aiScene *loadTextQuad(const char *file);
void drawModel(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha);
void drawModel2(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, const GLuint *color, GLuint alpha);
void render(GLFWwindow *window, struct aiScene *scene, struct aiScene *textquads, GLuint *textures, GLuint *shaders,
  struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *t_size, float *fps, struct airunit *airunits);
