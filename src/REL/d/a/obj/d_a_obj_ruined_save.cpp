#include <d/a/obj/d_a_obj_ruined_save.h>

SPECIAL_ACTOR_PROFILE(OBJ_RUINED_SAVE, dAcOruinedSave_c, fProfile::OBJ_RUINED_SAVE, 0x15C, 0, 0);

STATE_DEFINE(dAcOruinedSave_c, Wait);
STATE_DEFINE(dAcOruinedSave_c, Vanish);

void dAcOruinedSave_c::initializeState_Wait() {}
void dAcOruinedSave_c::executeState_Wait() {}
void dAcOruinedSave_c::finalizeState_Wait() {}
void dAcOruinedSave_c::initializeState_Vanish() {}
void dAcOruinedSave_c::executeState_Vanish() {}
void dAcOruinedSave_c::finalizeState_Vanish() {}
