#include "d/a/d_a_bombf.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "toBeSorted/time_area_mgr.h"

SPECIAL_ACTOR_PROFILE(BOMBF, dAcBombf_c, fProfile::BOMBF, 0x129, 0, 4099);

STATE_DEFINE(dAcBombf_c, Wait);

bool dAcBombf_c::createHeap() {
    nw4r::g3d::ResFile resFile(getOarcResFile("FlowerBomb"));
    nw4r::g3d::ResMdl resMdl = resFile.GetResMdl("LeafBomb");
    return mModel.create(resMdl, &mAllocator, 0x120, 1, nullptr);
}

int dAcBombf_c::actorCreate() {
    mDespawnSceneFlag = (mParams >> 8) & 0xFF;
    field_0x3D4 = (mParams >> 16) & 0xF;
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mDespawnSceneFlag)) {
        return FAILED;
    }

    CREATE_ALLOCATOR(dAcBombf_c);

    field_0x3D2 = (mParams >> 4) & 0xF;
    field_0x3D0 = field_0x3D2 == 1;
    if (field_0x3D0) {
        mModel.setPriorityDraw(0x82, 0x7F);
    }

    mStts.SetDefaultRank();
    mAcceleration = -2.0f;
    mMaxSpeed = -80.0f;
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-80.0, -50.0f, -80.0f), mVec3_c(80.0, 60.0f, 80.0f));
    mAngle = mRotation;
    if (mDespawnSceneFlag < 0xFF) {
        unsetActorProperty(AC_PROP_0x1);
        setActorProperty(AC_PROP_0x4);
    }

    return SUCCEEDED;
}

int dAcBombf_c::actorPostCreate() {
    // Preamble problem
    mMtx_c mtx;
    mtx.ZXYrotS(mRotation.x, mRotation.y, mRotation.z);
    mVec3_c v;
    PSMTXMultVecSR(mtx, mVec3_c::Ey, v);
    mVec3_c v3 = mPosition + v * 10.0f;
    mVec3_c v4 = mPosition - v * 10.0f;

    if (dBgS_ObjLinChk::LineCross(&v3, &v4, this)) {
        mPosition = dBgS_ObjLinChk::GetInstance().GetLinEnd();
        if (mRotation.x == 0 && mRotation.z == 0 && dBgS_ObjLinChk::ChkGround()) {
            cM3dGPla pla;
            dBgS::GetInstance()->GetTriPla(dBgS_ObjLinChk::GetInstance(), &pla);
            mRotation.x = pla.GetAngle(mRotation.y);
            mRotation.z = pla.GetAngle(mRotation.y - 0x4000);
        }

        if (dBgS::GetInstance()->ChkMoveBG(dBgS_ObjLinChk::GetInstance(), false)) {
            field_0x398.SetPolyInfo(dBgS_ObjLinChk::GetInstance());
            unsetActorProperty(AC_PROP_0x1);
            setActorProperty(AC_PROP_0x4);
        }
        mLightingInfo.mLightingCode = dBgS::GetInstance()->GetLightingCode(dBgS_ObjLinChk::GetInstance());
    }

    if (field_0x3D2 == 0 || field_0x3D2 == 2) {
        s32 b = dTimeAreaMgr_c::GetInstance()->fn_800B9B60(mRoomID, mPosition);
        if (b != 0) {
            mTimeAreaStruct.field_0x00 = 1.0f;
        }
        if ((b != 0 && field_0x3D2 == 2) || (b == 0 && field_0x3D2 == 0)) {
            mModel.setScale(0.0001f, 0.0001f, 0.0001f);
            if (mBombRef.get() != nullptr) {
                mBombRef.get()->mField_0xA50 = 0.0001f;
            }
        }
    }

    updateMatrix();
    mModel.setLocalMtx(mWorldMtx);
    regrowBomb();
    if (mBombRef.get() != nullptr) {
        mBombRef.get()->mScale.set(0.95f, 0.95f, 0.95f);
    }

    return SUCCEEDED;
}

int dAcBombf_c::doDelete() {
    return SUCCEEDED;
}

