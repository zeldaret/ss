#include <d/a/obj/d_a_obj_ground_cover.h>

SPECIAL_ACTOR_PROFILE(OBJ_GROUND_COVER, dAcOgroundCover_c, fProfile::OBJ_GROUND_COVER, 0x1FD, 0, 6);

STATE_DEFINE(dAcOgroundCover_c, Wait);
STATE_DEFINE(dAcOgroundCover_c, Wait2);
STATE_DEFINE(dAcOgroundCover_c, AppearWait);
STATE_DEFINE(dAcOgroundCover_c, AppearOpen);
STATE_DEFINE(dAcOgroundCover_c, AppearClose);
STATE_DEFINE(dAcOgroundCover_c, Fight);
STATE_DEFINE(dAcOgroundCover_c, Sealed);

dAcOgroundCover_c::dAcOgroundCover_c() : mStateMgr(*this, sStateID::null) {}
dAcOgroundCover_c::~dAcOgroundCover_c() {}

void dAcOgroundCover_c::initializeState_Wait() {}
void dAcOgroundCover_c::executeState_Wait() {}
void dAcOgroundCover_c::finalizeState_Wait() {}
void dAcOgroundCover_c::initializeState_Wait2() {}
void dAcOgroundCover_c::executeState_Wait2() {}
void dAcOgroundCover_c::finalizeState_Wait2() {}
void dAcOgroundCover_c::initializeState_AppearWait() {}
void dAcOgroundCover_c::executeState_AppearWait() {}
void dAcOgroundCover_c::finalizeState_AppearWait() {}
void dAcOgroundCover_c::initializeState_AppearOpen() {}
void dAcOgroundCover_c::executeState_AppearOpen() {}
void dAcOgroundCover_c::finalizeState_AppearOpen() {}
void dAcOgroundCover_c::initializeState_AppearClose() {}
void dAcOgroundCover_c::executeState_AppearClose() {}
void dAcOgroundCover_c::finalizeState_AppearClose() {}
void dAcOgroundCover_c::initializeState_Fight() {}
void dAcOgroundCover_c::executeState_Fight() {}
void dAcOgroundCover_c::finalizeState_Fight() {}
void dAcOgroundCover_c::initializeState_Sealed() {}
void dAcOgroundCover_c::executeState_Sealed() {}
void dAcOgroundCover_c::finalizeState_Sealed() {}
