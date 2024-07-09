#include <d/d_player.h>
#include <d/tg/d_tg.h>
#include <toBeSorted/dowsing_target.h>

class dTgHolyWater_c : public dTg_c {
public:
    dTgHolyWater_c() : mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}
    virtual ~dTgHolyWater_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool isLinkNearby();
    bool dowsingActive();

private:
    DowsingTarget mDowsingTarget; // 0x0FC
};

SPECIAL_ACTOR_PROFILE(TAG_HOLY_WATER, dTgHolyWater_c, fProfile::TAG_HOLY_WATER, 0x0224, 0, 4);

int dTgHolyWater_c::create() {
    mDowsingTarget.initialize(DowsingTarget::SLOT_0, 0, nullptr, 0.0f);
    return SUCCEEDED;
}

int dTgHolyWater_c::doDelete() {
    return 1;
}

int dTgHolyWater_c::actorExecute() {
    if (dowsingActive()) {
        mDowsingTarget.doRegister();
    } else {
        mDowsingTarget.doUnregister();
    }
    if (isLinkNearby()) {
        dPlayer::LINK->mForceOrPreventActionFlags |= 0x10000000;
    }
    return 1;
}

int dTgHolyWater_c::draw() {
    return 1;
}

bool dTgHolyWater_c::isLinkNearby() {
    mVec3_c diff = dPlayer::LINK->position - (position + mVec3_c::Ey * scale.y * 0.5f);
    diff.x /= scale.x;
    diff.y /= scale.y;
    diff.z /= scale.z;
    return diff.x <= 0.5f && diff.x >= -0.5f && diff.y <= 0.5f && diff.y >= -0.5f && diff.z <= 0.5f && diff.z >= -0.5f;
}

bool dTgHolyWater_c::dowsingActive() {
    return DowsingTarget::hasSacredWaterDowsing();
}
