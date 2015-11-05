#include "maths.h"


extern struct v3f nullv3f;

void movePitch(struct v3f *rot, struct v3f norm)
{
     rot->x = (asinf(-norm.z) * 180 / PI) * cosf(rot->y / 180.0f * PI);
     rot->x += (asinf(norm.x) * 180 / PI) * sinf(rot->y / 180.0f * PI);
     rot->z = (asinf(norm.x) * 180 / PI) * cosf(rot->y / 180.0f * PI);
     rot->z += (asinf(norm.z) * 180 / PI) * sinf(rot->y / 180.0f * PI);
}


void movement(struct v3f *cpos, struct v3f *crot, char direction,
              float speed, int tsize, int type)
{
     struct v3f pos = *cpos, norm;
     const float rotSpeed = 2.3f;
     float dir = 0.0f;
     char a = 0;
     if ((direction | INPUT_LEFT_SHIFT) == direction)
          speed *= 2.3f;
     switch (direction & ~(INPUT_LEFT_SHIFT | INPUT_SPACE)) {
     case INPUT_UP:
          a = -1;
          dir = 0.0f;
          break;
     case INPUT_DOWN:
          a = 1;
          dir = 0.0f;
          break;
     case INPUT_LEFT:
          if (type == INPUT_TYPE_PEDESTRIAN) {
               a = 1;
               dir = 90.0f;
          }
          else if (type == INPUT_TYPE_VEHICLE)
               crot->y -= rotSpeed;
          break;
     case INPUT_RIGHT:
          if (type == INPUT_TYPE_PEDESTRIAN) {
               a = 1;
               dir = 270.0f;
          }
          else if (type == INPUT_TYPE_VEHICLE)
               crot->y += rotSpeed;
          break;
     case INPUT_UP_RIGHT:
          a = -1;
          if (type == INPUT_TYPE_PEDESTRIAN)
               dir = 45.0f;
          else if (type == INPUT_TYPE_VEHICLE) {
               dir = 0.0f;
               crot->y += rotSpeed;
          }
          break;
     case INPUT_UP_LEFT:
          a = -1;
          if (type == INPUT_TYPE_PEDESTRIAN)
               dir = -45.0f;
          else if (type == INPUT_TYPE_VEHICLE) {
               dir = 0.0f;
               crot->y -= rotSpeed;
          }
          break;
     case INPUT_DOWN_RIGHT:
          a = 1;
          if (type == INPUT_TYPE_PEDESTRIAN)
               dir = -45.0f;
          else if (type == INPUT_TYPE_VEHICLE) {
               dir = 0.0f;
               crot->y += rotSpeed;
          }
          break;
     case INPUT_DOWN_LEFT:
          a = 1;
          if (type == INPUT_TYPE_PEDESTRIAN)
               dir = 45.0f;
          else if (type == INPUT_TYPE_VEHICLE) {
               dir = 0.0f;
               crot->y -= rotSpeed;
          }
          break;
     default: break;
     }
     if (type == INPUT_TYPE_VEHICLE)
          a *= -1;
     degreestovector3d(&pos, *crot, mv3f(0.0f, dir, 0.0f), a * speed);
     readTerrainHeightPlane(pos.x, pos.z, &norm, tsize);
     if (type == INPUT_TYPE_VEHICLE)
          movePitch(crot, norm);
     degreestovector3d(cpos, *crot, mv3f(0.0f, dir, 0.0f), a * speed);
     cpos->y = readTerrainHeightPlane(cpos->x, cpos->z, &norm, tsize);
     cpos->y = cpos->y < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : cpos->y;
     if (type == INPUT_TYPE_VEHICLE)
          cpos->y += 0.6f;
     else
          cpos->y += 1.8f;
}


