#include <d/a/e/d_a_e_kumite_gira.h>

SPECIAL_ACTOR_PROFILE(E_KGIRA, dAcEKumiteGira_c, fProfile::E_KGIRA, 0XCE, 0, 3);

STATE_DEFINE(dAcEKumiteGira_c, Wait);
STATE_DEFINE(dAcEKumiteGira_c, PlayAnm);
STATE_DEFINE(dAcEKumiteGira_c, Damage);

void dAcEKumiteGira_c::initializeState_Wait() {}
void dAcEKumiteGira_c::executeState_Wait() {}
void dAcEKumiteGira_c::finalizeState_Wait() {}
void dAcEKumiteGira_c::initializeState_PlayAnm() {}
void dAcEKumiteGira_c::executeState_PlayAnm() {}
void dAcEKumiteGira_c::finalizeState_PlayAnm() {}
void dAcEKumiteGira_c::initializeState_Damage() {}
void dAcEKumiteGira_c::executeState_Damage() {}
void dAcEKumiteGira_c::finalizeState_Damage() {}
