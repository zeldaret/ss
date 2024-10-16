#include <d/a/obj/d_a_obj_tr_shutter_cs.h>

SPECIAL_ACTOR_PROFILE(OBJ_TR_SHUTTER_CS, dAcOTrlyShtrChgScn_c, fProfile::OBJ_TR_SHUTTER_CS, 0x187, 0, 2);

STATE_DEFINE(dAcOTrlyShtrChgScn_c, Wait);
STATE_DEFINE(dAcOTrlyShtrChgScn_c, OpenEvent);

void dAcOTrlyShtrChgScn_c::initializeState_Wait() {}
void dAcOTrlyShtrChgScn_c::executeState_Wait() {}
void dAcOTrlyShtrChgScn_c::finalizeState_Wait() {}
void dAcOTrlyShtrChgScn_c::initializeState_OpenEvent() {}
void dAcOTrlyShtrChgScn_c::executeState_OpenEvent() {}
void dAcOTrlyShtrChgScn_c::finalizeState_OpenEvent() {}
