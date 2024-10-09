#include <d/a/obj/d_a_obj_fire_pillar.h>

SPECIAL_ACTOR_PROFILE(OBJ_FIRE_PILLAR, dAcOfirePillar_c, fProfile::OBJ_FIRE_PILLAR, 0x1F0, 0, 4);

STATE_DEFINE(dAcOfirePillar_c, Wait);
STATE_DEFINE(dAcOfirePillar_c, WaterWait);
STATE_DEFINE(dAcOfirePillar_c, Vacuum);
STATE_DEFINE(dAcOfirePillar_c, Small);

void dAcOfirePillar_c::initializeState_Wait() {}
void dAcOfirePillar_c::executeState_Wait() {}
void dAcOfirePillar_c::finalizeState_Wait() {}
void dAcOfirePillar_c::initializeState_WaterWait() {}
void dAcOfirePillar_c::executeState_WaterWait() {}
void dAcOfirePillar_c::finalizeState_WaterWait() {}
void dAcOfirePillar_c::initializeState_Vacuum() {}
void dAcOfirePillar_c::executeState_Vacuum() {}
void dAcOfirePillar_c::finalizeState_Vacuum() {}
void dAcOfirePillar_c::initializeState_Small() {}
void dAcOfirePillar_c::executeState_Small() {}
void dAcOfirePillar_c::finalizeState_Small() {}
