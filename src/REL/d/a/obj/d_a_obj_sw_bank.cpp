#include <d/a/obj/d_a_obj_sw_bank.h>

SPECIAL_ACTOR_PROFILE(OBJ_SW_BANK, dAcOSwBank_c, fProfile::OBJ_SW_BANK, 0x22B, 0, 3);

STATE_DEFINE(dAcOSwBank_c, Wait);
STATE_DEFINE(dAcOSwBank_c, Dial);
STATE_DEFINE(dAcOSwBank_c, Shift);
STATE_DEFINE(dAcOSwBank_c, Correct);

void dAcOSwBank_c::initializeState_Wait() {}
void dAcOSwBank_c::executeState_Wait() {}
void dAcOSwBank_c::finalizeState_Wait() {}
void dAcOSwBank_c::initializeState_Dial() {}
void dAcOSwBank_c::executeState_Dial() {}
void dAcOSwBank_c::finalizeState_Dial() {}
void dAcOSwBank_c::initializeState_Shift() {}
void dAcOSwBank_c::executeState_Shift() {}
void dAcOSwBank_c::finalizeState_Shift() {}
void dAcOSwBank_c::initializeState_Correct() {}
void dAcOSwBank_c::executeState_Correct() {}
void dAcOSwBank_c::finalizeState_Correct() {}
