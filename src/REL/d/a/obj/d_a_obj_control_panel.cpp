#include <d/a/obj/d_a_obj_control_panel.h>

SPECIAL_ACTOR_PROFILE(OBJ_CONTROL_PANEL, dAcOCtrlPnl_c, fProfile::OBJ_CONTROL_PANEL, 0X159, 0, 3);

STATE_DEFINE(dAcOCtrlPnl_c, Wait);
STATE_DEFINE(dAcOCtrlPnl_c, Control);
STATE_DEFINE(dAcOCtrlPnl_c, EndEvent);

void dAcOCtrlPnl_c::initializeState_Wait() {}
void dAcOCtrlPnl_c::executeState_Wait() {}
void dAcOCtrlPnl_c::finalizeState_Wait() {}
void dAcOCtrlPnl_c::initializeState_Control() {}
void dAcOCtrlPnl_c::executeState_Control() {}
void dAcOCtrlPnl_c::finalizeState_Control() {}
void dAcOCtrlPnl_c::initializeState_EndEvent() {}
void dAcOCtrlPnl_c::executeState_EndEvent() {}
void dAcOCtrlPnl_c::finalizeState_EndEvent() {}
