#include <d/a/e/d_a_e_oc.h>

SPECIAL_ACTOR_PROFILE(E_OC, dAcEoc_c, fProfile::E_OC, 0xCD, 0, 3);

STATE_DEFINE(dAcEoc_c, Wait);
STATE_DEFINE(dAcEoc_c, Standby);
STATE_DEFINE(dAcEoc_c, FindCall);
STATE_DEFINE(dAcEoc_c, Fight);
STATE_DEFINE(dAcEoc_c, Attack);
STATE_DEFINE(dAcEoc_c, BigAttack);
STATE_DEFINE(dAcEoc_c, Damage);
STATE_DEFINE(dAcEoc_c, Fire);
STATE_DEFINE(dAcEoc_c, Fail);
STATE_DEFINE(dAcEoc_c, StabGuard);
STATE_DEFINE(dAcEoc_c, Stun);
STATE_DEFINE(dAcEoc_c, PipeRun);
STATE_DEFINE(dAcEoc_c, PipeWait);
STATE_DEFINE(dAcEoc_c, Whistle);
STATE_DEFINE(dAcEoc_c, Flicked);
STATE_DEFINE(dAcEoc_c, AirDamage);
STATE_DEFINE(dAcEoc_c, Demo);
STATE_DEFINE(dAcEoc_c, PipeChase);
STATE_DEFINE(dAcEoc_c, SearchPipe);
STATE_DEFINE(dAcEoc_c, GetPipe);
STATE_DEFINE(dAcEoc_c, Summon);
STATE_DEFINE(dAcEoc_c, BombCarry);
STATE_DEFINE(dAcEoc_c, BombThrow);
STATE_DEFINE(dAcEoc_c, Escape);
STATE_DEFINE(dAcEoc_c, Equip);
STATE_DEFINE(dAcEoc_c, WhipCapture);
STATE_DEFINE(dAcEoc_c, DemoMove);

void dAcEoc_c::initializeState_Wait() {}
void dAcEoc_c::executeState_Wait() {}
void dAcEoc_c::finalizeState_Wait() {}
void dAcEoc_c::initializeState_Standby() {}
void dAcEoc_c::executeState_Standby() {}
void dAcEoc_c::finalizeState_Standby() {}
void dAcEoc_c::initializeState_FindCall() {}
void dAcEoc_c::executeState_FindCall() {}
void dAcEoc_c::finalizeState_FindCall() {}
void dAcEoc_c::initializeState_Fight() {}
void dAcEoc_c::executeState_Fight() {}
void dAcEoc_c::finalizeState_Fight() {}
void dAcEoc_c::initializeState_Attack() {}
void dAcEoc_c::executeState_Attack() {}
void dAcEoc_c::finalizeState_Attack() {}
void dAcEoc_c::initializeState_BigAttack() {}
void dAcEoc_c::executeState_BigAttack() {}
void dAcEoc_c::finalizeState_BigAttack() {}
void dAcEoc_c::initializeState_Damage() {}
void dAcEoc_c::executeState_Damage() {}
void dAcEoc_c::finalizeState_Damage() {}
void dAcEoc_c::initializeState_Fire() {}
void dAcEoc_c::executeState_Fire() {}
void dAcEoc_c::finalizeState_Fire() {}
void dAcEoc_c::initializeState_Fail() {}
void dAcEoc_c::executeState_Fail() {}
void dAcEoc_c::finalizeState_Fail() {}
void dAcEoc_c::initializeState_StabGuard() {}
void dAcEoc_c::executeState_StabGuard() {}
void dAcEoc_c::finalizeState_StabGuard() {}
void dAcEoc_c::initializeState_Stun() {}
void dAcEoc_c::executeState_Stun() {}
void dAcEoc_c::finalizeState_Stun() {}
void dAcEoc_c::initializeState_PipeRun() {}
void dAcEoc_c::executeState_PipeRun() {}
void dAcEoc_c::finalizeState_PipeRun() {}
void dAcEoc_c::initializeState_PipeWait() {}
void dAcEoc_c::executeState_PipeWait() {}
void dAcEoc_c::finalizeState_PipeWait() {}
void dAcEoc_c::initializeState_Whistle() {}
void dAcEoc_c::executeState_Whistle() {}
void dAcEoc_c::finalizeState_Whistle() {}
void dAcEoc_c::initializeState_Flicked() {}
void dAcEoc_c::executeState_Flicked() {}
void dAcEoc_c::finalizeState_Flicked() {}
void dAcEoc_c::initializeState_AirDamage() {}
void dAcEoc_c::executeState_AirDamage() {}
void dAcEoc_c::finalizeState_AirDamage() {}
void dAcEoc_c::initializeState_Demo() {}
void dAcEoc_c::executeState_Demo() {}
void dAcEoc_c::finalizeState_Demo() {}
void dAcEoc_c::initializeState_PipeChase() {}
void dAcEoc_c::executeState_PipeChase() {}
void dAcEoc_c::finalizeState_PipeChase() {}
void dAcEoc_c::initializeState_SearchPipe() {}
void dAcEoc_c::executeState_SearchPipe() {}
void dAcEoc_c::finalizeState_SearchPipe() {}
void dAcEoc_c::initializeState_GetPipe() {}
void dAcEoc_c::executeState_GetPipe() {}
void dAcEoc_c::finalizeState_GetPipe() {}
void dAcEoc_c::initializeState_Summon() {}
void dAcEoc_c::executeState_Summon() {}
void dAcEoc_c::finalizeState_Summon() {}
void dAcEoc_c::initializeState_BombCarry() {}
void dAcEoc_c::executeState_BombCarry() {}
void dAcEoc_c::finalizeState_BombCarry() {}
void dAcEoc_c::initializeState_BombThrow() {}
void dAcEoc_c::executeState_BombThrow() {}
void dAcEoc_c::finalizeState_BombThrow() {}
void dAcEoc_c::initializeState_Escape() {}
void dAcEoc_c::executeState_Escape() {}
void dAcEoc_c::finalizeState_Escape() {}
void dAcEoc_c::initializeState_Equip() {}
void dAcEoc_c::executeState_Equip() {}
void dAcEoc_c::finalizeState_Equip() {}
void dAcEoc_c::initializeState_WhipCapture() {}
void dAcEoc_c::executeState_WhipCapture() {}
void dAcEoc_c::finalizeState_WhipCapture() {}
void dAcEoc_c::initializeState_DemoMove() {}
void dAcEoc_c::executeState_DemoMove() {}
void dAcEoc_c::finalizeState_DemoMove() {}
