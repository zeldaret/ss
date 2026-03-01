#include "d/t/d_t_event.h"

#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/area_math.h"
#include "d/a/d_a_player.h"
#include "toBeSorted/event.h"


SPECIAL_ACTOR_PROFILE(EVENT_TAG, dTgEvent_c, fProfile::EVENT_TAG, 0X13, 0, 0);


int dTgEvent_c::create()
{
  matrixCreateFromPosRotYScale(mMatrix, mPosition, mRotation.y, mScale, 0x0, 0.0f);
  mEventId = mParams & 0xFF;
  setSceneflag = mParams >> 8 & 0xFF;
  trigSceneflag = mParams >> 0x10 & 0xFF;
  subtype = mParams >> 0x18 & 0x3;
  unkParamFlag = mParams >> 0x1A & 1;
  return SUCCEEDED;
}

int dTgEvent_c::doDelete() {
    return SUCCEEDED;
} 

inline u32 fun(const u32 &flag, const u32 &returnFlag) {
    u32 b = 0;
    if (flag & 1) b = returnFlag;
    return b;
}

inline bool dTgEvent_c::checkUncommittedFlag(u16 flag) {
    return SceneflagManager::sInstance->checkUncommittedFlag(mRoomID, flag);
}

inline bool dTgEvent_c::isValidFlag(u16 flag) {
    return flag < 0xFF;
}

int dTgEvent_c::actorExecute() {
    if (!isValidFlag(trigSceneflag) || checkUncommittedFlag(trigSceneflag)) {
        if ((subtype != 0 && !(isValidFlag(setSceneflag) && checkUncommittedFlag(setSceneflag))) || (subtype == 0 && (isValidFlag(setSceneflag) && checkUncommittedFlag(setSceneflag)))) {
            if (checkIfVec3fInMatrix(mMatrix,dAcPy_c::LINK->mPosition)) {
                Event EStack_48 = Event(mEventId, mRoomID, fun(unkParamFlag, 0x1),0,0);
                mActorEvent.scheduleEvent(EStack_48,0);
            }
        }
    }
    return SUCCEEDED;
}

int dTgEvent_c::actorExecuteInEvent() {
    if (mActorEvent.isThisActorInEvent() != 0) {
        mActorEvent.advanceNext();
        if (subtype != 0 && isValidFlag(setSceneflag) && !checkUncommittedFlag(setSceneflag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, setSceneflag);
        } else if (subtype == 0 && isValidFlag(setSceneflag) && checkUncommittedFlag(setSceneflag)) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, setSceneflag);
        }
  }
  return SUCCEEDED;
}

int dTgEvent_c::draw() {
    return SUCCEEDED;
}
