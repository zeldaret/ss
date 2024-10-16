#include "d/a/obj/d_a_obj_evil_field.h"

SPECIAL_ACTOR_PROFILE(OBJ_EVIL_FIELD, dAcOevilField_c, fProfile::OBJ_EVIL_FIELD, 0x1F2, 0, 7);

STATE_DEFINE(dAcOevilField_c, Wait);
STATE_DEFINE(dAcOevilField_c, UpDown);
STATE_DEFINE(dAcOevilField_c, Stop);

void dAcOevilField_c::initializeState_Wait() {}
void dAcOevilField_c::executeState_Wait() {}
void dAcOevilField_c::finalizeState_Wait() {}
void dAcOevilField_c::initializeState_UpDown() {}
void dAcOevilField_c::executeState_UpDown() {}
void dAcOevilField_c::finalizeState_UpDown() {}
void dAcOevilField_c::initializeState_Stop() {}
void dAcOevilField_c::executeState_Stop() {}
void dAcOevilField_c::finalizeState_Stop() {}
