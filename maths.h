#include <math.h>
#include "common.h"


#define PERLIN_SIZE 128

float pos(float);
float neg(float);
int snap(float, int);
struct v2f mv2f(float, float);
struct v3f mv3f(float, float, float);
float distance2d(struct v3f, struct v3f);
float distance3d(struct v3f, struct v3f);
struct v3f degreestovector2d(struct v3f, float, float, float);
void degreestovector3d(struct v3f*, struct v3f, struct v3f, float);
float vectorstodegree2d(struct v3f, struct v3f);
struct v3f normalize3d(struct v3f);
struct v3f calcNormal(float [3], float [3], float [3]);
void createMatrix(float *, float *);
float planeHeight(float [2], float [3], float [3], float [3], struct v3f*);
float lerp(float, float, float, float);
void createGradient(void);
float perlin(float, float, float);
