#include "d/a/b/d_a_b_bigboss2.h"

SPECIAL_ACTOR_PROFILE(B_BIGBOSS2, dAcBBigBoss2_c, fProfile::B_BIGBOSS2, 0xA1, 0, 0);

STATE_DEFINE(dAcBBigBoss2_c, AppearWait);
STATE_DEFINE(dAcBBigBoss2_c, AppearDemo);
STATE_DEFINE(dAcBBigBoss2_c, Wait);
STATE_DEFINE(dAcBBigBoss2_c, Walk);
STATE_DEFINE(dAcBBigBoss2_c, Reach);
STATE_DEFINE(dAcBBigBoss2_c, Turn);
STATE_DEFINE(dAcBBigBoss2_c, ImpaAdvice);
STATE_DEFINE(dAcBBigBoss2_c, ShakeHead);
STATE_DEFINE(dAcBBigBoss2_c, HeadDamage);
STATE_DEFINE(dAcBBigBoss2_c, HeadDamageThrow);
STATE_DEFINE(dAcBBigBoss2_c, HeadPain);
STATE_DEFINE(dAcBBigBoss2_c, StunPain);
STATE_DEFINE(dAcBBigBoss2_c, HeadRecover);
STATE_DEFINE(dAcBBigBoss2_c, DropByHand);
STATE_DEFINE(dAcBBigBoss2_c, Attack);
STATE_DEFINE(dAcBBigBoss2_c, FootBroken);
STATE_DEFINE(dAcBBigBoss2_c, BombHit);
STATE_DEFINE(dAcBBigBoss2_c, FootRecover);
STATE_DEFINE(dAcBBigBoss2_c, ArmBroken);
STATE_DEFINE(dAcBBigBoss2_c, ArmAttack);
STATE_DEFINE(dAcBBigBoss2_c, ArmRecover);
STATE_DEFINE(dAcBBigBoss2_c, Stun);
STATE_DEFINE(dAcBBigBoss2_c, StunWait);
STATE_DEFINE(dAcBBigBoss2_c, StunRecover);
STATE_DEFINE(dAcBBigBoss2_c, StunDamage);
STATE_DEFINE(dAcBBigBoss2_c, StunBombDamage);
STATE_DEFINE(dAcBBigBoss2_c, Mount);
STATE_DEFINE(dAcBBigBoss2_c, Fall);
STATE_DEFINE(dAcBBigBoss2_c, MountDamage);
STATE_DEFINE(dAcBBigBoss2_c, FrontFall);
STATE_DEFINE(dAcBBigBoss2_c, Rush);
STATE_DEFINE(dAcBBigBoss2_c, Rise);
STATE_DEFINE(dAcBBigBoss2_c, Tied);
STATE_DEFINE(dAcBBigBoss2_c, StunFrontFall);
STATE_DEFINE(dAcBBigBoss2_c, BadAdvice);
STATE_DEFINE(dAcBBigBoss2_c, CancelMount);
STATE_DEFINE(dAcBBigBoss2_c, MountArmBroken);
STATE_DEFINE(dAcBBigBoss2_c, LevelUp);
STATE_DEFINE(dAcBBigBoss2_c, RushDamage);
STATE_DEFINE(dAcBBigBoss2_c, GuardBallista);
STATE_DEFINE(dAcBBigBoss2_c, Evacuation);
STATE_DEFINE(dAcBBigBoss2_c, StoneWait);
STATE_DEFINE(dAcBBigBoss2_c, SwordGameWait);
STATE_DEFINE(dAcBBigBoss2_c, SwordGameEnd);

