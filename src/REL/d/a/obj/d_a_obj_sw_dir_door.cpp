#include <d/a/obj/d_a_obj_sw_dir_door.h>

SPECIAL_ACTOR_PROFILE(OBJ_SW_DIR_DOOR, dAcOSwDirDoor_c, fProfile::OBJ_SW_DIR_DOOR, 0X22A, 0, 3);

STATE_DEFINE(dAcOSwDirDoor_c, Wait);
STATE_DEFINE(dAcOSwDirDoor_c, CorrectStart);
STATE_DEFINE(dAcOSwDirDoor_c, Correct);
STATE_DEFINE(dAcOSwDirDoor_c, Broken);
STATE_DEFINE(dAcOSwDirDoor_c, BrokenEnd);
STATE_DEFINE(dAcOSwDirDoor_c, DemoEnd);

void dAcOSwDirDoor_c::initializeState_Wait() {}
void dAcOSwDirDoor_c::executeState_Wait() {}
void dAcOSwDirDoor_c::finalizeState_Wait() {}
void dAcOSwDirDoor_c::initializeState_CorrectStart() {}
void dAcOSwDirDoor_c::executeState_CorrectStart() {}
void dAcOSwDirDoor_c::finalizeState_CorrectStart() {}
void dAcOSwDirDoor_c::initializeState_Correct() {}
void dAcOSwDirDoor_c::executeState_Correct() {}
void dAcOSwDirDoor_c::finalizeState_Correct() {}
void dAcOSwDirDoor_c::initializeState_Broken() {}
void dAcOSwDirDoor_c::executeState_Broken() {}
void dAcOSwDirDoor_c::finalizeState_Broken() {}
void dAcOSwDirDoor_c::initializeState_BrokenEnd() {}
void dAcOSwDirDoor_c::executeState_BrokenEnd() {}
void dAcOSwDirDoor_c::finalizeState_BrokenEnd() {}
void dAcOSwDirDoor_c::initializeState_DemoEnd() {}
void dAcOSwDirDoor_c::executeState_DemoEnd() {}
void dAcOSwDirDoor_c::finalizeState_DemoEnd() {}
