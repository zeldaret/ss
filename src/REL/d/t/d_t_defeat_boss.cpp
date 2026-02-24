#include "d/t/d_t_defeat_boss.h"

#include "common.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"

SPECIAL_ACTOR_PROFILE(TAG_DEFEAT_BOSS, dTgDefeatBoss_c, fProfile::TAG_DEFEAT_BOSS, 0x29E, 0, 0);

void dTgDefeatBoss_c::fn_494_C0() {

    mID = (fBaseID_e)0x3;
    return;
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

int dTgDefeatBoss_c::update() {

    StoryflagManager* pSVar1;
    bool bVar4;
    u16 uVar2;
    /*StoryFlags*/int SVar3;

    bVar4 = fn_494_D0();
    if (bVar4) {
        switch (var8_1) {
        case 0x0:
            uVar2 = getSceneflag();
            SceneflagManager::sInstance->unsetFlag
            (/*SceneflagManager::sInstance,*/ (u16)/*(param_1->base).members.roomid*/(mRoomID & 0xffff),
                (u16)(uVar2 & 0xffff));
            break;
        case 0x1:
            uVar2 = getSceneflag();
            SceneflagManager::sInstance->setFlag
            (/*SceneflagManager::sInstance,*/ (u16)/*(param_1->base).members.roomid*/mRoomID & 0xffff,
                (u16)uVar2 & 0xffff);
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

u16 dTgDefeatBoss_c::fn_494_360() {
s32 dTgDefeatBoss_c::getSceneflagPolarity() {
    return mParams >> 0x8 & 0x1;

}

u32 dTgDefeatBoss_c::getStoryflag() {
    return mParams >> 9 & 0xffff;
}

u16 dTgDefeatBoss_c::fn_494_380() {

    return (mParams >> 0x19) & 0x1;

}

