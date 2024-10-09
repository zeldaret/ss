#include <d/a/e/d_a_e_bcz.h>

SPECIAL_ACTOR_PROFILE(E_BCZ, dAcEBcZ_c, fProfile::E_BCZ, 0xD6, 0, 3);

STATE_DEFINE(dAcEBcZ_c, Arise);
STATE_DEFINE(dAcEBcZ_c, Wait);
STATE_DEFINE(dAcEBcZ_c, Walk);
STATE_DEFINE(dAcEBcZ_c, Find);
STATE_DEFINE(dAcEBcZ_c, Chase);
STATE_DEFINE(dAcEBcZ_c, HoldAttack);
STATE_DEFINE(dAcEBcZ_c, HoldStart);
STATE_DEFINE(dAcEBcZ_c, HoldWait);
STATE_DEFINE(dAcEBcZ_c, HoldScream);
STATE_DEFINE(dAcEBcZ_c, WhipTurn);
STATE_DEFINE(dAcEBcZ_c, WhipStagger);
STATE_DEFINE(dAcEBcZ_c, WindBlow);
STATE_DEFINE(dAcEBcZ_c, KeepAwayFromShield);
STATE_DEFINE(dAcEBcZ_c, Damage);
STATE_DEFINE(dAcEBcZ_c, Down);
STATE_DEFINE(dAcEBcZ_c, AriseFromTag);
STATE_DEFINE(dAcEBcZ_c, RopeSearch);
STATE_DEFINE(dAcEBcZ_c, RopeSearchWalk);
STATE_DEFINE(dAcEBcZ_c, RopeSearchWait);
STATE_DEFINE(dAcEBcZ_c, RopeWait);
STATE_DEFINE(dAcEBcZ_c, RopeMove);
STATE_DEFINE(dAcEBcZ_c, RopeHoldStart);
STATE_DEFINE(dAcEBcZ_c, RopeHoldWait);
STATE_DEFINE(dAcEBcZ_c, RopeHoldScream);
STATE_DEFINE(dAcEBcZ_c, PoisonSwamp);

void dAcEBcZ_c::initializeState_Arise() {}
void dAcEBcZ_c::executeState_Arise() {}
void dAcEBcZ_c::finalizeState_Arise() {}
void dAcEBcZ_c::initializeState_Wait() {}
void dAcEBcZ_c::executeState_Wait() {}
void dAcEBcZ_c::finalizeState_Wait() {}
void dAcEBcZ_c::initializeState_Walk() {}
void dAcEBcZ_c::executeState_Walk() {}
void dAcEBcZ_c::finalizeState_Walk() {}
void dAcEBcZ_c::initializeState_Find() {}
void dAcEBcZ_c::executeState_Find() {}
void dAcEBcZ_c::finalizeState_Find() {}
void dAcEBcZ_c::initializeState_Chase() {}
void dAcEBcZ_c::executeState_Chase() {}
void dAcEBcZ_c::finalizeState_Chase() {}
void dAcEBcZ_c::initializeState_HoldAttack() {}
void dAcEBcZ_c::executeState_HoldAttack() {}
void dAcEBcZ_c::finalizeState_HoldAttack() {}
void dAcEBcZ_c::initializeState_HoldStart() {}
void dAcEBcZ_c::executeState_HoldStart() {}
void dAcEBcZ_c::finalizeState_HoldStart() {}
void dAcEBcZ_c::initializeState_HoldWait() {}
void dAcEBcZ_c::executeState_HoldWait() {}
void dAcEBcZ_c::finalizeState_HoldWait() {}
void dAcEBcZ_c::initializeState_HoldScream() {}
void dAcEBcZ_c::executeState_HoldScream() {}
void dAcEBcZ_c::finalizeState_HoldScream() {}
void dAcEBcZ_c::initializeState_WhipTurn() {}
void dAcEBcZ_c::executeState_WhipTurn() {}
void dAcEBcZ_c::finalizeState_WhipTurn() {}
void dAcEBcZ_c::initializeState_WhipStagger() {}
void dAcEBcZ_c::executeState_WhipStagger() {}
void dAcEBcZ_c::finalizeState_WhipStagger() {}
void dAcEBcZ_c::initializeState_WindBlow() {}
void dAcEBcZ_c::executeState_WindBlow() {}
void dAcEBcZ_c::finalizeState_WindBlow() {}
void dAcEBcZ_c::initializeState_KeepAwayFromShield() {}
void dAcEBcZ_c::executeState_KeepAwayFromShield() {}
void dAcEBcZ_c::finalizeState_KeepAwayFromShield() {}
void dAcEBcZ_c::initializeState_Damage() {}
void dAcEBcZ_c::executeState_Damage() {}
void dAcEBcZ_c::finalizeState_Damage() {}
void dAcEBcZ_c::initializeState_Down() {}
void dAcEBcZ_c::executeState_Down() {}
void dAcEBcZ_c::finalizeState_Down() {}
void dAcEBcZ_c::initializeState_AriseFromTag() {}
void dAcEBcZ_c::executeState_AriseFromTag() {}
void dAcEBcZ_c::finalizeState_AriseFromTag() {}
void dAcEBcZ_c::initializeState_RopeSearch() {}
void dAcEBcZ_c::executeState_RopeSearch() {}
void dAcEBcZ_c::finalizeState_RopeSearch() {}
void dAcEBcZ_c::initializeState_RopeSearchWalk() {}
void dAcEBcZ_c::executeState_RopeSearchWalk() {}
void dAcEBcZ_c::finalizeState_RopeSearchWalk() {}
void dAcEBcZ_c::initializeState_RopeSearchWait() {}
void dAcEBcZ_c::executeState_RopeSearchWait() {}
void dAcEBcZ_c::finalizeState_RopeSearchWait() {}
void dAcEBcZ_c::initializeState_RopeWait() {}
void dAcEBcZ_c::executeState_RopeWait() {}
void dAcEBcZ_c::finalizeState_RopeWait() {}
void dAcEBcZ_c::initializeState_RopeMove() {}
void dAcEBcZ_c::executeState_RopeMove() {}
void dAcEBcZ_c::finalizeState_RopeMove() {}
void dAcEBcZ_c::initializeState_RopeHoldStart() {}
void dAcEBcZ_c::executeState_RopeHoldStart() {}
void dAcEBcZ_c::finalizeState_RopeHoldStart() {}
void dAcEBcZ_c::initializeState_RopeHoldWait() {}
void dAcEBcZ_c::executeState_RopeHoldWait() {}
void dAcEBcZ_c::finalizeState_RopeHoldWait() {}
void dAcEBcZ_c::initializeState_RopeHoldScream() {}
void dAcEBcZ_c::executeState_RopeHoldScream() {}
void dAcEBcZ_c::finalizeState_RopeHoldScream() {}
void dAcEBcZ_c::initializeState_PoisonSwamp() {}
void dAcEBcZ_c::executeState_PoisonSwamp() {}
void dAcEBcZ_c::finalizeState_PoisonSwamp() {}
