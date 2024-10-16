#include <d/a/obj/d_a_obj_bb_broken_parts.h>

SPECIAL_ACTOR_PROFILE(OBJ_BB_BROKEN_PARTS, dAcObbBrokenParts_c, fProfile::OBJ_BB_BROKEN_PARTS, 0x215, 0, 3);

STATE_DEFINE(dAcObbBrokenParts_c, Wait);
STATE_DEFINE(dAcObbBrokenParts_c, Break);

void dAcObbBrokenParts_c::initializeState_Wait() {}
void dAcObbBrokenParts_c::executeState_Wait() {}
void dAcObbBrokenParts_c::finalizeState_Wait() {}
void dAcObbBrokenParts_c::initializeState_Break() {}
void dAcObbBrokenParts_c::executeState_Break() {}
void dAcObbBrokenParts_c::finalizeState_Break() {}
