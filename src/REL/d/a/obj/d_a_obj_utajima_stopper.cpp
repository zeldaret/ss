#include <d/a/obj/d_a_obj_utajima_stopper.h>

SPECIAL_ACTOR_PROFILE(OBJ_UTAJIMA_STOPPER, dAcOutajimaStopper_c, fProfile::OBJ_UTAJIMA_STOPPER, 0X1CF, 0, 6);

STATE_DEFINE(dAcOutajimaStopper_c, Down);
STATE_DEFINE(dAcOutajimaStopper_c, Rise);
STATE_DEFINE(dAcOutajimaStopper_c, Up);
STATE_DEFINE(dAcOutajimaStopper_c, Fall);
STATE_DEFINE(dAcOutajimaStopper_c, Wait);

void dAcOutajimaStopper_c::initializeState_Down() {}
void dAcOutajimaStopper_c::executeState_Down() {}
void dAcOutajimaStopper_c::finalizeState_Down() {}
void dAcOutajimaStopper_c::initializeState_Rise() {}
void dAcOutajimaStopper_c::executeState_Rise() {}
void dAcOutajimaStopper_c::finalizeState_Rise() {}
void dAcOutajimaStopper_c::initializeState_Up() {}
void dAcOutajimaStopper_c::executeState_Up() {}
void dAcOutajimaStopper_c::finalizeState_Up() {}
void dAcOutajimaStopper_c::initializeState_Fall() {}
void dAcOutajimaStopper_c::executeState_Fall() {}
void dAcOutajimaStopper_c::finalizeState_Fall() {}
void dAcOutajimaStopper_c::initializeState_Wait() {}
void dAcOutajimaStopper_c::executeState_Wait() {}
void dAcOutajimaStopper_c::finalizeState_Wait() {}
