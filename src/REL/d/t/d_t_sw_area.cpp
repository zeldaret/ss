#include "d/t/d_t_sw_area.h"

#include "d/a/d_a_player.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "toBeSorted/area_math.h"

SPECIAL_ACTOR_PROFILE(SW_AREA_TAG, dTgSwArea_c, fProfile::SW_AREA_TAG, 0x292, 0, 0);

int dTgSwArea_c::create() {
    setSceneflag = getSetSceneflag();
    unsetSceneflag = getUnsetSceneflag();
    isTemporary = !getIsPersistent();

    setStoryflag = getSetStoryflag();
    unsetStoryflag = getUnsetStoryflag();
    rotation.setX(0);
    rotation.setZ(0);

    matrixCreateFromPosRotYScale(area, position, rotation.y, mScale, nullptr, 0.0f);
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
    if (checkIfVec3fInMatrix(area, dAcPy_c::LINK->position)) {
        SceneflagManager::sInstance->setFlag(roomid, setSceneflag);
        SceneflagManager::sInstance->unsetFlag(roomid, unsetSceneflag);

        if (isValidStoryFlag(setStoryflag)) {
            StoryflagManager::sInstance->setFlag(setStoryflag);
        }

        if (isValidStoryFlag(unsetStoryflag)) {
            StoryflagManager::sInstance->unsetFlag(unsetStoryflag);
        }

        if (scale < 1.0f) {
            scale = 50.0f;
            matrixCreateFromPosRotYScale(area, position, rotation.y, mScale, nullptr, scale);
        }
    } else {
        if (scale > 1.0f) {
            scale = 0.0f;
            matrixCreateFromPosRotYScale(area, position, rotation.y, mScale, nullptr, scale);
        }

        if (isTemporary) {
            SceneflagManager::sInstance->unsetFlag(roomid, setSceneflag);
            SceneflagManager::sInstance->setFlag(roomid, unsetSceneflag);

            if (isValidStoryFlag(setStoryflag)) {
                StoryflagManager::sInstance->unsetFlag(setStoryflag);
            }

            if (isValidStoryFlag(unsetStoryflag)) {
                StoryflagManager::sInstance->setFlag(unsetStoryflag);
            }
        }
    }

    return SUCCEEDED;
}
