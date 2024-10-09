#include <d/a/obj/d_a_obj_god_mark.h>

SPECIAL_ACTOR_PROFILE(OBJ_GOD_MARK, dAcOgodMark_c, fProfile::OBJ_GOD_MARK, 0x20A, 0, 6);

STATE_DEFINE(dAcOgodMark_c, Wait);
STATE_DEFINE(dAcOgodMark_c, Shine);

void dAcOgodMark_c::initializeState_Wait() {}
void dAcOgodMark_c::executeState_Wait() {}
void dAcOgodMark_c::finalizeState_Wait() {}
void dAcOgodMark_c::initializeState_Shine() {}
void dAcOgodMark_c::executeState_Shine() {}
void dAcOgodMark_c::finalizeState_Shine() {}
