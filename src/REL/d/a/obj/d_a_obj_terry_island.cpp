#include <d/a/obj/d_a_obj_terry_island.h>

SPECIAL_ACTOR_PROFILE(OBJ_TERRY_ISLAND, dAcOterryIsland_c, fProfile::OBJ_TERRY_ISLAND, 0x1E7, 0, 3);

STATE_DEFINE(dAcOterryIsland_c, Wait);
STATE_DEFINE(dAcOterryIsland_c, Vanish);
STATE_DEFINE(dAcOterryIsland_c, Vacuum);
STATE_DEFINE(dAcOterryIsland_c, Smoke);

void dAcOterryIsland_c::initializeState_Wait() {}
void dAcOterryIsland_c::executeState_Wait() {}
void dAcOterryIsland_c::finalizeState_Wait() {}
void dAcOterryIsland_c::initializeState_Vanish() {}
void dAcOterryIsland_c::executeState_Vanish() {}
void dAcOterryIsland_c::finalizeState_Vanish() {}
void dAcOterryIsland_c::initializeState_Vacuum() {}
void dAcOterryIsland_c::executeState_Vacuum() {}
void dAcOterryIsland_c::finalizeState_Vacuum() {}
void dAcOterryIsland_c::initializeState_Smoke() {}
void dAcOterryIsland_c::executeState_Smoke() {}
void dAcOterryIsland_c::finalizeState_Smoke() {}
