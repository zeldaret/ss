#include "d/a/obj/d_a_obj_sw_sword_beam.h"

#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_player_act.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/math/eggMatrix.h"
#include "egg/math/eggQuat.h"
#include "f/f_base.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resanmtexsrt.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

const s16 dAcOSwSwordBeam_c::lbl_507_data_0 = 0;

SPECIAL_ACTOR_PROFILE(OBJ_SW_SWORD_BEAM, dAcOSwSwordBeam_c, fProfile::OBJ_SW_SWORD_BEAM, 0x1A2, 0, 3);

STATE_DEFINE(dAcOSwSwordBeam_c, Wait);
STATE_DEFINE(dAcOSwSwordBeam_c, Rotate);
STATE_DEFINE(dAcOSwSwordBeam_c, RotateEnd);
STATE_DEFINE(dAcOSwSwordBeam_c, OnSwitch);
STATE_DEFINE(dAcOSwSwordBeam_c, End);

// clang-format off
dCcD_SrcUnk dAcOSwSwordBeam_c::sCcSrc = {
    {{0, 0, {0, 0, 0}, 0, 0, 0, 0, 0},
    {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_GLITTERING_SPORES | AT_TYPE_0x8000), 0x111, 0x0, 0x06, 0x40F, 0, },
    {0xe9}},
    {-100.0f, -50.0f, -12.5f, 100.f, 50.f, 12.5f}};
// clang-format on

bool dAcOSwSwordBeam_c::createHeap() {
    void *data = getOarcResFile("GoddessSymbolSc");
    mResFile = nw4r::g3d::ResFile(data);
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("GoddessSymbolSc");
    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x327));
    nw4r::g3d::ResAnmTexPat anmTexPat = mResFile.GetResAnmTexPat("GoddessSymbolSc");
    TRY_CREATE(mAnmTexPat.create(mdl, anmTexPat, &mAllocator, nullptr, 1));
    nw4r::g3d::ResAnmTexSrt anmTexSrt = mResFile.GetResAnmTexSrt("GoddessSymbolSc");
    mAnmTexSrt.create(mdl, anmTexSrt, &mAllocator, nullptr, 1);
}

int dAcOSwSwordBeam_c::create() {
    mSubtype = getFromParams(8, 0xF);
    if (mSubtype >= 2) {
        mSubtype = 0;
    }
    CREATE_ALLOCATOR(dAcOSwSwordBeam_c);
    mStts.SetRank(0xD);
    mUnk1.Set(sCcSrc);
    mUnk1.SetStts(mStts);
    mUnk1.SetTg_0x4C(~0x800000);
    mUnk2.Set(sCcSrc);
    mUnk2.SetStts(mStts);
    mUnk2.SetTg_0x4C(~0x800000);
    mUnk2.SetTgType(0x800002);
    mUnk2.mCo.OffSPrm(1);
    mUnk2.Set(mVec3_c(-100.f, 10.f, -150.f), mVec3_c(100.f, 120.f, 150.f));
    mAcceleration = -1.f;
    mMaxSpeed = -40.f;
    field_0xC88 = 0;
    mSceneflag = getFromParams(0, 0xFF);
    field_0xCA0 = cM::rndInt(0x46) * lbl_507_data_50;
    field_0xCA2 = lbl_507_data_50;
    field_0xC9A = cM::rndInt(0x46) * lbl_507_data_50;
    field_0xC68.set(1.f, 0.f, 0.f, 0.f);
    field_0xC78.set(1.f, 0.f, 0.f, 0.f);
    field_0xC58.set(1.f, 0.f, 0.f, 0.f);
    mMdl.setAnm(mAnmTexSrt);
    mMdl.setAnm(mAnmTexPat);
    mAnmTexPat.setRate(0.f, 0);
    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(100.f, 50.f);
    mObjAcch.CrrPos(*dBgS::GetInstance());
    if (checkSceneFlag()) {
        mStateMgr.changeState(StateID_End);
    } else {
        mStateMgr.changeState(StateID_Wait);
    }
    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mUnk1.Set(min, max);
    mTargetFiTextID = 0x206;
    mBoundingBox.Set(mVec3_c(-100.f, -50.f, -50.f), mVec3_c(100.f, 55.f, 50.f));
    return SUCCEEDED;
}

int dAcOSwSwordBeam_c::doDelete() {
    return SUCCEEDED;
}

