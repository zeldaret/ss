#include "d/t/d_t_roll_attack_log.h"

SPECIAL_ACTOR_PROFILE(TAG_ROLL_ATTACK_LOG, dTgRollAttackLog_c, fProfile::TAG_ROLL_ATTACK_LOG, 0x227, 0, 0);

STATE_DEFINE(dTgRollAttackLog_c, Wait);
STATE_DEFINE(dTgRollAttackLog_c, RollAttack);
STATE_DEFINE(dTgRollAttackLog_c, Request);
STATE_DEFINE(dTgRollAttackLog_c, FinishAttack);
STATE_DEFINE(dTgRollAttackLog_c, None);

void dTgRollAttackLog_c::initializeState_Wait() {}
void dTgRollAttackLog_c::executeState_Wait() {}
void dTgRollAttackLog_c::finalizeState_Wait() {}
void dTgRollAttackLog_c::initializeState_RollAttack() {}
void dTgRollAttackLog_c::executeState_RollAttack() {}
void dTgRollAttackLog_c::finalizeState_RollAttack() {}
void dTgRollAttackLog_c::initializeState_Request() {}
void dTgRollAttackLog_c::executeState_Request() {}
void dTgRollAttackLog_c::finalizeState_Request() {}
void dTgRollAttackLog_c::initializeState_FinishAttack() {}
void dTgRollAttackLog_c::executeState_FinishAttack() {}
void dTgRollAttackLog_c::finalizeState_FinishAttack() {}
void dTgRollAttackLog_c::initializeState_None() {}
void dTgRollAttackLog_c::executeState_None() {}
void dTgRollAttackLog_c::finalizeState_None() {}
