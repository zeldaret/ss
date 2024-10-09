#include <d/a/obj/d_a_obj_ride_rock.h>

SPECIAL_ACTOR_PROFILE(OBJ_RIDE_ROCK, dAcOrideRock_c, fProfile::OBJ_RIDE_ROCK, 0X267, 0, 6);

STATE_DEFINE(dAcOrideRock_c, Wait);
STATE_DEFINE(dAcOrideRock_c, Slope);
STATE_DEFINE(dAcOrideRock_c, SlopeWait);
STATE_DEFINE(dAcOrideRock_c, RideMove);
STATE_DEFINE(dAcOrideRock_c, SwitchMove);

void dAcOrideRock_c::initializeState_Wait() {}
void dAcOrideRock_c::executeState_Wait() {}
void dAcOrideRock_c::finalizeState_Wait() {}
void dAcOrideRock_c::initializeState_Slope() {}
void dAcOrideRock_c::executeState_Slope() {}
void dAcOrideRock_c::finalizeState_Slope() {}
void dAcOrideRock_c::initializeState_SlopeWait() {}
void dAcOrideRock_c::executeState_SlopeWait() {}
void dAcOrideRock_c::finalizeState_SlopeWait() {}
void dAcOrideRock_c::initializeState_RideMove() {}
void dAcOrideRock_c::executeState_RideMove() {}
void dAcOrideRock_c::finalizeState_RideMove() {}
void dAcOrideRock_c::initializeState_SwitchMove() {}
void dAcOrideRock_c::executeState_SwitchMove() {}
void dAcOrideRock_c::finalizeState_SwitchMove() {}
