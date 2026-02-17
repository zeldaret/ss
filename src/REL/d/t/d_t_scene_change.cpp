#include "d/t/d_t_scene_change.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_salbage_robot.h"
#include "d/a/npc/d_a_npc_volcano_f2_salbo.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/d_path.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(SC_CHANGE_TAG, dTgSceneChange_c, fProfile::SC_CHANGE_TAG, 0x21, 0, 4);

int dTgSceneChange_c::create() {
    mSceneLink = getFromParams(0, 0xFF);
    mPathId = getFromParams(8, 0xFF);

    mSceneType = getFromParams(16, 0xFF);
    if (mSceneType == 0xFF) {
        mSceneType = 0 /* TODO: Change with enum when used. See note in this files header */;
    }

    mEnableSceneflag = getFromParams(24, 0xFF);
    mEnableStoryflag = (mRotation.x >> 0) & 0x7FF;

    field_0x134 = ((mRotation.x >> 11) & 0x3) == 1;
    mRotation.x = 0;

    mDisableStoryflag = mRotation.z & 0x7FF;
    field_0x136 = ((mRotation.z >> 12) & 0xF) != 0;

    mRotation.z = 0;

    matrixCreateFromPosRotYScale(mArea, mPosition, mRotation.y, mScale, nullptr, 0.f);

    return SUCCEEDED;
}

bool dTgSceneChange_c::checkDisabled() const {
    bool disabled = false;

    bool validFlag = mDisableStoryflag != 0 && mDisableStoryflag < 0x7FF;
    if (validFlag && StoryflagManager::sInstance->getFlag(mDisableStoryflag)) {
        disabled = true;
    }
    return disabled;
}

int dTgSceneChange_c::actorExecute() {
    if (checkDisabled()) {
        return SUCCEEDED;
    }

    if (mEnableStoryflag == 0 || mEnableStoryflag >= 0x7FF || !StoryflagManager::sInstance->getFlag(mEnableStoryflag)) {
        return SUCCEEDED;
    }

    if (mEnableSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mEnableSceneflag)) {
        return SUCCEEDED;
    }

    dAcPy_c *pPlayer = dAcPy_c::GetLinkM();
    if ((field_0x134 && checkAreaCyl(mArea, pPlayer->mPosition)) ||
        (!field_0x134 && checkAreaBox(mArea, pPlayer->mPosition))) {
        if (!(field_0x136 && checkScrapper())) {
            pPlayer->triggerExitRelated(mSceneLink, mPathId, mSceneType);
        }
    } else {
        if (field_0x136) {
            savePlayerPosition();
        }
    }

    return SUCCEEDED;
}

extern "C" dAcOrdinaryNpc_c *SCRAPPER_PTR;

bool dTgSceneChange_c::checkScrapper() {
    if (SCRAPPER_PTR == nullptr) {
        return false;
    }

    if (SCRAPPER_PTR->mProfileName != fProfile::NPC_SLB2) {
        return false;
    }

    if (!static_cast<dAcNpcSlb2_c*>(SCRAPPER_PTR)->fn_61_6A10()) {
        return false;
    }

    dPath_c path;
    if (path.initWithPathIndex(mPathId, mRoomID, 0) && path.getNumPoints() > 1) {
        if (EventManager::isInEvent(SCRAPPER_PTR, "HailToPlayerToStop")) {
            mVec3_c p0, p1;
            p0.copyFrom(path.getPoint(0));
            p1.copyFrom(path.getPoint(1));
            mAng a = cLib::targetAngleY(p0, p1);

            


        }
    }
    
    return false;


} 

void dTgSceneChange_c::savePlayerPosition() {
    if (SCRAPPER_PTR == nullptr) {
        return;
    }

    if (SCRAPPER_PTR->mProfileName != fProfile::NPC_SLB2) {
        return;
    }

    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    if (pPlayer->checkActionFlags(0x800000 | 0x400000 | 0x40000 | 0x20000 | 0x10000 | 0x800 | 0x40 | 0x10 | 0x2) &&
        !pPlayer->checkActionFlags(0x40000)) {
        return;
    }

    field_0x138 = pPlayer->mPosition;
}

int dTgSceneChange_c::draw() {
    return SUCCEEDED;
}
