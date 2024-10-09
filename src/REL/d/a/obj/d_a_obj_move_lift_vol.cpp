#include <d/a/obj/d_a_obj_move_lift_vol.h>

SPECIAL_ACTOR_PROFILE(OBJ_MOVE_LIFT_VOL, dAcOmoveLiftVol_c, fProfile::OBJ_MOVE_LIFT_VOL, 0x269, 0, 0);

STATE_DEFINE(dAcOmoveLiftVol_c, Wait);
STATE_DEFINE(dAcOmoveLiftVol_c, Ready);
STATE_DEFINE(dAcOmoveLiftVol_c, Move);
STATE_DEFINE(dAcOmoveLiftVol_c, Arrive);
STATE_DEFINE(dAcOmoveLiftVol_c, SwitchWait);
STATE_DEFINE(dAcOmoveLiftVol_c, RopeMove);

void dAcOmoveLiftVol_c::initializeState_Wait() {}
void dAcOmoveLiftVol_c::executeState_Wait() {}
void dAcOmoveLiftVol_c::finalizeState_Wait() {}
void dAcOmoveLiftVol_c::initializeState_Ready() {}
void dAcOmoveLiftVol_c::executeState_Ready() {}
void dAcOmoveLiftVol_c::finalizeState_Ready() {}
void dAcOmoveLiftVol_c::initializeState_Move() {}
void dAcOmoveLiftVol_c::executeState_Move() {}
void dAcOmoveLiftVol_c::finalizeState_Move() {}
void dAcOmoveLiftVol_c::initializeState_Arrive() {}
void dAcOmoveLiftVol_c::executeState_Arrive() {}
void dAcOmoveLiftVol_c::finalizeState_Arrive() {}
void dAcOmoveLiftVol_c::initializeState_SwitchWait() {}
void dAcOmoveLiftVol_c::executeState_SwitchWait() {}
void dAcOmoveLiftVol_c::finalizeState_SwitchWait() {}
void dAcOmoveLiftVol_c::initializeState_RopeMove() {}
void dAcOmoveLiftVol_c::executeState_RopeMove() {}
void dAcOmoveLiftVol_c::finalizeState_RopeMove() {}
