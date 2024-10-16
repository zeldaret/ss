#include "d/a/d_a_demo_npc_bird.h"

SPECIAL_ACTOR_PROFILE(DEMO_NPC_BIRD, dAcDemoNpcBird_c, fProfile::DEMO_NPC_BIRD, 0x2F, 0, 2);

STATE_DEFINE(dAcDemoNpcBird_c, Hide);
STATE_DEFINE(dAcDemoNpcBird_c, Call);
STATE_DEFINE(dAcDemoNpcBird_c, Fly1);
STATE_DEFINE(dAcDemoNpcBird_c, Fly2);
STATE_DEFINE(dAcDemoNpcBird_c, Fly3);

void dAcDemoNpcBird_c::initializeState_Hide() {}
void dAcDemoNpcBird_c::executeState_Hide() {}
void dAcDemoNpcBird_c::finalizeState_Hide() {}
void dAcDemoNpcBird_c::initializeState_Call() {}
void dAcDemoNpcBird_c::executeState_Call() {}
void dAcDemoNpcBird_c::finalizeState_Call() {}
void dAcDemoNpcBird_c::initializeState_Fly1() {}
void dAcDemoNpcBird_c::executeState_Fly1() {}
void dAcDemoNpcBird_c::finalizeState_Fly1() {}
void dAcDemoNpcBird_c::initializeState_Fly2() {}
void dAcDemoNpcBird_c::executeState_Fly2() {}
void dAcDemoNpcBird_c::finalizeState_Fly2() {}
void dAcDemoNpcBird_c::initializeState_Fly3() {}
void dAcDemoNpcBird_c::executeState_Fly3() {}
void dAcDemoNpcBird_c::finalizeState_Fly3() {}
