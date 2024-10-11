#include <d/a/b/d_a_b_asura_foot.h>

SPECIAL_ACTOR_PROFILE(ASURA_FOOT, dAcAsuraFoot_c, fProfile::ASURA_FOOT, 0x119, 0, 0);

STATE_DEFINE(dAcAsuraFoot_c, Pulled);
STATE_DEFINE(dAcAsuraFoot_c, Wait);

void dAcAsuraFoot_c::initializeState_Pulled() {}
void dAcAsuraFoot_c::executeState_Pulled() {}
void dAcAsuraFoot_c::finalizeState_Pulled() {}
void dAcAsuraFoot_c::initializeState_Wait() {}
void dAcAsuraFoot_c::executeState_Wait() {}
void dAcAsuraFoot_c::finalizeState_Wait() {}
