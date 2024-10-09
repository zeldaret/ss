#include <d/a/e/d_a_e_am.h>

SPECIAL_ACTOR_PROFILE(E_AM, dAcEAm_c, fProfile::E_AM, 0XC1, 0, 4131);

STATE_DEFINE(dAcEAm_c, Stop);
STATE_DEFINE(dAcEAm_c, Appear);
STATE_DEFINE(dAcEAm_c, StartWait);
STATE_DEFINE(dAcEAm_c, Normal);
STATE_DEFINE(dAcEAm_c, RetMove);
STATE_DEFINE(dAcEAm_c, LongJump);
STATE_DEFINE(dAcEAm_c, Wait);
STATE_DEFINE(dAcEAm_c, Chance);
STATE_DEFINE(dAcEAm_c, ChanceWait);
STATE_DEFINE(dAcEAm_c, Damage);
STATE_DEFINE(dAcEAm_c, Dead);

void dAcEAm_c::initializeState_Stop() {}
void dAcEAm_c::executeState_Stop() {}
void dAcEAm_c::finalizeState_Stop() {}
void dAcEAm_c::initializeState_Appear() {}
void dAcEAm_c::executeState_Appear() {}
void dAcEAm_c::finalizeState_Appear() {}
void dAcEAm_c::initializeState_StartWait() {}
void dAcEAm_c::executeState_StartWait() {}
void dAcEAm_c::finalizeState_StartWait() {}
void dAcEAm_c::initializeState_Normal() {}
void dAcEAm_c::executeState_Normal() {}
void dAcEAm_c::finalizeState_Normal() {}
void dAcEAm_c::initializeState_RetMove() {}
void dAcEAm_c::executeState_RetMove() {}
void dAcEAm_c::finalizeState_RetMove() {}
void dAcEAm_c::initializeState_LongJump() {}
void dAcEAm_c::executeState_LongJump() {}
void dAcEAm_c::finalizeState_LongJump() {}
void dAcEAm_c::initializeState_Wait() {}
void dAcEAm_c::executeState_Wait() {}
void dAcEAm_c::finalizeState_Wait() {}
void dAcEAm_c::initializeState_Chance() {}
void dAcEAm_c::executeState_Chance() {}
void dAcEAm_c::finalizeState_Chance() {}
void dAcEAm_c::initializeState_ChanceWait() {}
void dAcEAm_c::executeState_ChanceWait() {}
void dAcEAm_c::finalizeState_ChanceWait() {}
void dAcEAm_c::initializeState_Damage() {}
void dAcEAm_c::executeState_Damage() {}
void dAcEAm_c::finalizeState_Damage() {}
void dAcEAm_c::initializeState_Dead() {}
void dAcEAm_c::executeState_Dead() {}
void dAcEAm_c::finalizeState_Dead() {}
