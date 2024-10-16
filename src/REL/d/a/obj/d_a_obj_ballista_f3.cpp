#include "d/a/obj/d_a_obj_ballista_f3.h"

SPECIAL_ACTOR_PROFILE(OBJ_BALLISTA_F3, dAcOBallistaF3_c, fProfile::OBJ_BALLISTA_F3, 0x23E, 0, 2);

STATE_DEFINE(dAcOBallistaF3_c, Wait);
STATE_DEFINE(dAcOBallistaF3_c, Shot);
STATE_DEFINE(dAcOBallistaF3_c, Shot2);
STATE_DEFINE(dAcOBallistaF3_c, ShotEnd);
STATE_DEFINE(dAcOBallistaF3_c, FallStone);
STATE_DEFINE(dAcOBallistaF3_c, EndDemo);

void dAcOBallistaF3_c::initializeState_Wait() {}
void dAcOBallistaF3_c::executeState_Wait() {}
void dAcOBallistaF3_c::finalizeState_Wait() {}
void dAcOBallistaF3_c::initializeState_Shot() {}
void dAcOBallistaF3_c::executeState_Shot() {}
void dAcOBallistaF3_c::finalizeState_Shot() {}
void dAcOBallistaF3_c::initializeState_Shot2() {}
void dAcOBallistaF3_c::executeState_Shot2() {}
void dAcOBallistaF3_c::finalizeState_Shot2() {}
void dAcOBallistaF3_c::initializeState_ShotEnd() {}
void dAcOBallistaF3_c::executeState_ShotEnd() {}
void dAcOBallistaF3_c::finalizeState_ShotEnd() {}
void dAcOBallistaF3_c::initializeState_FallStone() {}
void dAcOBallistaF3_c::executeState_FallStone() {}
void dAcOBallistaF3_c::finalizeState_FallStone() {}
void dAcOBallistaF3_c::initializeState_EndDemo() {}
void dAcOBallistaF3_c::executeState_EndDemo() {}
void dAcOBallistaF3_c::finalizeState_EndDemo() {}
