#include <d/a/obj/d_a_obj_ship_window.h>

SPECIAL_ACTOR_PROFILE(OBJ_SHIP_WINDOW, dAcOshipWindow_c, fProfile::OBJ_SHIP_WINDOW, 0x1EB, 0, 6);

STATE_DEFINE(dAcOshipWindow_c, Close);
STATE_DEFINE(dAcOshipWindow_c, Move);
STATE_DEFINE(dAcOshipWindow_c, Open);

void dAcOshipWindow_c::initializeState_Close() {}
void dAcOshipWindow_c::executeState_Close() {}
void dAcOshipWindow_c::finalizeState_Close() {}
void dAcOshipWindow_c::initializeState_Move() {}
void dAcOshipWindow_c::executeState_Move() {}
void dAcOshipWindow_c::finalizeState_Move() {}
void dAcOshipWindow_c::initializeState_Open() {}
void dAcOshipWindow_c::executeState_Open() {}
void dAcOshipWindow_c::finalizeState_Open() {}
