#include "d/t/d_t_ship_window.h"

#include "d/flag/sceneflag_manager.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(TAG_SHIP_WINDOW, dTgShipWindow_c, fProfile::TAG_SHIP_WINDOW, 0x0228, 0, 0);

STATE_DEFINE(dTgShipWindow_c, On);
STATE_DEFINE(dTgShipWindow_c, Off);

int dTgShipWindow_c::create() {
    mInitialStateFlag = mParams & 0xFF;
    mOutputStateFlagBase = (mParams >> 8) & 0xFF;
    mOffEventId = (mParams >> 16) & 0xFF;
    mOnEventId = (mParams >> 24) & 0xFF;
    mAlwaysPlayEvent = mRotation.x & 1;
    mRotation.x = 0;
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mInitialStateFlag)) {
        SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase);
        mStateMgr.changeState(StateID_On);
    } else {
        SceneflagManager::sInstance->unsetFlag(mRoomID, mOutputStateFlagBase);
        mStateMgr.changeState(StateID_Off);
    }
    return SUCCEEDED;
}

int dTgShipWindow_c::doDelete() {
    return SUCCEEDED;
}

int dTgShipWindow_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgShipWindow_c::actorExecuteInEvent() {
    switch (mEvent.getCurrentEventCommand()) {
        case 'act0':
            actorExecute();
            mEvent.advanceNext();
            break;
        case 'act1':
            actorExecute();
            if (*mStateMgr.getStateID() == StateID_Off) {
                SceneflagManager::sInstance->unsetFlag(mRoomID, mOutputStateFlagBase);
            } else {
                SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase);
            }
            mEvent.advanceNext();
            break;
        case '????': actorExecute(); break;
        default:     actorExecute(); break;
    }

    return SUCCEEDED;
}

int dTgShipWindow_c::draw() {
    return SUCCEEDED;
}

void dTgShipWindow_c::initializeState_Off() {}
void dTgShipWindow_c::executeState_Off() {
    if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, mInitialStateFlag)) {
        return;
    }

    if ((mHasPlayedOffEvent == 0 || mAlwaysPlayEvent == 0) &&
        (mOutputStateFlagBase >= 0xFF ||
         !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOutputStateFlagBase + 1))) {
        if (mOffEventId != 0xFF) {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event(mOffEventId, mRoomID, f2 & ~f1, nullptr, nullptr);
                mEvent.scheduleEvent(e, 0);
            }

            if (!EventManager::isInEvent(this, nullptr)) {
                return;
            }
            mHasPlayedOffEvent = 1;
            if (mOutputStateFlagBase < 0xFF) {
                SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase + 1);
            }
            mStateMgr.changeState(StateID_On);
            return;
        } else {
            SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase);
            mStateMgr.changeState(StateID_On);
            return;
        }
    }
    SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase);
    mStateMgr.changeState(StateID_On);
}
void dTgShipWindow_c::finalizeState_Off() {}

void dTgShipWindow_c::initializeState_On() {}
void dTgShipWindow_c::executeState_On() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mInitialStateFlag)) {
        return;
    }

    if ((mHasPlayedOnEvent == 0 || mAlwaysPlayEvent == 0) &&
        (mOutputStateFlagBase >= 0xFF ||
         !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mOutputStateFlagBase + 2))) {
        if (mOnEventId != 0xFF) {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event(mOnEventId, mRoomID, f2 & ~f1, nullptr, nullptr);
                mEvent.scheduleEvent(e, 0);
            }

            if (!EventManager::isInEvent(this, nullptr)) {
                return;
            }
            mHasPlayedOnEvent = 1;
            if (mOutputStateFlagBase < 0xFF) {
                SceneflagManager::sInstance->setFlag(mRoomID, mOutputStateFlagBase + 2);
            }
            mStateMgr.changeState(StateID_Off);
            return;
        } else {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mOutputStateFlagBase);
            mStateMgr.changeState(StateID_Off);
            return;
        }
    }
    SceneflagManager::sInstance->unsetFlag(mRoomID, mOutputStateFlagBase);
    mStateMgr.changeState(StateID_Off);
}
void dTgShipWindow_c::finalizeState_On() {}
