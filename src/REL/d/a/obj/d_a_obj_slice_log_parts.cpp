#include <d/a/obj/d_a_obj_slice_log_parts.h>

SPECIAL_ACTOR_PROFILE(OBJ_SLICE_LOG_PARTS, dAcOsliceLogParts_c, fProfile::OBJ_SLICE_LOG_PARTS, 0x1F6, 0, 6);

STATE_DEFINE(dAcOsliceLogParts_c, Wait);
STATE_DEFINE(dAcOsliceLogParts_c, Fly);
STATE_DEFINE(dAcOsliceLogParts_c, Ground);
STATE_DEFINE(dAcOsliceLogParts_c, GroundSecond);
STATE_DEFINE(dAcOsliceLogParts_c, Shake);
STATE_DEFINE(dAcOsliceLogParts_c, Vanish);
STATE_DEFINE(dAcOsliceLogParts_c, Finish);
STATE_DEFINE(dAcOsliceLogParts_c, GroundAftFin);

void dAcOsliceLogParts_c::initializeState_Wait() {}
void dAcOsliceLogParts_c::executeState_Wait() {}
void dAcOsliceLogParts_c::finalizeState_Wait() {}
void dAcOsliceLogParts_c::initializeState_Fly() {}
void dAcOsliceLogParts_c::executeState_Fly() {}
void dAcOsliceLogParts_c::finalizeState_Fly() {}
void dAcOsliceLogParts_c::initializeState_Ground() {}
void dAcOsliceLogParts_c::executeState_Ground() {}
void dAcOsliceLogParts_c::finalizeState_Ground() {}
void dAcOsliceLogParts_c::initializeState_GroundSecond() {}
void dAcOsliceLogParts_c::executeState_GroundSecond() {}
void dAcOsliceLogParts_c::finalizeState_GroundSecond() {}
void dAcOsliceLogParts_c::initializeState_Shake() {}
void dAcOsliceLogParts_c::executeState_Shake() {}
void dAcOsliceLogParts_c::finalizeState_Shake() {}
void dAcOsliceLogParts_c::initializeState_Vanish() {}
void dAcOsliceLogParts_c::executeState_Vanish() {}
void dAcOsliceLogParts_c::finalizeState_Vanish() {}
void dAcOsliceLogParts_c::initializeState_Finish() {}
void dAcOsliceLogParts_c::executeState_Finish() {}
void dAcOsliceLogParts_c::finalizeState_Finish() {}
void dAcOsliceLogParts_c::initializeState_GroundAftFin() {}
void dAcOsliceLogParts_c::executeState_GroundAftFin() {}
void dAcOsliceLogParts_c::finalizeState_GroundAftFin() {}
