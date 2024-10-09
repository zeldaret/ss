#include <d/a/b/d_a_b_kr_hair.h>

SPECIAL_ACTOR_PROFILE(B_KRH, dAcBkrh_c, fProfile::B_KRH, 0XB8, 0, 0);

STATE_DEFINE(dAcBkrh_c, Normal);
STATE_DEFINE(dAcBkrh_c, Attack);
STATE_DEFINE(dAcBkrh_c, Escape);

void dAcBkrh_c::initializeState_Normal() {}
void dAcBkrh_c::executeState_Normal() {}
void dAcBkrh_c::finalizeState_Normal() {}
void dAcBkrh_c::initializeState_Attack() {}
void dAcBkrh_c::executeState_Attack() {}
void dAcBkrh_c::finalizeState_Attack() {}
void dAcBkrh_c::initializeState_Escape() {}
void dAcBkrh_c::executeState_Escape() {}
void dAcBkrh_c::finalizeState_Escape() {}
