#include <stdlib.h>
#include "common.h"


extern struct v3f nullv3f;

void initUnitList(struct unit **list)
{
     *list = malloc(sizeof(struct unit));
     (*list)->type = UNIT_START_LIST;
     (*list)->next = malloc(sizeof(struct unit));
     (*list)->next->type = UNIT_END_LIST;
}


void addUnit(struct unit **list, int type, struct v3f pos)
{
     while ((*list)->type != UNIT_END_LIST)
          (*list) = (*list)->next;
     (*list)->next = malloc(sizeof(struct unit));
     (*list)->next->type = UNIT_END_LIST;
     (*list)->type = type;
     (*list)->pos  = pos;
     (*list)->rot  = nullv3f;
     (*list)->vec  = nullv3f;
}


void addUnitAir(struct unit **list, int type, struct v3f pos)
{
     struct unit *p = *list;
     addUnit(list, type, pos);
     (*list)->p.airv.thrust     = 0.0f;
     (*list)->p.airv.vtolThrust = 0.0f;
     (*list)->p.airv.speed      = 0.0f;
     (*list)->p.airv.height     = 0.0f;
     (*list) = p;
}


void addUnitGround(struct unit **list, int type, struct v3f pos)
{
     struct unit *p = *list;
     addUnit(list, type, pos);
     (*list)->p.groundv.turret = 0.0f;
     (*list) = p;
}


void addUnitBuilding(struct unit **list, int type, struct v3f pos)
{
     struct unit *p = *list;
     addUnit(list, type, pos);
     (*list)->p.buildv.state = 0;
     (*list) = p;
}
