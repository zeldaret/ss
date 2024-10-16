#include "d/a/obj/d_a_obj_door_bossD101.h"

SPECIAL_ACTOR_PROFILE(OBJ_DOOR_BOSSD101, dAcODoorBossD101_c, fProfile::OBJ_DOOR_BOSSD101, 0x17D, 0, 3);

STATE_DEFINE(dAcODoorBossD101_c, Wait);
STATE_DEFINE(dAcODoorBossD101_c, End);

void dAcODoorBossD101_c::initializeState_Wait() {}
void dAcODoorBossD101_c::executeState_Wait() {}
void dAcODoorBossD101_c::finalizeState_Wait() {}
void dAcODoorBossD101_c::initializeState_End() {}
void dAcODoorBossD101_c::executeState_End() {}
void dAcODoorBossD101_c::finalizeState_End() {}
