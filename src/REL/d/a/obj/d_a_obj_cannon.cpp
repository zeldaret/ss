#include <d/a/obj/d_a_obj_cannon.h>

SPECIAL_ACTOR_PROFILE(OBJ_CANNON, dAcOcannon_c, fProfile::OBJ_CANNON, 0X1C9, 0, 2);

STATE_DEFINE(dAcOcannon_c, Wait);
STATE_DEFINE(dAcOcannon_c, PreCharge);
STATE_DEFINE(dAcOcannon_c, Move);
STATE_DEFINE(dAcOcannon_c, AfterCharge);
STATE_DEFINE(dAcOcannon_c, Shot);
STATE_DEFINE(dAcOcannon_c, After);

void dAcOcannon_c::initializeState_Wait() {}
void dAcOcannon_c::executeState_Wait() {}
void dAcOcannon_c::finalizeState_Wait() {}
void dAcOcannon_c::initializeState_PreCharge() {}
void dAcOcannon_c::executeState_PreCharge() {}
void dAcOcannon_c::finalizeState_PreCharge() {}
void dAcOcannon_c::initializeState_Move() {}
void dAcOcannon_c::executeState_Move() {}
void dAcOcannon_c::finalizeState_Move() {}
void dAcOcannon_c::initializeState_AfterCharge() {}
void dAcOcannon_c::executeState_AfterCharge() {}
void dAcOcannon_c::finalizeState_AfterCharge() {}
void dAcOcannon_c::initializeState_Shot() {}
void dAcOcannon_c::executeState_Shot() {}
void dAcOcannon_c::finalizeState_Shot() {}
void dAcOcannon_c::initializeState_After() {}
void dAcOcannon_c::executeState_After() {}
void dAcOcannon_c::finalizeState_After() {}
