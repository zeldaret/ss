#include <d/a/obj/d_a_obj_ballista.h>

SPECIAL_ACTOR_PROFILE(OBJ_BALLISTA, dAcOBallista_c, fProfile::OBJ_BALLISTA, 0x23C, 0, 2);

STATE_DEFINE(dAcOBallista_c, Wait);
STATE_DEFINE(dAcOBallista_c, Grab);
STATE_DEFINE(dAcOBallista_c, Shot);
STATE_DEFINE(dAcOBallista_c, ShotEnd);
STATE_DEFINE(dAcOBallista_c, Cancel);
STATE_DEFINE(dAcOBallista_c, DemoShot);
STATE_DEFINE(dAcOBallista_c, DemoShot2);
STATE_DEFINE(dAcOBallista_c, DemoShot3);

void dAcOBallista_c::initializeState_Wait() {}
void dAcOBallista_c::executeState_Wait() {}
void dAcOBallista_c::finalizeState_Wait() {}
void dAcOBallista_c::initializeState_Grab() {}
void dAcOBallista_c::executeState_Grab() {}
void dAcOBallista_c::finalizeState_Grab() {}
void dAcOBallista_c::initializeState_Shot() {}
void dAcOBallista_c::executeState_Shot() {}
void dAcOBallista_c::finalizeState_Shot() {}
void dAcOBallista_c::initializeState_ShotEnd() {}
void dAcOBallista_c::executeState_ShotEnd() {}
void dAcOBallista_c::finalizeState_ShotEnd() {}
void dAcOBallista_c::initializeState_Cancel() {}
void dAcOBallista_c::executeState_Cancel() {}
void dAcOBallista_c::finalizeState_Cancel() {}
void dAcOBallista_c::initializeState_DemoShot() {}
void dAcOBallista_c::executeState_DemoShot() {}
void dAcOBallista_c::finalizeState_DemoShot() {}
void dAcOBallista_c::initializeState_DemoShot2() {}
void dAcOBallista_c::executeState_DemoShot2() {}
void dAcOBallista_c::finalizeState_DemoShot2() {}
void dAcOBallista_c::initializeState_DemoShot3() {}
void dAcOBallista_c::executeState_DemoShot3() {}
void dAcOBallista_c::finalizeState_DemoShot3() {}
