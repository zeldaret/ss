#include "d/a/obj/d_a_obj_girahimu_floor.h"

SPECIAL_ACTOR_PROFILE(OBJ_GIRAHIMU_FLOOR, dAcOGirahimuFloor_c, fProfile::OBJ_GIRAHIMU_FLOOR, 0x25F, 0, 2);

STATE_DEFINE(dAcOGirahimuFloor_c, Wait);
STATE_DEFINE(dAcOGirahimuFloor_c, Return);
STATE_DEFINE(dAcOGirahimuFloor_c, Generate);
STATE_DEFINE(dAcOGirahimuFloor_c, Broken);
STATE_DEFINE(dAcOGirahimuFloor_c, Move);

void dAcOGirahimuFloor_c::initializeState_Wait() {}
void dAcOGirahimuFloor_c::executeState_Wait() {}
void dAcOGirahimuFloor_c::finalizeState_Wait() {}
void dAcOGirahimuFloor_c::initializeState_Return() {}
void dAcOGirahimuFloor_c::executeState_Return() {}
void dAcOGirahimuFloor_c::finalizeState_Return() {}
void dAcOGirahimuFloor_c::initializeState_Generate() {}
void dAcOGirahimuFloor_c::executeState_Generate() {}
void dAcOGirahimuFloor_c::finalizeState_Generate() {}
void dAcOGirahimuFloor_c::initializeState_Broken() {}
void dAcOGirahimuFloor_c::executeState_Broken() {}
void dAcOGirahimuFloor_c::finalizeState_Broken() {}
void dAcOGirahimuFloor_c::initializeState_Move() {}
void dAcOGirahimuFloor_c::executeState_Move() {}
void dAcOGirahimuFloor_c::finalizeState_Move() {}
