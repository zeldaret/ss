#include <c/c_math.h>
#include <d/tg/d_t_touch.h>
#include <toBeSorted/sceneflag_manager.h>
#include <toBeSorted/area_math.h>

SPECIAL_ACTOR_PROFILE(TOUCH_TAG, dTgTouchTag, fProfile::TOUCH_TAG, 0x028E, 0, 0);

STATE_DEFINE(dTgTouchTag, Wait);

const static u16 ACTIVATORS[] = {
    fProfile::BOMB,
    fProfile::BOMB,
};

int dTgTouchTag::create() {
    mActivatorIndex = !((params & 0xF) == 0xF) ? (params & 0xF) : 0;
    mZoneFlag = (params >> 4);
    mChkFlag = (params >> 0xC);
    matrixCreateFromPosRotYScale(mAreaOfEffect, position, rotation.y, scale, nullptr, 0.0f);
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dTgTouchTag::doDelete() {
    return SUCCEEDED;
}

int dTgTouchTag::actorExecute() {
    if (mChkFlag != 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mChkFlag)) {
        SceneflagManager::sInstance->unsetFlag(roomid, mZoneFlag);
        return SUCCEEDED;
    }

    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgTouchTag::draw() {
    return SUCCEEDED;
}

void dTgTouchTag::initializeState_Wait() {
    mFlagTimer = 0;
}
void dTgTouchTag::executeState_Wait() {
    dAcBase_c *actor = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(ACTIVATORS[mActivatorIndex], nullptr));

    while (actor != nullptr) {
        if (actor->roomid == roomid && checkIfVec3fInMatrix(mAreaOfEffect, actor->position)) {
            mFlagTimer = 5;
            if (mZoneFlag != 0xFF) {
                SceneflagManager::sInstance->setFlag(roomid, mZoneFlag);
            }
            break;
        }

        actor = static_cast<dAcBase_c *>(fManager_c::searchBaseByProfName(ACTIVATORS[mActivatorIndex], actor));
    }

    if (cM::calcTimer(&mFlagTimer) == 0) {
        if (mZoneFlag != 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mZoneFlag)) {
            SceneflagManager::sInstance->unsetFlag(roomid, mZoneFlag);
        }
    }
}
void dTgTouchTag::finalizeState_Wait() {}
