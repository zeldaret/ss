#include "d/t/d_t_plight.h"

#include "d/d_light_env.h"

SPECIAL_ACTOR_PROFILE(PLIGHT_TAG, dTgPlight_c, fProfile::PLIGHT_TAG, 0x1E, 0, 4);

int dTgPlight_c::create() {
    mType = mParams;
    mLight.field_0x18 = false;
    mLight.mPos = mPosition;
    mLight.mClr.r = mParams >> 8;
    mLight.mClr.g = mParams >> 16;
    mLight.mClr.b = mParams >> 24;
    mLight.mScale = mScale.x;
    dLightEnv_c::GetPInstance()->plight_set(&mLight);
    mScale.x = 1.0f;
    return SUCCEEDED;
}

int dTgPlight_c::doDelete() {
    if (mType == 0) {
        dLightEnv_c::GetPInstance()->plight_cut(&mLight);
    }
    return SUCCEEDED;
}

int dTgPlight_c::actorExecute() {
    return SUCCEEDED;
}

int dTgPlight_c::draw() {
    return SUCCEEDED;
}
