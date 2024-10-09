#include <d/a/obj/d_a_obj_boat.h>

SPECIAL_ACTOR_PROFILE(OBJ_BOAT, dAcOBoat_c, fProfile::OBJ_BOAT, 0X23A, 0, 7);

STATE_DEFINE(dAcOBoat_c, Wait);
STATE_DEFINE(dAcOBoat_c, Fall);
STATE_DEFINE(dAcOBoat_c, FallEnd);
STATE_DEFINE(dAcOBoat_c, FallStop);
STATE_DEFINE(dAcOBoat_c, Up);
STATE_DEFINE(dAcOBoat_c, UpEnd);

void dAcOBoat_c::initializeState_Wait() {}
void dAcOBoat_c::executeState_Wait() {}
void dAcOBoat_c::finalizeState_Wait() {}
void dAcOBoat_c::initializeState_Fall() {}
void dAcOBoat_c::executeState_Fall() {}
void dAcOBoat_c::finalizeState_Fall() {}
void dAcOBoat_c::initializeState_FallEnd() {}
void dAcOBoat_c::executeState_FallEnd() {}
void dAcOBoat_c::finalizeState_FallEnd() {}
void dAcOBoat_c::initializeState_FallStop() {}
void dAcOBoat_c::executeState_FallStop() {}
void dAcOBoat_c::finalizeState_FallStop() {}
void dAcOBoat_c::initializeState_Up() {}
void dAcOBoat_c::executeState_Up() {}
void dAcOBoat_c::finalizeState_Up() {}
void dAcOBoat_c::initializeState_UpEnd() {}
void dAcOBoat_c::executeState_UpEnd() {}
void dAcOBoat_c::finalizeState_UpEnd() {}
