#include "d/t/d_t_time_area_check.h"

#include "d/flag/sceneflag_manager.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(TAG_TIME_AREA_CHECK, dTgTimeAreaCheck_c, fProfile::TAG_TIME_AREA_CHECK, 0x01AC, 0, 0);

int dTgTimeAreaCheck_c::create() {
    mPastSceneFlag = mParams & 0xFF;
    mPresentSceneFlag = (mParams >> 8) & 0xFF;
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::doDelete() {
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::actorExecute() {
    f32 result = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(mRoomID, mPosition, nullptr, 10.0f);
    if (result > 0.0f) {
        if (mPastSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mPastSceneFlag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, mPastSceneFlag);
        }
        if (mPresentSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mPresentSceneFlag)) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mPresentSceneFlag);
        }
    } else {
        if (mPastSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mPastSceneFlag)) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mPastSceneFlag);
        }
        if (mPresentSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mPresentSceneFlag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, mPresentSceneFlag);
        }
    }
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::draw() {
    return SUCCEEDED;
}
