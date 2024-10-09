#include <d/a/obj/d_a_obj_pipe.h>

SPECIAL_ACTOR_PROFILE(OBJ_PIPE, dAcOPipe_c, fProfile::OBJ_PIPE, 0xCF, 0, 0);

STATE_DEFINE(dAcOPipe_c, Hold);
STATE_DEFINE(dAcOPipe_c, Wait);
STATE_DEFINE(dAcOPipe_c, Drop);

void dAcOPipe_c::initializeState_Hold() {}
void dAcOPipe_c::executeState_Hold() {}
void dAcOPipe_c::finalizeState_Hold() {}
void dAcOPipe_c::initializeState_Wait() {}
void dAcOPipe_c::executeState_Wait() {}
void dAcOPipe_c::finalizeState_Wait() {}
void dAcOPipe_c::initializeState_Drop() {}
void dAcOPipe_c::executeState_Drop() {}
void dAcOPipe_c::finalizeState_Drop() {}
