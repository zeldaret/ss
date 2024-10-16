#include <d/a/obj/d_a_obj_wind04.h>

SPECIAL_ACTOR_PROFILE(OBJ_WIND04, dAcOwind04_c, fProfile::OBJ_WIND04, 0xDE, 0, 263);

STATE_DEFINE(dAcOwind04_c, Wait);
STATE_DEFINE(dAcOwind04_c, Loop);
STATE_DEFINE(dAcOwind04_c, Stop);
STATE_DEFINE(dAcOwind04_c, Dead);

void dAcOwind04_c::initializeState_Wait() {}
void dAcOwind04_c::executeState_Wait() {}
void dAcOwind04_c::finalizeState_Wait() {}
void dAcOwind04_c::initializeState_Loop() {}
void dAcOwind04_c::executeState_Loop() {}
void dAcOwind04_c::finalizeState_Loop() {}
void dAcOwind04_c::initializeState_Stop() {}
void dAcOwind04_c::executeState_Stop() {}
void dAcOwind04_c::finalizeState_Stop() {}
void dAcOwind04_c::initializeState_Dead() {}
void dAcOwind04_c::executeState_Dead() {}
void dAcOwind04_c::finalizeState_Dead() {}