int dAcOSwSwordBeam_c::actorExecute() {
    mStateMgr.executeState();
    mAnmTexPat.play();
    mAnmTexSrt.play();
    fn_507_2130();
    mUnk1.Set(mPosition + mVec3_c::Ey * (field_0xC88 + 75.f), mRotation.y);
    dCcS::GetInstance()->Set(&mUnk1);
    if (mSpawnedFromOtherActor && mStateMgr.isState(StateID_Wait)) {
        mUnk2.Set(mVec3_c(-100.f, 10.f, -150.f), mVec3_c(100.f, 120.f, mSomeFloatFromOtherActor + 150.f));
        mUnk2.Set(mPosition, mRotation.y);
        dCcS::GetInstance()->Set(&mUnk2);
    }
    fn_507_1F80();
    mPositionCopy3 = mPosition;
    mPositionCopy3.y += 100.f;
    if (mSpawnedFromOtherActor && mSomeFloatFromOtherActor <= 0.f) {
        mPositionCopy3.y += 100.f;
    }
    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 75.f;
    if (!checkSceneFlag()) {
        // TODO fake
        AttentionManager *attMan = AttentionManager::GetInstance();
        float a = mSomeFloatFromOtherActor + 800.f;
        float b = 100.f;
        float c = -500.f;
        float d = 500.f;
        attMan->addUnk7Target(*this, mSpawnedFromOtherActor ? 1 : 3, a, b, c, d);
    }
    return SUCCEEDED;
}

int dAcOSwSwordBeam_c::actorExecuteInEvent() {
    if (mEvent.isThisActorInEvent()) {
        switch (mEvent.getCurrentEventCommand()) {
            case 'act0': {
                mStateMgr.executeState();
                mAnmTexPat.play();
                mAnmTexSrt.play();
                if (mStateMgr.isState(StateID_End)) {
                    if (mSubtype == 0) {
                        mEvent.advanceNext();
                    } else if (mCsExitID >= 2) {
                        dScGame_c::GetInstance()->triggerExit(mRoomID, (s32)mCsExitID);
                    } else {
                        mEvent.advanceNext();
                    }
                }
                break;
            }
            case '????': {
                mEvent.advanceNext();
                break;
            }
            default: {
                mEvent.advanceNext();
                break;
            }
        }
    } else if (field_0xCAA) {
        mColor color = mColor(field_0xC90, field_0xC90, field_0xC90, 0xFF);
        mMdl.setTevKColorAll(GX_KCOLOR3, color, false);
        mLightingInfo.mTevK3Color = mColor(field_0xC90, field_0xC90, field_0xC90, 0xFF);
        mLightingInfo.mUseTevK3 = true;
        field_0xCAA = false;
    }

    fn_507_2130();
    fn_507_1F80();
    return SUCCEEDED;
}

void dAcOSwSwordBeam_c::registerInEvent() {
    if (EventManager::sInstance->isCurrentEvent("SwSwordBeam")) {
        dAcPy_c *link = dAcPy_c::LINK;
        mAng angle = link->mRotation.y;
        link->setObjectProperty(OBJ_PROP_0x200);

        if (mSpawnedFromOtherActor && mSomeFloatFromOtherActor <= 0.f) {
            angle = getXZAngleToPlayer() - 0x8000;
        }
        link->setPosYRot(nullptr, angle, false, 0, 0);
        field_0xCAC = true;
    }
    if (mSpawnedFromOtherActor && mSomeFloatFromOtherActor <= 0.f) {
        mPositionCopy3.y = mPosition.y + 100.f;
    }
}

void dAcOSwSwordBeam_c::unkVirtFunc_0x6C() {
    dAcPy_c *link = dAcPy_c::LINK;
    if (field_0xCAC && getSquareDistToPlayer() < 160000.f) {
        mVec3_c a = mVec3_c::Ez * 400.f;
        a.rotY(mRotation.y);
        a += mPosition + (mVec3_c::Ey * 100.f);
        if (dBgS_ObjGndChk::CheckPos(a)) {
            a.y = dBgS_ObjGndChk::GetGroundHeight();
        }
        mAng c = mRotation.y - 0x8000;
        link->setPosYRot(&a, c, false, 0, 0);
    }
    field_0xCAC = false;
    if (link->checkObjectProperty(OBJ_PROP_0x200)) {
        link->unsetObjectProperty(OBJ_PROP_0x200);
    }
}

const s16 dAcOSwSwordBeam_c::lbl_507_data_50 = 0x38E;

int dAcOSwSwordBeam_c::draw() {
    drawModelType1(&mMdl);
    static mQuat_c rot(0.f, 0.f, 0.f, 10.f);
    drawShadow(mShadow, nullptr, mWorldMtx, &rot, -1, -1, -1, -1, -1, 0.f);
    return SUCCEEDED;
}

