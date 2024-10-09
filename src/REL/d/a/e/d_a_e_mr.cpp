#include <d/a/e/d_a_e_mr.h>

SPECIAL_ACTOR_PROFILE(E_MR, dAcEmr_c, fProfile::E_MR, 0X11E, 0, 3);

STATE_DEFINE(dAcEmr_c, Wait);
STATE_DEFINE(dAcEmr_c, Walk);
STATE_DEFINE(dAcEmr_c, Find);
STATE_DEFINE(dAcEmr_c, BattleMove);
STATE_DEFINE(dAcEmr_c, GuardSt);
STATE_DEFINE(dAcEmr_c, GuardEnd);
STATE_DEFINE(dAcEmr_c, Stick);
STATE_DEFINE(dAcEmr_c, BattleNoShieldMove);
STATE_DEFINE(dAcEmr_c, Swing);
STATE_DEFINE(dAcEmr_c, Rush);
STATE_DEFINE(dAcEmr_c, GuardJust);
STATE_DEFINE(dAcEmr_c, Lost);
STATE_DEFINE(dAcEmr_c, ShieldCut);
STATE_DEFINE(dAcEmr_c, ShieldThrow);
STATE_DEFINE(dAcEmr_c, ShieldOut);
STATE_DEFINE(dAcEmr_c, Dead);
STATE_DEFINE(dAcEmr_c, Piyori);
STATE_DEFINE(dAcEmr_c, WhipPiyori);
STATE_DEFINE(dAcEmr_c, BombReaction);
STATE_DEFINE(dAcEmr_c, Yoroke);
STATE_DEFINE(dAcEmr_c, EventWait);
STATE_DEFINE(dAcEmr_c, Damage);
STATE_DEFINE(dAcEmr_c, PathWait);
STATE_DEFINE(dAcEmr_c, PathWalk);
STATE_DEFINE(dAcEmr_c, Home);
STATE_DEFINE(dAcEmr_c, PathHome);

void dAcEmr_c::initializeState_Wait() {}
void dAcEmr_c::executeState_Wait() {}
void dAcEmr_c::finalizeState_Wait() {}
void dAcEmr_c::initializeState_Walk() {}
void dAcEmr_c::executeState_Walk() {}
void dAcEmr_c::finalizeState_Walk() {}
void dAcEmr_c::initializeState_Find() {}
void dAcEmr_c::executeState_Find() {}
void dAcEmr_c::finalizeState_Find() {}
void dAcEmr_c::initializeState_BattleMove() {}
void dAcEmr_c::executeState_BattleMove() {}
void dAcEmr_c::finalizeState_BattleMove() {}
void dAcEmr_c::initializeState_GuardSt() {}
void dAcEmr_c::executeState_GuardSt() {}
void dAcEmr_c::finalizeState_GuardSt() {}
void dAcEmr_c::initializeState_GuardEnd() {}
void dAcEmr_c::executeState_GuardEnd() {}
void dAcEmr_c::finalizeState_GuardEnd() {}
void dAcEmr_c::initializeState_Stick() {}
void dAcEmr_c::executeState_Stick() {}
void dAcEmr_c::finalizeState_Stick() {}
void dAcEmr_c::initializeState_BattleNoShieldMove() {}
void dAcEmr_c::executeState_BattleNoShieldMove() {}
void dAcEmr_c::finalizeState_BattleNoShieldMove() {}
void dAcEmr_c::initializeState_Swing() {}
void dAcEmr_c::executeState_Swing() {}
void dAcEmr_c::finalizeState_Swing() {}
void dAcEmr_c::initializeState_Rush() {}
void dAcEmr_c::executeState_Rush() {}
void dAcEmr_c::finalizeState_Rush() {}
void dAcEmr_c::initializeState_GuardJust() {}
void dAcEmr_c::executeState_GuardJust() {}
void dAcEmr_c::finalizeState_GuardJust() {}
void dAcEmr_c::initializeState_Lost() {}
void dAcEmr_c::executeState_Lost() {}
void dAcEmr_c::finalizeState_Lost() {}
void dAcEmr_c::initializeState_ShieldCut() {}
void dAcEmr_c::executeState_ShieldCut() {}
void dAcEmr_c::finalizeState_ShieldCut() {}
void dAcEmr_c::initializeState_ShieldThrow() {}
void dAcEmr_c::executeState_ShieldThrow() {}
void dAcEmr_c::finalizeState_ShieldThrow() {}
void dAcEmr_c::initializeState_ShieldOut() {}
void dAcEmr_c::executeState_ShieldOut() {}
void dAcEmr_c::finalizeState_ShieldOut() {}
void dAcEmr_c::initializeState_Dead() {}
void dAcEmr_c::executeState_Dead() {}
void dAcEmr_c::finalizeState_Dead() {}
void dAcEmr_c::initializeState_Piyori() {}
void dAcEmr_c::executeState_Piyori() {}
void dAcEmr_c::finalizeState_Piyori() {}
void dAcEmr_c::initializeState_WhipPiyori() {}
void dAcEmr_c::executeState_WhipPiyori() {}
void dAcEmr_c::finalizeState_WhipPiyori() {}
void dAcEmr_c::initializeState_BombReaction() {}
void dAcEmr_c::executeState_BombReaction() {}
void dAcEmr_c::finalizeState_BombReaction() {}
void dAcEmr_c::initializeState_Yoroke() {}
void dAcEmr_c::executeState_Yoroke() {}
void dAcEmr_c::finalizeState_Yoroke() {}
void dAcEmr_c::initializeState_EventWait() {}
void dAcEmr_c::executeState_EventWait() {}
void dAcEmr_c::finalizeState_EventWait() {}
void dAcEmr_c::initializeState_Damage() {}
void dAcEmr_c::executeState_Damage() {}
void dAcEmr_c::finalizeState_Damage() {}
void dAcEmr_c::initializeState_PathWait() {}
void dAcEmr_c::executeState_PathWait() {}
void dAcEmr_c::finalizeState_PathWait() {}
void dAcEmr_c::initializeState_PathWalk() {}
void dAcEmr_c::executeState_PathWalk() {}
void dAcEmr_c::finalizeState_PathWalk() {}
void dAcEmr_c::initializeState_Home() {}
void dAcEmr_c::executeState_Home() {}
void dAcEmr_c::finalizeState_Home() {}
void dAcEmr_c::initializeState_PathHome() {}
void dAcEmr_c::executeState_PathHome() {}
void dAcEmr_c::finalizeState_PathHome() {}
