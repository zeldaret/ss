#include <d/a/d_a_bird_npc.h>

SPECIAL_ACTOR_PROFILE(BIRD_NPC, dBird_Npc_c, fProfile::BIRD_NPC, 0XA9, 0, 2);

STATE_DEFINE(dBird_Npc_c, Hide);
STATE_DEFINE(dBird_Npc_c, Fly);
STATE_DEFINE(dBird_Npc_c, Chase);

void dBird_Npc_c::initializeState_Hide() {}
void dBird_Npc_c::executeState_Hide() {}
void dBird_Npc_c::finalizeState_Hide() {}
void dBird_Npc_c::initializeState_Fly() {}
void dBird_Npc_c::executeState_Fly() {}
void dBird_Npc_c::finalizeState_Fly() {}
void dBird_Npc_c::initializeState_Chase() {}
void dBird_Npc_c::executeState_Chase() {}
void dBird_Npc_c::finalizeState_Chase() {}
