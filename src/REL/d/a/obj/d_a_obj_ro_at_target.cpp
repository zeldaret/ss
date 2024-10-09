#include <d/a/obj/d_a_obj_ro_at_target.h>

SPECIAL_ACTOR_PROFILE(OBJ_RO_AT_TARGET, dAcOroAtTarget_c, fProfile::OBJ_RO_AT_TARGET, 0X1FE, 0, 0);

STATE_DEFINE(dAcOroAtTarget_c, Wait);
STATE_DEFINE(dAcOroAtTarget_c, Play);
STATE_DEFINE(dAcOroAtTarget_c, Death);

void dAcOroAtTarget_c::initializeState_Wait() {}
void dAcOroAtTarget_c::executeState_Wait() {}
void dAcOroAtTarget_c::finalizeState_Wait() {}
void dAcOroAtTarget_c::initializeState_Play() {}
void dAcOroAtTarget_c::executeState_Play() {}
void dAcOroAtTarget_c::finalizeState_Play() {}
void dAcOroAtTarget_c::initializeState_Death() {}
void dAcOroAtTarget_c::executeState_Death() {}
void dAcOroAtTarget_c::finalizeState_Death() {}
