#include <d/a/obj/d_a_obj_island_nusi.h>

SPECIAL_ACTOR_PROFILE(OBJ_ISLAND_NUSI, dAcOislandNusi_c, fProfile::OBJ_ISLAND_NUSI, 0X204, 0, 3);

STATE_DEFINE(dAcOislandNusi_c, Wait);
STATE_DEFINE(dAcOislandNusi_c, NusiFight);

void dAcOislandNusi_c::initializeState_Wait() {}
void dAcOislandNusi_c::executeState_Wait() {}
void dAcOislandNusi_c::finalizeState_Wait() {}
void dAcOislandNusi_c::initializeState_NusiFight() {}
void dAcOislandNusi_c::executeState_NusiFight() {}
void dAcOislandNusi_c::finalizeState_NusiFight() {}
