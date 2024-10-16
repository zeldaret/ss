#include "d/a/npc/d_a_npc_mole_mg.h"

SPECIAL_ACTOR_PROFILE(NPC_MOLE_MG, dAcNpcMoleMg_c, fProfile::NPC_MOLE_MG, 0x7D, 0, 3);

STATE_DEFINE(dAcNpcMoleMg_c, Result);
STATE_DEFINE(dAcNpcMoleMg_c, Hide);

void dAcNpcMoleMg_c::initializeState_Result() {}
void dAcNpcMoleMg_c::executeState_Result() {}
void dAcNpcMoleMg_c::finalizeState_Result() {}
void dAcNpcMoleMg_c::initializeState_Hide() {}
void dAcNpcMoleMg_c::executeState_Hide() {}
void dAcNpcMoleMg_c::finalizeState_Hide() {}
