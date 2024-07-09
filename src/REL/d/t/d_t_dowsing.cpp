#include <d/tg/d_tg.h>
#include <toBeSorted/dowsing_target.h>
#include <toBeSorted/item_story_flag_manager.h>
#include <toBeSorted/room_manager.h>
#include <toBeSorted/sceneflag_manager.h>

class dTgDowsing_c : public dTg_c {
public:
    dTgDowsing_c();
    virtual ~dTgDowsing_c() {}

    virtual int create() override;
    virtual int doDelete() override;
    virtual int actorExecute() override;
    virtual int draw() override;

    bool checkDowsing() const;

private:
    enum DowsingOption {
        TRIAL_0,
        TRIAL_1,
        TRIAL_2,
        TRIAL_3,
        PROPELLER,
        WATER_BASIN,
        CRYSTAL_BALL,
        PUMPKIN,
        NEW_PLANT_SPECIES,
        PARTY_WHEEL,
        ZELDA,
        DOWSING_MAX
    };

    DowsingTarget mDowsingTarget; // 0x0FC
    DowsingOption mDowsingOption; // 0x11C
    u16 mUntriggerStoryFlag;      // 0x120
    u16 mTriggerStoryFlag;        // 0x122
    bool mTargetActive;           // 0x124

    u16 getTriggerSceneFlag() {
        return params & 0xFF;
    }

    u16 getUntriggerSceneFlag() {
        return params >> 0x8 & 0xFF;
    }

    DowsingOption getDowsingOption() {
        return (DowsingOption)(params >> 0x10 & 0xF);
    }

    u32 getDowsingOptionNum() {
        return mDowsingOption;
    }

    u16 getUntriggerStoryFlag() {
        return params >> 0x14 & 0x7FF;
    }

    u16 getTriggerStoryFlag() {
        return rotation.z.mVal & 0x7FF;
    }
};

SPECIAL_ACTOR_PROFILE(T_DOWSING, dTgDowsing_c, fProfile::T_DOWSING, 0x0293, 0, 0);

dTgDowsing_c::dTgDowsing_c() : mDowsingTarget(this, DowsingTarget::SLOT_NONE) {}

int dTgDowsing_c::create() {
    mDowsingOption = getDowsingOption();
    if (mDowsingOption > 11) {
        mDowsingOption = DOWSING_MAX;
    }
    mUntriggerStoryFlag = getUntriggerStoryFlag();
    mTriggerStoryFlag = getTriggerStoryFlag();
    rotation.z = 0;
    bool doThing = getDowsingOptionNum() <= DOWSING_MAX && (1 << getDowsingOptionNum() & 0xC3F) != 0;

    mDowsingTarget.initialize(!doThing ? DowsingTarget::SLOT_2 : DowsingTarget::SLOT_0, 0, nullptr, 0);
    mTargetActive = false;
    return SUCCEEDED;
}

int dTgDowsing_c::doDelete() {
    return 1;
}

int dTgDowsing_c::actorExecute() {
    if (getTriggerSceneFlag() < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, getTriggerSceneFlag()) ||
            getUntriggerSceneFlag() < 0xFF &&
                    !SceneflagManager::sInstance->checkBoolFlag(roomid, getUntriggerSceneFlag()) ||
            !checkDowsing()) {
        mDowsingTarget.doUnregister();
        mTargetActive = false;
    } else if (!mTargetActive && checkDowsing()) {
        mDowsingTarget.doRegister();
        mTargetActive = true;
    }
    return 1;
}

int dTgDowsing_c::draw() {
    return 1;
}

bool dTgDowsing_c::checkDowsing() const {
    if (!((mUntriggerStoryFlag == 0 || mUntriggerStoryFlag >= 0x7FF ||
                  !StoryFlagManager::sInstance->getCounterOrFlag(mUntriggerStoryFlag)) &&
                (mTriggerStoryFlag == 0 || mTriggerStoryFlag >= 0x7FF ||
                        StoryFlagManager::sInstance->getCounterOrFlag(mTriggerStoryFlag)))) {
        return false;
    }

    switch (mDowsingOption) {
    case TRIAL_0:
        return ItemFlagManager::sInstance->getFlag(187) && !StoryFlagManager::sInstance->getCounterOrFlag(93);
    case TRIAL_1:
        return ItemFlagManager::sInstance->getFlag(188) && !StoryFlagManager::sInstance->getCounterOrFlag(97);
    case TRIAL_2:
        return ItemFlagManager::sInstance->getFlag(189) && !StoryFlagManager::sInstance->getCounterOrFlag(98);
    case TRIAL_3:
        return ItemFlagManager::sInstance->getFlag(193) && !StoryFlagManager::sInstance->getCounterOrFlag(99);
    case PROPELLER:
        return DowsingTarget::hasPropellerDowsing();
    case WATER_BASIN:
        return DowsingTarget::hasWaterBasinDowsing();
    case CRYSTAL_BALL:
        return DowsingTarget::hasCrystalBallDowsing();
    case PUMPKIN:
        return DowsingTarget::hasPumpkinDowsing();
    case NEW_PLANT_SPECIES:
        return DowsingTarget::hasNewPlantSpeciesDowsing();
    case PARTY_WHEEL:
        return DowsingTarget::hasPartyWheelDowsing();
    case ZELDA:
        return DowsingTarget::hasZeldaDowsing();
    default:
        return true;
    }
}
