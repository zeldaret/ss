#include <d/a/b/d_a_b_bigboss3.h>

SPECIAL_ACTOR_PROFILE(B_BIGBOSS3, dAcBBigBoss3_c, fProfile::B_BIGBOSS3, 0xA2, 0, 0);

STATE_DEFINE(dAcBBigBoss3_c, AppearWait);
STATE_DEFINE(dAcBBigBoss3_c, AppearDemo);
STATE_DEFINE(dAcBBigBoss3_c, Wait);
STATE_DEFINE(dAcBBigBoss3_c, Walk);
STATE_DEFINE(dAcBBigBoss3_c, Reach);
STATE_DEFINE(dAcBBigBoss3_c, Turn);
STATE_DEFINE(dAcBBigBoss3_c, ImpaAdvice);
STATE_DEFINE(dAcBBigBoss3_c, ShakeHead);
STATE_DEFINE(dAcBBigBoss3_c, HeadDamage);
STATE_DEFINE(dAcBBigBoss3_c, HeadDamageThrow);
STATE_DEFINE(dAcBBigBoss3_c, HeadPain);
STATE_DEFINE(dAcBBigBoss3_c, StunPain);
STATE_DEFINE(dAcBBigBoss3_c, DropByHand);
STATE_DEFINE(dAcBBigBoss3_c, Attack);
STATE_DEFINE(dAcBBigBoss3_c, FootBroken);
STATE_DEFINE(dAcBBigBoss3_c, BombHit);
STATE_DEFINE(dAcBBigBoss3_c, FootRecover);
STATE_DEFINE(dAcBBigBoss3_c, HeadRecover);
STATE_DEFINE(dAcBBigBoss3_c, ArmBroken);
STATE_DEFINE(dAcBBigBoss3_c, ArmRecover);
STATE_DEFINE(dAcBBigBoss3_c, Stun);
STATE_DEFINE(dAcBBigBoss3_c, StunWait);
STATE_DEFINE(dAcBBigBoss3_c, StunRecover);
STATE_DEFINE(dAcBBigBoss3_c, StunDamage);
STATE_DEFINE(dAcBBigBoss3_c, StunBombDamage);
STATE_DEFINE(dAcBBigBoss3_c, LevelUp);
STATE_DEFINE(dAcBBigBoss3_c, GuardBallista);
STATE_DEFINE(dAcBBigBoss3_c, ArmAttack);
STATE_DEFINE(dAcBBigBoss3_c, PreFly);
STATE_DEFINE(dAcBBigBoss3_c, PreFlyDemo);
STATE_DEFINE(dAcBBigBoss3_c, Fly);
STATE_DEFINE(dAcBBigBoss3_c, FlyPain);
STATE_DEFINE(dAcBBigBoss3_c, FlyFall);
STATE_DEFINE(dAcBBigBoss3_c, FlyFallGround);
STATE_DEFINE(dAcBBigBoss3_c, FlyFallGroundWait);
STATE_DEFINE(dAcBBigBoss3_c, FlyFallRecover);
STATE_DEFINE(dAcBBigBoss3_c, FlyReach);
STATE_DEFINE(dAcBBigBoss3_c, Evacuation);
STATE_DEFINE(dAcBBigBoss3_c, StoneWait);
STATE_DEFINE(dAcBBigBoss3_c, SwordGameWait);
STATE_DEFINE(dAcBBigBoss3_c, SwordGameEnd);

