#include <d/a/b/d_a_b_kr_arm.h>

SPECIAL_ACTOR_PROFILE(B_KRA, dAcBkra_c, fProfile::B_KRA, 0XB9, 0, 0);

STATE_DEFINE(dAcBkra_c, Wait);
STATE_DEFINE(dAcBkra_c, InRoom);
STATE_DEFINE(dAcBkra_c, InRoomDemo);
STATE_DEFINE(dAcBkra_c, Escape);
STATE_DEFINE(dAcBkra_c, AttackA);
STATE_DEFINE(dAcBkra_c, AttackB);
STATE_DEFINE(dAcBkra_c, Damage);

void dAcBkra_c::initializeState_Wait() {}
void dAcBkra_c::executeState_Wait() {}
void dAcBkra_c::finalizeState_Wait() {}
void dAcBkra_c::initializeState_InRoom() {}
void dAcBkra_c::executeState_InRoom() {}
void dAcBkra_c::finalizeState_InRoom() {}
void dAcBkra_c::initializeState_InRoomDemo() {}
void dAcBkra_c::executeState_InRoomDemo() {}
void dAcBkra_c::finalizeState_InRoomDemo() {}
void dAcBkra_c::initializeState_Escape() {}
void dAcBkra_c::executeState_Escape() {}
void dAcBkra_c::finalizeState_Escape() {}
void dAcBkra_c::initializeState_AttackA() {}
void dAcBkra_c::executeState_AttackA() {}
void dAcBkra_c::finalizeState_AttackA() {}
void dAcBkra_c::initializeState_AttackB() {}
void dAcBkra_c::executeState_AttackB() {}
void dAcBkra_c::finalizeState_AttackB() {}
void dAcBkra_c::initializeState_Damage() {}
void dAcBkra_c::executeState_Damage() {}
void dAcBkra_c::finalizeState_Damage() {}
