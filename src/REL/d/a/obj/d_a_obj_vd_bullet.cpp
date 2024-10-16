#include "d/a/obj/d_a_obj_vd_bullet.h"

SPECIAL_ACTOR_PROFILE(OBJ_VDB, dAcObjVdb_c, fProfile::OBJ_VDB, 0xBD, 0, 0);

STATE_DEFINE(dAcObjVdb_c, BulletMove);
STATE_DEFINE(dAcObjVdb_c, BulletLost);

void dAcObjVdb_c::initializeState_BulletMove() {}
void dAcObjVdb_c::executeState_BulletMove() {}
void dAcObjVdb_c::finalizeState_BulletMove() {}
void dAcObjVdb_c::initializeState_BulletLost() {}
void dAcObjVdb_c::executeState_BulletLost() {}
void dAcObjVdb_c::finalizeState_BulletLost() {}
