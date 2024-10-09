#include <d/a/obj/d_a_obj_sw_dir.h>

SPECIAL_ACTOR_PROFILE(OBJ_SW_DIR, dAcOSwDir_c, fProfile::OBJ_SW_DIR, 0X229, 0, 7);

STATE_DEFINE(dAcOSwDir_c, Wait);
STATE_DEFINE(dAcOSwDir_c, Done);

void dAcOSwDir_c::initializeState_Wait() {}
void dAcOSwDir_c::executeState_Wait() {}
void dAcOSwDir_c::finalizeState_Wait() {}
void dAcOSwDir_c::initializeState_Done() {}
void dAcOSwDir_c::executeState_Done() {}
void dAcOSwDir_c::finalizeState_Done() {}
