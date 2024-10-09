#include <d/a/b/d_a_b_bigboss.h>

SPECIAL_ACTOR_PROFILE(B_BIGBOSS, dAcBBigBoss_c, fProfile::B_BIGBOSS, 0xA0, 0, 0);

STATE_DEFINE(dAcBBigBoss_c, AppearWait);
STATE_DEFINE(dAcBBigBoss_c, AppearDemo);
STATE_DEFINE(dAcBBigBoss_c, Wait);
STATE_DEFINE(dAcBBigBoss_c, Walk);
STATE_DEFINE(dAcBBigBoss_c, Reach);
STATE_DEFINE(dAcBBigBoss_c, Turn);
STATE_DEFINE(dAcBBigBoss_c, ImpaAdvice);
STATE_DEFINE(dAcBBigBoss_c, ShakeHead);
STATE_DEFINE(dAcBBigBoss_c, HeadDamage);
STATE_DEFINE(dAcBBigBoss_c, HeadDamageThrow);
STATE_DEFINE(dAcBBigBoss_c, HeadPain);
STATE_DEFINE(dAcBBigBoss_c, StunPain);
STATE_DEFINE(dAcBBigBoss_c, HeadRecover);
STATE_DEFINE(dAcBBigBoss_c, Attack);
STATE_DEFINE(dAcBBigBoss_c, FrontFall);
STATE_DEFINE(dAcBBigBoss_c, Rush);
STATE_DEFINE(dAcBBigBoss_c, Rise);
STATE_DEFINE(dAcBBigBoss_c, Tied);
STATE_DEFINE(dAcBBigBoss_c, LevelUp);
STATE_DEFINE(dAcBBigBoss_c, FootBroken);
STATE_DEFINE(dAcBBigBoss_c, FootRecover);
STATE_DEFINE(dAcBBigBoss_c, Stun);
STATE_DEFINE(dAcBBigBoss_c, StunWait);
STATE_DEFINE(dAcBBigBoss_c, StunRecover);
STATE_DEFINE(dAcBBigBoss_c, StunDamage);
STATE_DEFINE(dAcBBigBoss_c, StunFrontFall);
STATE_DEFINE(dAcBBigBoss_c, Evacuation);
STATE_DEFINE(dAcBBigBoss_c, StoneWait);
STATE_DEFINE(dAcBBigBoss_c, SwordGameWait);
STATE_DEFINE(dAcBBigBoss_c, SwordGameEnd);

