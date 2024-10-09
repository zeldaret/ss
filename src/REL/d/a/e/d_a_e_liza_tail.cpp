#include <d/a/e/d_a_e_liza_tail.h>

SPECIAL_ACTOR_PROFILE(E_LIZA_TAIL, dAcEliza_tail_c, fProfile::E_LIZA_TAIL, 0XE3, 0, 3);

STATE_DEFINE(dAcEliza_tail_c, Wait);
STATE_DEFINE(dAcEliza_tail_c, Damage);
STATE_DEFINE(dAcEliza_tail_c, WhipCatch);
STATE_DEFINE(dAcEliza_tail_c, Get);
STATE_DEFINE(dAcEliza_tail_c, InLava);

void dAcEliza_tail_c::initializeState_Wait() {}
void dAcEliza_tail_c::executeState_Wait() {}
void dAcEliza_tail_c::finalizeState_Wait() {}
void dAcEliza_tail_c::initializeState_Damage() {}
void dAcEliza_tail_c::executeState_Damage() {}
void dAcEliza_tail_c::finalizeState_Damage() {}
void dAcEliza_tail_c::initializeState_WhipCatch() {}
void dAcEliza_tail_c::executeState_WhipCatch() {}
void dAcEliza_tail_c::finalizeState_WhipCatch() {}
void dAcEliza_tail_c::initializeState_Get() {}
void dAcEliza_tail_c::executeState_Get() {}
void dAcEliza_tail_c::finalizeState_Get() {}
void dAcEliza_tail_c::initializeState_InLava() {}
void dAcEliza_tail_c::executeState_InLava() {}
void dAcEliza_tail_c::finalizeState_InLava() {}
