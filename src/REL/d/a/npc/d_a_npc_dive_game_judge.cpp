#include <d/a/npc/d_a_npc_dive_game_judge.h>

SPECIAL_ACTOR_PROFILE(NPC_DIVE_GAME_JUDGE, dAcNpcDiveGameJudge_c, fProfile::NPC_DIVE_GAME_JUDGE, 0x8D, 0, 3);

STATE_DEFINE(dAcNpcDiveGameJudge_c, Sad);
STATE_DEFINE(dAcNpcDiveGameJudge_c, PreWatch);
STATE_DEFINE(dAcNpcDiveGameJudge_c, Watch);
STATE_DEFINE(dAcNpcDiveGameJudge_c, Escape);
STATE_DEFINE(dAcNpcDiveGameJudge_c, EscapeEnd);

void dAcNpcDiveGameJudge_c::initializeState_Sad() {}
void dAcNpcDiveGameJudge_c::executeState_Sad() {}
void dAcNpcDiveGameJudge_c::finalizeState_Sad() {}
void dAcNpcDiveGameJudge_c::initializeState_PreWatch() {}
void dAcNpcDiveGameJudge_c::executeState_PreWatch() {}
void dAcNpcDiveGameJudge_c::finalizeState_PreWatch() {}
void dAcNpcDiveGameJudge_c::initializeState_Watch() {}
void dAcNpcDiveGameJudge_c::executeState_Watch() {}
void dAcNpcDiveGameJudge_c::finalizeState_Watch() {}
void dAcNpcDiveGameJudge_c::initializeState_Escape() {}
void dAcNpcDiveGameJudge_c::executeState_Escape() {}
void dAcNpcDiveGameJudge_c::finalizeState_Escape() {}
void dAcNpcDiveGameJudge_c::initializeState_EscapeEnd() {}
void dAcNpcDiveGameJudge_c::executeState_EscapeEnd() {}
void dAcNpcDiveGameJudge_c::finalizeState_EscapeEnd() {}
