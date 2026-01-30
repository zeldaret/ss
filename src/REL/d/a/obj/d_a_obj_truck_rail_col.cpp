#include "d/a/obj/d_a_obj_truck_rail_col.h"

SPECIAL_ACTOR_PROFILE(OBJ_TRUCK_RAIL_COL, dAcOTruckRailCol_c, fProfile::OBJ_TRUCK_RAIL_COL, 0x29C, 0, 0);

STATE_DEFINE(dAcOTruckRailCol_c, Wait);

dAcOTruckRailCol_c::dAcOTruckRailCol_c() : mStateMgr(*this) {}
dAcOTruckRailCol_c::~dAcOTruckRailCol_c() {}

void dAcOTruckRailCol_c::initializeState_Wait() {}
void dAcOTruckRailCol_c::executeState_Wait() {}
void dAcOTruckRailCol_c::finalizeState_Wait() {}
