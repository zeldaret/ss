#include "d/t/d_t_dowsing.h"

#include "toBeSorted/item_story_flag_manager.h"
#include "toBeSorted/room_manager.h"
#include "toBeSorted/sceneflag_manager.h"


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

    mDowsingTarget.initialize(!doThing ? DowsingTarget::SLOT_QUEST : DowsingTarget::SLOT_STORY_EVENT, 0, nullptr, 0);
    mTargetActive = false;
    return SUCCEEDED;
}

int dTgDowsing_c::doDelete() {
    return SUCCEEDED;
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
    return SUCCEEDED;
}

int dTgDowsing_c::draw() {
    return SUCCEEDED;
}

bool dTgDowsing_c::checkDowsing() const {
    if (!((mUntriggerStoryFlag == 0 || mUntriggerStoryFlag >= 0x7FF ||
           !StoryflagManager::sInstance->getCounterOrFlag(mUntriggerStoryFlag)) &&
          (mTriggerStoryFlag == 0 || mTriggerStoryFlag >= 0x7FF ||
           StoryflagManager::sInstance->getCounterOrFlag(mTriggerStoryFlag)))) {
        return false;
    }

    switch (mDowsingOption) {
        case TRIAL_0:
            return ItemflagManager::sInstance->getFlag(187) && !StoryflagManager::sInstance->getCounterOrFlag(93);
        case TRIAL_1:
            return ItemflagManager::sInstance->getFlag(188) && !StoryflagManager::sInstance->getCounterOrFlag(97);
        case TRIAL_2:
            return ItemflagManager::sInstance->getFlag(189) && !StoryflagManager::sInstance->getCounterOrFlag(98);
        case TRIAL_3:
            return ItemflagManager::sInstance->getFlag(193) && !StoryflagManager::sInstance->getCounterOrFlag(99);
        case PROPELLER:         return DowsingTarget::hasPropellerDowsing();
        case WATER_BASIN:       return DowsingTarget::hasWaterBasinDowsing();
        case CRYSTAL_BALL:      return DowsingTarget::hasCrystalBallDowsing();
        case PUMPKIN:           return DowsingTarget::hasPumpkinDowsing();
        case NEW_PLANT_SPECIES: return DowsingTarget::hasNewPlantSpeciesDowsing();
        case PARTY_WHEEL:       return DowsingTarget::hasPartyWheelDowsing();
        case ZELDA:             return DowsingTarget::hasZeldaDowsing();
        default:                return true;
    }
}
