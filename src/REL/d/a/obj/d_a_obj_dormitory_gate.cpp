#include <d/a/obj/d_a_obj_dormitory_gate.h>

SPECIAL_ACTOR_PROFILE(OBJ_DORMITORY_GATE, dAcODormitoryGate_c, fProfile::OBJ_DORMITORY_GATE, 0x17A, 0, 3);

STATE_DEFINE(dAcODormitoryGate_c, Wait);
STATE_DEFINE(dAcODormitoryGate_c, End);

void dAcODormitoryGate_c::initializeState_Wait() {}
void dAcODormitoryGate_c::executeState_Wait() {}
void dAcODormitoryGate_c::finalizeState_Wait() {}
void dAcODormitoryGate_c::initializeState_End() {}
void dAcODormitoryGate_c::executeState_End() {}
void dAcODormitoryGate_c::finalizeState_End() {}
