#include <d/a/obj/d_a_obj_belt_obstacle.h>

SPECIAL_ACTOR_PROFILE(OBJ_BELT_OBSTACLE, dAcObeltObstacle_c, fProfile::OBJ_BELT_OBSTACLE, 0x270, 0, 2);

STATE_DEFINE(dAcObeltObstacle_c, Wait);
STATE_DEFINE(dAcObeltObstacle_c, Init);
STATE_DEFINE(dAcObeltObstacle_c, BeltMove);
STATE_DEFINE(dAcObeltObstacle_c, TeniN);
STATE_DEFINE(dAcObeltObstacle_c, TeniNonly);

void dAcObeltObstacle_c::initializeState_Wait() {}
void dAcObeltObstacle_c::executeState_Wait() {}
void dAcObeltObstacle_c::finalizeState_Wait() {}
void dAcObeltObstacle_c::initializeState_Init() {}
void dAcObeltObstacle_c::executeState_Init() {}
void dAcObeltObstacle_c::finalizeState_Init() {}
void dAcObeltObstacle_c::initializeState_BeltMove() {}
void dAcObeltObstacle_c::executeState_BeltMove() {}
void dAcObeltObstacle_c::finalizeState_BeltMove() {}
void dAcObeltObstacle_c::initializeState_TeniN() {}
void dAcObeltObstacle_c::executeState_TeniN() {}
void dAcObeltObstacle_c::finalizeState_TeniN() {}
void dAcObeltObstacle_c::initializeState_TeniNonly() {}
void dAcObeltObstacle_c::executeState_TeniNonly() {}
void dAcObeltObstacle_c::finalizeState_TeniNonly() {}
