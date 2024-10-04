#include <d/a/d_a_player.h>
#include <d/tg/d_t_holy_water.h>

SPECIAL_ACTOR_PROFILE(TAG_HOLY_WATER, dTgHolyWater_c, fProfile::TAG_HOLY_WATER, 0x0224, 0, 4);

int dTgHolyWater_c::create() {
    mDowsingTarget.initialize(DowsingTarget::SLOT_STORY_EVENT, 0, nullptr, 0.0f);
    return SUCCEEDED;
}

int dTgHolyWater_c::doDelete() {
    return SUCCEEDED;
}

int dTgHolyWater_c::actorExecute() {
    if (dowsingActive()) {
        mDowsingTarget.doRegister();
    } else {
        mDowsingTarget.doUnregister();
    }
    if (isLinkNearby()) {
        dAcPy_c::LINK->mForceOrPreventActionFlags |= 0x10000000;
    }
    return SUCCEEDED;
}

int dTgHolyWater_c::draw() {
    return SUCCEEDED;
}

bool dTgHolyWater_c::isLinkNearby() {
    mVec3_c diff = dAcPy_c::LINK->position - (position + mVec3_c::Ey * mScale.y * 0.5f);
    diff.x /= mScale.x;
    diff.y /= mScale.y;
    diff.z /= mScale.z;
    return diff.x <= 0.5f && diff.x >= -0.5f && diff.y <= 0.5f && diff.y >= -0.5f && diff.z <= 0.5f && diff.z >= -0.5f;
}

bool dTgHolyWater_c::dowsingActive() {
    return DowsingTarget::hasSacredWaterDowsing();
}
