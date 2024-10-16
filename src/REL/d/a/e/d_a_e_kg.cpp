#include "d/a/e/d_a_e_kg.h"

SPECIAL_ACTOR_PROFILE(E_KG, dAcEkg_c, fProfile::E_KG, 0x122, 0, 4099);

STATE_DEFINE(dAcEkg_c, Wait);
STATE_DEFINE(dAcEkg_c, Walk);
STATE_DEFINE(dAcEkg_c, Find);
STATE_DEFINE(dAcEkg_c, Chase);
STATE_DEFINE(dAcEkg_c, Step);
STATE_DEFINE(dAcEkg_c, Damage);
STATE_DEFINE(dAcEkg_c, PlayerLost);
STATE_DEFINE(dAcEkg_c, Return);
STATE_DEFINE(dAcEkg_c, WindBlow);
STATE_DEFINE(dAcEkg_c, Reverse);
STATE_DEFINE(dAcEkg_c, BlowReturn);
STATE_DEFINE(dAcEkg_c, JumpReady);
STATE_DEFINE(dAcEkg_c, Jump);
STATE_DEFINE(dAcEkg_c, JumpAttack_Hold);
STATE_DEFINE(dAcEkg_c, Release);
STATE_DEFINE(dAcEkg_c, Landing);
STATE_DEFINE(dAcEkg_c, WallWait);
STATE_DEFINE(dAcEkg_c, WallWalk);
STATE_DEFINE(dAcEkg_c, WallFind);
STATE_DEFINE(dAcEkg_c, WallStep);
STATE_DEFINE(dAcEkg_c, WallJumpReady);
STATE_DEFINE(dAcEkg_c, WallJump);
STATE_DEFINE(dAcEkg_c, WindBlowAtWall);
STATE_DEFINE(dAcEkg_c, HitAtWall);
STATE_DEFINE(dAcEkg_c, DamageAtWall);
STATE_DEFINE(dAcEkg_c, RoofWait);
STATE_DEFINE(dAcEkg_c, RoofWalk);
STATE_DEFINE(dAcEkg_c, RoofFall);
STATE_DEFINE(dAcEkg_c, HangOn);
STATE_DEFINE(dAcEkg_c, UnderWait);
STATE_DEFINE(dAcEkg_c, UnderWalk);
STATE_DEFINE(dAcEkg_c, UnderStep);
STATE_DEFINE(dAcEkg_c, UnderJump);
STATE_DEFINE(dAcEkg_c, SoundReaction);
STATE_DEFINE(dAcEkg_c, SoundReactionAtWallRoof);

