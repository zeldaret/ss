#include <d/a/e/d_a_e_st_wire.h>

SPECIAL_ACTOR_PROFILE(E_ST_WIRE, dAcEstwire_c, fProfile::E_ST_WIRE, 0xCB, 0, 0);

STATE_DEFINE(dAcEstwire_c, Shoot);
STATE_DEFINE(dAcEstwire_c, Fix);
STATE_DEFINE(dAcEstwire_c, Out);
STATE_DEFINE(dAcEstwire_c, AirBack);

void dAcEstwire_c::initializeState_Shoot() {}
void dAcEstwire_c::executeState_Shoot() {}
void dAcEstwire_c::finalizeState_Shoot() {}
void dAcEstwire_c::initializeState_Fix() {}
void dAcEstwire_c::executeState_Fix() {}
void dAcEstwire_c::finalizeState_Fix() {}
void dAcEstwire_c::initializeState_Out() {}
void dAcEstwire_c::executeState_Out() {}
void dAcEstwire_c::finalizeState_Out() {}
void dAcEstwire_c::initializeState_AirBack() {}
void dAcEstwire_c::executeState_AirBack() {}
void dAcEstwire_c::finalizeState_AirBack() {}
