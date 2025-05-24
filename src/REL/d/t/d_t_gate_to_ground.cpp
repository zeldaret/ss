#include "d/t/d_t_gate_to_ground.h"

#include "f/f_base.h"


SPECIAL_ACTOR_PROFILE(GATE2GND_TAG, dTgGateToGround_c, fProfile::GATE2GND_TAG, 0x22, 0, 0);

int dTgGateToGround_c::create() {}

int dTgGateToGround_c::doDelete() {
    return SUCCEEDED;
}

int dTgGateToGround_c::actorExecute() {}

int dTgGateToGround_c::actorExecuteInEvent() {}

int dTgGateToGround_c::draw() {
    return SUCCEEDED;
}
