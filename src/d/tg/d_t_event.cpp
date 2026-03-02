#include "d/t/d_t_event.h"

#include "d/a/d_a_player.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/event.h"

SPECIAL_ACTOR_PROFILE(EVENT_TAG, dTgEvent_c, fProfile::EVENT_TAG, 0X13, 0, 0);

int dTgEvent_c::create() {
    matrixCreateFromPosRotYScale(mMatrix, mPosition, mRotation.y, mScale, 0x0, 0.0f);
    mEventId = getFromParams(0, 0xFF);
    mSetSceneflag = getFromParams(8, 0xFF);
    mTrigSceneflag = getFromParams(16, 0xFF);
    mSubtype = getFromParams(24, 0x3);
    mUnkParamFlag = getFromParams(26, 0x1);
    return SUCCEEDED;
}

int dTgEvent_c::doDelete() {
    return SUCCEEDED;
}

inline u32 fun(const u32 &flag, const u32 &returnFlag) {
    u32 b = 0;
    if (flag & 1) {
        b = returnFlag;
    }
    return b;
}

inline bool dTgEvent_c::checkUncommittedFlag(u16 flag) {
    return SceneflagManager::sInstance->checkUncommittedFlag(mRoomID, flag);
}

inline bool dTgEvent_c::isValidFlag(u16 flag) {
    return flag < 0xFF;
}

int dTgEvent_c::actorExecute() {
    if (!isValidFlag(mTrigSceneflag) || checkUncommittedFlag(mTrigSceneflag)) {
        if ((mSubtype != 0 && !(isValidFlag(mSetSceneflag) && checkUncommittedFlag(mSetSceneflag))) ||
            (mSubtype == 0 && (isValidFlag(mSetSceneflag) && checkUncommittedFlag(mSetSceneflag)))) {
            if (checkAreaBox(mMatrix, dAcPy_c::LINK->mPosition)) {
                Event EStack_48 = Event(mEventId, mRoomID, fun(mUnkParamFlag, 0x1), 0, 0);
                mActorEvent.scheduleEvent(EStack_48, 0);
            }
        }
    }
    return SUCCEEDED;
}

int dTgEvent_c::actorExecuteInEvent() {
    if (mActorEvent.isThisActorInEvent() != 0) {
        mActorEvent.advanceNext();
        if (mSubtype != 0 && isValidFlag(mSetSceneflag) && !checkUncommittedFlag(mSetSceneflag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, mSetSceneflag);
        } else if (mSubtype == 0 && isValidFlag(mSetSceneflag) && checkUncommittedFlag(mSetSceneflag)) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSetSceneflag);
        }
    }
    return SUCCEEDED;
}

int dTgEvent_c::draw() {
    return SUCCEEDED;
}
