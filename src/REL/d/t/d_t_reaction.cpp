#include <d/t/d_t_reaction.h>

SPECIAL_ACTOR_PROFILE(TAG_REACTION, dTgReaction_c, fProfile::TAG_REACTION, 0x151, 0, 0);

STATE_DEFINE(dTgReaction_c, Wait);

void dTgReaction_c::initializeState_Wait() {}
void dTgReaction_c::executeState_Wait() {}
void dTgReaction_c::finalizeState_Wait() {}
