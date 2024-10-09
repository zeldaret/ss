#include <d/a/obj/d_a_obj_roll_pillar.h>

SPECIAL_ACTOR_PROFILE(OBJ_ROLL_PILLAR, dAcORollPillar_c, fProfile::OBJ_ROLL_PILLAR, 0X13C, 0, 3);

STATE_DEFINE(dAcORollPillar_c, Wait);
STATE_DEFINE(dAcORollPillar_c, Rotate);
STATE_DEFINE(dAcORollPillar_c, TimerWait);

void dAcORollPillar_c::initializeState_Wait() {}
void dAcORollPillar_c::executeState_Wait() {}
void dAcORollPillar_c::finalizeState_Wait() {}
void dAcORollPillar_c::initializeState_Rotate() {}
void dAcORollPillar_c::executeState_Rotate() {}
void dAcORollPillar_c::finalizeState_Rotate() {}
void dAcORollPillar_c::initializeState_TimerWait() {}
void dAcORollPillar_c::executeState_TimerWait() {}
void dAcORollPillar_c::finalizeState_TimerWait() {}
