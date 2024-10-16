#include "d/a/e/d_a_e_gumarm.h"

SPECIAL_ACTOR_PROFILE(E_GUMARM, dAcGumarm_c, fProfile::E_GUMARM, 0x11F, 0, 3);

STATE_DEFINE(dAcGumarm_c, Hide);
STATE_DEFINE(dAcGumarm_c, HideWait);
STATE_DEFINE(dAcGumarm_c, HideWalk);
STATE_DEFINE(dAcGumarm_c, HideMove);
STATE_DEFINE(dAcGumarm_c, Wait);
STATE_DEFINE(dAcGumarm_c, Walk);
STATE_DEFINE(dAcGumarm_c, Appear);
STATE_DEFINE(dAcGumarm_c, KeepRock);
STATE_DEFINE(dAcGumarm_c, AttackMove);
STATE_DEFINE(dAcGumarm_c, Attack);
STATE_DEFINE(dAcGumarm_c, Lost);
STATE_DEFINE(dAcGumarm_c, Find);
STATE_DEFINE(dAcGumarm_c, FreezeStart);
STATE_DEFINE(dAcGumarm_c, Freeze);
STATE_DEFINE(dAcGumarm_c, FreezeEnd);
STATE_DEFINE(dAcGumarm_c, Sink);
STATE_DEFINE(dAcGumarm_c, KeepSink);
STATE_DEFINE(dAcGumarm_c, Dead);

void dAcGumarm_c::initializeState_Hide() {}
void dAcGumarm_c::executeState_Hide() {}
void dAcGumarm_c::finalizeState_Hide() {}
void dAcGumarm_c::initializeState_HideWait() {}
void dAcGumarm_c::executeState_HideWait() {}
void dAcGumarm_c::finalizeState_HideWait() {}
void dAcGumarm_c::initializeState_HideWalk() {}
void dAcGumarm_c::executeState_HideWalk() {}
void dAcGumarm_c::finalizeState_HideWalk() {}
void dAcGumarm_c::initializeState_HideMove() {}
void dAcGumarm_c::executeState_HideMove() {}
void dAcGumarm_c::finalizeState_HideMove() {}
void dAcGumarm_c::initializeState_Wait() {}
void dAcGumarm_c::executeState_Wait() {}
void dAcGumarm_c::finalizeState_Wait() {}
void dAcGumarm_c::initializeState_Walk() {}
void dAcGumarm_c::executeState_Walk() {}
void dAcGumarm_c::finalizeState_Walk() {}
void dAcGumarm_c::initializeState_Appear() {}
void dAcGumarm_c::executeState_Appear() {}
void dAcGumarm_c::finalizeState_Appear() {}
void dAcGumarm_c::initializeState_KeepRock() {}
void dAcGumarm_c::executeState_KeepRock() {}
void dAcGumarm_c::finalizeState_KeepRock() {}
void dAcGumarm_c::initializeState_AttackMove() {}
void dAcGumarm_c::executeState_AttackMove() {}
void dAcGumarm_c::finalizeState_AttackMove() {}
void dAcGumarm_c::initializeState_Attack() {}
void dAcGumarm_c::executeState_Attack() {}
void dAcGumarm_c::finalizeState_Attack() {}
void dAcGumarm_c::initializeState_Lost() {}
void dAcGumarm_c::executeState_Lost() {}
void dAcGumarm_c::finalizeState_Lost() {}
void dAcGumarm_c::initializeState_Find() {}
void dAcGumarm_c::executeState_Find() {}
void dAcGumarm_c::finalizeState_Find() {}
void dAcGumarm_c::initializeState_FreezeStart() {}
void dAcGumarm_c::executeState_FreezeStart() {}
void dAcGumarm_c::finalizeState_FreezeStart() {}
void dAcGumarm_c::initializeState_Freeze() {}
void dAcGumarm_c::executeState_Freeze() {}
void dAcGumarm_c::finalizeState_Freeze() {}
void dAcGumarm_c::initializeState_FreezeEnd() {}
void dAcGumarm_c::executeState_FreezeEnd() {}
void dAcGumarm_c::finalizeState_FreezeEnd() {}
void dAcGumarm_c::initializeState_Sink() {}
void dAcGumarm_c::executeState_Sink() {}
void dAcGumarm_c::finalizeState_Sink() {}
void dAcGumarm_c::initializeState_KeepSink() {}
void dAcGumarm_c::executeState_KeepSink() {}
void dAcGumarm_c::finalizeState_KeepSink() {}
void dAcGumarm_c::initializeState_Dead() {}
void dAcGumarm_c::executeState_Dead() {}
void dAcGumarm_c::finalizeState_Dead() {}
