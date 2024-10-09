#include <d/a/npc/d_a_npc_kyui_first.h>

SPECIAL_ACTOR_PROFILE(NPC_KYUI_FIRST, dAcNpcKyuiFirst_c, fProfile::NPC_KYUI_FIRST, 0X43, 0, 3);

STATE_DEFINE(dAcNpcKyuiFirst_c, Fear);
STATE_DEFINE(dAcNpcKyuiFirst_c, Rest);
STATE_DEFINE(dAcNpcKyuiFirst_c, Hide);
STATE_DEFINE(dAcNpcKyuiFirst_c, Kill);
STATE_DEFINE(dAcNpcKyuiFirst_c, Ambash);
STATE_DEFINE(dAcNpcKyuiFirst_c, AmbashWalk);
STATE_DEFINE(dAcNpcKyuiFirst_c, InitAmbash);

void dAcNpcKyuiFirst_c::initializeState_Fear() {}
void dAcNpcKyuiFirst_c::executeState_Fear() {}
void dAcNpcKyuiFirst_c::finalizeState_Fear() {}
void dAcNpcKyuiFirst_c::initializeState_Rest() {}
void dAcNpcKyuiFirst_c::executeState_Rest() {}
void dAcNpcKyuiFirst_c::finalizeState_Rest() {}
void dAcNpcKyuiFirst_c::initializeState_Hide() {}
void dAcNpcKyuiFirst_c::executeState_Hide() {}
void dAcNpcKyuiFirst_c::finalizeState_Hide() {}
void dAcNpcKyuiFirst_c::initializeState_Kill() {}
void dAcNpcKyuiFirst_c::executeState_Kill() {}
void dAcNpcKyuiFirst_c::finalizeState_Kill() {}
void dAcNpcKyuiFirst_c::initializeState_Ambash() {}
void dAcNpcKyuiFirst_c::executeState_Ambash() {}
void dAcNpcKyuiFirst_c::finalizeState_Ambash() {}
void dAcNpcKyuiFirst_c::initializeState_AmbashWalk() {}
void dAcNpcKyuiFirst_c::executeState_AmbashWalk() {}
void dAcNpcKyuiFirst_c::finalizeState_AmbashWalk() {}
void dAcNpcKyuiFirst_c::initializeState_InitAmbash() {}
void dAcNpcKyuiFirst_c::executeState_InitAmbash() {}
void dAcNpcKyuiFirst_c::finalizeState_InitAmbash() {}
