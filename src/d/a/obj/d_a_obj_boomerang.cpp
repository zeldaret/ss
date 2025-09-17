#include "d/a/obj/d_a_obj_boomerang.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_pad.h"
#include "d/d_player_act.h"
#include "d/d_rumble.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_fanm.h"
#include "m/m_color.h"
#include "m/m_fader_base.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

dBgS_BeetleLinChk dAcBoomerang_c::sLinChk;
dCcD_SrcSph dAcBoomerang_c::sSphSrc = {
    {{AT_TYPE_BEETLE, BoomerangAtFlags, {4, 0, 0}, 0, 1, 0, 0, 0},
     {~AT_TYPE_COMMON0, 0x291, {0, 0, 0x407}, 0, 0},
     {0x28}},
    {15.f}
};
const u32 dAcBoomerang_c::BoomerangAtFlags = 0x209B;
;

SPECIAL_ACTOR_PROFILE(BOOMERANG, dAcBoomerang_c, fProfile::BOOMERANG, 0x125, 0, 0x4);

STATE_DEFINE(dAcBoomerang_c, Wait);
STATE_DEFINE(dAcBoomerang_c, Move);
STATE_DEFINE(dAcBoomerang_c, MoveCancelWait);
STATE_DEFINE(dAcBoomerang_c, ReturnWait);
STATE_DEFINE(dAcBoomerang_c, EventReturnWait);

void dAcBoomerang_areaCallback(dAcObjBase_c *param0, mVec3_c *param1, u32 param2) {
    if (param2 == 0) {
        static_cast<dAcBoomerang_c *>(param0)->areaCallback(param1, param2);
    }
    return;
}
// This function is odd. only called by the areaCallback
void dAcBoomerang_c::areaCallback(mVec3_c *param1, u32 param2) {
    setChrAnimation(dAcBoomerang_c::RB_CUT);
}
void dAcBoomerang_c::atHitCallback(cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB) {
    if (i_actorB != nullptr && i_actorB->GetLinkage().checkFlag(0x80)) {
        // Check if beetle can grab things
        if (dAcPy_c::hasBeetleVariantOrBetter(HOOK_BEETLE)) {
            // If beetle is already holding the object or is stationary
            if (i_actorB == mGrabbedActor.get() || !mStateMgr.isState(StateID_Move)) {
                return;
            }

            // Try grabbing the object
            if (tryGrabObject(i_actorB)) {
                setChrAnimation(RB_HOLD);
                mAnmChr[BOOMERANG_ANIM_PINCERS].setRate(0.f);
                mAnmChr[BOOMERANG_ANIM_PINCERS].setFrameOnly(i_actorB->GetLinkage().field_0x24);
                return;
            }
        }
    }

    if (mStateMgr.isState(StateID_MoveCancelWait)) {
        return;
    }

    if (!i_objInfB->ChkTgBonk()) {
        field_0x8D8 = mPosition - i_actorB->mPosition;
        field_0x8D8.y = 0.f;

        if (cM::isZero(field_0x8D8.normalize())) {
            field_0x8D8.set(-angle.y.sin(), 0.f, -angle.y.cos());
        }
        field_0x8D8 *= velocity.absXZ();
        field_0x8D8.y = -velocity.y;

        field_0x8D8.normalize();

        setField_0x8CC(FLAG_CANCEL);
        mStateMgr.changeState(StateID_MoveCancelWait);
    } else {
        // Play the animation to move the pincers
        setChrAnimation(RB_CUT);

        // I think this plays the vibration and drops the object
        bonk();
    }
}

bool dAcBoomerang_atHitCallback(
    dAcObjBase_c *i_actorA, cCcD_Obj *i_objInfA, dAcObjBase_c *i_actorB, cCcD_Obj *i_objInfB
) {
    static_cast<dAcBoomerang_c *>(i_actorA)->atHitCallback(i_objInfA, i_actorB, i_objInfB);
    return true;
}

