#include <d/a/obj/d_a_obj_asura_sword.h>

SPECIAL_ACTOR_PROFILE(ASURA_SWORD, dAcAsuraSword_c, fProfile::ASURA_SWORD, 0x11B, 0, 0);

STATE_DEFINE(dAcAsuraSword_c, Carried);
STATE_DEFINE(dAcAsuraSword_c, Stop);
STATE_DEFINE(dAcAsuraSword_c, Wait);

void dAcAsuraSword_c::initializeState_Carried() {}
void dAcAsuraSword_c::executeState_Carried() {}
void dAcAsuraSword_c::finalizeState_Carried() {}
void dAcAsuraSword_c::initializeState_Stop() {}
void dAcAsuraSword_c::executeState_Stop() {}
void dAcAsuraSword_c::finalizeState_Stop() {}
void dAcAsuraSword_c::initializeState_Wait() {}
void dAcAsuraSword_c::executeState_Wait() {}
void dAcAsuraSword_c::finalizeState_Wait() {}
