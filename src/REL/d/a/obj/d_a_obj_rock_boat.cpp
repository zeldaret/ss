#include <d/a/obj/d_a_obj_rock_boat.h>

SPECIAL_ACTOR_PROFILE(OBJ_ROCK_BOAT, dAcORockBoat_c, fProfile::OBJ_ROCK_BOAT, 0X148, 0, 3);

STATE_DEFINE(dAcORockBoat_c, Appear);
STATE_DEFINE(dAcORockBoat_c, Wait);
STATE_DEFINE(dAcORockBoat_c, Move);
STATE_DEFINE(dAcORockBoat_c, PreSink);
STATE_DEFINE(dAcORockBoat_c, Sink);

void dAcORockBoat_c::initializeState_Appear() {}
void dAcORockBoat_c::executeState_Appear() {}
void dAcORockBoat_c::finalizeState_Appear() {}
void dAcORockBoat_c::initializeState_Wait() {}
void dAcORockBoat_c::executeState_Wait() {}
void dAcORockBoat_c::finalizeState_Wait() {}
void dAcORockBoat_c::initializeState_Move() {}
void dAcORockBoat_c::executeState_Move() {}
void dAcORockBoat_c::finalizeState_Move() {}
void dAcORockBoat_c::initializeState_PreSink() {}
void dAcORockBoat_c::executeState_PreSink() {}
void dAcORockBoat_c::finalizeState_PreSink() {}
void dAcORockBoat_c::initializeState_Sink() {}
void dAcORockBoat_c::executeState_Sink() {}
void dAcORockBoat_c::finalizeState_Sink() {}
