#include <stdlib.h>
#include "maths.h"


extern struct v3f nullv3f;

void addSprite(struct spriteA *sprite, int type, struct v3f *pos)
{
     int i = 0;
     while (sprite->p[i].type != SPRITE_NULL && i < MAX_SPRITE)
          i++;
     if (i < MAX_SPRITE) {
          sprite->p[i].type = type;
          sprite->p[i].life = 500; /* Temporary value. */
          /* sprite->p[i].pos  = malloc(sizeof(struct v3f)); */
          /* *(*sprite).p[i].pos = *pos; */
          sprite->p[i].pos = pos;
          sprite->a++;
     }
}


void updateSprites(struct spriteA *sprite)
{
     int i, j = 0;
     for (i = 0; i < sprite->a && i < MAX_SPRITE; i++) {
          if (sprite->p[i].life <= 0) {
               sprite->p[i].life = 0;
               sprite->p[i].type = SPRITE_NULL;
          }
          else {
               sprite->p[i].life--;
               j++;
          }
     }
     sprite->a = j;
}
