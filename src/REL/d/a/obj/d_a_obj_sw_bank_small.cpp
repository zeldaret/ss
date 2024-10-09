#include <d/a/obj/d_a_obj_sw_bank_small.h>

SPECIAL_ACTOR_PROFILE(OBJ_SW_BANK_SMALL, dAcOSwBankSmall_c, fProfile::OBJ_SW_BANK_SMALL, 0x22C, 0, 6);

STATE_DEFINE(dAcOSwBankSmall_c, Wait);
STATE_DEFINE(dAcOSwBankSmall_c, Dial);
STATE_DEFINE(dAcOSwBankSmall_c, Shift);
STATE_DEFINE(dAcOSwBankSmall_c, Correct);
STATE_DEFINE(dAcOSwBankSmall_c, CorrectEnd);
STATE_DEFINE(dAcOSwBankSmall_c, BcDemo);

void dAcOSwBankSmall_c::initializeState_Wait() {}
void dAcOSwBankSmall_c::executeState_Wait() {}
void dAcOSwBankSmall_c::finalizeState_Wait() {}
void dAcOSwBankSmall_c::initializeState_Dial() {}
void dAcOSwBankSmall_c::executeState_Dial() {}
void dAcOSwBankSmall_c::finalizeState_Dial() {}
void dAcOSwBankSmall_c::initializeState_Shift() {}
void dAcOSwBankSmall_c::executeState_Shift() {}
void dAcOSwBankSmall_c::finalizeState_Shift() {}
void dAcOSwBankSmall_c::initializeState_Correct() {}
void dAcOSwBankSmall_c::executeState_Correct() {}
void dAcOSwBankSmall_c::finalizeState_Correct() {}
void dAcOSwBankSmall_c::initializeState_CorrectEnd() {}
void dAcOSwBankSmall_c::executeState_CorrectEnd() {}
void dAcOSwBankSmall_c::finalizeState_CorrectEnd() {}
void dAcOSwBankSmall_c::initializeState_BcDemo() {}
void dAcOSwBankSmall_c::executeState_BcDemo() {}
void dAcOSwBankSmall_c::finalizeState_BcDemo() {}
