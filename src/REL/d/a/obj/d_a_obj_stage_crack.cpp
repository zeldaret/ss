#include <d/a/obj/d_a_obj_stage_crack.h>

SPECIAL_ACTOR_PROFILE(OBJ_STAGE_CRACK, dAcOstageCrack_c, fProfile::OBJ_STAGE_CRACK, 0x1E0, 0, 2);

STATE_DEFINE(dAcOstageCrack_c, Wait);
STATE_DEFINE(dAcOstageCrack_c, Stay);
STATE_DEFINE(dAcOstageCrack_c, Reverse);
STATE_DEFINE(dAcOstageCrack_c, Delete);

void dAcOstageCrack_c::initializeState_Wait() {}
void dAcOstageCrack_c::executeState_Wait() {}
void dAcOstageCrack_c::finalizeState_Wait() {}
void dAcOstageCrack_c::initializeState_Stay() {}
void dAcOstageCrack_c::executeState_Stay() {}
void dAcOstageCrack_c::finalizeState_Stay() {}
void dAcOstageCrack_c::initializeState_Reverse() {}
void dAcOstageCrack_c::executeState_Reverse() {}
void dAcOstageCrack_c::finalizeState_Reverse() {}
void dAcOstageCrack_c::initializeState_Delete() {}
void dAcOstageCrack_c::executeState_Delete() {}
void dAcOstageCrack_c::finalizeState_Delete() {}
