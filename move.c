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
              float speed, int type)
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
               crot->y -= rotSpeed;
          }
          break;
     case INPUT_DOWN_LEFT:
          a = 1;
          if (type == INPUT_TYPE_PEDESTRIAN)
               dir = 45.0f;
          else if (type == INPUT_TYPE_VEHICLE) {
               dir = 0.0f;
               crot->y += rotSpeed;
          }
          break;
     default: break;
     }
     if (type == INPUT_TYPE_VEHICLE)
          a *= -1;
     degreestovector3d(&pos, *crot, mv3f(0.0f, dir, 0.0f), a * speed);
     readTerrainHeightPlane(pos.x, pos.z, &norm);
     if (type == INPUT_TYPE_VEHICLE)
          movePitch(crot, norm);
     degreestovector3d(cpos, *crot, mv3f(0.0f, dir, 0.0f), a * speed);
     cpos->y = readTerrainHeightPlane(cpos->x, cpos->z, &norm);
     cpos->y = cpos->y < TERRAIN_WATER_LEVEL ? TERRAIN_WATER_LEVEL : cpos->y;
     if (type == INPUT_TYPE_VEHICLE)
          cpos->y += 0.5f;
     else
          cpos->y += 1.7f;
}


void flyMovement(struct unit *unit, char input)
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
          unit->p.airv.thrust = unit->p.airv.thrust + thrustStep;
          break;
     case INPUT_DOWN:
          unit->p.airv.thrust = unit->p.airv.thrust - thrustStep;
          break;
     case INPUT_LEFT:
          unit->rot.z -= 5.0f;
          break;
     case INPUT_RIGHT:
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_RIGHT:
          unit->p.airv.thrust = unit->p.airv.thrust + thrustStep;
          unit->rot.z += 5.0f;
          break;
     case INPUT_UP_LEFT:
          unit->p.airv.thrust = unit->p.airv.thrust + thrustStep;
          unit->rot.z -= 5.0f;
          break;
     case INPUT_DOWN_RIGHT:
          unit->p.airv.thrust = unit->p.airv.thrust - thrustStep;
          unit->rot.z += 5.0f;
          break;
     case INPUT_DOWN_LEFT:
          unit->p.airv.thrust = unit->p.airv.thrust - thrustStep;
          unit->rot.z -= 5.0f;
          break;
     default: break;
     }
     ground = readTerrainHeightPlane(unit->pos.x, unit->pos.z, &pos);
     ground = ground < TERRAIN_WATER_LEVEL + 7 ?
          TERRAIN_WATER_LEVEL + 7 : ground;
     /* Airspeed.  The speed of sound is 340.29 metres per second. */
     unit->p.airv.speed = distance3d(nullv3f, unit->vec);
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
          unit->p.airv.vtolThrust = unit->p.airv.vtolThrust + 0.05f >
               maxThrustV ? maxThrustV : unit->p.airv.vtolThrust + 0.05f;
          degreestovector3d(&pos, unit->rot, mv3f(90.0f, 180.0f, 0.0f),
                            unit->p.airv.vtolThrust * tLapse);
     }
     else
          unit->p.airv.vtolThrust = unit->p.airv.vtolThrust - 0.025f >
               0.0f ? unit->p.airv.vtolThrust - 0.025f : 0.0f;
     /* Normal thrust. */
     clamp(&unit->p.airv.thrust, 0.0f, maxThrust);
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->p.airv.thrust * tLapse +
                       unit->p.airv.speed * aero * pressure);
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
                       sqrt(unit->p.airv.speed) * temp * pressure * lift);
     temp = unit->vec.y + WORLD_GRAVITY;
     if (temp < 0.0f && unit->p.airv.speed < 40)
          degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                            -temp * pressure * glide);
     unit->vec.x += pos.x;
     unit->vec.y += pos.y;
     unit->vec.z += pos.z;
     /* Aircraft banking. */
     pos = nullv3f;
     degreestovector3d(&pos, unit->rot, mv3f(180.0f, 180.0f, 0.0f),
                       unit->p.airv.speed);
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
     unit->p.airv.height = fabs(unit->pos.y - ground);
     if (unit->pos.y < ground) {
          unit->pos.y = ground;
          unit->rot.z = 0.0f;
          unit->vec.y *= 0.2f;
          unit->vec.x -= unit->vec.x * 0.2f;
          unit->vec.z -= unit->vec.z * 0.2f;
          if (unit->vec.y < -WORLD_GRAVITY * 0.7f ||
              unit->p.airv.speed > 11) {
               unit->vec.x -= unit->vec.x * 0.5f;
               unit->vec.y = 0.0f;
               unit->vec.z -= unit->vec.z * 0.5f;
               unit->p.airv.thrust = 0.0f;
          }
     }
}


void airUnitMove(struct unit *unit, struct v3f pos)
{
     struct v3f tPos = unit->pos;
     float dist = distance2d(unit->pos, pos);
     float fHeight, t1;
     int speed, thrust;
     /* Terrain look-ahead. */
     degreestovector3d(&tPos, unit->rot, nullv3f,
                       unit->p.airv.speed * 50);
     fHeight = readTerrainHeightPlane2(tPos.x, tPos.z);
     /* Heading (y rotation). */
     if (dist < 750.0f)
          unit->rot.y += 1.1f;
     else {
          t1 = vectorstodegree2d(unit->pos, pos) - unit->rot.y;
          clamp(&t1, -85.0f, 85.0f);
          unit->rot.y += t1 * 0.05f;
     }
     /* Desired speed. */
     if (unit->p.airv.height < 410.0f || dist < 1000.0f)
          speed = 5;
     else if (unit->p.airv.height < 750.0f || dist < 2000.0f)
          speed = 10;
     else if (unit->p.airv.height < 1250.0f || dist < 5000.0f)
          speed = 15;
     else if (unit->p.airv.height > 1500.0f && dist < 10000.0f)
          speed = 25;
     else
          speed = 250;
     /* Thrust. */
     if (unit->p.airv.speed > speed + 5.0f)
          thrust = INPUT_DOWN;
     else if (unit->p.airv.speed > speed)
          thrust = INPUT_NONE;
     else
          thrust = INPUT_UP;
     thrust += unit->p.airv.height < 250.0f ||
          unit->pos.y < fHeight ? INPUT_SPACE : 0;
     /* Pitch. */
     if (unit->pos.y < fHeight && unit->p.airv.speed > 5)
          t1 = -25.0f;
     else if (unit->pos.y + 200.0f < fHeight
              && unit->p.airv.speed > 10)
          t1 = -12.0f;
     else if (unit->p.airv.height > 1000.0f && unit->p.airv.speed < 6)
          t1 = 7.0f;
     else if (unit->p.airv.height > 5000.0f && unit->p.airv.speed > 13)
          t1 = 1.0f;
     else
          t1 = 0.0f;
     unit->rot.x += (t1 - unit->rot.x) * 0.05f;
     flyMovement(unit, thrust);
}
