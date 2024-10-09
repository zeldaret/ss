#include <d/a/obj/d_a_obj_warp.h>

SPECIAL_ACTOR_PROFILE(OBJ_WARP, dAcOWarp_c, fProfile::OBJ_WARP, 0X238, 0, 7);

STATE_DEFINE(dAcOWarp_c, Wait);
STATE_DEFINE(dAcOWarp_c, SirenFinish);
STATE_DEFINE(dAcOWarp_c, GetItem);
STATE_DEFINE(dAcOWarp_c, GateWait);
STATE_DEFINE(dAcOWarp_c, GateOpen);
STATE_DEFINE(dAcOWarp_c, GateIn);
STATE_DEFINE(dAcOWarp_c, GateClear);

void dAcOWarp_c::initializeState_Wait() {}
void dAcOWarp_c::executeState_Wait() {}
void dAcOWarp_c::finalizeState_Wait() {}
void dAcOWarp_c::initializeState_SirenFinish() {}
void dAcOWarp_c::executeState_SirenFinish() {}
void dAcOWarp_c::finalizeState_SirenFinish() {}
void dAcOWarp_c::initializeState_GetItem() {}
void dAcOWarp_c::executeState_GetItem() {}
void dAcOWarp_c::finalizeState_GetItem() {}
void dAcOWarp_c::initializeState_GateWait() {}
void dAcOWarp_c::executeState_GateWait() {}
void dAcOWarp_c::finalizeState_GateWait() {}
void dAcOWarp_c::initializeState_GateOpen() {}
void dAcOWarp_c::executeState_GateOpen() {}
void dAcOWarp_c::finalizeState_GateOpen() {}
void dAcOWarp_c::initializeState_GateIn() {}
void dAcOWarp_c::executeState_GateIn() {}
void dAcOWarp_c::finalizeState_GateIn() {}
void dAcOWarp_c::initializeState_GateClear() {}
void dAcOWarp_c::executeState_GateClear() {}
void dAcOWarp_c::finalizeState_GateClear() {}
