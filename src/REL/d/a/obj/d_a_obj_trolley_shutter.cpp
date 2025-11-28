#include "d/a/obj/d_a_obj_trolley_shutter.h"

SPECIAL_ACTOR_PROFILE(OBJ_TROLLEY_SHUTTER, dAcOTrolleyShutter_c, fProfile::OBJ_TROLLEY_SHUTTER, 0x186, 0, 6);

STATE_DEFINE(dAcOTrolleyShutter_c, Wait);
STATE_DEFINE(dAcOTrolleyShutter_c, OpenEvent);

bool dAcOTrolleyShutter_c::canOpen() {
    // Not implemented - Used to emit dtor
    return true;
}
void dAcOTrolleyShutter_c::initializeState_Wait() {}
void dAcOTrolleyShutter_c::executeState_Wait() {}
void dAcOTrolleyShutter_c::finalizeState_Wait() {}
void dAcOTrolleyShutter_c::initializeState_OpenEvent() {}
void dAcOTrolleyShutter_c::executeState_OpenEvent() {}
void dAcOTrolleyShutter_c::finalizeState_OpenEvent() {}
