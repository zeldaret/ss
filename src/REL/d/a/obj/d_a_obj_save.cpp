#include "d/a/obj/d_a_obj_save.h"

SPECIAL_ACTOR_PROFILE(OBJ_SAVE, dAcOSave_c, fProfile::OBJ_SAVE, 0x273, 0, 7);

STATE_DEFINE(dAcOSave_c, Moss);
STATE_DEFINE(dAcOSave_c, Shine);
STATE_DEFINE(dAcOSave_c, Wait);
STATE_DEFINE(dAcOSave_c, Query);
STATE_DEFINE(dAcOSave_c, LightShaft);
STATE_DEFINE(dAcOSave_c, Escape);

dAcOSave_c::dAcOSave_c() : mStateMgr(*this, sStateID::null) {}
dAcOSave_c::~dAcOSave_c() {}

void dAcOSave_c::initializeState_Moss() {}
void dAcOSave_c::executeState_Moss() {}
void dAcOSave_c::finalizeState_Moss() {}
void dAcOSave_c::initializeState_Shine() {}
void dAcOSave_c::executeState_Shine() {}
void dAcOSave_c::finalizeState_Shine() {}
void dAcOSave_c::initializeState_Wait() {}
void dAcOSave_c::executeState_Wait() {}
void dAcOSave_c::finalizeState_Wait() {}
void dAcOSave_c::initializeState_Query() {}
void dAcOSave_c::executeState_Query() {}
void dAcOSave_c::finalizeState_Query() {}
void dAcOSave_c::initializeState_LightShaft() {}
void dAcOSave_c::executeState_LightShaft() {}
void dAcOSave_c::finalizeState_LightShaft() {}
void dAcOSave_c::initializeState_Escape() {}
void dAcOSave_c::executeState_Escape() {}
void dAcOSave_c::finalizeState_Escape() {}
