#include <GLFW/glfw3.h>

#define PI                     3.14159265358979323846f
#define PIx180                 180.0f*PI
#define VIEW_DISTANCE                       4000
#define VIEW_DISTANCE_HALF                  2000
#define WORLD_SIZE                          50000
#define WORLD_GRAVITY                       0.98f
#define TERRAIN_GRID_SIZE                   120
#define TERRAIN_GRID_SIZE_HALF              60
#define TERRAIN_GRID_SIZE_QUARTER           30
#define TERRAIN_SQUARE_SIZE                 300
#define TERRAIN_SQUARE_SIZE_HALF            150
// #define TERRAIN_CENTRE_DISTANCE             (TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE*0.1f)
#define TERRAIN_STEP_SIZE                   8
#define TERRAIN_WATER_LEVEL                 0
#define T_TYPE_CRATER                       2
#define T_TYPE_GRASS1                       3
#define T_TYPE_GRASS2                       4
#define T_TYPE_GRASS3                       5
#define T_TYPE_DIRT                         6
#define T_TYPE_ROCK                         7
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

struct terrain
{
  float height;
  float diff;
  char type;
};

struct model {
  GLenum type;
  GLuint num_indices;
  GLfloat (*verts)[3];
  GLfloat (*norms)[3];
  GLuint *indices;
  GLuint *tex_indices;
  GLfloat (*tex_coords)[2];
};

struct airunit {
  int type;
  struct v3f pos;
  struct v3f rot;
  struct v3f vec;
  float thrust;
  float speed;
  float height;
};

float algorithmicTerrainHeight(float x, float z);
struct terrain readTerrain(float x, float y);
struct terrain readTerrainB(float x, float y);
float readTerrainHeight(float x, float y);
void moveTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, int *swapb, int squaresize);
void drawTerrain(struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *swapb, int *squaresize);
const struct aiScene *loadFromOBJFile(const char *name);
// void loadFromOBJFile(const char *name, struct model *model);
void drawModel(const struct aiScene *scene, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha);
//void drawModel(struct model model, struct v3f pos, struct v3f rot, GLfloat size, GLuint alpha);
void render(GLFWwindow *window, struct aiScene *scene, GLuint *textures, GLuint *shaders, int *swapb, struct v3f camerapos, struct v3f camerarot, struct v2f *sector, float camheight, int *squaresize, float *fogend, struct airunit *airunits);
