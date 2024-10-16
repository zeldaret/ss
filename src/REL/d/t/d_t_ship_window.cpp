#include <d/t/d_t_ship_window.h>
#include <toBeSorted/event_manager.h>
#include <toBeSorted/sceneflag_manager.h>

SPECIAL_ACTOR_PROFILE(TAG_SHIP_WINDOW, dTgShipWindow_c, fProfile::TAG_SHIP_WINDOW, 0x0228, 0, 0);

STATE_DEFINE(dTgShipWindow_c, On);
STATE_DEFINE(dTgShipWindow_c, Off);

// sDefaultRotX got placed in .data despite being zero.
// There is a handy pragma for this but it's probably not
// the correct solution.
#pragma explicit_zero_data on
u32 dTgShipWindow_c::sDefaultRotX = 0;
#pragma explicit_zero_data off

int dTgShipWindow_c::create() {
    mInitialStateFlag = params & 0xFF;
    mOutputStateFlagBase = (params >> 8) & 0xFF;
    mOffEventId = (params >> 16) & 0xFF;
    mOnEventId = (params >> 24) & 0xFF;
    mAlwaysPlayEvent = rotation.x & 1;
    rotation.x = sDefaultRotX;
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, mInitialStateFlag)) {
        SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase);
        mStateMgr.changeState(StateID_On);
    } else {
        SceneflagManager::sInstance->unsetFlag(roomid, mOutputStateFlagBase);
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
                SceneflagManager::sInstance->unsetFlag(roomid, mOutputStateFlagBase);
            } else {
                SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase);
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
    if (!SceneflagManager::sInstance->checkBoolFlag(roomid, mInitialStateFlag)) {
        return;
    }

    if ((mHasPlayedOffEvent == 0 || mAlwaysPlayEvent == 0) &&
        (mOutputStateFlagBase >= 0xFF || !SceneflagManager::sInstance->checkBoolFlag(roomid, mOutputStateFlagBase + 1))) {
        if (mOffEventId != 0xFF) {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event(mOffEventId, roomid, f2 & ~f1, nullptr, nullptr);
                mEvent.scheduleEvent(e, 0);
            }

            if (!EventManager::isInEvent(this, nullptr)) {
                return;
            }
            mHasPlayedOffEvent = 1;
            if (mOutputStateFlagBase < 0xFF) {
                SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase + 1);
            }
            mStateMgr.changeState(StateID_On);
            return;
        } else {
            SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase);
            mStateMgr.changeState(StateID_On);
            return;
        }
    }
    SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase);
    mStateMgr.changeState(StateID_On);
}
void dTgShipWindow_c::finalizeState_Off() {}

void dTgShipWindow_c::initializeState_On() {}
void dTgShipWindow_c::executeState_On() {
    if (SceneflagManager::sInstance->checkBoolFlag(roomid, mInitialStateFlag)) {
        return;
    }

    if ((mHasPlayedOnEvent == 0 || mAlwaysPlayEvent == 0) &&
        (mOutputStateFlagBase >= 0xFF || !SceneflagManager::sInstance->checkBoolFlag(roomid, mOutputStateFlagBase + 2))) {
        if (mOnEventId != 0xFF) {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event(mOnEventId, roomid, f2 & ~f1, nullptr, nullptr);
                mEvent.scheduleEvent(e, 0);
            }

            if (!EventManager::isInEvent(this, nullptr)) {
                return;
            }
            mHasPlayedOnEvent = 1;
            if (mOutputStateFlagBase < 0xFF) {
                SceneflagManager::sInstance->setFlag(roomid, mOutputStateFlagBase + 2);
            }
            mStateMgr.changeState(StateID_Off);
            return;
        } else {
            SceneflagManager::sInstance->unsetFlag(roomid, mOutputStateFlagBase);
            mStateMgr.changeState(StateID_Off);
            return;
        }
    }
    SceneflagManager::sInstance->unsetFlag(roomid, mOutputStateFlagBase);
    mStateMgr.changeState(StateID_Off);
}
void dTgShipWindow_c::finalizeState_On() {}