void dAcEkg_c::initializeState_Wait() {}
void dAcEkg_c::executeState_Wait() {}
void dAcEkg_c::finalizeState_Wait() {}
void dAcEkg_c::initializeState_Walk() {}
void dAcEkg_c::executeState_Walk() {}
void dAcEkg_c::finalizeState_Walk() {}
void dAcEkg_c::initializeState_Find() {}
void dAcEkg_c::executeState_Find() {}
void dAcEkg_c::finalizeState_Find() {}
void dAcEkg_c::initializeState_Chase() {}
void dAcEkg_c::executeState_Chase() {}
void dAcEkg_c::finalizeState_Chase() {}
void dAcEkg_c::initializeState_Step() {}
void dAcEkg_c::executeState_Step() {}
void dAcEkg_c::finalizeState_Step() {}
void dAcEkg_c::initializeState_Damage() {}
void dAcEkg_c::executeState_Damage() {}
void dAcEkg_c::finalizeState_Damage() {}
void dAcEkg_c::initializeState_PlayerLost() {}
void dAcEkg_c::executeState_PlayerLost() {}
void dAcEkg_c::finalizeState_PlayerLost() {}
void dAcEkg_c::initializeState_Return() {}
void dAcEkg_c::executeState_Return() {}
void dAcEkg_c::finalizeState_Return() {}
void dAcEkg_c::initializeState_WindBlow() {}
void dAcEkg_c::executeState_WindBlow() {}
void dAcEkg_c::finalizeState_WindBlow() {}
void dAcEkg_c::initializeState_Reverse() {}
void dAcEkg_c::executeState_Reverse() {}
void dAcEkg_c::finalizeState_Reverse() {}
void dAcEkg_c::initializeState_BlowReturn() {}
void dAcEkg_c::executeState_BlowReturn() {}
void dAcEkg_c::finalizeState_BlowReturn() {}
void dAcEkg_c::initializeState_JumpReady() {}
void dAcEkg_c::executeState_JumpReady() {}
void dAcEkg_c::finalizeState_JumpReady() {}
void dAcEkg_c::initializeState_Jump() {}
void dAcEkg_c::executeState_Jump() {}
void dAcEkg_c::finalizeState_Jump() {}
void dAcEkg_c::initializeState_JumpAttack_Hold() {}
void dAcEkg_c::executeState_JumpAttack_Hold() {}
void dAcEkg_c::finalizeState_JumpAttack_Hold() {}
void dAcEkg_c::initializeState_Release() {}
void dAcEkg_c::executeState_Release() {}
void dAcEkg_c::finalizeState_Release() {}
void dAcEkg_c::initializeState_Landing() {}
void dAcEkg_c::executeState_Landing() {}
void dAcEkg_c::finalizeState_Landing() {}
void dAcEkg_c::initializeState_WallWait() {}
void dAcEkg_c::executeState_WallWait() {}
void dAcEkg_c::finalizeState_WallWait() {}
void dAcEkg_c::initializeState_WallWalk() {}
void dAcEkg_c::executeState_WallWalk() {}
void dAcEkg_c::finalizeState_WallWalk() {}
void dAcEkg_c::initializeState_WallFind() {}
void dAcEkg_c::executeState_WallFind() {}
void dAcEkg_c::finalizeState_WallFind() {}
void dAcEkg_c::initializeState_WallStep() {}
void dAcEkg_c::executeState_WallStep() {}
void dAcEkg_c::finalizeState_WallStep() {}
void dAcEkg_c::initializeState_WallJumpReady() {}
void dAcEkg_c::executeState_WallJumpReady() {}
void dAcEkg_c::finalizeState_WallJumpReady() {}
void dAcEkg_c::initializeState_WallJump() {}
void dAcEkg_c::executeState_WallJump() {}
void dAcEkg_c::finalizeState_WallJump() {}
void dAcEkg_c::initializeState_WindBlowAtWall() {}
void dAcEkg_c::executeState_WindBlowAtWall() {}
void dAcEkg_c::finalizeState_WindBlowAtWall() {}
void dAcEkg_c::initializeState_HitAtWall() {}
void dAcEkg_c::executeState_HitAtWall() {}
void dAcEkg_c::finalizeState_HitAtWall() {}
void dAcEkg_c::initializeState_DamageAtWall() {}
void dAcEkg_c::executeState_DamageAtWall() {}
void dAcEkg_c::finalizeState_DamageAtWall() {}
void dAcEkg_c::initializeState_RoofWait() {}
void dAcEkg_c::executeState_RoofWait() {}
void dAcEkg_c::finalizeState_RoofWait() {}
void dAcEkg_c::initializeState_RoofWalk() {}
void dAcEkg_c::executeState_RoofWalk() {}
void dAcEkg_c::finalizeState_RoofWalk() {}
void dAcEkg_c::initializeState_RoofFall() {}
void dAcEkg_c::executeState_RoofFall() {}
void dAcEkg_c::finalizeState_RoofFall() {}
void dAcEkg_c::initializeState_HangOn() {}
void dAcEkg_c::executeState_HangOn() {}
void dAcEkg_c::finalizeState_HangOn() {}
void dAcEkg_c::initializeState_UnderWait() {}
void dAcEkg_c::executeState_UnderWait() {}
void dAcEkg_c::finalizeState_UnderWait() {}
void dAcEkg_c::initializeState_UnderWalk() {}
void dAcEkg_c::executeState_UnderWalk() {}
void dAcEkg_c::finalizeState_UnderWalk() {}
void dAcEkg_c::initializeState_UnderStep() {}
void dAcEkg_c::executeState_UnderStep() {}
void dAcEkg_c::finalizeState_UnderStep() {}
void dAcEkg_c::initializeState_UnderJump() {}
void dAcEkg_c::executeState_UnderJump() {}
void dAcEkg_c::finalizeState_UnderJump() {}
void dAcEkg_c::initializeState_SoundReaction() {}
void dAcEkg_c::executeState_SoundReaction() {}
void dAcEkg_c::finalizeState_SoundReaction() {}
void dAcEkg_c::initializeState_SoundReactionAtWallRoof() {}
void dAcEkg_c::executeState_SoundReactionAtWallRoof() {}
void dAcEkg_c::finalizeState_SoundReactionAtWallRoof() {}
