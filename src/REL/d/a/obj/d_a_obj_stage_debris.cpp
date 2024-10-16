#include <d/a/obj/d_a_obj_stage_debris.h>

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_DEBRIS, dAcOstageDebris_c, fProfile::OBJ_STAGE_DEBRIS, 0x1F8, 0, 7);

STATE_DEFINE(dAcOstageDebris_c, Wait);
STATE_DEFINE(dAcOstageDebris_c, Break);
STATE_DEFINE(dAcOstageDebris_c, After);

void dAcOstageDebris_c::initializeState_Wait() {}
void dAcOstageDebris_c::executeState_Wait() {}
void dAcOstageDebris_c::finalizeState_Wait() {}
void dAcOstageDebris_c::initializeState_Break() {}
void dAcOstageDebris_c::executeState_Break() {}
void dAcOstageDebris_c::finalizeState_Break() {}
void dAcOstageDebris_c::initializeState_After() {}
void dAcOstageDebris_c::executeState_After() {}
void dAcOstageDebris_c::finalizeState_After() {}
