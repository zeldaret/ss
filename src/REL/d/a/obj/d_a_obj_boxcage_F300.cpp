#include <d/a/obj/d_a_obj_boxcage_F300.h>

SPECIAL_ACTOR_PROFILE(OBJ_BOXCAGE_F300, dAcOBoxCageF300_c, fProfile::OBJ_BOXCAGE_F300, 0x179, 0, 3);

STATE_DEFINE(dAcOBoxCageF300_c, Wait);
STATE_DEFINE(dAcOBoxCageF300_c, Move);
STATE_DEFINE(dAcOBoxCageF300_c, End);

void dAcOBoxCageF300_c::initializeState_Wait() {}
void dAcOBoxCageF300_c::executeState_Wait() {}
void dAcOBoxCageF300_c::finalizeState_Wait() {}
void dAcOBoxCageF300_c::initializeState_Move() {}
void dAcOBoxCageF300_c::executeState_Move() {}
void dAcOBoxCageF300_c::finalizeState_Move() {}
void dAcOBoxCageF300_c::initializeState_End() {}
void dAcOBoxCageF300_c::executeState_End() {}
void dAcOBoxCageF300_c::finalizeState_End() {}
