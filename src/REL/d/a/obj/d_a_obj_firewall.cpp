#include <d/a/obj/d_a_obj_firewall.h>

SPECIAL_ACTOR_PROFILE(OBJ_FIREWALL, dAcOFireWall_c, fProfile::OBJ_FIREWALL, 0x24E, 0, 6);

STATE_DEFINE(dAcOFireWall_c, Wait);

void dAcOFireWall_c::initializeState_Wait() {}
void dAcOFireWall_c::executeState_Wait() {}
void dAcOFireWall_c::finalizeState_Wait() {}
