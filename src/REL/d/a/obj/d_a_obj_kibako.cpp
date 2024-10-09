#include <d/a/obj/d_a_obj_kibako.h>

SPECIAL_ACTOR_PROFILE(OBJ_KIBAKO, dAcOkibako_c, fProfile::OBJ_KIBAKO, 0X12C, 0, 2);

STATE_DEFINE(dAcOkibako_c, Wait);
STATE_DEFINE(dAcOkibako_c, Fire);
STATE_DEFINE(dAcOkibako_c, PushPull);
STATE_DEFINE(dAcOkibako_c, RopeGrab);
STATE_DEFINE(dAcOkibako_c, DeleteEvent);

void dAcOkibako_c::initializeState_Wait() {}
void dAcOkibako_c::executeState_Wait() {}
void dAcOkibako_c::finalizeState_Wait() {}
void dAcOkibako_c::initializeState_Fire() {}
void dAcOkibako_c::executeState_Fire() {}
void dAcOkibako_c::finalizeState_Fire() {}
void dAcOkibako_c::initializeState_PushPull() {}
void dAcOkibako_c::executeState_PushPull() {}
void dAcOkibako_c::finalizeState_PushPull() {}
void dAcOkibako_c::initializeState_RopeGrab() {}
void dAcOkibako_c::executeState_RopeGrab() {}
void dAcOkibako_c::finalizeState_RopeGrab() {}
void dAcOkibako_c::initializeState_DeleteEvent() {}
void dAcOkibako_c::executeState_DeleteEvent() {}
void dAcOkibako_c::finalizeState_DeleteEvent() {}
