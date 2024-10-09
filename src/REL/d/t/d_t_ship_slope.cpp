#include <d/t/d_t_ship_slope.h>

SPECIAL_ACTOR_PROFILE(TAG_SHIP_SLOPE, dTgShipSlope_c, fProfile::TAG_SHIP_SLOPE, 0X21F, 0, 0);

STATE_DEFINE(dTgShipSlope_c, Wait);
STATE_DEFINE(dTgShipSlope_c, Stop);

void dTgShipSlope_c::initializeState_Wait() {}
void dTgShipSlope_c::executeState_Wait() {}
void dTgShipSlope_c::finalizeState_Wait() {}
void dTgShipSlope_c::initializeState_Stop() {}
void dTgShipSlope_c::executeState_Stop() {}
void dTgShipSlope_c::finalizeState_Stop() {}
