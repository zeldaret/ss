#include "d/t/d_t_mole_prohibit.h"

SPECIAL_ACTOR_PROFILE(MOLE_PROHIBIT_TAG, dTgMoleProhibit_c, fProfile::MOLE_PROHIBIT_TAG, 0x0025, 0, 0);

STATE_DEFINE(dTgMoleProhibit_c, Wait);

int dTgMoleProhibit_c::create() {
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dTgMoleProhibit_c::doDelete() {
    return SUCCEEDED;
}

int dTgMoleProhibit_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgMoleProhibit_c::draw() {
    return SUCCEEDED;
}

void dTgMoleProhibit_c::initializeState_Wait() {}
void dTgMoleProhibit_c::executeState_Wait() {}
void dTgMoleProhibit_c::finalizeState_Wait() {}
