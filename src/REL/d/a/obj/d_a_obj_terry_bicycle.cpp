#include <d/a/obj/d_a_obj_terry_bicycle.h>

SPECIAL_ACTOR_PROFILE(OBJ_TERRY_BIKE, dAcOTerryBicycle_c, fProfile::OBJ_TERRY_BIKE, 0x27A, 0, 2);

STATE_DEFINE(dAcOTerryBicycle_c, Wait);

void dAcOTerryBicycle_c::initializeState_Wait() {}
void dAcOTerryBicycle_c::executeState_Wait() {}
void dAcOTerryBicycle_c::finalizeState_Wait() {}
