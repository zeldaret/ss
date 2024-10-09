#include <d/a/obj/d_a_obj_light_line.h>

SPECIAL_ACTOR_PROFILE(OBJ_LIGHT_LINE, dAcOlightLine_c, fProfile::OBJ_LIGHT_LINE, 0x197, 0, 4);

STATE_DEFINE(dAcOlightLine_c, OutSide);
STATE_DEFINE(dAcOlightLine_c, InSide);

void dAcOlightLine_c::initializeState_OutSide() {}
void dAcOlightLine_c::executeState_OutSide() {}
void dAcOlightLine_c::finalizeState_OutSide() {}
void dAcOlightLine_c::initializeState_InSide() {}
void dAcOlightLine_c::executeState_InSide() {}
void dAcOlightLine_c::finalizeState_InSide() {}
