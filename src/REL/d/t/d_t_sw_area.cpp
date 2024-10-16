#include "d/t/d_t_sw_area.h"
#include "d/a/d_a_player.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/sceneflag_manager.h"
#include "toBeSorted/item_story_flag_manager.h"

SPECIAL_ACTOR_PROFILE(SW_AREA_TAG, dTgSwArea_c, fProfile::SW_AREA_TAG, 0x292, 0, 0);

u32 dTgSwArea_c::sDefaultRotX = 0;
u32 dTgSwArea_c::sDefaultRotZ = 0;

int dTgSwArea_c::create() {
    setSceneflag = getSetSceneflag();
    unsetSceneflag = getUnsetSceneflag();
    unsetOnLeave = !getSetOnLeave();

    setStoryflag = getSetStoryflag();
    unsetStoryflag = getUnsetStoryflag();
    rotation.x = sDefaultRotX;
    rotation.z = sDefaultRotZ;

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

        if (unsetOnLeave) {
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
