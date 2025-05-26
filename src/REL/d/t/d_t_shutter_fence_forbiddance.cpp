#include "d/t/d_t_shutter_fence_forbiddance.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(
    TAG_SHUTTER_FENCE_FORBIDDANCE, dTgShutterFenceForbiddance_c, fProfile::TAG_SHUTTER_FENCE_FORBIDDANCE, 0x2A4, 0, 4
);

static dTgShutterFenceForbiddance_c *FIRST_PTR;

dTgShutterFenceForbiddance_c::dTgShutterFenceForbiddance_c() {
    if (!FIRST_PTR) {
        FIRST_PTR = this;
    }
}
dTgShutterFenceForbiddance_c::~dTgShutterFenceForbiddance_c() {}

int dTgShutterFenceForbiddance_c::create() {
    if (this != FIRST_PTR) {
        return FAILED;
    }
    return SUCCEEDED;
}

int dTgShutterFenceForbiddance_c::actorExecute() {
    return SUCCEEDED;
}
