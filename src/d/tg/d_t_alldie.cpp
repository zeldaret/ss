#include "d/t/d_t_alldie.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/e/d_a_en_base.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "s/s_Math.h"
#include "toBeSorted/area_math.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(ALLDIE_TAG, dTgAllDie_c, fProfile::ALLDIE_TAG, 0x23, 0, 4);

int dTgAllDie_c::create() {
    mSceneflag = getFromParams(4, 0xFF);
    mType = getFromParams(12, 0x3);
    mTimer0x140 = getFromParams(22, 0xFF);
    field_0x148 = 0;
    field_0x144 = getFromParams(30, 0xFF);
    mRotation.x.set(0);
    matrixCreateFromPosRotYScale(mArea, mPosition, mRotation.y, mScale, nullptr, 0.0f);
    return SUCCEEDED;
}

int dTgAllDie_c::doDelete() {
    return SUCCEEDED;
}

int dTgAllDie_c::actorExecute() {
    if (!dStage_c::GetInstance()->isRoomID(mRoomID)) {
        return SUCCEEDED;
    }

    if (mType <= 1) {
        if (dTimeAreaMgr_c::fn_800BBD40()) {
            return SUCCEEDED;
        }

        mTimeArea.check(mRoomID, mPosition, 0, 10.0f, 0.1f);
        f32 f = mTimeArea.getDistMaybe();
        if (mType == 0 && f < 1.0f) {
            return SUCCEEDED;
        }
        if (mType == 1 && 0.0f < f) {
            return SUCCEEDED;
        }
    }

    mNumEnemiesInArea = 0;
    dAcEnBase_c *pEnemy = nullptr;
    while ((pEnemy = dAcEnBase_c::GetNextEnemy(pEnemy))) {
        if (pEnemy->mRoomID == mRoomID && !pEnemy->checkInteractionFlags(0x2)) {
            if (pEnemy->checkProcControl(DISABLE_EXECUTE) == 0 && pEnemy->mLifecycleState != TO_BE_DELETED) {
                if (checkAreaBox(mArea, pEnemy->mPosition)) {
                    mNumEnemiesInArea++;
                }
            }
        }
    }

    if (mNumEnemiesInArea == 0) {
        if (field_0x148 != 0 && EventManager::isInEvent()) {
            return SUCCEEDED;
        }

        if (sLib::calcTimer(&mTimer0x13C) == 0) {
            if (mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
                SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
            }
        }
    } else {
        mTimer0x13C = (mTimer0x140 <= 128 ? mTimer0x140 + 40 : 40 - (256 - mTimer0x140));
        if (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
            SceneflagManager::sInstance->unsetFlag(mRoomID, mSceneflag);
        }
    }
    return SUCCEEDED;
}

int dTgAllDie_c::draw() {
    return SUCCEEDED;
}

dTgAllDie_c *dTgAllDie_c::getNearestToPlayer(s32 roomId) {
    fBase_c *pPrev = nullptr;
    dTgAllDie_c *pReturn = nullptr;
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    f32 minDist = 1e10f;

    while (true) {
        dTgAllDie_c *pTgAllDie =
            static_cast<dTgAllDie_c *>(fManager_c::searchBaseByProfName(fProfile::ALLDIE_TAG, pPrev));
        pPrev = pTgAllDie;

        if (pTgAllDie == nullptr) {
            break;
        }

        if (roomId == pTgAllDie->mRoomID) {
            f32 dist = pPlayer->getPosition().squareDistanceToXZ(pTgAllDie->getPosition());
            if (dist < minDist) {
                pReturn = pTgAllDie;
                minDist = dist;
            }
        }
    }
    return pReturn;
}
