#include <d/a/obj/d_a_obj_needle_underground.h>

SPECIAL_ACTOR_PROFILE(OBJ_NEEDLE_UNDERGROUND, dAcONeedleUnderground_c, fProfile::OBJ_NEEDLE_UNDERGROUND, 0X27C, 0, 2);

STATE_DEFINE(dAcONeedleUnderground_c, RemainOpen);
STATE_DEFINE(dAcONeedleUnderground_c, Close);
STATE_DEFINE(dAcONeedleUnderground_c, RemainClosed);
STATE_DEFINE(dAcONeedleUnderground_c, Open);

void dAcONeedleUnderground_c::initializeState_RemainOpen() {}
void dAcONeedleUnderground_c::executeState_RemainOpen() {}
void dAcONeedleUnderground_c::finalizeState_RemainOpen() {}
void dAcONeedleUnderground_c::initializeState_Close() {}
void dAcONeedleUnderground_c::executeState_Close() {}
void dAcONeedleUnderground_c::finalizeState_Close() {}
void dAcONeedleUnderground_c::initializeState_RemainClosed() {}
void dAcONeedleUnderground_c::executeState_RemainClosed() {}
void dAcONeedleUnderground_c::finalizeState_RemainClosed() {}
void dAcONeedleUnderground_c::initializeState_Open() {}
void dAcONeedleUnderground_c::executeState_Open() {}
void dAcONeedleUnderground_c::finalizeState_Open() {}
