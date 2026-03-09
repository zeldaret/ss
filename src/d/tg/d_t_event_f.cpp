#include "d/t/d_t_event_f.h"

#include "d/a/d_a_player.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "toBeSorted/area_math.h"

SPECIAL_ACTOR_PROFILE(EVENTF_TAG, dTgEventF_c, fProfile::EVENTF_TAG, 0X14, 0, 0);

int dTgEventF_c::create() {
    matrixCreateFromPosRotYScale(mMatrix, mPosition, mRotation.y, mScale, 0x0, 0.0f);
    mEventId = getFromParams(0, 0xFF);
    p1_s8_0x7FF = getFromParams(8, 0x7FF);
    if ((s32)p1_s8_0x7FF == 0x3FF) {
        p1_s8_0x7FF = 0xFFFFFFFF;
    }
    p1_s19_0x7FF = getFromParams(19, 0x7FF);
    if ((s32)p1_s19_0x7FF == 0x3FF) {
        p1_s19_0x7FF = 0xFFFFFFFF;
    }
    p1_s30_0x3 = getFromParams(30, 0x3);
    return SUCCEEDED;
}

int dTgEventF_c::doDelete() {
    return SUCCEEDED;
}

int dTgEventF_c::checkStoryflagValid(u32 counterIdx) {
    return StoryflagManager::sInstance->checkFlagValid(counterIdx);
}

bool dTgEventF_c::checkUncommitedStoryFlagValue(u32 flag) {
    return StoryflagManager::sInstance->getUncommittedValue(flag);
}

void dTgEventF_c::setStoryFlag(StoryFlags_e flag) {
    StoryflagManager::sInstance->setFlag(flag);
    return;
}

void dTgEventF_c::unsetStoryFlag(StoryFlags_e flag) {
    StoryflagManager::sInstance->unsetFlag(flag);
    return;
}

int dTgEventF_c::actorExecute() {
    if ((!checkStoryflagValid(p1_s19_0x7FF) || checkUncommitedStoryFlagValue(p1_s19_0x7FF))) {
        if ((p1_s30_0x3 != 0 && !(checkStoryflagValid(p1_s8_0x7FF) && checkUncommitedStoryFlagValue(p1_s8_0x7FF))) ||
            (p1_s30_0x3 == 0 && checkStoryflagValid(p1_s8_0x7FF) && checkUncommitedStoryFlagValue(p1_s8_0x7FF))) {
            if (checkIfVec3fInMatrix(mMatrix, dAcPy_c::LINK->mPosition)) {
                Event EStack_48 = Event(mEventId, (int)mRoomID, 0x100001, 0, 0);
                mActorEvent.scheduleEvent(EStack_48, 0);
            }
        }
    }
    return SUCCEEDED;
}
int dTgEventF_c::actorExecuteInEvent() {
    int iVar1;

    // iVar1 = mActorEvent.isThisActorInEvent();
    if (mActorEvent.isThisActorInEvent()) {
        mActorEvent.advanceNext();
        if (!(p1_s30_0x3 == 0 || !checkStoryflagValid(p1_s8_0x7FF) || checkUncommitedStoryFlagValue(p1_s8_0x7FF))) {
            setStoryFlag((StoryFlags_e)p1_s8_0x7FF);
        } else if ((p1_s30_0x3 == 0 && checkStoryflagValid(p1_s8_0x7FF)) &&
                   checkUncommitedStoryFlagValue(p1_s8_0x7FF)) {
            unsetStoryFlag((StoryFlags_e)p1_s8_0x7FF);
        }
    }
    return SUCCEEDED;
}

int dTgEventF_c::draw() {
    return SUCCEEDED;
}
