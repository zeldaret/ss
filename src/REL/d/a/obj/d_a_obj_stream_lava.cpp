#include "d/a/obj/d_a_obj_stream_lava.h"

SPECIAL_ACTOR_PROFILE(OBJ_STREAM_LAVA, dAcOstreamLava_c, fProfile::OBJ_STREAM_LAVA, 0x1FA, 0, 6);

STATE_DEFINE(dAcOstreamLava_c, Wait);
STATE_DEFINE(dAcOstreamLava_c, Stream);

void dAcOstreamLava_c::initializeState_Wait() {}
void dAcOstreamLava_c::executeState_Wait() {}
void dAcOstreamLava_c::finalizeState_Wait() {}
void dAcOstreamLava_c::initializeState_Stream() {}
void dAcOstreamLava_c::executeState_Stream() {}
void dAcOstreamLava_c::finalizeState_Stream() {}
