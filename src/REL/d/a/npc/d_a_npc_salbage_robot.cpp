#include "d/a/npc/d_a_npc_salbage_robot.h"

SPECIAL_ACTOR_PROFILE(NPC_SLB, dAcNpcSlb_c, fProfile::NPC_SLB, 0x94, 0, 0);

STATE_DEFINE(dAcNpcSlb_c, Hide);
STATE_DEFINE(dAcNpcSlb_c, HideDelivary);
STATE_DEFINE(dAcNpcSlb_c, DelivaryReq);
STATE_DEFINE(dAcNpcSlb_c, Kill);
STATE_DEFINE(dAcNpcSlb_c, Kill2);

void dAcNpcSlb_c::initializeState_Hide() {}
void dAcNpcSlb_c::executeState_Hide() {}
void dAcNpcSlb_c::finalizeState_Hide() {}
void dAcNpcSlb_c::initializeState_HideDelivary() {}
void dAcNpcSlb_c::executeState_HideDelivary() {}
void dAcNpcSlb_c::finalizeState_HideDelivary() {}
void dAcNpcSlb_c::initializeState_DelivaryReq() {}
void dAcNpcSlb_c::executeState_DelivaryReq() {}
void dAcNpcSlb_c::finalizeState_DelivaryReq() {}
void dAcNpcSlb_c::initializeState_Kill() {}
void dAcNpcSlb_c::executeState_Kill() {}
void dAcNpcSlb_c::finalizeState_Kill() {}
void dAcNpcSlb_c::initializeState_Kill2() {}
void dAcNpcSlb_c::executeState_Kill2() {}
void dAcNpcSlb_c::finalizeState_Kill2() {}
