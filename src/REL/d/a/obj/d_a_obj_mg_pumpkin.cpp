#include <d/a/obj/d_a_obj_mg_pumpkin.h>

SPECIAL_ACTOR_PROFILE(OBJ_MG_PUMPKIN, dAcOMgPumpkin_c, fProfile::OBJ_MG_PUMPKIN, 0x14D, 0, 3);

STATE_DEFINE(dAcOMgPumpkin_c, Wait);
STATE_DEFINE(dAcOMgPumpkin_c, Carry);
STATE_DEFINE(dAcOMgPumpkin_c, Drop);
STATE_DEFINE(dAcOMgPumpkin_c, DropAll);
STATE_DEFINE(dAcOMgPumpkin_c, DropPartial);

void dAcOMgPumpkin_c::initializeState_Wait() {}
void dAcOMgPumpkin_c::executeState_Wait() {}
void dAcOMgPumpkin_c::finalizeState_Wait() {}
void dAcOMgPumpkin_c::initializeState_Carry() {}
void dAcOMgPumpkin_c::executeState_Carry() {}
void dAcOMgPumpkin_c::finalizeState_Carry() {}
void dAcOMgPumpkin_c::initializeState_Drop() {}
void dAcOMgPumpkin_c::executeState_Drop() {}
void dAcOMgPumpkin_c::finalizeState_Drop() {}
void dAcOMgPumpkin_c::initializeState_DropAll() {}
void dAcOMgPumpkin_c::executeState_DropAll() {}
void dAcOMgPumpkin_c::finalizeState_DropAll() {}
void dAcOMgPumpkin_c::initializeState_DropPartial() {}
void dAcOMgPumpkin_c::executeState_DropPartial() {}
void dAcOMgPumpkin_c::finalizeState_DropPartial() {}
