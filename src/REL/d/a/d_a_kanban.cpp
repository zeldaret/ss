#include "d/a/d_a_kanban.h"

SPECIAL_ACTOR_PROFILE(KANBAN, dAcKanban_c, fProfile::KANBAN, 0x181, 0, 3);

STATE_DEFINE(dAcKanban_c, Wait);
STATE_DEFINE(dAcKanban_c, Message);
STATE_DEFINE(dAcKanban_c, PullOut);

dAcKanban_c::dAcKanban_c() : mStateMgr(*this, sStateID::null) {}
dAcKanban_c::~dAcKanban_c() {}

void dAcKanban_c::initializeState_Wait() {}
void dAcKanban_c::executeState_Wait() {}
void dAcKanban_c::finalizeState_Wait() {}
void dAcKanban_c::initializeState_Message() {}
void dAcKanban_c::executeState_Message() {}
void dAcKanban_c::finalizeState_Message() {}
void dAcKanban_c::initializeState_PullOut() {}
void dAcKanban_c::executeState_PullOut() {}
void dAcKanban_c::finalizeState_PullOut() {}
