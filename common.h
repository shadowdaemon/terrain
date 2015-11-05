#include <GLFW/glfw3.h>

#define RESX                                1366
#define RESY                                768
#define USE_SHADERS                         1
#define PI                                  3.14159265358979323846f
#define PIx180                              180.0f*PI
#define WORLD_GRAVITY                       0.4f

/* Terrain defines. */
#define TERRAIN_GRID_SIZE                   100
#define TERRAIN_GRID_SIZE_HALF              50
#define TERRAIN_GRID_SIZE_QUARTER           25
#define TERRAIN_SQUARE_SIZE                 500
#define TERRAIN_STEP_SIZE                   10
#define TERRAIN_WATER_LEVEL                 0
/* Also upper cloud height. */
#define TERRAIN_SCALE_HEIGHT                9500
#define LOWER_CLOUD_HEIGHT                  4500
#define CLOUD_DENSITY                       1.0
#define VIEW_DISTANCE                       5000
#define VIEW_DISTANCE_HALF                  2500
#define VIEW_DISTANCE_QUARTER               1250
/* Lower number is more dense, don't go below 1000. */
#define SCENERY_DENSITY                     1200
#define SCENERY_SIZE TERRAIN_GRID_SIZE * TERRAIN_GRID_SIZE
#define SCENERY_DENSITY_GRASS               1000
#define SCENERY_SIZE_GRASS TERRAIN_GRID_SIZE_HALF * TERRAIN_GRID_SIZE_HALF
#define T_TYPE_NULL                         0
#define T_TYPE_CRATER                       1
#define T_TYPE_VILLAGE                      2
#define T_TYPE_GRASS_1                      3
#define T_TYPE_GRASS_2                      4
#define T_TYPE_GRASS_3                      5
#define T_TYPE_DIRT                         6
#define T_TYPE_ROCK                         7
#define T_TYPE_FOREST1                      8
#define T_TYPE_FOREST2                      9
#define T_TYPE_SNOW                         10
#define T_TYPE_DESERT                       11

/* Input. */
#define INPUT_TYPE_PEDESTRIAN               0
#define INPUT_TYPE_VEHICLE                  1
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

/* Textures, models, etc. */
#define MAX_TEXTURES                        16
#define UNIT_AIR_FIGHTER_1                  0
#define UNIT_GROUND_JEEP_1                  1
#define TEX_TERRAIN_1                       0
#define TEX_TERRAIN_2                       1
#define TEX_FOLIAGE                         2
#define TEX_CLOUD                           3
#define TEX_FONT                            4
#define TEX_RENDER                          5
#define TEX_BUILDING                        6
#define TEX_FOLIAGE_GRASS                   7
#define TEX_BODY_1                          8
#define TEX_PROP_1                          9
#define TEX_WEAPON_1                        10
#define TEX_BARB_1                          11
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
#define MODEL_BODY_1                        14
#define MODEL_PROP_1                        15
#define MODEL_WEAPON_1                      16
#define MODEL_JEEP_1                        17
#define GRASS_GRASS1                        0
#define GRASS_GRASS2                        1
#define GRASS_BUSH1                         2
#define GRASS_BUSH2                         3
#define GRASS_BAMBOO                        4
#define GRASS_DEAD                          5
#define GRASS_FLOWERS                       6

/* #define glActiveTextureARB _ActiveTextureARB */
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
#define glVertexAttrib3fARB _VertexAttrib3fARB
#define glVertexAttrib3fvARB _VertexAttrib3fvARB
#define glMultiTexCoordPointerEXT _MultiTexCoordPointerEXT
/* PFNGLACTIVETEXTUREARBPROC _ActiveTextureARB; */
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
PFNGLVERTEXATTRIB3FARBPROC _VertexAttrib3fARB;
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
     float mod;
     unsigned char type;
};

struct fx {
     struct v3f pos;
     unsigned char type;
     unsigned char life;
};

struct airparams {
     float thrust;
     float vtolThrust;
     float speed;
     float height;
};

struct groundparams {
     float turret;
};

struct unit {
     int type;
     struct v3f pos;
     struct v3f rot;
     struct v3f vec;
     union {
          struct airparams airp;
          struct groundparams groundp;
     } p;
};

/* Only used for rendering. */
struct grass {
     struct v3f p; /* Position. */
     float r;      /* Rotation. */
     float s;      /* Size. */
     float d;      /* Distance from observer. */
     int t;        /* Type. */
     GLubyte a;    /* Alpha. */
};

struct terrain algorithmicTerrain(float, float);
float readTerrainHeight(float, float);
float readTerrainHeightPlane(float, float, struct v3f*, int);
float readTerrainHeightPlane2(float, float, int);
unsigned char readTerrainType(float, float);
void drawTerrain(GLuint*, struct v3f, struct v3f, struct v2f*, int*, char*);
const struct aiScene *loadModel(const char*);
const struct aiScene *loadTextQuad(const char*);
void drawModel(const struct aiScene*, struct v3f, struct v3f,
               GLfloat, GLuint);
void drawModel2(const struct aiScene*, struct v3f, struct v3f, GLfloat,
                const GLuint*, GLuint);
void renderGroundScenery(struct aiScene *, GLuint *, struct v3f, struct v3f,
                         int, float);
void renderGrass(GLuint *, struct v3f, struct v3f, int, float);
void render(GLFWwindow*, struct aiScene*, struct aiScene*, GLuint*, GLuint*,
            struct v3f, struct v3f, struct v2f*, int*, float*,
            struct unit*, struct unit*);
void movePitch(struct v3f *, struct v3f);
void movement(struct v3f *, struct v3f *, char, float, int, int);
void flyMovement(struct unit *, char, int);
void airUnitMove(struct unit *, struct v3f, int);
void airUnitMoveVTOL(struct unit *, struct v3f, int);
