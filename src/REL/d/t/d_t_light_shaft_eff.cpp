#include <d/tg/d_t_light_shaft_eff.h>

SPECIAL_ACTOR_PROFILE(TAG_LIGHT_SHAFT_EFF, dTgLightShaftEff_c, fProfile::TAG_LIGHT_SHAFT_EFF, 0x025E, 0, 0);

int dTgLightShaftEff_c::create() {
    return SUCCEEDED;
}

int dTgLightShaftEff_c::doDelete() {
    return 1;
}

int dTgLightShaftEff_c::actorExecute() {
    return 1;
}

int dTgLightShaftEff_c::draw() {
    return 1;
}
