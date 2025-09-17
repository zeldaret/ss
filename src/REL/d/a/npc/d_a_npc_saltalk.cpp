#include "d/a/npc/d_a_npc_saltalk.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "toBeSorted/area_utils.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(NPC_SLTK, dAcNpcSltk_c, fProfile::NPC_SLTK, 0x63, 0, 0);

STATE_VIRTUAL_DEFINE(dAcNpcSltk_c, Wait);

int dAcNpcSltk_c::create() {
    return dAcBase_c::create();
}

int dAcNpcSltk_c::actorCreate() {
    field_0x748 = getParam4();
    if (isSomething0()) {
        field_0x754 = getParam0_0();
        field_0x74C = 0xFF;
        field_0x750 = 0xFF;
    } else if (isSomething1()) {
        field_0x754 = 0xFF;
        field_0x74C = getParam0_1();
        field_0x750 = getParam1();
    } else {
        field_0x754 = 0xFF;
        field_0x74C = 0xFF;
        field_0x750 = 0xFF;
    }

    mAreaIndex = mRotation.z & 0xFF;
    if (mAreaIndex < 0xFF) {
        mpArea = getAreaForIndexInRoom(mAreaIndex, mRoomID);
        if (mpArea == nullptr) {
            return FAILED;
        }
    }

    mRotation.set(0, 0, 0);

    field_0x758 = getParam2() * 100;
    field_0x75C = getParam3() * 100;

    setState(StateID_Wait);

    field_0x76C = 0;

    boundingBox.Set(mVec3_c(-0.0f, -0.0f, -0.0f), mVec3_c(0.0f, 0.0f, 0.0f));

    return SUCCEEDED;
}

int dAcNpcSltk_c::actorPostCreate() {
    mLinkPos = getLinkPos();
    mRef.unlink();

    f32 closestDistance = FLOAT_MAX;

    dAcNpcSlb2_c *cursor = nullptr;
    dAcNpcSlb2_c *closest = nullptr;
    do {
        cursor = static_cast<dAcNpcSlb2_c *>(fManager_c::searchBaseByProfName(fProfile::NPC_SLB2, cursor));
        if (cursor != nullptr) {
            if (cursor->acNpc_vt_0x1F4()) {
                f32 thisDist = cursor->getSquareDistanceTo(mPosition);
                if (thisDist < closestDistance) {
                    closest = cursor;
                    closestDistance = cursor->getSquareDistanceTo(mPosition);
                }
            }
        }
    } while (cursor != nullptr);

    if (closest != nullptr) {
        mRef.link(closest);
    }
    return SUCCEEDED;
}

int dAcNpcSltk_c::actorExecute() {
    npcExecute();
    return SUCCEEDED;
}

int dAcNpcSltk_c::acNpc_vt_0xDC() {
    return 0;
}

int dAcNpcSltk_c::draw() {
    return SUCCEEDED;
}

int dAcNpcSltk_c::getParam0_0() const {
    return mParams & 0xFF;
}
int dAcNpcSltk_c::getParam0_1() const {
    return mParams & 0xFF;
}

int dAcNpcSltk_c::getParam1() const {
    return (mParams >> 8) & 0xFF;
}
int dAcNpcSltk_c::getParam2() const {
    return (mParams >> 16) & 0xFF;
}
int dAcNpcSltk_c::getParam3() const {
    return (mParams >> 24) & 0xFF;
}
int dAcNpcSltk_c::getParam4() const {
    return getParams2Lower() & 0xF;
}

int dAcNpcSltk_c::getSomething() const {
    return field_0x748;
}
bool dAcNpcSltk_c::isSomething0() const {
    return getSomething() == 0;
}
bool dAcNpcSltk_c::isSomething1() const {
    return getSomething() == 1;
}

bool dAcNpcSltk_c::checkSomething(mVec3_c pos) const {
    if (mAreaIndex != 0xFF) {
        return checkPosInArea(mAreaIndex, mRoomID, pos, &mpArea) == 1;
    } else {
        mVec3_c dist = pos - mPosition;
        if (dist.squareMagXZ() <= field_0x758 * field_0x758) {
            f32 fDist = field_0x75C + mPosition.y;
            f32 fDist2 = mPosition.y;
            if (fDist <= fDist2) {
                fDist2 = fDist;
                fDist = mPosition.y;
            }
            return pos.y <= fDist && pos.y >= fDist2;
        } else {
            return false;
        }
    }
}

void dAcNpcSltk_c::initializeState_Wait() {}
void dAcNpcSltk_c::executeState_Wait() {
    bool checkResult = false;

    const dAcPy_c *link = dAcPy_c::GetLink();

    dAcNpcSlb2_c *slb = mRef.get();
    if (slb != nullptr) {
        if (isSomething0()) {
            checkResult = checkSomething(link->mPosition);
            if (field_0x76C != 0) {
                field_0x76C = checkResult;
            } else if (!EventManager::isInEvent() && !slb->fn_61_6A10() && checkResult) {
                mAng3_c ang = mAng3_c(0, mpArea->mAngleY, 0);
                mVec3_c proj = mVec3_c::createProjectionXZ(ang, 1.0f);
                mVec3_c dist = mLinkPos - mpArea->mPos;
                dist.y = 0.0f;
                dist.normalize();
                slb->fn_61_68D0(field_0x754);
                field_0x76C = 1;
            }

            if (!checkResult) {
                if (!link->checkActionFlags(0xC70852) || link->checkActionFlags(0x40000)) {
                    mLinkPos = link->mPosition;
                }
            }
        } else if (isSomething1()) {
            checkResult = checkSomething(slb->mPosition);
            if (field_0x76C == 0 && checkResult) {
                slb->fn_61_58C0(field_0x74C, field_0x750);
            }
        }
        field_0x76C = checkResult;
    }
}
void dAcNpcSltk_c::finalizeState_Wait() {}
