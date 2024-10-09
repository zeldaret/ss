#include <d/a/obj/d_a_obj_cloud_dive.h>

SPECIAL_ACTOR_PROFILE(OBJ_CLOUD_DIVE, dAcOcloudDive_c, fProfile::OBJ_CLOUD_DIVE, 0X1C2, 0, 0);

STATE_DEFINE(dAcOcloudDive_c, Wait);
STATE_DEFINE(dAcOcloudDive_c, Dead);

void dAcOcloudDive_c::initializeState_Wait() {}
void dAcOcloudDive_c::executeState_Wait() {}
void dAcOcloudDive_c::finalizeState_Wait() {}
void dAcOcloudDive_c::initializeState_Dead() {}
void dAcOcloudDive_c::executeState_Dead() {}
void dAcOcloudDive_c::finalizeState_Dead() {}
