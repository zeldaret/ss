#include "d/t/d_t_action.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/d_player.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "toBeSorted/area_math.h"

SPECIAL_ACTOR_PROFILE(ACTION_TAG, dTgAction_c, fProfile::ACTION_TAG, 0x20, 0, 0);

int dTgAction_c::create() {
    mEnableFlag1 = (mParams >> 6) & 0xFF;
    mEnableFlag2 = (mParams >> 14) & 0xFF;
    mType = (mParams >> 0) & 0x3F;
    matrixCreateFromPosRotYScale(mActiveMtx, mPosition, mRotation.y, mScale, nullptr, 0.f);
    return SUCCEEDED;
}

int dTgAction_c::doDelete() {
    return SUCCEEDED;
}

void dTgAction_c::setActiveArea(f32 scale) {
    if (mScaleIncrease < 0.1f) {
        mScaleIncrease = scale;
        matrixCreateFromPosRotYScale(mActiveMtx, mPosition, mRotation.y, mScale, nullptr, mScaleIncrease);
    }
}

void dTgAction_c::resetActiveArea() {
    if (mScaleIncrease > 0.f) {
        mScaleIncrease = 0.f;
        matrixCreateFromPosRotYScale(mActiveMtx, mPosition, mRotation.y, mScale, nullptr, 0.f);
        field_0x134 = 0;
    }
}

int dTgAction_c::actorExecute() {
    dAcPy_c *player = getLinkPtr();
    bool enabled = !checkActive1() && checkActive2();

    // Check For Bomb within area
    if (enabled && mType == WATER_RELATED) {
        s32 numRefs = player->getBeetleActorReferencesSize();
        for (s32 i = 0; i < numRefs; i++) {
            dAcObjBase_c *pObj = player->checkBeetleActorReferences(i);
            if (pObj == nullptr) {
                break;
            }
            if (pObj->mProfileName == fProfile::BOMB) {
                if (checkIfVec3fInMatrix(mActiveMtx, pObj->getPosition())) {
                    reinterpret_cast<dAcBomb_c *>(pObj)->setField0xA3C(0x20000000);
                }
            }
        }
    }

    // Check For Player Within Area
    if (enabled && checkIfVec3fInMatrix(mActiveMtx, player->getPosition())) {
        if (mType == FORCE_FADE_RESTART) {
            player->onFlags_0x358(0x10000000);
            setActiveArea(50.f);
        } else if (mType == PREVENT_RESPAWN) {
            player->onForceOrPreventActionFlags(0x80);
        } else if (mType == FORCE_AUTO_JUMP) {
            player->onForceOrPreventActionFlags(0x200000);
        } else if (mType == PREVENT_LOFTWING_CALL) {
            player->onForceOrPreventActionFlags(0x20000);
            setActiveArea(50.f);
        } else if (mType == WATER_RELATED) {
            player->onForceOrPreventActionFlags(0x400000);
            setActiveArea(15.f);
        } else if (mType == SHOW_DASH_PROMPT) {
            if (mAng::abs(player->mRotation.y - mRotation.y) < (0x2000 + field_0x134)) {
                player->onForceOrPreventActionFlags(0x20000000);
                setActiveArea(50.f);
                field_0x134.set(0x38E);
            } else {
                resetActiveArea();
            }
        }
    } else {
        resetActiveArea();
    }
    return SUCCEEDED;
}

int dTgAction_c::draw() {
    return SUCCEEDED;
}
