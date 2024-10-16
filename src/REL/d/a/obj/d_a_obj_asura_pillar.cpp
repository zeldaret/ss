#include <d/a/obj/d_a_obj_asura_pillar.h>

SPECIAL_ACTOR_PROFILE(ASURA_PILLAR, dAcAsuraPillar_c, fProfile::ASURA_PILLAR, 0x11C, 0, 2);

STATE_DEFINE(dAcAsuraPillar_c, Damage);
STATE_DEFINE(dAcAsuraPillar_c, Wait);

void dAcAsuraPillar_c::initializeState_Damage() {}
void dAcAsuraPillar_c::executeState_Damage() {}
void dAcAsuraPillar_c::finalizeState_Damage() {}
void dAcAsuraPillar_c::initializeState_Wait() {}
void dAcAsuraPillar_c::executeState_Wait() {}
void dAcAsuraPillar_c::finalizeState_Wait() {}
