#include "d/t/d_t_defeat_boss.h"

#include "common.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"

SPECIAL_ACTOR_PROFILE(TAG_DEFEAT_BOSS, dTgDefeatBoss_c, fProfile::TAG_DEFEAT_BOSS, 0x29E, 0, 0);


void dTgDefeatBoss_c::getDungeonFlagIndex(u32 &outFlagIdx) {
    outFlagIdx = 0x3;
}

bool dTgDefeatBoss_c::checkDungeonFlag() {
    u32 dungeonFlagIdx;
    getDungeonFlagIndex(dungeonFlagIdx);
    return DungeonflagManager::sInstance->getCounterOrFlag(dungeonFlagIdx, 8) != 0;
}

int dTgDefeatBoss_c::create() {
    if (checkDungeonFlag()) {
        return FAILED;
    }

    mSceneflagPolarity = getSceneflagPolarity();
    if (getSceneflag() >= 0xFF) {
        mSceneflagPolarity = -1;
    }

    mStoryflagPolarity = getStoryflagPolarity();
    if (!StoryflagManager::sInstance->checkFlagValid(getStoryflag())) {
        mStoryflagPolarity = -1;
    }

    if (mSceneflagPolarity == -1 && mStoryflagPolarity == -1) {
        return FAILED;
    }

    return SUCCEEDED;
}


int dTgDefeatBoss_c::actorExecute() {
    if (checkDungeonFlag()) {
        switch (mSceneflagPolarity) {
            case 0x0: {
                SceneflagManager::sInstance->unsetFlag(mRoomID, getSceneflag());
            } break;
            case 0x1: {
                SceneflagManager::sInstance->setFlag(mRoomID, getSceneflag());
            } break;
        }
        switch (mStoryflagPolarity) {
            case 0x0: {
                StoryflagManager *pStoryflagMgr = StoryflagManager::sInstance;
                pStoryflagMgr->unsetFlag(getStoryflag());
            } break;
            case 0x1: {
                StoryflagManager *pStoryflagMgr = StoryflagManager::sInstance;
                pStoryflagMgr->setFlag(getStoryflag());
            } break;
        }
        fBase_c::deleteRequest();
    }
    return SUCCEEDED;
}

u16 dTgDefeatBoss_c::getSceneflag() {
    return mParams & 0xff;
}


s32 dTgDefeatBoss_c::getSceneflagPolarity() {
    return mParams >> 0x8 & 0x1;

}

u32 dTgDefeatBoss_c::getStoryflag() {
    return mParams >> 9 & 0xffff;
}

s32 dTgDefeatBoss_c::getStoryflagPolarity() {

    return (mParams >> 0x19) & 0x1;

}

