#include "d/a/obj/d_a_obj_lotus.h"

SPECIAL_ACTOR_PROFILE(OBJ_LOTUS, dAcOLotus_c, fProfile::OBJ_LOTUS, 0x234, 0, 3);

STATE_DEFINE(dAcOLotus_c, Wait);
STATE_DEFINE(dAcOLotus_c, Reverse);
STATE_DEFINE(dAcOLotus_c, Rotate);
STATE_DEFINE(dAcOLotus_c, EndRotate);

void dAcOLotus_c::initializeState_Wait() {}
void dAcOLotus_c::executeState_Wait() {}
void dAcOLotus_c::finalizeState_Wait() {}
void dAcOLotus_c::initializeState_Reverse() {}
void dAcOLotus_c::executeState_Reverse() {}
void dAcOLotus_c::finalizeState_Reverse() {}
void dAcOLotus_c::initializeState_Rotate() {}
void dAcOLotus_c::executeState_Rotate() {}
void dAcOLotus_c::finalizeState_Rotate() {}
void dAcOLotus_c::initializeState_EndRotate() {}
void dAcOLotus_c::executeState_EndRotate() {}
void dAcOLotus_c::finalizeState_EndRotate() {}
