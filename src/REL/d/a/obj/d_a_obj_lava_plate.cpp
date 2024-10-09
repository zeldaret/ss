#include <d/a/obj/d_a_obj_lava_plate.h>

SPECIAL_ACTOR_PROFILE(OBJ_LAVA_PLATE, dAcOLavaPlate_c, fProfile::OBJ_LAVA_PLATE, 0X242, 0, 2);

STATE_DEFINE(dAcOLavaPlate_c, Wait);
STATE_DEFINE(dAcOLavaPlate_c, Move);
STATE_DEFINE(dAcOLavaPlate_c, Success);

void dAcOLavaPlate_c::initializeState_Wait() {}
void dAcOLavaPlate_c::executeState_Wait() {}
void dAcOLavaPlate_c::finalizeState_Wait() {}
void dAcOLavaPlate_c::initializeState_Move() {}
void dAcOLavaPlate_c::executeState_Move() {}
void dAcOLavaPlate_c::finalizeState_Move() {}
void dAcOLavaPlate_c::initializeState_Success() {}
void dAcOLavaPlate_c::executeState_Success() {}
void dAcOLavaPlate_c::finalizeState_Success() {}