int dAcBombf_c::actorExecute() {
    if (field_0x3D3 != 0) {
        mMtx_c &mtx = mWorldMtx;
        mtx.getTranslation(mPosition);
        dAcBomb_c *bomb = mBombRef.get();
        if (bomb != nullptr) {
            bomb->setTransformFromFlower(mtx);
        }
        mModel.setLocalMtx(mWorldMtx);
        mPositionCopy2 = mPosition;
        mPositionCopy3 = mPosition;
        field_0x3D3 = 0;
    } else {
        if (dBgS::GetInstance()->ChkMoveBG(field_0x398, true)) {
            dBgS::GetInstance()->MoveBgTransPos(field_0x398, true, &mPosition, &mAngle, &mRotation);
            updateMatrix();
            dAcBomb_c *bomb = mBombRef.get();
            if (bomb != nullptr) {
                bomb->setTransformFromFlower(mWorldMtx);
            }
            mModel.setLocalMtx(mWorldMtx);
            mPositionCopy2 = mPosition;
            mPositionCopy3 = mPosition;
        }
    }

    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcBombf_c::draw() {
    if (field_0x3D0 != 0 && !dAcPy_c::LINK->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }
    drawModelType1(&mModel);
    return SUCCEEDED;
}

void dAcBombf_c::regrowBomb() {
    // These params are hell
    s8 viewclip_idx = checkActorProperty(dAcBase_c::AC_PROP_0x1) ? mViewClipIdx : -1;
    u32 actorParams1;
    actorParams1 = 1;
    if (field_0x3D0) {
        actorParams1 = 2;
    }
    dAcObjBase_c *ac = dAcObjBase_c::create(
        "Bomb", mRoomID, actorParams1, &mPosition, nullptr, nullptr, 0xFFFFFFFF, 0xFFFF, viewclip_idx
    );
    mBombRef.link(static_cast<dAcBomb_c *>(ac));
    dAcBomb_c *bomb = mBombRef.get();
    if (bomb != nullptr) {
        field_0x394 = 0x3C;
        bomb->setTransformFromFlower(mWorldMtx);
        if (checkActorProperty(dAcBase_c::AC_PROP_0x1)) {
            bomb->setActorProperty(AC_PROP_0x1);
        }
        if (field_0x3D4 == 0) {
            bomb->mField_0xA44 *= 1.5f;
        }
    }
}

void dAcBombf_c::initializeState_Wait() {}

extern "C" u16 lbl_8057A750;

void dAcBombf_c::executeState_Wait() {
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mDespawnSceneFlag)) {
        deleteRequest();
        dAcBomb_c *bomb = mBombRef.get();
        if (bomb != nullptr) {
            bomb->deleteRequest();
            bomb->mEffect1.remove(true);
            bomb->mEffect2.remove(true);
        }
        return;
    }

    f32 scaleFactor = mScale.x;
    if (field_0x3D2 == 0 || field_0x3D2 == 2) {
        mVec3_c up;
        PSMTXMultVecSR(mWorldMtx, mVec3_c::Ey, up);
        mVec3_c upScaled = up * 30.0f;
        mVec3_c checkPos = mPosition + upScaled;

        if (mTimeAreaStruct.check(mRoomID, checkPos, 0, 30.0f, 0.1f) && field_0x3D4 != 1) {
            if (mTimeAreaStruct.field_0x04 == 1) {
                startSound(SE_TIMESLIP_TIMESLIP);
            } else {
                startSound(SE_TIMESLIP_TIMESLIP_REV);
            }
            dJEffManager_c::spawnEffect(lbl_8057A750, mPosition, nullptr, nullptr, nullptr, nullptr, 0, 0);
        }

        scaleFactor *= mTimeAreaStruct.field_0x00;
        if (field_0x3D2 == 2) {
            scaleFactor = 1.0f - scaleFactor;
        }
        scaleFactor = nw4r::ut::Max(scaleFactor, 0.0001f);
        mModel.setScale(scaleFactor, scaleFactor, scaleFactor);
    }
    dAcBomb_c *bomb = mBombRef.get();
    if (bomb != nullptr) {
        if (!bomb->mStateMgr.isState(dAcBomb_c::StateID_FlowerWait) && field_0x3D0 == 0) {
            mBombRef.unlink();
            field_0x394 = 0x3C;
        }
    } else {
        if (field_0x394 != 0) {
            field_0x394--;
        } else if (field_0x3D0 == 0 || dAcPy_c::LINK->getSquareDistanceTo(mPosition) > 22500.0f) {
            regrowBomb();
        }
    }

    bomb = mBombRef.get();
    if (bomb != nullptr) {
        bomb->mField_0xA50 = scaleFactor;
    }
}

void dAcBombf_c::finalizeState_Wait() {}
