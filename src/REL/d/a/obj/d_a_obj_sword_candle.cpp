#include <d/a/obj/d_a_obj_sword_candle.h>

SPECIAL_ACTOR_PROFILE(OBJ_SWORD_CANDLE, dAcOSwordCandle_c, fProfile::OBJ_SWORD_CANDLE, 0X24C, 0, 7);

STATE_DEFINE(dAcOSwordCandle_c, Wait);
STATE_DEFINE(dAcOSwordCandle_c, WaitClear);

void dAcOSwordCandle_c::initializeState_Wait() {}
void dAcOSwordCandle_c::executeState_Wait() {}
void dAcOSwordCandle_c::finalizeState_Wait() {}
void dAcOSwordCandle_c::initializeState_WaitClear() {}
void dAcOSwordCandle_c::executeState_WaitClear() {}
void dAcOSwordCandle_c::finalizeState_WaitClear() {}
