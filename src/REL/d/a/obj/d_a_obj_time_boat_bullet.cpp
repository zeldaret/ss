#include <d/a/obj/d_a_obj_time_boat_bullet.h>

SPECIAL_ACTOR_PROFILE(OBJ_TIME_BOAT_BULLET, dAcOTimeBoatBullet_c, fProfile::OBJ_TIME_BOAT_BULLET, 0x19C, 0, 2);

STATE_DEFINE(dAcOTimeBoatBullet_c, Wait);
STATE_DEFINE(dAcOTimeBoatBullet_c, Explode);

void dAcOTimeBoatBullet_c::initializeState_Wait() {}
void dAcOTimeBoatBullet_c::executeState_Wait() {}
void dAcOTimeBoatBullet_c::finalizeState_Wait() {}
void dAcOTimeBoatBullet_c::initializeState_Explode() {}
void dAcOTimeBoatBullet_c::executeState_Explode() {}
void dAcOTimeBoatBullet_c::finalizeState_Explode() {}
