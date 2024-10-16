#include "d/a/obj/d_a_obj_sealed_door.h"

SPECIAL_ACTOR_PROFILE(OBJ_SEALED_DOOR, dAcOsealedDoor_c, fProfile::OBJ_SEALED_DOOR, 0x1F1, 0, 6);

STATE_DEFINE(dAcOsealedDoor_c, Wait);
STATE_DEFINE(dAcOsealedDoor_c, MarkAppear);
STATE_DEFINE(dAcOsealedDoor_c, Light);
STATE_DEFINE(dAcOsealedDoor_c, AnmWait);
STATE_DEFINE(dAcOsealedDoor_c, Vanish);
STATE_DEFINE(dAcOsealedDoor_c, Appear);

void dAcOsealedDoor_c::initializeState_Wait() {}
void dAcOsealedDoor_c::executeState_Wait() {}
void dAcOsealedDoor_c::finalizeState_Wait() {}
void dAcOsealedDoor_c::initializeState_MarkAppear() {}
void dAcOsealedDoor_c::executeState_MarkAppear() {}
void dAcOsealedDoor_c::finalizeState_MarkAppear() {}
void dAcOsealedDoor_c::initializeState_Light() {}
void dAcOsealedDoor_c::executeState_Light() {}
void dAcOsealedDoor_c::finalizeState_Light() {}
void dAcOsealedDoor_c::initializeState_AnmWait() {}
void dAcOsealedDoor_c::executeState_AnmWait() {}
void dAcOsealedDoor_c::finalizeState_AnmWait() {}
void dAcOsealedDoor_c::initializeState_Vanish() {}
void dAcOsealedDoor_c::executeState_Vanish() {}
void dAcOsealedDoor_c::finalizeState_Vanish() {}
void dAcOsealedDoor_c::initializeState_Appear() {}
void dAcOsealedDoor_c::executeState_Appear() {}
void dAcOsealedDoor_c::finalizeState_Appear() {}
