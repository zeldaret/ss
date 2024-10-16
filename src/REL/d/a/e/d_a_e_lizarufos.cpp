#include "d/a/e/d_a_e_lizarufos.h"

SPECIAL_ACTOR_PROFILE(E_LIZARUFOS, dAcELizarufos_c, fProfile::E_LIZARUFOS, 0xE2, 0, 34);

STATE_DEFINE(dAcELizarufos_c, Demo);
STATE_DEFINE(dAcELizarufos_c, Wait);
STATE_DEFINE(dAcELizarufos_c, Kyoro);
STATE_DEFINE(dAcELizarufos_c, Find);
STATE_DEFINE(dAcELizarufos_c, Find2);
STATE_DEFINE(dAcELizarufos_c, Move);
STATE_DEFINE(dAcELizarufos_c, Search);
STATE_DEFINE(dAcELizarufos_c, PathMove);
STATE_DEFINE(dAcELizarufos_c, FightWait);
STATE_DEFINE(dAcELizarufos_c, Provoke);
STATE_DEFINE(dAcELizarufos_c, ArmAttack);
STATE_DEFINE(dAcELizarufos_c, TailAttack);
STATE_DEFINE(dAcELizarufos_c, FireAttack);
STATE_DEFINE(dAcELizarufos_c, Step);
STATE_DEFINE(dAcELizarufos_c, BackJump);
STATE_DEFINE(dAcELizarufos_c, CliffJump);
STATE_DEFINE(dAcELizarufos_c, GuardWait);
STATE_DEFINE(dAcELizarufos_c, Guard);
STATE_DEFINE(dAcELizarufos_c, JustGuard);
STATE_DEFINE(dAcELizarufos_c, RetMove);
STATE_DEFINE(dAcELizarufos_c, Damage);
STATE_DEFINE(dAcELizarufos_c, Falter);
STATE_DEFINE(dAcELizarufos_c, Stun);
STATE_DEFINE(dAcELizarufos_c, Fall);
STATE_DEFINE(dAcELizarufos_c, Dead);

void dAcELizarufos_c::initializeState_Demo() {}
void dAcELizarufos_c::executeState_Demo() {}
void dAcELizarufos_c::finalizeState_Demo() {}
void dAcELizarufos_c::initializeState_Wait() {}
void dAcELizarufos_c::executeState_Wait() {}
void dAcELizarufos_c::finalizeState_Wait() {}
void dAcELizarufos_c::initializeState_Kyoro() {}
void dAcELizarufos_c::executeState_Kyoro() {}
void dAcELizarufos_c::finalizeState_Kyoro() {}
void dAcELizarufos_c::initializeState_Find() {}
void dAcELizarufos_c::executeState_Find() {}
void dAcELizarufos_c::finalizeState_Find() {}
void dAcELizarufos_c::initializeState_Find2() {}
void dAcELizarufos_c::executeState_Find2() {}
void dAcELizarufos_c::finalizeState_Find2() {}
void dAcELizarufos_c::initializeState_Move() {}
void dAcELizarufos_c::executeState_Move() {}
void dAcELizarufos_c::finalizeState_Move() {}
void dAcELizarufos_c::initializeState_Search() {}
void dAcELizarufos_c::executeState_Search() {}
void dAcELizarufos_c::finalizeState_Search() {}
void dAcELizarufos_c::initializeState_PathMove() {}
void dAcELizarufos_c::executeState_PathMove() {}
void dAcELizarufos_c::finalizeState_PathMove() {}
void dAcELizarufos_c::initializeState_FightWait() {}
void dAcELizarufos_c::executeState_FightWait() {}
void dAcELizarufos_c::finalizeState_FightWait() {}
void dAcELizarufos_c::initializeState_Provoke() {}
void dAcELizarufos_c::executeState_Provoke() {}
void dAcELizarufos_c::finalizeState_Provoke() {}
void dAcELizarufos_c::initializeState_ArmAttack() {}
void dAcELizarufos_c::executeState_ArmAttack() {}
void dAcELizarufos_c::finalizeState_ArmAttack() {}
void dAcELizarufos_c::initializeState_TailAttack() {}
void dAcELizarufos_c::executeState_TailAttack() {}
void dAcELizarufos_c::finalizeState_TailAttack() {}
void dAcELizarufos_c::initializeState_FireAttack() {}
void dAcELizarufos_c::executeState_FireAttack() {}
void dAcELizarufos_c::finalizeState_FireAttack() {}
void dAcELizarufos_c::initializeState_Step() {}
void dAcELizarufos_c::executeState_Step() {}
void dAcELizarufos_c::finalizeState_Step() {}
void dAcELizarufos_c::initializeState_BackJump() {}
void dAcELizarufos_c::executeState_BackJump() {}
void dAcELizarufos_c::finalizeState_BackJump() {}
void dAcELizarufos_c::initializeState_CliffJump() {}
void dAcELizarufos_c::executeState_CliffJump() {}
void dAcELizarufos_c::finalizeState_CliffJump() {}
void dAcELizarufos_c::initializeState_GuardWait() {}
void dAcELizarufos_c::executeState_GuardWait() {}
void dAcELizarufos_c::finalizeState_GuardWait() {}
void dAcELizarufos_c::initializeState_Guard() {}
void dAcELizarufos_c::executeState_Guard() {}
void dAcELizarufos_c::finalizeState_Guard() {}
void dAcELizarufos_c::initializeState_JustGuard() {}
void dAcELizarufos_c::executeState_JustGuard() {}
void dAcELizarufos_c::finalizeState_JustGuard() {}
void dAcELizarufos_c::initializeState_RetMove() {}
void dAcELizarufos_c::executeState_RetMove() {}
void dAcELizarufos_c::finalizeState_RetMove() {}
void dAcELizarufos_c::initializeState_Damage() {}
void dAcELizarufos_c::executeState_Damage() {}
void dAcELizarufos_c::finalizeState_Damage() {}
void dAcELizarufos_c::initializeState_Falter() {}
void dAcELizarufos_c::executeState_Falter() {}
void dAcELizarufos_c::finalizeState_Falter() {}
void dAcELizarufos_c::initializeState_Stun() {}
void dAcELizarufos_c::executeState_Stun() {}
void dAcELizarufos_c::finalizeState_Stun() {}
void dAcELizarufos_c::initializeState_Fall() {}
void dAcELizarufos_c::executeState_Fall() {}
void dAcELizarufos_c::finalizeState_Fall() {}
void dAcELizarufos_c::initializeState_Dead() {}
void dAcELizarufos_c::executeState_Dead() {}
void dAcELizarufos_c::finalizeState_Dead() {}
