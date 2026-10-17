#include "d/t/d_t_sw_area.h"

#include "d/a/d_a_player.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"

SPECIAL_ACTOR_PROFILE(SW_AREA_TAG, dTgSwArea_c, fProfile::SW_AREA_TAG, 0x292, 0, 0);

int dTgSwArea_c::create() {
    mSetSceneflag = getFromParams(0, 0xFF);
    mUnsetSceneflag = getFromParams(8, 0xFF);
    mbTemporary = !getFromParams(16, 0x1);

    mSetStoryflag = mRotation.x & 0x7FF;
    mUnsetStoryflag = mRotation.z & 0x7FF;
    mRotation.x = 0;
    mRotation.z = 0;

    mArea.set(mPosition, mRotation.y, mScale, nullptr, 0.0f);
    return SUCCEEDED;
}

bool isValidStoryFlag(u16 storyflag) {
    bool valid = false;

    if (storyflag != 0 && storyflag < 0x7FF) {
        valid = true;
    }

    return valid;
}

int dTgSwArea_c::actorExecute() {
    if (mArea.checkAreaBox(dAcPy_c::GetLink()->mPosition)) {
        SceneflagManager::sInstance->setFlag(mRoomID, mSetSceneflag);
        SceneflagManager::sInstance->unsetFlag(mRoomID, mUnsetSceneflag);

        if (isValidStoryFlag(mSetStoryflag)) {
            StoryflagManager::sInstance->setFlag(mSetStoryflag);
        }

        if (isValidStoryFlag(mUnsetStoryflag)) {
            StoryflagManager::sInstance->unsetFlag(mUnsetStoryflag);
        }

        if (mScaleModify < 1.0f) {
            mScaleModify = 50.0f;
            mArea.set(mPosition, mRotation.y, mScale, nullptr, mScaleModify);
        }
    } else {
        if (mScaleModify > 1.0f) {
            mScaleModify = 0.0f;
            mArea.set(mPosition, mRotation.y, mScale, nullptr, mScaleModify);
        }

        if (mbTemporary) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSetSceneflag);
            SceneflagManager::sInstance->setFlag(mRoomID, mUnsetSceneflag);

            if (isValidStoryFlag(mSetStoryflag)) {
                StoryflagManager::sInstance->unsetFlag(mSetStoryflag);
            }

            if (isValidStoryFlag(mUnsetStoryflag)) {
                StoryflagManager::sInstance->setFlag(mUnsetStoryflag);
            }
        }
    }

    return SUCCEEDED;
}
