#include "d/t/d_t_timer.h"

#include "toBeSorted/sceneflag_manager.h"


SPECIAL_ACTOR_PROFILE(TAG_TIMER, dTgTimer_c, fProfile::TAG_TIMER, 0x029F, 0, 0);

// TODO counter abstraction
bool increment(u16 *t) {
    if (*t < 0xFFFF) {
        (*t)++;
        return true;
    }
    return false;
}

int dTgTimer_c::create() {
    switch (getSubtypeFromParams()) {
        case 0: mGetTargetTimeFunc = &dTgTimer_c::getConstant0x50_Thunk; break;
        case 1:
            mTargetTime = getTimerFromParams() * 0x1e;
            mGetTargetTimeFunc = &dTgTimer_c::getStoredTargetTime;
            break;
        default:
            mTargetTime = getTimerFromParams();
            mGetTargetTimeFunc = &dTgTimer_c::getStoredTargetTime;
            break;
    }
    resetTimer();
    return SUCCEEDED;
}

int dTgTimer_c::actorExecute() {
    bool getFlag = SceneflagManager::sInstance->checkFlag(roomid, getCheckSceneflag());
    if (getFlag) {
        if (checkShouldTrigger()) {
            SceneflagManager::sInstance->setFlag(roomid, getSetSceneflag());
        } else {
            incrementTimer();
        }
    } else {
        resetTimer();
    }
    return SUCCEEDED;
}

int dTgTimer_c::getSubtypeFromParams() {
    return params & 0xFF;
}

u16 dTgTimer_c::getTimerFromParams() {
    return params >> 8 & 0xFF;
}

u16 dTgTimer_c::getCheckSceneflag() {
    return params >> 0x10 & 0xFF;
}

u16 dTgTimer_c::getSetSceneflag() {
    return params >> 0x18 & 0xFF;
}

u16 dTgTimer_c::getTimer() {
    return mTimer;
}

u16 dTgTimer_c::getConstant0x50() {
    return 0x50;
}

void dTgTimer_c::setTimer(u16 val) {
    mTimer = val;
}

u16 dTgTimer_c::getTargetTime() {
    return (this->*mGetTargetTimeFunc)();
}

void dTgTimer_c::resetTimer() {
    setTimer(0);
}

void dTgTimer_c::incrementTimer() {
    u16 t = getTimer();
    increment(&t);
    setTimer(t);
}

bool dTgTimer_c::checkShouldTrigger() {
    return getTargetTime() <= getTimer();
}

u16 dTgTimer_c::getStoredTargetTime() {
    return mTargetTime;
}

// TODO what causes this (likely weak) thunk?
u16 dTgTimer_c::getConstant0x50_Thunk() {
    return getConstant0x50();
}
