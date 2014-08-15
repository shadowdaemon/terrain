#include <math.h>
#include "common.h"


struct v2f mv2f(float x, float y);
struct v3f mv3f(float x, float y, float z);
float distance2d(struct v3f pos_a, struct v3f pos_b);
float distance3d(struct v3f pos_a, struct v3f pos_b);
struct v3f degreestovector2d(struct v3f pos_self, float rot_self, float rot, float dist);
void degreestovector3d(struct v3f *pos_self, struct v3f rot_self, struct v3f rot, float dist);
float vectorstodegree2d(struct v3f pos_self, struct v3f pos_b);
struct v3f calcNormal(float in_v1[3], float in_v2[3], float in_v3[3]);
float planeHeight(float in_p[2], float in_v1[3], float in_v2[3], float in_v3[3], struct v3f *normal);