void dAcBoomerang_c::hideModel(dAcBoomerang_c::ModelType_e type) {
    // Implicity ModelType_e -> int conversion
    nw4r::g3d::ResShp obj(mMdl.getResMdl().GetResShp(type));
    obj.SetVisibility(false);
    obj.DCStore(false);
}

bool dAcBoomerang_c::createHeap() {
    mResFile = dAcPy_c::GetLink2()->getHeldResFile();
    nw4r::g3d::ResMdl mdl = mResFile.GetResMdl("EquipBeetle");

    TRY_CREATE(mMdl.create(mdl, &heap_allocator, 0x120, 1));

    // Decide the Pincers
    if (dAcPy_c::hasBeetleVariantOrBetter(HOOK_BEETLE)) {
        hideModel(MDL_MOUTH_BASIC);
    } else {
        hideModel(MDL_MOUTH_HOOK);
    }

    // Decide the Body
    if (dAcPy_c::hasBeetleVariantOrBetter(TOUGH_BEETLE)) {
        hideModel(MDL_BODY_BASIC);
    } else {
        hideModel(MDL_BODY_ADV);
    }

    // Decide the Wings
    if (dAcPy_c::hasBeetleVariantOrBetter(QUICK_BEETLE)) {
        hideModel(MDL_WINGS_BASIC);
    } else {
        hideModel(MDL_WINGS_ADV);
    }

    mLeftWingNodeID = mdl.GetResNode("wing_L").GetID();
    mRightWingNodeID = mdl.GetResNode("wing_R").GetID();

    if (mLeftWingNodeID > mRightWingNodeID) {
        mWindNodeID = mRightWingNodeID;
    } else {
        mWindNodeID = mLeftWingNodeID;
    }

    TRY_CREATE(mAnmChrBlend.create(mdl, 2, &heap_allocator));

    m3d::anmChr_c *pAnmChr = mAnmChr;
    nw4r::g3d::AnmObjChr *pAnimChr;
    nw4r::g3d::ResAnmChr resAnmChr = mResFile.GetResAnmChr("RB_Set");
    for (s32 i = 0; i < 2; ++i, ++pAnmChr) {
        TRY_CREATE(pAnmChr->create2(mdl, resAnmChr, &heap_allocator));

        pAnmChr->setAnm(mMdl, resAnmChr, m3d::PLAY_MODE_0);
        if (i == BOOMERANG_ANIM_WINGS) {
            pAnimChr = static_cast<nw4r::g3d::AnmObjChr *>(pAnmChr->getAnimObj());
            pAnimChr->Release();
            pAnimChr->Bind(mdl, mLeftWingNodeID, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
            pAnimChr->Bind(mdl, mRightWingNodeID, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
        }
        mAnmChrBlend.attach(i, pAnmChr, 1.f);
    }
    mMdl.setAnm(mAnmChrBlend);
    TRY_CREATE(mProc.create2(&mMdl, mColor(0x00, 0x10, 0x14, 0xFF), 0x27, &heap_allocator));
    if (mLytFader.init()) {
        return true;
    }
    return false;
}

int dAcBoomerang_c::create() {
    CREATE_ALLOCATOR(dAcBoomerang_c);

    mAcch.Set(this, 1, &mAcchCir);
    mAcch.SetGroundUpY(20.f);
    mAcch.SetField_0xD4(30.f);
    mAcch.ClrRoofNone();
    mAcch.SetBeetle();
    mAcch.OnWaterGrp();
    mAcch.OnLineCheck();
    mAcch.GetRoof().SetField_0x7C(1);
    mAcch.Set_0x2000000();
    mAcch.Set_0x40000000();
    mAcch.SetLineDown();

    mAcchCir.SetWall(0.f, dAcPy_c::GetLink2()->getBeetleSmallRadius());

    mCurrentAnimation = RB_MAX;
    mStateMgr.changeState(StateID_Wait);

    placeOnArm();

    mStts.SetRank(3);

    mSph0.Set(sSphSrc);
    mSph0.SetStts(mStts);
    mSph0.SetAtCallback(dAcBoomerang_atHitCallback);
    mSph0.ClrTgSet();

    mSph1.Set(sSphSrc);
    mSph1.SetStts(mStts);
    mSph1.SetAtCallback(dAcBoomerang_atHitCallback);
    mSph1.SetR(dAcPy_c::GetLink2()->getBeetleSmallRadius());
    mSph1.ClrCoSet();
    mSph1.SetAtType(AT_TYPE_0x40);

    mSph1.SetCoGrp(0xE);

    if (!dAcPy_c::hasBeetleVariantOrBetter(HOOK_BEETLE)) {
        mSph0.OnAt_0x4000();
        mSph1.OnAt_0x4000();
    }

    sLinChk.SetPreRoofChk(false);
    mAcch.CrrPos(*dBgS::GetInstance());
    setRoomId();
    mLytFader.setStatus(mFaderBase_c::FADED_IN);
    mFader.setFader(&mLytFader);

    return SUCCEEDED;
}

dAcBoomerang_c::~dAcBoomerang_c() {
    deleteCheck();
}

void dAcBoomerang_c::deleteReady() {
    deleteCheck();

    if (getSoundSource()) {
        getSoundSource()->stopAllSound(5);
    }

    if (dAcPy_c::GetLink2()) {
        dAcPy_c::GetLink2()->onModelUpdateFlag(0x10000000);
    }
}

void dAcBoomerang_c::retrieve() {
    if (mFader.isStatus(mFaderBase_c::FADED_OUT)) {
        mFader.setFrame(5);
        mFader.fadeIn();
        startSound(SE_BE_COME_BACK);
    }
    mFader.calc();
}

int dAcBoomerang_c::doDelete() {
    return SUCCEEDED;
}

void dAcBoomerang_c::setRoomId() {
    if (mAcch.GetGroundH() != 1e-9f) {
        roomid = dBgS::GetInstance()->GetRoomId(mAcch.GetGnd());
    } else if (roomid == -1) {
        roomid = dAcPy_c::GetLink2()->roomid;
    }
}

void dAcBoomerang_c::placeOnArm() {
    dAcPy_c::GetLink2()->getBodyMtx(&mWorldMtx, dAcPy_c::PLAYER_MAIN_NODE_ARM_R2);
    mMtx_c m;
    m.transS(18.5f, 0.f, 14.5f);
    mWorldMtx += m;
    mWorldMtx.ZYXrotM(mAng::fromDeg(90.f), 0, mAng::fromDeg(90.f));
    mMdl.setLocalMtx(mWorldMtx);
    mWorldMtx.getTranslation(mPosition);
    mWorldMtx.getTranslation(mOldPosition);
}

const dAcBoomerang_c::ChrAnimation_t dAcBoomerang_c::sChrAnims[dAcBoomerang_c::RB_MAX] = {
    {    "RB_Set", 0.f},
    {"RB_Default", 3.f},
    {    "RB_Cut", 3.f},
    {   "RB_Hold", 3.f},
    {"RB_Hold_ed", 3.f},
    {   "RB_Back", 3.f},
};

// ...

bool dAcBoomerang_c::tryGrabObject(dAcObjBase_c *pObj) {
    dAcPy_c *player = dAcPy_c::GetLink2();
    if (checkField_0x8CC(FLAG_CONTROLLABLE) && GetLinkage().checkFlag(0x80)) {}
}

// ...

void dAcBoomerang_c::bonk() {
    if (checkField_0x8CC(FLAG_RUMBLE_ACTIVE)) {
        return;
    }
    setField_0x8CC(FLAG_RUMBLE_ACTIVE);
    dRumble_c::start(dRumble_c::sRumblePreset1, 1);
}

void dAcBoomerang_c::setChrAnimation(dAcBoomerang_c::ChrAnimation_e requestedAnimation) {
    // Do not set anim if there is no change, or the request is to cut from hold
    if (requestedAnimation == mCurrentAnimation || (mCurrentAnimation == RB_HOLD && requestedAnimation == RB_CUT)) {
        return;
    }

    mAnmChr[BOOMERANG_ANIM_PINCERS].setAnm(
        mMdl, mResFile.GetResAnmChr(sChrAnims[requestedAnimation].mName), m3d::PLAY_MODE_4
    );
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChr[BOOMERANG_ANIM_PINCERS].getAnimObj());

    pAnmObj->Release();

    nw4r::g3d::ResMdl mdl = mMdl.getResMdl();
    for (s32 i = 0; i < mWindNodeID; ++i) {
        pAnmObj->Bind(mdl, i, nw4r::g3d::AnmObjChr::BIND_ONE);
    }

    if (requestedAnimation == RB_SET) {
        pAnmObj->Bind(mdl, mLeftWingNodeID, nw4r::g3d::AnmObjChr::BIND_ONE);
        pAnmObj->Bind(mdl, mRightWingNodeID, nw4r::g3d::AnmObjChr::BIND_ONE);
    }

    mCurrentAnimation = requestedAnimation;

    mMdl.setAnm(mAnmChrBlend, sChrAnims[requestedAnimation].mRate);
}

// HELP: I need this to be const to fix the load, but I cant do that without ruining sdata
const char *dAcBoomerang_c::sFlyChrAnims[RB_FLY_MAX] = {"RB_Fly", "RB_FlyFast"};
void dAcBoomerang_c::setFlyChrAnimation(FlyAnimation_e requestedAnimation) {
    nw4r::g3d::ResAnmChr resAnmChr = mResFile.GetResAnmChr(sFlyChrAnims[requestedAnimation]);
    mAnmChr[BOOMERANG_ANIM_WINGS].setAnm(mMdl, resAnmChr, m3d::PLAY_MODE_4);
    nw4r::g3d::AnmObjChr *pAnmObj = static_cast<nw4r::g3d::AnmObjChr *>(mAnmChr[BOOMERANG_ANIM_WINGS].getAnimObj());
    pAnmObj->Release();
    pAnmObj->Bind(mMdl.getResMdl(), mLeftWingNodeID, nw4r::g3d::AnmObjChr::BIND_PARTIAL);
    pAnmObj->Bind(mMdl.getResMdl(), mRightWingNodeID, nw4r::g3d::AnmObjChr::BIND_PARTIAL);

    mMdl.setAnm(mAnmChrBlend);
}

void dAcBoomerang_c::setRemainingFlightTime(s16 time) {
    if (time < 0) {
        time = dAcPy_c::GetLink2()->getBeetleWarningTimeLeft();
    }
    if (mRemainingFlightTime > time) {
        mRemainingFlightTime = time;
        mFlashTimer = time;
    }
}

void dAcBoomerang_c::initializeState_Wait() {
    setChrAnimation(RB_SET);
    mAnmChrBlend.setWeight(1, 0.f);
    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();
    unsetField_0x8CC(FLAG_COMMON_INIT);

    placeOnArm();
    field_0x8D0 = 0.f;

    dAcPy_c::GetLink2()->onModelUpdateFlag(dAcPy_c::UPDATE_MODEL_BEETLE);
}
void dAcBoomerang_c::executeState_Wait() {
    forwardSpeed = 0.f;
    placeOnArm();
    if (checkField_0x8CC(FLAG_REQUEST_MOVE)) {
        mStateMgr.changeState(StateID_Move);
    }
}
void dAcBoomerang_c::finalizeState_Wait() {}

void dAcBoomerang_c::initializeState_ReturnWait() {
    setChrAnimation(RB_BACK);
    setFlyChrAnimation(RB_FLY_FAST);
    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();
    unsetField_0x8CC(FLAG_COMMON_INIT);
    placeOnArm();
}
void dAcBoomerang_c::executeState_ReturnWait() {
    forwardSpeed = 0.f;
    placeOnArm();
    if (mAnmChr[BOOMERANG_ANIM_PINCERS].isStop() || !dAcPy_c::GetLink2()->checkActionFlagsCont(0x10)) {
        startSound(SE_BE_CATCH);
        mStateMgr.changeState(StateID_Wait);
    }
}
void dAcBoomerang_c::finalizeState_ReturnWait() {
    if (dAcPy_c::GetLink2()->checkActionFlagsCont(0x10)) {
        dAcPy_c::GetLink2()->setBeetleBackAnim();
    }
}

void dAcBoomerang_c::initializeState_Move() {
    setChrAnimation(RB_DEFAULT);
    mAnmChrBlend.setWeight(1, 1.f);
    setFlyChrAnimation(RB_FLY);
    field_0x8B1 = 45; // 1.5 seconds if that means anything
    field_0x8C8 = 0;

    dAcPy_c *player = dAcPy_c::GetLink2();
    forwardSpeed = player->getBeetleNormalSpeed();

    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();

    mRemainingFlightTime = player->getBeetleFlightTime();
    mFlashTimer = player->getBeetleWarningTimeLeft();
    unsetField_0x8CC(FLAG_REQUEST_MOVE | FLAG_WING_EFFECT_ACTIVE | FLAG_0x10000);
    setField_0x8CC(FLAG_CONTROLLABLE | FLAG_0x80000);

    angle.x = 2000 - player->vt_0x198();

    mRotation.x.set(-angle.x);
    mRotation.y.set(angle.y);
    mRotation.z.set(0);
    field_0x8B2 = 0;

    mOldPosition = mPosition;
    mOldPosition.y += 60.f;

    field_0x8BC = dPad::ex_c::m_current_ex->mMPLS.getVerticalAngle();
    field_0x8BA = dPad::ex_c::m_current_ex->mMPLS.getHorizontalAngle();
    field_0x8B8 = daPlayerActBase_c::fn_8005BAA0();

    // TODO regswap
    field_0x8BE = angle.x - dPad::ex_c::m_current_ex->mMPLS.getVerticalAngle();

    field_0x8E4.fromXY(angle.x, angle.y, forwardSpeed);

    startSound(SE_BE_THROW);
    dJEffManager_c::spawnUIEffect(PARTICLE_RESOURCE_ID_MAPPING_3_, mVec3_c::Zero, nullptr, nullptr, nullptr, nullptr);
    field_0x8F0 = mVec3_c::Zero;
    field_0x8D4 = 0.0f;
}
void dAcBoomerang_c::executeState_Move() {}
void dAcBoomerang_c::finalizeState_Move() {}

void dAcBoomerang_c::initializeState_MoveCancelWait() {}
void dAcBoomerang_c::executeState_MoveCancelWait() {}
void dAcBoomerang_c::finalizeState_MoveCancelWait() {}

void dAcBoomerang_c::initializeState_EventReturnWait() {
    mSph0.ClrCoHit();
    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();

    unsetField_0x8CC(FLAG_COMMON_INIT);
}
void dAcBoomerang_c::executeState_EventReturnWait() {}
void dAcBoomerang_c::finalizeState_EventReturnWait() {}

void dAcBoomerang_c::executeTimeWarning() {
    s16 timeLimit = dAcPy_c::GetLink2()->getBeetleWarningTimeLeft();
    mColor flashClr(0, 0, 0, 0xFF);

    if (mRemainingFlightTime <= timeLimit) {
        setField_0x8CC(FLAG_STOP_TIMER_ACTIVE);

        if (mFlashTimer != 0) {
            mFlashTimer--;
        }

        s32 r;
        if (mFlashTimer > 75) {
            r = 37;
        } else if (mFlashTimer > 37) {
            r = 18;
        } else {
            r = 9;
        }

        flashClr.r = (1.f / (r - 6)) * MAX(0, (mFlashTimer % r) - 5) * 255.f;
        if (flashClr.r == 0xFF && !mStateMgr.isState(StateID_MoveCancelWait)) {
            startSound(SE_BE_WARNING);
        }
    }

    mMdl.setTevKColorAll(GX_KCOLOR3, flashClr, false);
    dAcPy_c::GetLink2()->setBeetleFlashClr(flashClr);
}

void dAcBoomerang_c::registerInEvent() {
    return;
}

int dAcBoomerang_c::actorExecute() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    if (checkField_0x8CC(FLAG_REQUEST_0x400)) {
        unsetField_0x8CC(FLAG_REQUEST_0x400);
    }

    if (player->checkModelUpdateFlag(0x10000 | 0x80)) {
        deleteRequest();
        unsetField_0x8CC(FLAG_REQUEST_0x400);
        return SUCCEEDED;
    }

    if (EventManager::isInEvent() && mEventRelated.getSomeEventRelatedNumber() != -1) {
        setField_0x8CC(FLAG_0x40);
        mEventRelated.advanceNext();
    }

    unsetField_0x8CC(FLAG_0x40000 | FLAG_RUMBLE_ACTIVE);

    for (int i = 0; i < 2; ++i) {
        mAnmChr[i].play();
    }
    mMdl.play();
    mStateMgr.executeState();

    sLib::chase(&field_0x8D4, 0.f, 0.05f);
    mAcch.CrrPos(*dBgS::GetInstance());

    setRoomId();

    mMdl.calc(false);
    mMdl.getNodeWorldMtxMultVecZero(0, poscopy2);
    poscopy3 = poscopy2;

    if (checkField_0x8CC(FLAG_WING_EFFECT_ACTIVE)) {
        mEff0.createContinuousEffect(PARTICLE_RESOURCE_ID_MAPPING_6_, mWorldMtx, nullptr, nullptr);
        mEff1.setTransform(mWorldMtx);

        f32 ang = field_0x8E4.angle(velocity);
        ang = 10.f - mAng::fromRad(ang) * (7.f / 256.f);
        ang = nw4r::ut::Max(ang, 3.f);
        mEff1.setAwayFromCenterSpeed(ang);
    } else {
        mEff0.remove(true);
    }

    if (field_0x8B1 == 0 && (mStateMgr.isState(StateID_Move) || mStateMgr.isState(StateID_MoveCancelWait))) {
        mEff2.createContinuousEffect(
            PARTICLE_RESOURCE_ID_MAPPING_5_, player->GetPosition(), nullptr, nullptr, nullptr, nullptr
        );
    } else {
        mEff2.remove(true);
    }

    if (checkField_0x8CC(FLAG_0x20) && !checkField_0x8CC(FLAG_CANCEL)) {
        if (mStateMgr.isState(StateID_Move) || mStateMgr.isState(StateID_MoveCancelWait)) {
            mEff3.createContinuousEffect(PARTICLE_RESOURCE_ID_MAPPING_7_, mWorldMtx, nullptr, nullptr);
            holdSound(SE_BE_HIT_LEAVES_LV);
        }
    }

    unsetField_0x8CC(FLAG_0x20 | FLAG_0x40 | FLAG_0x80 | FLAG_DROP_ITEM);
    retrieve();
    return SUCCEEDED;
}

int dAcBoomerang_c::draw() {
    if (mStateMgr.isState(StateID_EventReturnWait) || dAcPy_c::GetLink2()->checkModelUpdateFlag(0x10000 | 0x80)) {
        return SUCCEEDED;
    }
    drawModelType1(&mMdl);
    if (!mStateMgr.isState(StateID_Wait)) {
        mProc.entry();
        static mQuat_c shadow(mVec3_c::Zero, 50.f);
        drawShadow(mShadow, nullptr, mWorldMtx, &shadow, -1, -1, -1, -1, -1, mPosition.y - mAcch.GetGroundH());
    }
    return SUCCEEDED;
}

bool dAcBoomerangProc_c::create(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc) {
    if (!d3d::UnkProc::create(prioOpa, -1, alloc)) {
        return false;
    }
    mpMdl = mdl;
    mClr = clr;
    return true;
}
