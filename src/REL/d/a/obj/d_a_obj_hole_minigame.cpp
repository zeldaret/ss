#include <d/a/obj/d_a_obj_hole_minigame.h>

SPECIAL_ACTOR_PROFILE(OBJ_HOLE_MINIGAME, dAcOholeMinigame_c, fProfile::OBJ_HOLE_MINIGAME, 0x1C1, 0, 4);

STATE_DEFINE(dAcOholeMinigame_c, Wait);
STATE_DEFINE(dAcOholeMinigame_c, Start);
STATE_DEFINE(dAcOholeMinigame_c, Play);
STATE_DEFINE(dAcOholeMinigame_c, Message);
STATE_DEFINE(dAcOholeMinigame_c, Clear);

void dAcOholeMinigame_c::initializeState_Wait() {}
void dAcOholeMinigame_c::executeState_Wait() {}
void dAcOholeMinigame_c::finalizeState_Wait() {}
void dAcOholeMinigame_c::initializeState_Start() {}
void dAcOholeMinigame_c::executeState_Start() {}
void dAcOholeMinigame_c::finalizeState_Start() {}
void dAcOholeMinigame_c::initializeState_Play() {}
void dAcOholeMinigame_c::executeState_Play() {}
void dAcOholeMinigame_c::finalizeState_Play() {}
void dAcOholeMinigame_c::initializeState_Message() {}
void dAcOholeMinigame_c::executeState_Message() {}
void dAcOholeMinigame_c::finalizeState_Message() {}
void dAcOholeMinigame_c::initializeState_Clear() {}
void dAcOholeMinigame_c::executeState_Clear() {}
void dAcOholeMinigame_c::finalizeState_Clear() {}