void flyMovement(struct unit *unit, char input, int tsize)
{
     struct v3f pos = nullv3f;
     float ground,
           maxThrust,
           maxThrustV, /* VTOL thrust. */
           thrustStep, /* Thrust changes by this amount. */
           thrustC,    /* Thrust ceiling. */
           tLapse,     /* Thrust lapse. */
           aero,       /* Aerodynamics. */
           drag,
           lift,
           glide,
           pressure,   /* Atmospheric pressure. */
           temp;
     switch (unit->type) {
     case UNIT_AIR_FIGHTER_1:
          maxThrust = WORLD_GRAVITY + 0.1f;
          maxThrustV = WORLD_GRAVITY + 0.03f;
          thrustStep = 0.01f;
          thrustC = 21000.0f;
          aero = 0.04f; /* This needs to be less than drag. */
          drag = 0.053f;
          lift = 0.095f;
          glide = 0.035f;
     }
     switch (input & ~INPUT_SPACE) {
     case INPUT_UP:
          unit->p.airp.thrust = unit->p.airp.thrust + thrustStep;
          break;
     case INPUT_DOWN:
          unit->p.airp.thrust = unit->p.airp.thrust - thrustStep;
          break;
     case INPUT_LEFT:
          unit->rot.z -= 5.0f;
          break;
     case INPUT_RIGHT:
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_RIGHT:
          unit->p.airp.thrust = unit->p.airp.thrust + thrustStep;
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_LEFT:
          unit->p.airp.thrust = unit->p.airp.thrust + thrustStep;
          unit->rot.z -= 5.0f;
          break;
     case INPUT_DOWN_RIGHT:
          unit->p.airp.thrust = unit->p.airp.thrust - thrustStep;
          unit->rot.z += 5.0f;
          break;
     case INPUT_DOWN_LEFT:
          unit->p.airp.thrust = unit->p.airp.thrust - thrustStep;
          unit->rot.z -= 5.0f;
          break;
     default: break;
     }
     ground = readTerrainHeightPlane(unit->pos.x, unit->pos.z, &pos, tsize);
     ground = ground < TERRAIN_WATER_LEVEL + 7 ?
          TERRAIN_WATER_LEVEL + 7 : ground;
     /* Airspeed.  The speed of sound is 340.29 metres per second. */
     unit->p.airp.speed = distance3d(nullv3f, unit->vec);
     pressure = unit->pos.y < 30000.0f ?
          (30000.0f - unit->pos.y) / 30000.0f : 0.0f;
     /* Thrust lapse, atmospheric density.  See
        https://en.wikipedia.org/wiki/Jet_engine_performance */
     tLapse = unit->pos.y < thrustC ?
          (thrustC - unit->pos.y) / (thrustC * 0.7f) : 0.0f;
     tLapse = tLapse > 1.0f ? 1.0f : tLapse;
     pos = nullv3f;
     /* VTOL thrust. */
     if ((input | INPUT_SPACE) == input) {
          unit->p.airp.vtolThrust = unit->p.airp.vtolThrust + 0.05f >
               maxThrustV ? maxThrustV : unit->p.airp.vtolThrust + 0.05f;
          degreestovector3d(&pos, unit->rot, mv3f(90.0f, 180.0f, 0.0f),
                            unit->p.airp.vtolThrust * tLapse);
     }
     else
          unit->p.airp.vtolThrust = unit->p.airp.vtolThrust - 0.025f >
               0.0f ? unit->p.airp.vtolThrust - 0.025f : 0.0f;
     /* Normal thrust. */
     clamp(&unit->p.airp.thrust, 0.0f, maxThrust);
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->p.airp.thrust * tLapse +
                       unit->p.airp.speed * aero * pressure);
     /* Update position and vector. */
     unit->vec.x += pos.x;
     unit->vec.y += pos.y - WORLD_GRAVITY;
     unit->vec.z += pos.z;
     unit->pos.x += unit->vec.x;
     unit->pos.y += unit->vec.y;
     unit->pos.z += unit->vec.z;
     /* Drag. */
     pos = nullv3f;
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f), 1.0f);
     temp = 1 - distance3d(pos, normalize3d(unit->vec)) / 2.0f;
     drag = drag * (pressure + 0.01f) * temp;
     /* if (drag < 0.0f) */
     /*   drag = 0.0f; */
     /* else if (drag > 0.9f) */
     /*   drag = 0.9f; */
     unit->vec.x -= unit->vec.x * drag;
     unit->vec.y -= unit->vec.y * drag;
     unit->vec.z -= unit->vec.z * drag;
     /* Lift and glide. */
     pos = nullv3f;
     lift *= fabs(unit->rot.x) < 50.0f ?
          (50.0f - fabs(unit->rot.x)) / 50.0f : 0.0f;
     degreestovector3d(&pos, unit->rot, mv3f(90.0f, 180.0f, 0.0f),
                       sqrt(unit->p.airp.speed) * temp * pressure * lift);
     temp = unit->vec.y + WORLD_GRAVITY;
     if (temp < 0.0f && unit->p.airp.speed < 40)
          degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                            -temp * pressure * glide);
     unit->vec.x += pos.x;
     unit->vec.y += pos.y;
     unit->vec.z += pos.z;
     /* Aircraft banking. */
     pos = nullv3f;
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->p.airp.speed);
     temp = (vectorstodegree2d(pos, nullv3f) - vectorstodegree2d
             (unit->vec, nullv3f)) * 0.09f;
     if (vectorstodegree2d(pos, nullv3f) > vectorstodegree2d
         (unit->vec, nullv3f) + 0.8f)
          unit->rot.z += temp > 7 ? 0 : temp;
     else if (vectorstodegree2d(unit->vec, nullv3f) > vectorstodegree2d
              (pos, nullv3f) + 0.8f)
          unit->rot.z += temp < -7 ? 0 : temp;
     unit->rot.z -= unit->rot.z * 0.12f;
     unit->rot.z = unit->rot.z > 70 ? 70 :
          unit->rot.z < -70 ? -70 : unit->rot.z;
     if (unit->rot.y > 360.0f)
          unit->rot.y -= 360.0f;
     else if (unit->rot.y < 0.0f)
          unit->rot.y += 360.0f;
     /* Ground detection and collision handling. */
     unit->p.airp.height = fabs(unit->pos.y - ground);
     if (unit->pos.y < ground) {
          unit->pos.y = ground;
          unit->rot.z = 0.0f;
          unit->vec.y *= 0.2f;
          unit->vec.x -= unit->vec.x * 0.2f;
          unit->vec.z -= unit->vec.z * 0.2f;
          if (unit->vec.y < -WORLD_GRAVITY * 0.7f ||
              unit->p.airp.speed > 11) {
               unit->vec.x -= unit->vec.x * 0.5f;
               unit->vec.y = 0.0f;
               unit->vec.z -= unit->vec.z * 0.5f;
               unit->p.airp.thrust = 0.0f;
          }
     }
}


