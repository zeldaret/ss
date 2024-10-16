#include "d/t/d_t_ship_flood.h"

SPECIAL_ACTOR_PROFILE(TAG_SHIP_FLOOD, dTgShipFlood_c, fProfile::TAG_SHIP_FLOOD, 0x220, 0, 0);

STATE_DEFINE(dTgShipFlood_c, Wait);
STATE_DEFINE(dTgShipFlood_c, Stop);

void dTgShipFlood_c::initializeState_Wait() {}
void dTgShipFlood_c::executeState_Wait() {}
void dTgShipFlood_c::finalizeState_Wait() {}
void dTgShipFlood_c::initializeState_Stop() {}
void dTgShipFlood_c::executeState_Stop() {}
void dTgShipFlood_c::finalizeState_Stop() {}