void dAcBBigBoss3_c::initializeState_AppearWait() {}
void dAcBBigBoss3_c::executeState_AppearWait() {}
void dAcBBigBoss3_c::finalizeState_AppearWait() {}
void dAcBBigBoss3_c::initializeState_AppearDemo() {}
void dAcBBigBoss3_c::executeState_AppearDemo() {}
void dAcBBigBoss3_c::finalizeState_AppearDemo() {}
void dAcBBigBoss3_c::initializeState_Wait() {}
void dAcBBigBoss3_c::executeState_Wait() {}
void dAcBBigBoss3_c::finalizeState_Wait() {}
void dAcBBigBoss3_c::initializeState_Walk() {}
void dAcBBigBoss3_c::executeState_Walk() {}
void dAcBBigBoss3_c::finalizeState_Walk() {}
void dAcBBigBoss3_c::initializeState_Reach() {}
void dAcBBigBoss3_c::executeState_Reach() {}
void dAcBBigBoss3_c::finalizeState_Reach() {}
void dAcBBigBoss3_c::initializeState_Turn() {}
void dAcBBigBoss3_c::executeState_Turn() {}
void dAcBBigBoss3_c::finalizeState_Turn() {}
void dAcBBigBoss3_c::initializeState_ImpaAdvice() {}
void dAcBBigBoss3_c::executeState_ImpaAdvice() {}
void dAcBBigBoss3_c::finalizeState_ImpaAdvice() {}
void dAcBBigBoss3_c::initializeState_ShakeHead() {}
void dAcBBigBoss3_c::executeState_ShakeHead() {}
void dAcBBigBoss3_c::finalizeState_ShakeHead() {}
void dAcBBigBoss3_c::initializeState_HeadDamage() {}
void dAcBBigBoss3_c::executeState_HeadDamage() {}
void dAcBBigBoss3_c::finalizeState_HeadDamage() {}
void dAcBBigBoss3_c::initializeState_HeadDamageThrow() {}
void dAcBBigBoss3_c::executeState_HeadDamageThrow() {}
void dAcBBigBoss3_c::finalizeState_HeadDamageThrow() {}
void dAcBBigBoss3_c::initializeState_HeadPain() {}
void dAcBBigBoss3_c::executeState_HeadPain() {}
void dAcBBigBoss3_c::finalizeState_HeadPain() {}
void dAcBBigBoss3_c::initializeState_StunPain() {}
void dAcBBigBoss3_c::executeState_StunPain() {}
void dAcBBigBoss3_c::finalizeState_StunPain() {}
void dAcBBigBoss3_c::initializeState_DropByHand() {}
void dAcBBigBoss3_c::executeState_DropByHand() {}
void dAcBBigBoss3_c::finalizeState_DropByHand() {}
void dAcBBigBoss3_c::initializeState_Attack() {}
void dAcBBigBoss3_c::executeState_Attack() {}
void dAcBBigBoss3_c::finalizeState_Attack() {}
void dAcBBigBoss3_c::initializeState_FootBroken() {}
void dAcBBigBoss3_c::executeState_FootBroken() {}
void dAcBBigBoss3_c::finalizeState_FootBroken() {}
void dAcBBigBoss3_c::initializeState_BombHit() {}
void dAcBBigBoss3_c::executeState_BombHit() {}
void dAcBBigBoss3_c::finalizeState_BombHit() {}
void dAcBBigBoss3_c::initializeState_FootRecover() {}
void dAcBBigBoss3_c::executeState_FootRecover() {}
void dAcBBigBoss3_c::finalizeState_FootRecover() {}
void dAcBBigBoss3_c::initializeState_HeadRecover() {}
void dAcBBigBoss3_c::executeState_HeadRecover() {}
void dAcBBigBoss3_c::finalizeState_HeadRecover() {}
void dAcBBigBoss3_c::initializeState_ArmBroken() {}
void dAcBBigBoss3_c::executeState_ArmBroken() {}
void dAcBBigBoss3_c::finalizeState_ArmBroken() {}
void dAcBBigBoss3_c::initializeState_ArmRecover() {}
void dAcBBigBoss3_c::executeState_ArmRecover() {}
void dAcBBigBoss3_c::finalizeState_ArmRecover() {}
void dAcBBigBoss3_c::initializeState_Stun() {}
void dAcBBigBoss3_c::executeState_Stun() {}
void dAcBBigBoss3_c::finalizeState_Stun() {}
void dAcBBigBoss3_c::initializeState_StunWait() {}
void dAcBBigBoss3_c::executeState_StunWait() {}
void dAcBBigBoss3_c::finalizeState_StunWait() {}
void dAcBBigBoss3_c::initializeState_StunRecover() {}
void dAcBBigBoss3_c::executeState_StunRecover() {}
void dAcBBigBoss3_c::finalizeState_StunRecover() {}
void dAcBBigBoss3_c::initializeState_StunDamage() {}
void dAcBBigBoss3_c::executeState_StunDamage() {}
void dAcBBigBoss3_c::finalizeState_StunDamage() {}
void dAcBBigBoss3_c::initializeState_StunBombDamage() {}
void dAcBBigBoss3_c::executeState_StunBombDamage() {}
void dAcBBigBoss3_c::finalizeState_StunBombDamage() {}
void dAcBBigBoss3_c::initializeState_LevelUp() {}
void dAcBBigBoss3_c::executeState_LevelUp() {}
void dAcBBigBoss3_c::finalizeState_LevelUp() {}
void dAcBBigBoss3_c::initializeState_GuardBallista() {}
void dAcBBigBoss3_c::executeState_GuardBallista() {}
void dAcBBigBoss3_c::finalizeState_GuardBallista() {}
void dAcBBigBoss3_c::initializeState_ArmAttack() {}
void dAcBBigBoss3_c::executeState_ArmAttack() {}
void dAcBBigBoss3_c::finalizeState_ArmAttack() {}
void dAcBBigBoss3_c::initializeState_PreFly() {}
void dAcBBigBoss3_c::executeState_PreFly() {}
void dAcBBigBoss3_c::finalizeState_PreFly() {}
void dAcBBigBoss3_c::initializeState_PreFlyDemo() {}
void dAcBBigBoss3_c::executeState_PreFlyDemo() {}
void dAcBBigBoss3_c::finalizeState_PreFlyDemo() {}
void dAcBBigBoss3_c::initializeState_Fly() {}
void dAcBBigBoss3_c::executeState_Fly() {}
void dAcBBigBoss3_c::finalizeState_Fly() {}
void dAcBBigBoss3_c::initializeState_FlyPain() {}
void dAcBBigBoss3_c::executeState_FlyPain() {}
void dAcBBigBoss3_c::finalizeState_FlyPain() {}
void dAcBBigBoss3_c::initializeState_FlyFall() {}
void dAcBBigBoss3_c::executeState_FlyFall() {}
void dAcBBigBoss3_c::finalizeState_FlyFall() {}
void dAcBBigBoss3_c::initializeState_FlyFallGround() {}
void dAcBBigBoss3_c::executeState_FlyFallGround() {}
void dAcBBigBoss3_c::finalizeState_FlyFallGround() {}
void dAcBBigBoss3_c::initializeState_FlyFallGroundWait() {}
void dAcBBigBoss3_c::executeState_FlyFallGroundWait() {}
void dAcBBigBoss3_c::finalizeState_FlyFallGroundWait() {}
void dAcBBigBoss3_c::initializeState_FlyFallRecover() {}
void dAcBBigBoss3_c::executeState_FlyFallRecover() {}
void dAcBBigBoss3_c::finalizeState_FlyFallRecover() {}
void dAcBBigBoss3_c::initializeState_FlyReach() {}
void dAcBBigBoss3_c::executeState_FlyReach() {}
void dAcBBigBoss3_c::finalizeState_FlyReach() {}
void dAcBBigBoss3_c::initializeState_Evacuation() {}
void dAcBBigBoss3_c::executeState_Evacuation() {}
void dAcBBigBoss3_c::finalizeState_Evacuation() {}
void dAcBBigBoss3_c::initializeState_StoneWait() {}
void dAcBBigBoss3_c::executeState_StoneWait() {}
void dAcBBigBoss3_c::finalizeState_StoneWait() {}
void dAcBBigBoss3_c::initializeState_SwordGameWait() {}
void dAcBBigBoss3_c::executeState_SwordGameWait() {}
void dAcBBigBoss3_c::finalizeState_SwordGameWait() {}
void dAcBBigBoss3_c::initializeState_SwordGameEnd() {}
void dAcBBigBoss3_c::executeState_SwordGameEnd() {}
void dAcBBigBoss3_c::finalizeState_SwordGameEnd() {}
