#include <d/a/obj/d_a_obj_roll_rock.h>

SPECIAL_ACTOR_PROFILE(OBJ_ROLL_ROCK, dAcOrollRock_c, fProfile::OBJ_ROLL_ROCK, 0x266, 0, 2);

STATE_DEFINE(dAcOrollRock_c, Wait);
STATE_DEFINE(dAcOrollRock_c, Slope);
STATE_DEFINE(dAcOrollRock_c, SlopeWait);
STATE_DEFINE(dAcOrollRock_c, SwitchStop);
STATE_DEFINE(dAcOrollRock_c, PathMove);
STATE_DEFINE(dAcOrollRock_c, PathJump);
STATE_DEFINE(dAcOrollRock_c, PathJumpArrive);
STATE_DEFINE(dAcOrollRock_c, GrabCarry);
STATE_DEFINE(dAcOrollRock_c, StopSink);
STATE_DEFINE(dAcOrollRock_c, BossDemoWait);
STATE_DEFINE(dAcOrollRock_c, BossDemoPathMove);
STATE_DEFINE(dAcOrollRock_c, BossDemoStop);
STATE_DEFINE(dAcOrollRock_c, BossDemoPreWait);
STATE_DEFINE(dAcOrollRock_c, BossDemoPrePathMove);

void dAcOrollRock_c::initializeState_Wait() {}
void dAcOrollRock_c::executeState_Wait() {}
void dAcOrollRock_c::finalizeState_Wait() {}
void dAcOrollRock_c::initializeState_Slope() {}
void dAcOrollRock_c::executeState_Slope() {}
void dAcOrollRock_c::finalizeState_Slope() {}
void dAcOrollRock_c::initializeState_SlopeWait() {}
void dAcOrollRock_c::executeState_SlopeWait() {}
void dAcOrollRock_c::finalizeState_SlopeWait() {}
void dAcOrollRock_c::initializeState_SwitchStop() {}
void dAcOrollRock_c::executeState_SwitchStop() {}
void dAcOrollRock_c::finalizeState_SwitchStop() {}
void dAcOrollRock_c::initializeState_PathMove() {}
void dAcOrollRock_c::executeState_PathMove() {}
void dAcOrollRock_c::finalizeState_PathMove() {}
void dAcOrollRock_c::initializeState_PathJump() {}
void dAcOrollRock_c::executeState_PathJump() {}
void dAcOrollRock_c::finalizeState_PathJump() {}
void dAcOrollRock_c::initializeState_PathJumpArrive() {}
void dAcOrollRock_c::executeState_PathJumpArrive() {}
void dAcOrollRock_c::finalizeState_PathJumpArrive() {}
void dAcOrollRock_c::initializeState_GrabCarry() {}
void dAcOrollRock_c::executeState_GrabCarry() {}
void dAcOrollRock_c::finalizeState_GrabCarry() {}
void dAcOrollRock_c::initializeState_StopSink() {}
void dAcOrollRock_c::executeState_StopSink() {}
void dAcOrollRock_c::finalizeState_StopSink() {}
void dAcOrollRock_c::initializeState_BossDemoWait() {}
void dAcOrollRock_c::executeState_BossDemoWait() {}
void dAcOrollRock_c::finalizeState_BossDemoWait() {}
void dAcOrollRock_c::initializeState_BossDemoPathMove() {}
void dAcOrollRock_c::executeState_BossDemoPathMove() {}
void dAcOrollRock_c::finalizeState_BossDemoPathMove() {}
void dAcOrollRock_c::initializeState_BossDemoStop() {}
void dAcOrollRock_c::executeState_BossDemoStop() {}
void dAcOrollRock_c::finalizeState_BossDemoStop() {}
void dAcOrollRock_c::initializeState_BossDemoPreWait() {}
void dAcOrollRock_c::executeState_BossDemoPreWait() {}
void dAcOrollRock_c::finalizeState_BossDemoPreWait() {}
void dAcOrollRock_c::initializeState_BossDemoPrePathMove() {}
void dAcOrollRock_c::executeState_BossDemoPrePathMove() {}
void dAcOrollRock_c::finalizeState_BossDemoPrePathMove() {}
