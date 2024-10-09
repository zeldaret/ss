#include <d/a/obj/d_a_obj_fire_obstacle.h>

SPECIAL_ACTOR_PROFILE(OBJ_FIRE_OBSTACLE, dAcOfireObstacle_c, fProfile::OBJ_FIRE_OBSTACLE, 0X1EF, 0, 0);

STATE_DEFINE(dAcOfireObstacle_c, Wait);
STATE_DEFINE(dAcOfireObstacle_c, Vanish);

void dAcOfireObstacle_c::initializeState_Wait() {}
void dAcOfireObstacle_c::executeState_Wait() {}
void dAcOfireObstacle_c::finalizeState_Wait() {}
void dAcOfireObstacle_c::initializeState_Vanish() {}
void dAcOfireObstacle_c::executeState_Vanish() {}
void dAcOfireObstacle_c::finalizeState_Vanish() {}