void airUnitMove(struct unit *unit, struct v3f pos, int tsize)
{
     struct v3f temp1 = unit->pos;
     struct v3f temp2 = unit->pos;
     struct v3f temp3 = unit->pos;
     struct v3f temp4 = unit->pos;
     struct v3f temp5 = unit->pos;
     float dist = distance2d(unit->pos, pos);
     float speed = dist < 7000.0f ? 35.0f : 55.0f;
     int thrust;
     unit->rot.y += (vectorstodegree2d(unit->pos, pos) - unit->rot.y) * 0.1f;
     degreestovector3d(&temp1, unit->rot, nullv3f, 7500.0f);
     degreestovector3d(&temp2, unit->rot, nullv3f, 6000.0f);
     degreestovector3d(&temp3, unit->rot, nullv3f, 4500.0f);
     degreestovector3d(&temp4, unit->rot, nullv3f, 3000.0f);
     degreestovector3d(&temp5, unit->rot, nullv3f, 1500.0f);
     if (unit->p.airp.speed > speed + 20.0f)
          thrust = INPUT_DOWN;
     else if (unit->p.airp.speed > speed)
          thrust = INPUT_NONE;
     else
          thrust = INPUT_UP;
     if (readTerrainHeight(temp1.x, temp1.z) > unit->pos.y ||
         readTerrainHeight(temp2.x, temp2.z) > unit->pos.y ||
         readTerrainHeight(temp3.x, temp3.z) > unit->pos.y ||
         readTerrainHeight(temp4.x, temp4.z) > unit->pos.y ||
         readTerrainHeight(temp5.x, temp5.z) > unit->pos.y) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (-40.0f - unit->rot.x) * 0.1f;
          unit->rot.y += 2.5f;
     }
     else if (unit->vec.y < -WORLD_GRAVITY * 2.5f
              && unit->p.airp.height < 2500.0f) {
          flyMovement(unit, INPUT_SPACE + thrust, tsize);
          unit->rot.x += (-25.0f - unit->rot.x) * 0.05f;
     }
     else if (dist < 5000.0f) {
          flyMovement(unit, INPUT_NONE, tsize);
          unit->rot.x += (-15.0f - unit->rot.x) * 0.1f;
     }
     else if (unit->p.airp.height < 1200.0f) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (-10.0f - unit->rot.x) * 0.1f;
     }
     else if (unit->p.airp.height > 10000.0f) {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (3.5f - unit->rot.x) * 0.02f;
     }
     else {
          flyMovement(unit, thrust, tsize);
          unit->rot.x += (0.0f - unit->rot.x) * 0.03f;
     }
}


void airUnitMoveVTOL(struct unit *unit, struct v3f pos, int tsize)
{
     float dist = distance2d(unit->pos, pos);
     float pitch = unit->p.airp.speed > 20.0f ? -5.0f : 7.5f;
     int thrust = unit->p.airp.thrust > 0.0f ? INPUT_DOWN : INPUT_NONE;
     if (dist > 200.0f)
          unit->rot.y += (vectorstodegree2d
                          (unit->pos, pos) - unit->rot.y) * 0.1f;
     else
          unit->rot.y += 2.0f;
     if (unit->p.airp.height > 150.0f) {
          if (unit->vec.y < -WORLD_GRAVITY - 7.0f)
               flyMovement(unit, INPUT_SPACE + thrust, tsize);
          else
               flyMovement(unit, INPUT_NONE + thrust, tsize);
          unit->rot.x += (pitch - unit->rot.x) * 0.1f;
     }
     else if (unit->vec.y < -WORLD_GRAVITY || unit->p.airp.height < 70.0f) {
          flyMovement(unit, INPUT_SPACE + thrust, tsize);
          unit->rot.x += (pitch - unit->rot.x) * 0.1f;
     }
     else {
          flyMovement(unit, INPUT_NONE + thrust, tsize);
          unit->rot.x += (0.0f - unit->rot.x) * 0.05f;
     }
}