void dAcOSwSwordBeam_c::initializeState_Wait() {
    mAnmTexPat.setFrame(0.f, 0);
    field_0xCAB = false;
}

void dAcOSwSwordBeam_c::executeState_Wait() {
    if (mUnk1.ChkTgHit()) {
        if (!mUnk1.ChkTgAtHitType(AT_TYPE_SWORD) || !mUnk1.ChkTgBit25()) {
            if (!mUnk1.ChkTgAtHitType(AT_TYPE_0x800000)) {
                goto LAB_80e4e72c;
            }
        }
        if (mSubtype != 0) {
            if (!isleOfSongsCanGetHit()) {
                goto LAB_80e4e72c;
            }
        }
        if (!field_0xCAB) {
            field_0xCAB = true;
        }
    }
LAB_80e4e72c:
    if (mUnk1.ChkTgHit()) {
        field_0xC96.mVal += lbl_507_data_50;
    }
    if (mSpawnedFromOtherActor && !field_0xCAB && mUnk2.ChkTgHit()) {
        if (mUnk2.ChkTgAtHitType(AT_TYPE_0x800000) || (mUnk2.ChkTgAtHitType(AT_TYPE_SWORD) && mUnk2.ChkTgBit25())) {
            field_0xCAB = true;
        }
    }
    if (field_0xCAB) {
        fn_507_22A0();
    }
    if (mEvent.isInEvent("SwSwordBeam")) {
        mStateMgr.changeState(StateID_Rotate);
    }
}

void dAcOSwSwordBeam_c::finalizeState_Wait() {
    field_0xC96.mVal = 0;
    field_0xCA8 = fn_507_1F30();
}

void dAcOSwSwordBeam_c::initializeState_Rotate() {
    mAnmTexPat.setFrame(1.f, 0);
    field_0xC9E = field_0xCA8 ? 0x3333 : -0x3333;
    field_0xCA6 = 0x19;
    startSound(SE_SwSB_REACT_01);
    startSound(SE_SwSB_REACT_02);
}

void dAcOSwSwordBeam_c::executeState_Rotate() {
    if (sLib::calcTimer(&field_0xCA6) == 0) {
        mStateMgr.changeState(StateID_End);
    } else {
        fn_507_1A50();
    }
}

void dAcOSwSwordBeam_c::finalizeState_Rotate() {}
void dAcOSwSwordBeam_c::initializeState_RotateEnd() {}

void dAcOSwSwordBeam_c::executeState_RotateEnd() {
    bool b = false;
    if (sLib::absDiff(field_0xC9E, 0) < b) {
        field_0xC9E = 0;
        b = sLib::addCalcAngle2(field_0xC9C.ref(), 0, field_0xCA8 ? 0xf : -0xf, b, 0x20) == 0;
    } else {
        sLib::addCalcAngle(&field_0xC9E, 0, 0x14, b, 0x20);
    }
    if (b) {
        mStateMgr.changeState(StateID_OnSwitch);
    } else {
        fn_507_1A50();
    }
}

void dAcOSwSwordBeam_c::finalizeState_RotateEnd() {
    if (mSubtype == 0) {
        if (mSceneflag < 0xff && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, mSceneflag);
        }
    } else if (mSubtype == 1) {
        s32 sword = getEquippedSword();
        bool checkSceneFlag = false;
        u8 sceneflag = mSceneflag;
        switch (sword) {
            case daPlayerActBase_c::GODDESS_SWORD: {
                mCsExitID = 2;
                checkSceneFlag = true;
                break;
            }
            case daPlayerActBase_c::LONG_SWORD: {
                mCsExitID = 3;
                checkSceneFlag = true;
                sceneflag++;
                break;
            }
            case daPlayerActBase_c::WHITE_SWORD: {
                mCsExitID = 4;
                checkSceneFlag = true;
                sceneflag += 2;
                break;
            }
        }
        if (checkSceneFlag && sceneflag < 0xff && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, sceneflag)) {
            SceneflagManager::sInstance->setFlag(mRoomID, sceneflag);
        }
    }
}

s32 dAcOSwSwordBeam_c::getEquippedSword() {
    return daPlayerActBase_c::sCurrentSword;
}

