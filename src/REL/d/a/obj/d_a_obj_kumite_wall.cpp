#include <d/a/obj/d_a_obj_kumite_wall.h>

SPECIAL_ACTOR_PROFILE(OBJ_KUMITE_WALL, dAcOkumiteWall_c, fProfile::OBJ_KUMITE_WALL, 0X217, 0, 2);

STATE_DEFINE(dAcOkumiteWall_c, Wait);
STATE_DEFINE(dAcOkumiteWall_c, Appear);
STATE_DEFINE(dAcOkumiteWall_c, Exist);
STATE_DEFINE(dAcOkumiteWall_c, Vanish);

void dAcOkumiteWall_c::initializeState_Wait() {}
void dAcOkumiteWall_c::executeState_Wait() {}
void dAcOkumiteWall_c::finalizeState_Wait() {}
void dAcOkumiteWall_c::initializeState_Appear() {}
void dAcOkumiteWall_c::executeState_Appear() {}
void dAcOkumiteWall_c::finalizeState_Appear() {}
void dAcOkumiteWall_c::initializeState_Exist() {}
void dAcOkumiteWall_c::executeState_Exist() {}
void dAcOkumiteWall_c::finalizeState_Exist() {}
void dAcOkumiteWall_c::initializeState_Vanish() {}
void dAcOkumiteWall_c::executeState_Vanish() {}
void dAcOkumiteWall_c::finalizeState_Vanish() {}
