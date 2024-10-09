#include <d/a/obj/d_a_obj_bell.h>

SPECIAL_ACTOR_PROFILE(OBJ_BELL, dAcObell_c, fProfile::OBJ_BELL, 0X208, 0, 2);

STATE_DEFINE(dAcObell_c, Wait);
STATE_DEFINE(dAcObell_c, WaitRupee);
STATE_DEFINE(dAcObell_c, After);

void dAcObell_c::initializeState_Wait() {}
void dAcObell_c::executeState_Wait() {}
void dAcObell_c::finalizeState_Wait() {}
void dAcObell_c::initializeState_WaitRupee() {}
void dAcObell_c::executeState_WaitRupee() {}
void dAcObell_c::finalizeState_WaitRupee() {}
void dAcObell_c::initializeState_After() {}
void dAcObell_c::executeState_After() {}
void dAcObell_c::finalizeState_After() {}
