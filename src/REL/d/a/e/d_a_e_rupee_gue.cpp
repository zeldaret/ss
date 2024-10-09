#include <d/a/e/d_a_e_rupee_gue.h>

SPECIAL_ACTOR_PROFILE(E_RUPEE_GUE, dAcErupeeGue_c, fProfile::E_RUPEE_GUE, 0X107, 0, 3);

STATE_DEFINE(dAcErupeeGue_c, Wait);
STATE_DEFINE(dAcErupeeGue_c, RunAway);
STATE_DEFINE(dAcErupeeGue_c, Hit);
STATE_DEFINE(dAcErupeeGue_c, GHit);
STATE_DEFINE(dAcErupeeGue_c, Hide);
STATE_DEFINE(dAcErupeeGue_c, Death);

void dAcErupeeGue_c::initializeState_Wait() {}
void dAcErupeeGue_c::executeState_Wait() {}
void dAcErupeeGue_c::finalizeState_Wait() {}
void dAcErupeeGue_c::initializeState_RunAway() {}
void dAcErupeeGue_c::executeState_RunAway() {}
void dAcErupeeGue_c::finalizeState_RunAway() {}
void dAcErupeeGue_c::initializeState_Hit() {}
void dAcErupeeGue_c::executeState_Hit() {}
void dAcErupeeGue_c::finalizeState_Hit() {}
void dAcErupeeGue_c::initializeState_GHit() {}
void dAcErupeeGue_c::executeState_GHit() {}
void dAcErupeeGue_c::finalizeState_GHit() {}
void dAcErupeeGue_c::initializeState_Hide() {}
void dAcErupeeGue_c::executeState_Hide() {}
void dAcErupeeGue_c::finalizeState_Hide() {}
void dAcErupeeGue_c::initializeState_Death() {}
void dAcErupeeGue_c::executeState_Death() {}
void dAcErupeeGue_c::finalizeState_Death() {}
