#include <d/a/obj/d_a_obj_stopper_rock.h>

SPECIAL_ACTOR_PROFILE(OBJ_STOPPER_ROCK, dAcOstopperRock_c, fProfile::OBJ_STOPPER_ROCK, 0X26C, 0, 2);

STATE_DEFINE(dAcOstopperRock_c, Wait);
STATE_DEFINE(dAcOstopperRock_c, Bomb);

void dAcOstopperRock_c::initializeState_Wait() {}
void dAcOstopperRock_c::executeState_Wait() {}
void dAcOstopperRock_c::finalizeState_Wait() {}
void dAcOstopperRock_c::initializeState_Bomb() {}
void dAcOstopperRock_c::executeState_Bomb() {}
void dAcOstopperRock_c::finalizeState_Bomb() {}