void dAcBBigBoss_c::initializeState_AppearWait() {}
void dAcBBigBoss_c::executeState_AppearWait() {}
void dAcBBigBoss_c::finalizeState_AppearWait() {}
void dAcBBigBoss_c::initializeState_AppearDemo() {}
void dAcBBigBoss_c::executeState_AppearDemo() {}
void dAcBBigBoss_c::finalizeState_AppearDemo() {}
void dAcBBigBoss_c::initializeState_Wait() {}
void dAcBBigBoss_c::executeState_Wait() {}
void dAcBBigBoss_c::finalizeState_Wait() {}
void dAcBBigBoss_c::initializeState_Walk() {}
void dAcBBigBoss_c::executeState_Walk() {}
void dAcBBigBoss_c::finalizeState_Walk() {}
void dAcBBigBoss_c::initializeState_Reach() {}
void dAcBBigBoss_c::executeState_Reach() {}
void dAcBBigBoss_c::finalizeState_Reach() {}
void dAcBBigBoss_c::initializeState_Turn() {}
void dAcBBigBoss_c::executeState_Turn() {}
void dAcBBigBoss_c::finalizeState_Turn() {}
void dAcBBigBoss_c::initializeState_ImpaAdvice() {}
void dAcBBigBoss_c::executeState_ImpaAdvice() {}
void dAcBBigBoss_c::finalizeState_ImpaAdvice() {}
void dAcBBigBoss_c::initializeState_ShakeHead() {}
void dAcBBigBoss_c::executeState_ShakeHead() {}
void dAcBBigBoss_c::finalizeState_ShakeHead() {}
void dAcBBigBoss_c::initializeState_HeadDamage() {}
void dAcBBigBoss_c::executeState_HeadDamage() {}
void dAcBBigBoss_c::finalizeState_HeadDamage() {}
void dAcBBigBoss_c::initializeState_HeadDamageThrow() {}
void dAcBBigBoss_c::executeState_HeadDamageThrow() {}
void dAcBBigBoss_c::finalizeState_HeadDamageThrow() {}
void dAcBBigBoss_c::initializeState_HeadPain() {}
void dAcBBigBoss_c::executeState_HeadPain() {}
void dAcBBigBoss_c::finalizeState_HeadPain() {}
void dAcBBigBoss_c::initializeState_StunPain() {}
void dAcBBigBoss_c::executeState_StunPain() {}
void dAcBBigBoss_c::finalizeState_StunPain() {}
void dAcBBigBoss_c::initializeState_HeadRecover() {}
void dAcBBigBoss_c::executeState_HeadRecover() {}
void dAcBBigBoss_c::finalizeState_HeadRecover() {}
void dAcBBigBoss_c::initializeState_Attack() {}
void dAcBBigBoss_c::executeState_Attack() {}
void dAcBBigBoss_c::finalizeState_Attack() {}
void dAcBBigBoss_c::initializeState_FrontFall() {}
void dAcBBigBoss_c::executeState_FrontFall() {}
void dAcBBigBoss_c::finalizeState_FrontFall() {}
void dAcBBigBoss_c::initializeState_Rush() {}
void dAcBBigBoss_c::executeState_Rush() {}
void dAcBBigBoss_c::finalizeState_Rush() {}
void dAcBBigBoss_c::initializeState_Rise() {}
void dAcBBigBoss_c::executeState_Rise() {}
void dAcBBigBoss_c::finalizeState_Rise() {}
void dAcBBigBoss_c::initializeState_Tied() {}
void dAcBBigBoss_c::executeState_Tied() {}
void dAcBBigBoss_c::finalizeState_Tied() {}
void dAcBBigBoss_c::initializeState_LevelUp() {}
void dAcBBigBoss_c::executeState_LevelUp() {}
void dAcBBigBoss_c::finalizeState_LevelUp() {}
void dAcBBigBoss_c::initializeState_FootBroken() {}
void dAcBBigBoss_c::executeState_FootBroken() {}
void dAcBBigBoss_c::finalizeState_FootBroken() {}
void dAcBBigBoss_c::initializeState_FootRecover() {}
void dAcBBigBoss_c::executeState_FootRecover() {}
void dAcBBigBoss_c::finalizeState_FootRecover() {}
void dAcBBigBoss_c::initializeState_Stun() {}
void dAcBBigBoss_c::executeState_Stun() {}
void dAcBBigBoss_c::finalizeState_Stun() {}
void dAcBBigBoss_c::initializeState_StunWait() {}
void dAcBBigBoss_c::executeState_StunWait() {}
void dAcBBigBoss_c::finalizeState_StunWait() {}
void dAcBBigBoss_c::initializeState_StunRecover() {}
void dAcBBigBoss_c::executeState_StunRecover() {}
void dAcBBigBoss_c::finalizeState_StunRecover() {}
void dAcBBigBoss_c::initializeState_StunDamage() {}
void dAcBBigBoss_c::executeState_StunDamage() {}
void dAcBBigBoss_c::finalizeState_StunDamage() {}
void dAcBBigBoss_c::initializeState_StunFrontFall() {}
void dAcBBigBoss_c::executeState_StunFrontFall() {}
void dAcBBigBoss_c::finalizeState_StunFrontFall() {}
void dAcBBigBoss_c::initializeState_Evacuation() {}
void dAcBBigBoss_c::executeState_Evacuation() {}
void dAcBBigBoss_c::finalizeState_Evacuation() {}
void dAcBBigBoss_c::initializeState_StoneWait() {}
void dAcBBigBoss_c::executeState_StoneWait() {}
void dAcBBigBoss_c::finalizeState_StoneWait() {}
void dAcBBigBoss_c::initializeState_SwordGameWait() {}
void dAcBBigBoss_c::executeState_SwordGameWait() {}
void dAcBBigBoss_c::finalizeState_SwordGameWait() {}
void dAcBBigBoss_c::initializeState_SwordGameEnd() {}
void dAcBBigBoss_c::executeState_SwordGameEnd() {}
void dAcBBigBoss_c::finalizeState_SwordGameEnd() {}
