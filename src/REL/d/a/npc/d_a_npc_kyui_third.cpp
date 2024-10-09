#include <d/a/npc/d_a_npc_kyui_third.h>

SPECIAL_ACTOR_PROFILE(NPC_KYUI_THIRD, dAcNpcKyuiThird_c, fProfile::NPC_KYUI_THIRD, 0X46, 0, 2);

STATE_DEFINE(dAcNpcKyuiThird_c, Wait2);
STATE_DEFINE(dAcNpcKyuiThird_c, Hang);
STATE_DEFINE(dAcNpcKyuiThird_c, HelpReq);

void dAcNpcKyuiThird_c::initializeState_Wait2() {}
void dAcNpcKyuiThird_c::executeState_Wait2() {}
void dAcNpcKyuiThird_c::finalizeState_Wait2() {}
void dAcNpcKyuiThird_c::initializeState_Hang() {}
void dAcNpcKyuiThird_c::executeState_Hang() {}
void dAcNpcKyuiThird_c::finalizeState_Hang() {}
void dAcNpcKyuiThird_c::initializeState_HelpReq() {}
void dAcNpcKyuiThird_c::executeState_HelpReq() {}
void dAcNpcKyuiThird_c::finalizeState_HelpReq() {}
