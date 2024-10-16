#include "d/a/e/d_a_e_chaser_b.h"

SPECIAL_ACTOR_PROFILE(E_CHB, dAcEChb_c, fProfile::E_CHB, 0x103, 0, 3);

STATE_DEFINE(dAcEChb_c, Standby);
STATE_DEFINE(dAcEChb_c, Chase);
STATE_DEFINE(dAcEChb_c, Start);
STATE_DEFINE(dAcEChb_c, Attack);

void dAcEChb_c::initializeState_Standby() {}
void dAcEChb_c::executeState_Standby() {}
void dAcEChb_c::finalizeState_Standby() {}
void dAcEChb_c::initializeState_Chase() {}
void dAcEChb_c::executeState_Chase() {}
void dAcEChb_c::finalizeState_Chase() {}
void dAcEChb_c::initializeState_Start() {}
void dAcEChb_c::executeState_Start() {}
void dAcEChb_c::finalizeState_Start() {}
void dAcEChb_c::initializeState_Attack() {}
void dAcEChb_c::executeState_Attack() {}
void dAcEChb_c::finalizeState_Attack() {}
