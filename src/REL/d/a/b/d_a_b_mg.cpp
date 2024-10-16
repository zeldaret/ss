#include "d/a/b/d_a_b_mg.h"

SPECIAL_ACTOR_PROFILE(B_MG, dAcBmg_c, fProfile::B_MG, 0xB6, 0, 2);

STATE_DEFINE(dAcBmg_c, Start);
STATE_DEFINE(dAcBmg_c, DiveWait);
STATE_DEFINE(dAcBmg_c, Dive);
STATE_DEFINE(dAcBmg_c, DiveWalk);
STATE_DEFINE(dAcBmg_c, Fight);
STATE_DEFINE(dAcBmg_c, HandAttack);
STATE_DEFINE(dAcBmg_c, TailAttack);
STATE_DEFINE(dAcBmg_c, DiveAttack);
STATE_DEFINE(dAcBmg_c, LostArm);
STATE_DEFINE(dAcBmg_c, MiddleDemo);
STATE_DEFINE(dAcBmg_c, ArmDamage);
STATE_DEFINE(dAcBmg_c, CoreDamage);
STATE_DEFINE(dAcBmg_c, EyeBind);
STATE_DEFINE(dAcBmg_c, BombDamage);
STATE_DEFINE(dAcBmg_c, VacuumDamage);
STATE_DEFINE(dAcBmg_c, End);

void dAcBmg_c::initializeState_Start() {}
void dAcBmg_c::executeState_Start() {}
void dAcBmg_c::finalizeState_Start() {}
void dAcBmg_c::initializeState_DiveWait() {}
void dAcBmg_c::executeState_DiveWait() {}
void dAcBmg_c::finalizeState_DiveWait() {}
void dAcBmg_c::initializeState_Dive() {}
void dAcBmg_c::executeState_Dive() {}
void dAcBmg_c::finalizeState_Dive() {}
void dAcBmg_c::initializeState_DiveWalk() {}
void dAcBmg_c::executeState_DiveWalk() {}
void dAcBmg_c::finalizeState_DiveWalk() {}
void dAcBmg_c::initializeState_Fight() {}
void dAcBmg_c::executeState_Fight() {}
void dAcBmg_c::finalizeState_Fight() {}
void dAcBmg_c::initializeState_HandAttack() {}
void dAcBmg_c::executeState_HandAttack() {}
void dAcBmg_c::finalizeState_HandAttack() {}
void dAcBmg_c::initializeState_TailAttack() {}
void dAcBmg_c::executeState_TailAttack() {}
void dAcBmg_c::finalizeState_TailAttack() {}
void dAcBmg_c::initializeState_DiveAttack() {}
void dAcBmg_c::executeState_DiveAttack() {}
void dAcBmg_c::finalizeState_DiveAttack() {}
void dAcBmg_c::initializeState_LostArm() {}
void dAcBmg_c::executeState_LostArm() {}
void dAcBmg_c::finalizeState_LostArm() {}
void dAcBmg_c::initializeState_MiddleDemo() {}
void dAcBmg_c::executeState_MiddleDemo() {}
void dAcBmg_c::finalizeState_MiddleDemo() {}
void dAcBmg_c::initializeState_ArmDamage() {}
void dAcBmg_c::executeState_ArmDamage() {}
void dAcBmg_c::finalizeState_ArmDamage() {}
void dAcBmg_c::initializeState_CoreDamage() {}
void dAcBmg_c::executeState_CoreDamage() {}
void dAcBmg_c::finalizeState_CoreDamage() {}
void dAcBmg_c::initializeState_EyeBind() {}
void dAcBmg_c::executeState_EyeBind() {}
void dAcBmg_c::finalizeState_EyeBind() {}
void dAcBmg_c::initializeState_BombDamage() {}
void dAcBmg_c::executeState_BombDamage() {}
void dAcBmg_c::finalizeState_BombDamage() {}
void dAcBmg_c::initializeState_VacuumDamage() {}
void dAcBmg_c::executeState_VacuumDamage() {}
void dAcBmg_c::finalizeState_VacuumDamage() {}
void dAcBmg_c::initializeState_End() {}
void dAcBmg_c::executeState_End() {}
void dAcBmg_c::finalizeState_End() {}
