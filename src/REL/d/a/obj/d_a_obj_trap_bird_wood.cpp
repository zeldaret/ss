#include <d/a/obj/d_a_obj_trap_bird_wood.h>

SPECIAL_ACTOR_PROFILE(OBJ_TRAP_BIRD_WOOD, dAcOTrapBirdWood_c, fProfile::OBJ_TRAP_BIRD_WOOD, 0X23F, 0, 7);

STATE_DEFINE(dAcOTrapBirdWood_c, Wait);
STATE_DEFINE(dAcOTrapBirdWood_c, Broken);

void dAcOTrapBirdWood_c::initializeState_Wait() {}
void dAcOTrapBirdWood_c::executeState_Wait() {}
void dAcOTrapBirdWood_c::finalizeState_Wait() {}
void dAcOTrapBirdWood_c::initializeState_Broken() {}
void dAcOTrapBirdWood_c::executeState_Broken() {}
void dAcOTrapBirdWood_c::finalizeState_Broken() {}
