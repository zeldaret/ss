#include "d/a/obj/d_a_obj_tower_gearD101.h"

#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/obj/d_a_obj_tower_D101.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/d_stage.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

SPECIAL_ACTOR_PROFILE(OBJ_TOWER_GEAR_D101, dAcOTowerGearD101_c, fProfile::OBJ_TOWER_GEAR_D101, 0x17E, 0, 7);

STATE_DEFINE(dAcOTowerGearD101_c, Wait);

extern "C" void fn_80067340(mVec3_c &, nw4r::g3d::ResMdl *, const char *);

bool dAcOTowerGearD101_c::createHeap() {
    const char *name = "TowerGearD101";
    mRes = nw4r::g3d::ResFile(getOarcResFile(name));
    dStage_c::bindStageResToFile(&mRes);
    nw4r::g3d::ResMdl mdl = mRes.GetResMdl(name);
    if (!mMdl.create(mdl, &mAllocator, 0x120, 1, nullptr)) {
        return false;
    }
    fn_80067340(field_0x3A0, &mdl, "model0");
    mPosition = field_0x3A0;
    mPosition.rotY(mRotation.y);

    return true;
}

int dAcOTowerGearD101_c::actorCreate() {
    if (!initAllocatorWork1Heap(0x1000, "dAcOTowerGearD101_c::m_allocator", 0x20)) {
        return FAILED;
    }
    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    mMdlCallback.init(mdl.GetResNode("model0").GetID());
    mMdl.setCallback(&mMdlCallback);
    mAcceleration = 0.0f;
    mMaxSpeed = 0.0f;
    mStateMgr.changeState(StateID_Wait);

    mVec3_c s = field_0x3AC;
    mBoundingBox.Set(
        mVec3_c(s.x * -200.0f, s.y * -200.0f, s.z * -50.0f), mVec3_c(s.x * 200.0f, s.y * 200.0f, s.z * 50.0f)
    );

    initTransform();
    if (dBgS_WtrChk::CheckPos(&mPosition, true, 500.0f, -500.0f)) {
        field_0x3F4 = mVec3_c::Zero;
        field_0x3F4.y = dBgS_WtrChk::GetWaterHeight();
    } else {
        field_0x3F4 = mVec3_c::Zero;
    }
    mEffects.init(this);
    return SUCCEEDED;
}

int dAcOTowerGearD101_c::actorPostCreate() {
    fBase_c *ac = fManager_c::searchBaseByProfName(fProfile::OBJ_TOWER_D101, nullptr);
    if (ac == nullptr) {
        return NOT_READY;
    }
    setActorRef(static_cast<dAcOTowerD101_c *>(ac));
    return SUCCEEDED;
}

int dAcOTowerGearD101_c::doDelete() {
    return SUCCEEDED;
}

int dAcOTowerGearD101_c::actorExecute() {
    mStateMgr.executeState();
    return SUCCEEDED;
}

int dAcOTowerGearD101_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOTowerGearD101_c::initializeState_Wait() {}
void dAcOTowerGearD101_c::executeState_Wait() {
    dAcOTowerD101_c *tower = static_cast<dAcOTowerD101_c *>(mActorNode.get());
    if (tower != nullptr) {
        f32 diff = tower->mGoalElevation - tower->mCurrentElevation;
        s32 scale5 = 0x50000;
        mAng prevRotation = mCurrRotation;
        mAng newRotation = diff * scale5 / 2400.0f;
        if (newRotation != mCurrRotation) {
            if (mPreviousTurnSpeed == 0) {
                startSound(SE_TGrD101_ROLL_START);
            } else {
                holdSound(SE_TGrD101_ROLL_LV);
                playVisualEffect();
            }
        }
        s32 turnSpeed = newRotation - prevRotation;

        mCurrRotation = newRotation;
        mPreviousTurnSpeed = turnSpeed;
        mMdlCallback.mGearRotation = newRotation;
    }
}
void dAcOTowerGearD101_c::finalizeState_Wait() {}

void dAcOTowerGearD101_callback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *manip, nw4r::g3d::ResMdl mdl) {
    if (nodeId == mNodeId) {
        mMtx_c mtx1;
        manip->GetMtx(mtx1);
        mtx1.ZrotM(mGearRotation);
        manip->SetMtx(mtx1);
    }
}

void dAcOTowerGearD101_c::initTransform() {
    mWorldMtx.transS(mVec3_c::Zero);
    mMtx_c mtx1;
    mtx1.transS(mPosition);
    mWorldMtx += mtx1;
    field_0x3C4.transS(mVec3_c::Zero);
    field_0x3C4.ZXYrotM(mRotation.x, mRotation.y, mRotation.z);
    mMdl.setLocalMtx(field_0x3C4);
}

void dAcOTowerGearD101_c::playVisualEffect() {
    mEffects.createContinuousEffect(
        PARTICLE_RESOURCE_ID_MAPPING_572_, field_0x3F4, &mRotation, nullptr, nullptr, nullptr
    );
}
