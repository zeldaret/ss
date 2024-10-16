#include <d/a/e/d_a_e_tn2.h>

SPECIAL_ACTOR_PROFILE(E_TN2, dAcEtn2_c, fProfile::E_TN2, 0x109, 0, 3);

STATE_DEFINE(dAcEtn2_c, ArmorAttack);
STATE_DEFINE(dAcEtn2_c, ArmorWait);
STATE_DEFINE(dAcEtn2_c, ArmorWalk);

void dAcEtn2_c::initializeState_ArmorAttack() {}
void dAcEtn2_c::executeState_ArmorAttack() {}
void dAcEtn2_c::finalizeState_ArmorAttack() {}
void dAcEtn2_c::initializeState_ArmorWait() {}
void dAcEtn2_c::executeState_ArmorWait() {}
void dAcEtn2_c::finalizeState_ArmorWait() {}
void dAcEtn2_c::initializeState_ArmorWalk() {}
void dAcEtn2_c::executeState_ArmorWalk() {}
void dAcEtn2_c::finalizeState_ArmorWalk() {}
