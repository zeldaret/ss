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
    DowsingTarget mDowsingTarget; // 0x0FC
    s32 mDowsingOption;           // 0x11C
    u16 mUntriggerStoryFlag;      // 0x120
    u16 mTriggerStoryFlag;        // 0x122
    bool mTargetActive;           // 0x124

    u16 getTriggerSceneFlag() {
        return params & 0xFF;
    }

    u16 getUntriggerSceneFlag() {
        return params >> 0x8 & 0xFF;
    }

    s32 getDowsingOption() {
        return params >> 0x10 & 0xF;
    }

    u16 getUntriggerStoryFlag() {
        return params >> 0x14 & 0x7FF;
    }

    u16 getTriggerStoryFlag() {
        return rotation.z.mVal & 0x7FF;
    }
};

SPECIAL_ACTOR_PROFILE(T_DOWSING, dTgDowsing_c, fProfile::T_DOWSING, 0x0293, 0, 0);

dTgDowsing_c::dTgDowsing_c() : mDowsingTarget(this, 8) {}

int dTgDowsing_c::create() {
    // TODO
    mDowsingOption = getDowsingOption();
    if (mDowsingOption > 11) {
        mDowsingOption = 11;
    }
    mTriggerStoryFlag = getTriggerStoryFlag();
    mUntriggerStoryFlag = getUntriggerStoryFlag();
    rotation.z = 0;
    u32 dws = mDowsingOption;
    bool blah = (dws <= 11 && (1 << dws & 0xC3F));
    u8 type = 0;
    if (!blah) {
        type = 2;
    }

    mDowsingTarget.initialize(!blah ? 2 : 0, 0, nullptr, 0);
    mTargetActive = true;
    return SUCCEEDED;
}

int dTgDowsing_c::doDelete() {
    return 1;
}

int dTgDowsing_c::actorExecute() {
    if (getTriggerSceneFlag() < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, getTriggerSceneFlag()) ||
            getUntriggerSceneFlag() < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, getUntriggerSceneFlag()) ||
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
    case 0:
        return ItemFlagManager::sInstance->getFlag(187) && !StoryFlagManager::sInstance->getCounterOrFlag(93);
    case 1:
        return ItemFlagManager::sInstance->getFlag(188) && !StoryFlagManager::sInstance->getCounterOrFlag(97);
    case 2:
        return ItemFlagManager::sInstance->getFlag(189) && !StoryFlagManager::sInstance->getCounterOrFlag(98);
    case 3:
        return ItemFlagManager::sInstance->getFlag(193) && !StoryFlagManager::sInstance->getCounterOrFlag(99);
    case 4:
        return DowsingTarget::hasPropellerDowsing();
    case 5:
        return DowsingTarget::hasWaterBasinDowsing();
    case 6:
        return DowsingTarget::hasCrystalBallDowsing();
    case 7:
        return DowsingTarget::hasPumpkinDowsing();
    case 8:
        return DowsingTarget::hasNewPlantSpeciesDowsing();
    case 9:
        return DowsingTarget::hasPartyWheelDowsing();
    case 10:
        return DowsingTarget::hasZeldaDowsing();
    default:
        return true;
    }
}
