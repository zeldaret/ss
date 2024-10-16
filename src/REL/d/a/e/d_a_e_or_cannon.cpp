#include <d/a/e/d_a_e_or_cannon.h>

SPECIAL_ACTOR_PROFILE(E_OR_CANNON, dAcEorCannon_c, fProfile::E_OR_CANNON, 0xF4, 0, 3);

STATE_DEFINE(dAcEorCannon_c, Wait);
STATE_DEFINE(dAcEorCannon_c, Attack);
STATE_DEFINE(dAcEorCannon_c, Death);

void dAcEorCannon_c::initializeState_Wait() {}
void dAcEorCannon_c::executeState_Wait() {}
void dAcEorCannon_c::finalizeState_Wait() {}
void dAcEorCannon_c::initializeState_Attack() {}
void dAcEorCannon_c::executeState_Attack() {}
void dAcEorCannon_c::finalizeState_Attack() {}
void dAcEorCannon_c::initializeState_Death() {}
void dAcEorCannon_c::executeState_Death() {}
void dAcEorCannon_c::finalizeState_Death() {}