void dAcOSwSwordBeam_c::initializeState_OnSwitch() {
    mTimer = 0x3c;
    field_0xC96.mVal = 0;
    field_0xC94.mVal = 0;
    field_0xC98.mVal = 0;
    mMdl.setTevKColorAll(GX_KCOLOR3, nw4r::ut::Color(0x40, 0xc0, 0xff, 0xff), false);
    dJEffManager_c::spawnEffect(
        PARTICLE_RESOURCE_ID_MAPPING_352_, mPosition + (mVec3_c::Ey * (field_0xC88 + 75)), &mRotation, &mScale, nullptr,
        nullptr, 0, 0
    );
    startSound(SE_SwSB_REACT_FIN);
}

void dAcOSwSwordBeam_c::executeState_OnSwitch() {
    if (sLib::calcTimer(&mTimer) != 0) {
        f32 multiplier = mTimer / 60.f;
        mMdl.setTevKColorAll(
            GX_KCOLOR3, nw4r::ut::Color(multiplier * 64.f, multiplier * 192.f, multiplier * 255.f, 0xff), false
        );
    } else {
        mStateMgr.changeState(StateID_End);
    }
}

void dAcOSwSwordBeam_c::finalizeState_OnSwitch() {}

void dAcOSwSwordBeam_c::initializeState_End() {
    mAnmTexPat.setFrame(1.f, 0);
    mUnk1.mTg.Set_0x4C(0xfeb77dff);
}

void dAcOSwSwordBeam_c::executeState_End() {
    if (mUnk1.ChkTgHit()) {
        field_0xC96.mVal += lbl_507_data_50;
    }
    if (!EventManager::sInstance->isInEvent() && mSubtype == 1) {
        u16 flag = mSceneflag + 2;
        if (flag >= 0xff || !SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag)) {
            mStateMgr.changeState(StateID_Wait);
        }
    }
}

void dAcOSwSwordBeam_c::finalizeState_End() {}

void dAcOSwSwordBeam_c::fn_507_1A50() {
    field_0xC9C += field_0xC9E;
    mQuat_c quat;
    quat.setAxisRotation(mVec3_c::Ey, mAng::s2r(field_0xC9C));
    field_0xC68 = quat;
}

void dAcOSwSwordBeam_c::fn_507_1AF0() {
    field_0xC9A += field_0xCA2 * 0.5f;
    field_0xC94 = field_0xC98 * .9f;
    field_0xC96 += field_0xC94;
    field_0xC96 *= 0.7f;
    field_0xC98 += field_0xC96;
    f32 cos = nw4r::math::CosIdx(field_0xCA0);
    mQuat_c quat;
    quat.setAxisRotation(mVec3_c::Ey, mAng::s2r(cos * 546.f));
    f32 sin = nw4r::math::SinIdx(field_0xC9A);
    mQuat_c quat2;
    quat2.setAxisRotation(mVec3_c::Ez, mAng::s2r(sin * 546.f));
    mQuat_c quat3;
    quat3.setAxisRotation(mVec3_c::Ez, mAng::s2r(field_0xC98));
    field_0xC78 = quat2 * quat3 * quat;
}

bool dAcOSwSwordBeam_c::fn_507_1F30() {
    return dAcPy_c::GetLink()->vt_0x258() > 0 ? -1 : 0;
}

void dAcOSwSwordBeam_c::fn_507_1F80() {
    updateMatrix();
    EGG::Quatf a = field_0xC68 * field_0xC78;
    mMtx_c mtx1;
    mtx1.fromQuat(a);
    mMtx_c mtx2;
    f32 temp = field_0xC88 + 75.f;
    PSMTXTrans(mtx2, 0.f, temp, 0.f);
    PSMTXConcat(mWorldMtx.m, mtx2.m, mWorldMtx.m);
    PSMTXConcat(mWorldMtx.m, mtx1.m, mWorldMtx.m);
    mMdl.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);
    mMdl.calc(false);
}

void dAcOSwSwordBeam_c::fn_507_2130() {
    sLib::addCalcScaledDiff(&field_0xC88, 5.f * field_0xCA0.sin() * mScale.y, 0.2f, 1.f);
    field_0xCA0 += field_0xCA2;
    bool a = true;
    if (!mStateMgr.isState(StateID_Wait) && !mStateMgr.isState(StateID_End)) {
        a = false;
    }
    if (a) {
        if (!EventManager::sInstance->isInEvent()) {
            fn_507_1AF0();
            return;
        }
    }
    field_0xC78.slerpTo(field_0xC58, 0.5f, field_0xC78);
    field_0xC9A = lbl_507_data_0;
}

static volatile u32 FLAGS_1 = 0x00000001;
static u32 FLAGS_2 = 0x00100001;