void dAcBBigBoss2_c::initializeState_AppearWait() {}
void dAcBBigBoss2_c::executeState_AppearWait() {}
void dAcBBigBoss2_c::finalizeState_AppearWait() {}
void dAcBBigBoss2_c::initializeState_AppearDemo() {}
void dAcBBigBoss2_c::executeState_AppearDemo() {}
void dAcBBigBoss2_c::finalizeState_AppearDemo() {}
void dAcBBigBoss2_c::initializeState_Wait() {}
void dAcBBigBoss2_c::executeState_Wait() {}
void dAcBBigBoss2_c::finalizeState_Wait() {}
void dAcBBigBoss2_c::initializeState_Walk() {}
void dAcBBigBoss2_c::executeState_Walk() {}
void dAcBBigBoss2_c::finalizeState_Walk() {}
void dAcBBigBoss2_c::initializeState_Reach() {}
void dAcBBigBoss2_c::executeState_Reach() {}
void dAcBBigBoss2_c::finalizeState_Reach() {}
void dAcBBigBoss2_c::initializeState_Turn() {}
void dAcBBigBoss2_c::executeState_Turn() {}
void dAcBBigBoss2_c::finalizeState_Turn() {}
void dAcBBigBoss2_c::initializeState_ImpaAdvice() {}
void dAcBBigBoss2_c::executeState_ImpaAdvice() {}
void dAcBBigBoss2_c::finalizeState_ImpaAdvice() {}
void dAcBBigBoss2_c::initializeState_ShakeHead() {}
void dAcBBigBoss2_c::executeState_ShakeHead() {}
void dAcBBigBoss2_c::finalizeState_ShakeHead() {}
void dAcBBigBoss2_c::initializeState_HeadDamage() {}
void dAcBBigBoss2_c::executeState_HeadDamage() {}
void dAcBBigBoss2_c::finalizeState_HeadDamage() {}
void dAcBBigBoss2_c::initializeState_HeadDamageThrow() {}
void dAcBBigBoss2_c::executeState_HeadDamageThrow() {}
void dAcBBigBoss2_c::finalizeState_HeadDamageThrow() {}
void dAcBBigBoss2_c::initializeState_HeadPain() {}
void dAcBBigBoss2_c::executeState_HeadPain() {}
void dAcBBigBoss2_c::finalizeState_HeadPain() {}
void dAcBBigBoss2_c::initializeState_StunPain() {}
void dAcBBigBoss2_c::executeState_StunPain() {}
void dAcBBigBoss2_c::finalizeState_StunPain() {}
void dAcBBigBoss2_c::initializeState_HeadRecover() {}
void dAcBBigBoss2_c::executeState_HeadRecover() {}
void dAcBBigBoss2_c::finalizeState_HeadRecover() {}
void dAcBBigBoss2_c::initializeState_DropByHand() {}
void dAcBBigBoss2_c::executeState_DropByHand() {}
void dAcBBigBoss2_c::finalizeState_DropByHand() {}
void dAcBBigBoss2_c::initializeState_Attack() {}
void dAcBBigBoss2_c::executeState_Attack() {}
void dAcBBigBoss2_c::finalizeState_Attack() {}
void dAcBBigBoss2_c::initializeState_FootBroken() {}
void dAcBBigBoss2_c::executeState_FootBroken() {}
void dAcBBigBoss2_c::finalizeState_FootBroken() {}
void dAcBBigBoss2_c::initializeState_BombHit() {}
void dAcBBigBoss2_c::executeState_BombHit() {}
void dAcBBigBoss2_c::finalizeState_BombHit() {}
void dAcBBigBoss2_c::initializeState_FootRecover() {}
void dAcBBigBoss2_c::executeState_FootRecover() {}
void dAcBBigBoss2_c::finalizeState_FootRecover() {}
void dAcBBigBoss2_c::initializeState_ArmBroken() {}
void dAcBBigBoss2_c::executeState_ArmBroken() {}
void dAcBBigBoss2_c::finalizeState_ArmBroken() {}
void dAcBBigBoss2_c::initializeState_ArmAttack() {}
void dAcBBigBoss2_c::executeState_ArmAttack() {}
void dAcBBigBoss2_c::finalizeState_ArmAttack() {}
void dAcBBigBoss2_c::initializeState_ArmRecover() {}
void dAcBBigBoss2_c::executeState_ArmRecover() {}
void dAcBBigBoss2_c::finalizeState_ArmRecover() {}
void dAcBBigBoss2_c::initializeState_Stun() {}
void dAcBBigBoss2_c::executeState_Stun() {}
void dAcBBigBoss2_c::finalizeState_Stun() {}
void dAcBBigBoss2_c::initializeState_StunWait() {}
void dAcBBigBoss2_c::executeState_StunWait() {}
void dAcBBigBoss2_c::finalizeState_StunWait() {}
void dAcBBigBoss2_c::initializeState_StunRecover() {}
void dAcBBigBoss2_c::executeState_StunRecover() {}
void dAcBBigBoss2_c::finalizeState_StunRecover() {}
void dAcBBigBoss2_c::initializeState_StunDamage() {}
void dAcBBigBoss2_c::executeState_StunDamage() {}
void dAcBBigBoss2_c::finalizeState_StunDamage() {}
void dAcBBigBoss2_c::initializeState_StunBombDamage() {}
void dAcBBigBoss2_c::executeState_StunBombDamage() {}
void dAcBBigBoss2_c::finalizeState_StunBombDamage() {}
void dAcBBigBoss2_c::initializeState_Mount() {}
void dAcBBigBoss2_c::executeState_Mount() {}
void dAcBBigBoss2_c::finalizeState_Mount() {}
void dAcBBigBoss2_c::initializeState_Fall() {}
void dAcBBigBoss2_c::executeState_Fall() {}
void dAcBBigBoss2_c::finalizeState_Fall() {}
void dAcBBigBoss2_c::initializeState_MountDamage() {}
void dAcBBigBoss2_c::executeState_MountDamage() {}
void dAcBBigBoss2_c::finalizeState_MountDamage() {}
void dAcBBigBoss2_c::initializeState_FrontFall() {}
void dAcBBigBoss2_c::executeState_FrontFall() {}
void dAcBBigBoss2_c::finalizeState_FrontFall() {}
void dAcBBigBoss2_c::initializeState_Rush() {}
void dAcBBigBoss2_c::executeState_Rush() {}
void dAcBBigBoss2_c::finalizeState_Rush() {}
void dAcBBigBoss2_c::initializeState_Rise() {}
void dAcBBigBoss2_c::executeState_Rise() {}
void dAcBBigBoss2_c::finalizeState_Rise() {}
void dAcBBigBoss2_c::initializeState_Tied() {}
void dAcBBigBoss2_c::executeState_Tied() {}
void dAcBBigBoss2_c::finalizeState_Tied() {}
void dAcBBigBoss2_c::initializeState_StunFrontFall() {}
void dAcBBigBoss2_c::executeState_StunFrontFall() {}
void dAcBBigBoss2_c::finalizeState_StunFrontFall() {}
void dAcBBigBoss2_c::initializeState_BadAdvice() {}
void dAcBBigBoss2_c::executeState_BadAdvice() {}
void dAcBBigBoss2_c::finalizeState_BadAdvice() {}
void dAcBBigBoss2_c::initializeState_CancelMount() {}
void dAcBBigBoss2_c::executeState_CancelMount() {}
void dAcBBigBoss2_c::finalizeState_CancelMount() {}
void dAcBBigBoss2_c::initializeState_MountArmBroken() {}
void dAcBBigBoss2_c::executeState_MountArmBroken() {}
void dAcBBigBoss2_c::finalizeState_MountArmBroken() {}
void dAcBBigBoss2_c::initializeState_LevelUp() {}
void dAcBBigBoss2_c::executeState_LevelUp() {}
void dAcBBigBoss2_c::finalizeState_LevelUp() {}
void dAcBBigBoss2_c::initializeState_RushDamage() {}
void dAcBBigBoss2_c::executeState_RushDamage() {}
void dAcBBigBoss2_c::finalizeState_RushDamage() {}
void dAcBBigBoss2_c::initializeState_GuardBallista() {}
void dAcBBigBoss2_c::executeState_GuardBallista() {}
void dAcBBigBoss2_c::finalizeState_GuardBallista() {}
void dAcBBigBoss2_c::initializeState_Evacuation() {}
void dAcBBigBoss2_c::executeState_Evacuation() {}
void dAcBBigBoss2_c::finalizeState_Evacuation() {}
void dAcBBigBoss2_c::initializeState_StoneWait() {}
void dAcBBigBoss2_c::executeState_StoneWait() {}
void dAcBBigBoss2_c::finalizeState_StoneWait() {}
void dAcBBigBoss2_c::initializeState_SwordGameWait() {}
void dAcBBigBoss2_c::executeState_SwordGameWait() {}
void dAcBBigBoss2_c::finalizeState_SwordGameWait() {}
void dAcBBigBoss2_c::initializeState_SwordGameEnd() {}
void dAcBBigBoss2_c::executeState_SwordGameEnd() {}
void dAcBBigBoss2_c::finalizeState_SwordGameEnd() {}
