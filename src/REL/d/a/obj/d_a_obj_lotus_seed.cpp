#include "d/a/obj/d_a_obj_lotus_seed.h"

SPECIAL_ACTOR_PROFILE(OBJ_LOTUS_SEED, dAcOLotusSeed_c, fProfile::OBJ_LOTUS_SEED, 0x175, 0, 3);

STATE_DEFINE(dAcOLotusSeed_c, Init);
STATE_DEFINE(dAcOLotusSeed_c, Stick);
STATE_DEFINE(dAcOLotusSeed_c, Wait);
STATE_DEFINE(dAcOLotusSeed_c, CarryGrab);
STATE_DEFINE(dAcOLotusSeed_c, CarryWhip);
STATE_DEFINE(dAcOLotusSeed_c, CarryBeetle);
STATE_DEFINE(dAcOLotusSeed_c, Dead);

void dAcOLotusSeed_c::initializeState_Init() {}
void dAcOLotusSeed_c::executeState_Init() {}
void dAcOLotusSeed_c::finalizeState_Init() {}
void dAcOLotusSeed_c::initializeState_Stick() {}
void dAcOLotusSeed_c::executeState_Stick() {}
void dAcOLotusSeed_c::finalizeState_Stick() {}
void dAcOLotusSeed_c::initializeState_Wait() {}
void dAcOLotusSeed_c::executeState_Wait() {}
void dAcOLotusSeed_c::finalizeState_Wait() {}
void dAcOLotusSeed_c::initializeState_CarryGrab() {}
void dAcOLotusSeed_c::executeState_CarryGrab() {}
void dAcOLotusSeed_c::finalizeState_CarryGrab() {}
void dAcOLotusSeed_c::initializeState_CarryWhip() {}
void dAcOLotusSeed_c::executeState_CarryWhip() {}
void dAcOLotusSeed_c::finalizeState_CarryWhip() {}
void dAcOLotusSeed_c::initializeState_CarryBeetle() {}
void dAcOLotusSeed_c::executeState_CarryBeetle() {}
void dAcOLotusSeed_c::finalizeState_CarryBeetle() {}
void dAcOLotusSeed_c::initializeState_Dead() {}
void dAcOLotusSeed_c::executeState_Dead() {}
void dAcOLotusSeed_c::finalizeState_Dead() {}
