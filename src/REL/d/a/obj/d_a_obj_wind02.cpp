#include <d/a/obj/d_a_obj_wind02.h>

SPECIAL_ACTOR_PROFILE(OBJ_WIND02, dAcOwind02_c, fProfile::OBJ_WIND02, 0xDC, 0, 263);

STATE_DEFINE(dAcOwind02_c, Wait);
STATE_DEFINE(dAcOwind02_c, Loop);
STATE_DEFINE(dAcOwind02_c, Stop);
STATE_DEFINE(dAcOwind02_c, Dead);

void dAcOwind02_c::initializeState_Wait() {}
void dAcOwind02_c::executeState_Wait() {}
void dAcOwind02_c::finalizeState_Wait() {}
void dAcOwind02_c::initializeState_Loop() {}
void dAcOwind02_c::executeState_Loop() {}
void dAcOwind02_c::finalizeState_Loop() {}
void dAcOwind02_c::initializeState_Stop() {}
void dAcOwind02_c::executeState_Stop() {}
void dAcOwind02_c::finalizeState_Stop() {}
void dAcOwind02_c::initializeState_Dead() {}
void dAcOwind02_c::executeState_Dead() {}
void dAcOwind02_c::finalizeState_Dead() {}
