#include <d/a/b/d_a_b_asura_arm.h>

SPECIAL_ACTOR_PROFILE(ASURA_ARM, dAcAsuraArm_c, fProfile::ASURA_ARM, 0X118, 0, 0);

STATE_DEFINE(dAcAsuraArm_c, Pulled);
STATE_DEFINE(dAcAsuraArm_c, Wait);
STATE_DEFINE(dAcAsuraArm_c, Demo);

void dAcAsuraArm_c::initializeState_Pulled() {}
void dAcAsuraArm_c::executeState_Pulled() {}
void dAcAsuraArm_c::finalizeState_Pulled() {}
void dAcAsuraArm_c::initializeState_Wait() {}
void dAcAsuraArm_c::executeState_Wait() {}
void dAcAsuraArm_c::finalizeState_Wait() {}
void dAcAsuraArm_c::initializeState_Demo() {}
void dAcAsuraArm_c::executeState_Demo() {}
void dAcAsuraArm_c::finalizeState_Demo() {}
