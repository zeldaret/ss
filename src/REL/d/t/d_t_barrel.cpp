#include "d/t/d_t_barrel.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_barrel.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_stage_sink.h"
#include "d/flag/sceneflag_manager.h"
#include "d/t/d_t_barrel_pos.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_Math.h"

SPECIAL_ACTOR_PROFILE(TAG_BARREL, dTgBarrel_c, fProfile::TAG_BARREL, 0x221, 0, 0);

STATE_DEFINE(dTgBarrel_c, Wait);
STATE_DEFINE(dTgBarrel_c, Stop);

// Weak function ordering issue of the ctor and dtor of the objects
// for the following in the member arrays
//   dAcRef_c<dTgBarrelPos_c>
//   dAcRef_c<dAcOBarrel_c>

int dTgBarrel_c::actorCreate() {
    mStopFlag = getFromParams(0, 0xFF);
    mLinkId = getFromParams(16, 0xF);
    mSpawnInterval = getFromParams(8, 0xFF);
    mSpawnTimer = 1;

    mStageRef.link(dAcOstageSink_c::GetInstance());
    mSpawnPosition = mPosition;

    for (int i = 0; i < (int)ARRAY_LENGTH(mBarrelArr); ++i) {
        mBarrelArr[i].unlink();
    }

    mStateMgr.changeState(StateID_Wait);

    return SUCCEEDED;
}

int dTgBarrel_c::actorPostCreate() {
    dTgBarrelPos_c *pTgBarrelPos = nullptr;
    do {
        pTgBarrelPos =
            static_cast<dTgBarrelPos_c *>(fManager_c::searchBaseByProfName(fProfile::TAG_BARREL_POS, pTgBarrelPos));

        if (pTgBarrelPos != nullptr) {
            u8 index = pTgBarrelPos->getLinkIndex();
            if (mLinkId == pTgBarrelPos->getLinkId()) {
                mTgBarrelPosArr[index].link(pTgBarrelPos);
                if (index >= mBarrelPosCount) {
                    mBarrelPosCount = index + 1;
                }
            }
        }

    } while (pTgBarrelPos != nullptr);

    return SUCCEEDED;
}

int dTgBarrel_c::doDelete() {
    return SUCCEEDED;
}

int dTgBarrel_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dTgBarrel_c::draw() {
    return SUCCEEDED;
}

void dTgBarrel_c::initializeState_Wait() {}
void dTgBarrel_c::executeState_Wait() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mStopFlag)) {
        mStateMgr.changeState(StateID_Stop);
        return;
    }
    dAcOstageSink_c *pStage = mStageRef.get();

    mVec3_c pos = pStage->mPosition;
    mAng3_c rot = pStage->mRotation;

    mVec3_c stageDist = mSpawnPosition - pos;
    mMtx_c m;
    m.transS(pos);
    m.ZXYrotM(rot);
    m.multVecSR(stageDist, mPosition);

    mPosition += pos;

    if (checkPlayerPos(dAcPy_c::GetLink()->mPosition)) {
        if (0 == sLib::calcTimer(&mSpawnTimer)) {
            mSpawnActive = true;

            u8 idx = 0xFF;
            for (int i = 0; i < (int)ARRAY_LENGTH(mBarrelArr); ++i) {
                if (!mBarrelArr[i].isLinked()) {
                    idx = i;
                    // I think they forgor a break here
                }
            }

            if (idx != 0xFF) {
                dTgBarrelPos_c *pTgBarrelPos = mTgBarrelPosArr[mBarrelPosIndex].get();
                if (pTgBarrelPos != nullptr) {
                    mVec3_c spawnPos = pTgBarrelPos->mPosition;
                    mAng3_c spawnRot(0, 0x4000, 0);
                    dAcObjBase_c *pObj = dAcObjBase_c::create(
                        fProfile::OBJ_BARREL, mRoomID, 0xFF00FF2, &spawnPos, &spawnRot, nullptr, 0xFFFFFFFF
                    );
                    mBarrelArr[idx].link(static_cast<dAcOBarrel_c *>(pObj));
                }
                if (++mBarrelPosIndex >= mBarrelPosCount) {
                    mBarrelPosIndex = 0;
                }
            }
            mSpawnTimer = mSpawnInterval * 30;
        }
    } else {
        if (mSpawnActive) {
            mSpawnTimer = mSpawnInterval * 30;
        }
    }
}
void dTgBarrel_c::finalizeState_Wait() {}

void dTgBarrel_c::initializeState_Stop() {}
void dTgBarrel_c::executeState_Stop() {
    if (!SceneflagManager::sInstance->checkBoolFlag(mRoomID, mStopFlag)) {
        mStateMgr.changeState(StateID_Wait);
    }
}
void dTgBarrel_c::finalizeState_Stop() {}

bool dTgBarrel_c::checkPlayerPos(const mVec3_c &playerPos) {
    f32 scale = 0.5f;

    mVec3_c pos = playerPos - (mPosition + (mVec3_c::Ey * mScale.y * scale));
    mAng rot = mStageRef.get()->mRotation.z;

    pos.x = pos.x / mScale.x;
    pos.y = pos.y / mScale.y;
    pos.z = pos.z / mScale.z;

    f32 halfCos = rot.cos() * scale;
    if (rot.cos() != 0.f) {
        scale = mScale.x * ((pos.x * rot.sin()) / rot.cos()) / mScale.y;
    }

    return (
        (pos.x <= halfCos && pos.x >= -halfCos)                            //
        && pos.y <= scale + 0.5f + 1e-6f && pos.y >= scale + -0.5f - 1e-6f //
        && pos.z <= 0.5f && pos.z >= -0.5f
    );
}
