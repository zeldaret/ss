#include "d/a/obj/d_a_obj_utajima_pedestal.h"

SPECIAL_ACTOR_PROFILE(OBJ_UTAJIMA_PEDESTAL, dAcOutajimaPedestal_c, fProfile::OBJ_UTAJIMA_PEDESTAL, 0x1D1, 0, 2);

STATE_DEFINE(dAcOutajimaPedestal_c, Wait);
STATE_DEFINE(dAcOutajimaPedestal_c, Move);
STATE_DEFINE(dAcOutajimaPedestal_c, Reset);

void dAcOutajimaPedestal_c::initializeState_Wait() {}
void dAcOutajimaPedestal_c::executeState_Wait() {}
void dAcOutajimaPedestal_c::finalizeState_Wait() {}
void dAcOutajimaPedestal_c::initializeState_Move() {}
void dAcOutajimaPedestal_c::executeState_Move() {}
void dAcOutajimaPedestal_c::finalizeState_Move() {}
void dAcOutajimaPedestal_c::initializeState_Reset() {}
void dAcOutajimaPedestal_c::executeState_Reset() {}
void dAcOutajimaPedestal_c::finalizeState_Reset() {}
