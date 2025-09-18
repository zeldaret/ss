#include "d/t/d_t_switch.h"

#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "s/s_Math.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(SW_TAG, dTgSw_c, fProfile::SW_TAG, 0x24, 0, 0);

STATE_DEFINE(dTgSw_c, OnWait);
STATE_DEFINE(dTgSw_c, On);
STATE_DEFINE(dTgSw_c, OffWait);
STATE_DEFINE(dTgSw_c, Off);

static const u8 D201_BossKeyPuzzleFlags[] = {0x25, 0x2E, 0x49, 0x4B};

int dTgSw_c::create() {
    mFirst2 = mParams & 0b11;
    if (mFirst2 >= 2) {
        mFirst2 = 0;
    }
    mSecond2 = (mParams >> 2) & 0b11;
    if (mSecond2 >= 3) {
        mSecond2 = 0;
    }
    mThird2 = (mParams >> 4) & 0b11;
    if (mThird2 >= 3) {
        mThird2 = 0;
    }
    mSetSceneFlagId = (mParams >> 6) & 0xFF;
    mTrigSceneFlagIdBegin = (mParams >> 14) & 0xFF;
    mNumSwitchesToWin = (mParams >> 22) & 0x3F;
    mTrigSceneFlagIdEnd = mNumSwitchesToWin;
    if ((int)(mParams >> 0x1C) < 0xF) {
        mTrigSceneFlagIdEnd += (mParams >> 0x1C);
    }

    if (dScGame_c::isCurrentStage("D201")) {
        if (mRoomID == 4) {
            if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, 0x2F)) {
                SceneflagManager::sInstance->unsetFlag(mRoomID, mSetSceneFlagId);
                resetAllSwitches();
                const u8 *flag = D201_BossKeyPuzzleFlags;
                for (u32 i = 0; i < 4; i++) {
                    SceneflagManager::sInstance->unsetFlag(mRoomID, *flag);
                    flag++;
                }
            }
            mIsFsBossKeyPuzzle = true;
        }
    }

    bool b = SceneflagManager::sInstance->checkUncommittedFlag(mRoomID, mSetSceneFlagId);
    mStateMgr.changeState(!b ? StateID_OnWait : StateID_OffWait);
    return SUCCEEDED;
}

int dTgSw_c::doDelete() {
    return SUCCEEDED;
}

int dTgSw_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgSw_c::draw() {
    return SUCCEEDED;
}

void dTgSw_c::resetAllSwitches() {
    for (int i = mTrigSceneFlagIdBegin; i < mTrigSceneFlagIdBegin + mTrigSceneFlagIdEnd; i++) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, i);
    }
    mCurrentFlagPosition = 0;
}

void dTgSw_c::resetSwitchesAfterCurrent() {
    for (int i = mTrigSceneFlagIdBegin + mCurrentFlagPosition; i < mTrigSceneFlagIdBegin + mTrigSceneFlagIdEnd; i++) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, i);
    }
}

bool dTgSw_c::isPuzzleSolved() {
    for (int i = 0; i < mCurrentFlagPosition; i++) {
        if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, mTrigSceneFlagIdBegin + i)) {
            mCurrentFlagPosition = i;
            field_0x192 = 1;
        }
    }

    if (field_0x192 == 0 &&
        SceneflagManager::sInstance->checkBoolFlag(mRoomID, mTrigSceneFlagIdBegin + mCurrentFlagPosition)) {
        mCurrentFlagPosition++;
    }

    s32 numCorrectFlags = mCurrentFlagPosition;
    for (int i = mCurrentFlagPosition; i < mTrigSceneFlagIdEnd; i++) {
        int t = i < mNumSwitchesToWin ? i : mNumSwitchesToWin;
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mTrigSceneFlagIdBegin + t)) {
            field_0x192 = 1;
            numCorrectFlags++;
        }
    }

    return numCorrectFlags >= mNumSwitchesToWin;
}

bool dTgSw_c::isLmfBossKeyPuzzle() {
    return dScGame_c::isCurrentStage("D300_1") && mRoomID == 8 && mSetSceneFlagId == 0x1C;
}

void dTgSw_c::initializeState_OnWait() {
    SceneflagManager::sInstance->unsetFlag(mRoomID, mSetSceneFlagId);
    field_0x192 = 0;
}
void dTgSw_c::executeState_OnWait() {
    bool state = isPuzzleSolved();
    if (mSecond2 == 0) {
        field_0x192 = 0;
        if (state) {
            mStateMgr.changeState(StateID_On);
        }
    } else if (mSecond2 == 1) {
        if (field_0x192) {
            mStateMgr.changeState(StateID_Off);
        } else {
            if (state) {
                mStateMgr.changeState(mIsFsBossKeyPuzzle ? StateID_OffWait : StateID_On);
            }
        }
    } else if (state) {
        if (field_0x192) {
            mStateMgr.changeState(StateID_Off);
        } else {
            mStateMgr.changeState(StateID_On);
        }
    }
}
void dTgSw_c::finalizeState_OnWait() {}

void dTgSw_c::initializeState_On() {
    mTimer = 4;
}
void dTgSw_c::executeState_On() {
    if (sLib::calcTimer(&mTimer) == 0) {
        mStateMgr.changeState(StateID_OffWait);
    }
}
void dTgSw_c::finalizeState_On() {}

void dTgSw_c::initializeState_OffWait() {
    if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSetSceneFlagId) && isLmfBossKeyPuzzle()) {
        SceneflagManager::sInstance->setFlag(mRoomID, 0xC0);
    }
    SceneflagManager::sInstance->setFlag(mRoomID, mSetSceneFlagId);
}
void dTgSw_c::executeState_OffWait() {
    if (mFirst2 == 1 && !isPuzzleSolved()) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, mSetSceneFlagId);
        mStateMgr.changeState(StateID_Off);
    }
}
void dTgSw_c::finalizeState_OffWait() {}

void dTgSw_c::initializeState_Off() {
    mTimer = 4;
}
void dTgSw_c::executeState_Off() {
    if (mIsFsBossKeyPuzzle && !mActorEvent.isThisActorInEvent()) {
        Event event = Event("FireCandleOn", 100, 0x100001, nullptr, nullptr);
        mActorEvent.scheduleEvent(event, 0);
        return;
    }
    if (sLib::calcTimer(&mTimer) == 0) {
        if (mThird2 == 1) {
            resetAllSwitches();
        } else if (mThird2 != 2) {
            resetSwitchesAfterCurrent();
        }

        if (isLmfBossKeyPuzzle()) {
            SceneflagManager::sInstance->setFlag(mRoomID, 0xC1);
        }
        mStateMgr.changeState(StateID_OnWait);
    }
}
void dTgSw_c::finalizeState_Off() {}
