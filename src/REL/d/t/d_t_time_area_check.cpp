#include "d/t/d_t_time_area_check.h"

#include "d/flag/sceneflag_manager.h"
#include "toBeSorted/time_area_mgr.h"


SPECIAL_ACTOR_PROFILE(TAG_TIME_AREA_CHECK, dTgTimeAreaCheck_c, fProfile::TAG_TIME_AREA_CHECK, 0x01AC, 0, 0);

int dTgTimeAreaCheck_c::create() {
    mPastSceneFlag = params & 0xFF;
    mPresentSceneFlag = (params >> 8) & 0xFF;
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::doDelete() {
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::actorExecute() {
    f32 result = dTimeAreaMgr_c::GetInstance()->checkPositionIsInPastState(roomid, position, nullptr, 10.0f);
    if (result > 0.0f) {
        if (mPastSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, mPastSceneFlag)) {
            SceneflagManager::sInstance->setFlag(roomid, mPastSceneFlag);
        }
        if (mPresentSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mPresentSceneFlag)) {
            SceneflagManager::sInstance->unsetFlag(roomid, mPresentSceneFlag);
        }
    } else {
        if (mPastSceneFlag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(roomid, mPastSceneFlag)) {
            SceneflagManager::sInstance->unsetFlag(roomid, mPastSceneFlag);
        }
        if (mPresentSceneFlag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, mPresentSceneFlag)) {
            SceneflagManager::sInstance->setFlag(roomid, mPresentSceneFlag);
        }
    }
    return SUCCEEDED;
}

int dTgTimeAreaCheck_c::draw() {
    return SUCCEEDED;
}
