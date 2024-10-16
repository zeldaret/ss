#include <d/a/obj/d_a_obj_tackle.h>

SPECIAL_ACTOR_PROFILE(OBJ_TACKLE, dAcOTackle_c, fProfile::OBJ_TACKLE, 0x240, 0, 6);

STATE_DEFINE(dAcOTackle_c, Wait);
STATE_DEFINE(dAcOTackle_c, Broken);
STATE_DEFINE(dAcOTackle_c, ResqueMole);

void dAcOTackle_c::initializeState_Wait() {}
void dAcOTackle_c::executeState_Wait() {}
void dAcOTackle_c::finalizeState_Wait() {}
void dAcOTackle_c::initializeState_Broken() {}
void dAcOTackle_c::executeState_Broken() {}
void dAcOTackle_c::finalizeState_Broken() {}
void dAcOTackle_c::initializeState_ResqueMole() {}
void dAcOTackle_c::executeState_ResqueMole() {}
void dAcOTackle_c::finalizeState_ResqueMole() {}