void dAcOSwSwordBeam_c::fn_507_22A0() {
    u32 f1 = ~FLAGS_1;
    u32 f2 = FLAGS_2;

    Event event("SwSwordBeam", getOarcZev("GoddessSymbolSc"), 100, f2 & f1, nullptr, nullptr);
    mEvent.scheduleEvent(event, 0);
}

// TODO fake
bool dAcOSwSwordBeam_c::checkSceneFlag() {
    bool subtype0andFlagTrue = false;
    bool ret = true;
    if (mSubtype == 0) {
        bool tmp = (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag));
        if (tmp) {
            subtype0andFlagTrue = true;
        }
    }

    if (!subtype0andFlagTrue) {
        bool subtype0andFlagTrue = false;
        if (mSubtype == 1) {
            u16 flag = mSceneflag + 2;
            bool tmp = (flag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag));
            if (tmp) {
                subtype0andFlagTrue = true;
            }
        }
        if (!subtype0andFlagTrue) {
            ret = false;
        }
    }
    return ret;

    // functionally equivalent to
    // switch (mSubtype) {
    //     case 0: {
    //         return (mSceneflag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag));
    //     }
    //     case 1: {
    //         u16 flag = mSceneflag + 2;
    //         return (flag < 0xFF && SceneflagManager::sInstance->checkBoolFlag(mRoomID, flag));
    //     }
    //     default: return false;
    // }
}

bool dAcOSwSwordBeam_c::isleOfSongsCanGetHit() {
    switch (getEquippedSword()) {
        case daPlayerActBase_c::GODDESS_SWORD:
            return mSceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSceneflag);
        case daPlayerActBase_c::LONG_SWORD: {
            u16 sceneflag = mSceneflag + 1;
            return sceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, sceneflag);
        }
        case daPlayerActBase_c::WHITE_SWORD: {
            u16 sceneflag = mSceneflag + 2;
            return sceneflag < 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, sceneflag);
        }
        default: return false;
    }
}

void dAcGoddessCrestHolder_c::setCrestAtBone(char *bone_name, m3d::smdl_c *model, u8 *sceneflag) {
    nw4r::g3d::ResMdl mdl = model->getResMdl();
    nw4r::g3d::ResNode bone = mdl.GetResNode(bone_name);
    nw4r::g3d::ResNode parentBone = bone.GetParentNode();
    // mVec3_c(bone.ref().scale) * mVec3_c(parentBone.ref().scale);
    mSwScale.x = bone.ref().scale.x * parentBone.ref().scale.x;
    mSwScale.y = bone.ref().scale.y * parentBone.ref().scale.y;
    mSwScale.z = bone.ref().scale.z * parentBone.ref().scale.z;
    // mSwScale = mVec3_c(
    //     bone.ref().scale.x * parentBone.ref().scale.x, bone.ref().scale.y * parentBone.ref().scale.y,
    //     bone.ref().scale.z * parentBone.ref().scale.z
    // );

    mBoneID = bone.GetID();
    model->getNodeWorldMtxMultVecZero(mBoneID, mSwPos);

    nw4r::math::VEC3 boneRot = bone.ref().rot;
    nw4r::math::VEC3 parentRot = parentBone.ref().rot;

    // nw4r::math::VEC3 rot = boneRot + parentRot;

    mSwRot.x = mAng::fromDeg(boneRot.x + parentRot.x);
    mSwRot.y = mAng::fromDeg(boneRot.y + parentRot.y);
    mSwRot.z = mAng::fromDeg(boneRot.z + parentRot.z);

    dAcOSwSwordBeam_c *swSwordBeam = (dAcOSwSwordBeam_c *)dAcObjBase_c::create(
        fProfile::OBJ_SW_SWORD_BEAM, mRoomID, *sceneflag, &mSwPos, &mSwRot, &mSwScale, -1
    );
    if (swSwordBeam != nullptr) {
        mSwSwordBeamRef.link(swSwordBeam);
        if (mTransferToCrest) {
            swSwordBeam->setSpawnedFromOtherActor(true);
            swSwordBeam->setSomeFloatFromOtherActor(mSendToSwSB);
        }
    }
}

void dAcGoddessCrestHolder_c::setCrestPosRot(m3d::smdl_c *mdl) {
    dAcOSwSwordBeam_c *swordBeam = (dAcOSwSwordBeam_c *)mSwSwordBeamRef.get();
    mdl->getNodeWorldMtxMultVecZero(mBoneID, mSwPos);
    swordBeam->mPosition = mSwPos;
    swordBeam->mRotation.set(mSwRot);
}

void dAcGoddessCrestHolder_c::vt_0x88(f32 &param) {}
