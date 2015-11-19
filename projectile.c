#include "maths.h"


extern struct v3f nullv3f;

void addProjectile(struct projA *proj, struct spriteA *sprites, int type,
                   struct v3f pos, struct v3f rot)
{
     int i = 0;
     float speed = 57.1f;
     while (proj->p[i].type != PROJ_NULL && i < MAX_PROJ)
          i++;
     if (i < MAX_PROJ) {
          proj->p[i].pos  = pos;
          pos = nullv3f;
          degreestovector3d(&pos, mv3f(-rot.x, rot.y, rot.z), nullv3f, speed);
          proj->p[i].type = type;
          proj->p[i].life = 500; /* Temporary value. */
          proj->p[i].vec  = pos;
          proj->a++;
          addSprite(sprites, SPRITE_BULLET, &proj->p[i].pos);
     }
}


float projCollision(struct proj proj)
{
     return 1.0f;
}


void updateProjectiles(struct projA *proj)
{
     int i, j = 0;
     float drag = 0.04f;
     for (i = 0; i < proj->a && i < MAX_PROJ; i++) {
          if (proj->p[i].life <= 0) {
               proj->p[i].life = 0;
               proj->p[i].type = PROJ_NULL;
          }
          else if (projCollision(proj->p[i]) < 0.0f)
               proj->p[i].type = PROJ_COL;
          else {
               proj->p[i].life--;
               j++;
               proj->p[i].pos.x += proj->p[i].vec.x;
               proj->p[i].pos.y += proj->p[i].vec.y;
               proj->p[i].pos.z += proj->p[i].vec.z;
               proj->p[i].vec.x -= proj->p[i].vec.x * drag;
               proj->p[i].vec.y -= proj->p[i].vec.y * drag;
               proj->p[i].vec.z -= proj->p[i].vec.z * drag;
          }
     }
     proj->a = j;
}
