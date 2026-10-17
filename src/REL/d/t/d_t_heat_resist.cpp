#include "d/t/d_t_heat_resist.h"

#include "d/a/d_a_player.h"
#include "f/f_base.h"

SPECIAL_ACTOR_PROFILE(TAG_HEAT_RESIST, dTgHeatResist_c, fProfile::TAG_HEAT_RESIST, 0x223, 0, 4);

int dTgHeatResist_c::create() {
    mInverted = getFromParams(0, 0x3);

    mArea.set(mPosition, mRotation.y, mScale, nullptr, 0.0f);
    return SUCCEEDED;
}

int dTgHeatResist_c::doDelete() {
    return SUCCEEDED;
}

int dTgHeatResist_c::actorExecute() {
    dAcPy_c *player = dAcPy_c::GetLinkM();

    if (mInverted == 1) {
        if (!mArea.checkAreaBox(player->mPosition)) {
            player->onForceOrPreventActionFlags(0x8000000);
        }
    } else {
        if (mArea.checkAreaBox(player->mPosition)) {
            player->onForceOrPreventActionFlags(0x8000000);
        }
    }
    return SUCCEEDED;
}

int dTgHeatResist_c::draw() {
    return SUCCEEDED;
}
