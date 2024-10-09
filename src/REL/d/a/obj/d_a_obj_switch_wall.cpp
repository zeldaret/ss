#include <d/a/obj/d_a_obj_switch_wall.h>

SPECIAL_ACTOR_PROFILE(OBJ_SWITCH_WALL, dAcOswitchWall_c, fProfile::OBJ_SWITCH_WALL, 0x191, 0, 2);

STATE_DEFINE(dAcOswitchWall_c, Up);
STATE_DEFINE(dAcOswitchWall_c, Down);

void dAcOswitchWall_c::initializeState_Up() {}
void dAcOswitchWall_c::executeState_Up() {}
void dAcOswitchWall_c::finalizeState_Up() {}
void dAcOswitchWall_c::initializeState_Down() {}
void dAcOswitchWall_c::executeState_Down() {}
void dAcOswitchWall_c::finalizeState_Down() {}
