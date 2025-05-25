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
    return SUCCEEDED + ((reinterpret_cast<u32>(FIRST_PTR) - reinterpret_cast<u32>(this) |
                         reinterpret_cast<u32>(this) - reinterpret_cast<u32>(FIRST_PTR)) >>
                        31);
}

int dTgShutterFenceForbiddance_c::actorExecute() {
    return SUCCEEDED;
}
