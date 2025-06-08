#include "d/a/obj/d_a_obj_boomerang.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_rumble.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_fanm.h"
#include "m/m_color.h"
#include "m/m_fader_base.h"
#include "m/m_mtx.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "s/s_State.hpp"

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
        if ((s32)(dAcPy_c::getCurrentBeetleType() >= HOOK_BEETLE) != FALSE) {
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
        field_0x8D8 = position - i_actorB->position;
        field_0x8D8.y = 0.f;

        if (cM::isZero(field_0x8D8.normalize())) {
            field_0x8D8.set(-angle.y.sin(), 0.f, -angle.y.cos());
        }
        field_0x8D8 *= velocity.absXZ();
        field_0x8D8.y = -velocity.y;

        field_0x8D8.normalize();

        onField_0x8CC(FLAG_BOOMERANG_CANCEL);
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
    if ((s32)(dAcPy_c::getCurrentBeetleType() >= HOOK_BEETLE) != FALSE) {
        hideModel(MDL_MOUTH_BASIC);
    } else {
        hideModel(MDL_MOUTH_HOOK);
    }

    // Decide the Body
    if ((s32)(dAcPy_c::getCurrentBeetleType() >= TOUGH_BEETLE) != FALSE) {
        hideModel(MDL_BODY_BASIC);
    } else {
        hideModel(MDL_BODY_ADV);
    }

    // Decide the Wings
    if ((s32)(dAcPy_c::getCurrentBeetleType() >= QUICK_BEETLE) != FALSE) {
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

    mAcchCir.SetWall(0.f, dAcPy_c::GetLink2()->getBeetleRadius());

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
    mSph1.SetR(dAcPy_c::GetLink2()->getBeetleRadius());
    mSph1.ClrCoSet();
    mSph1.SetAtType(AT_TYPE_0x40);

    mSph1.SetCoGrp(0xE);

    if ((s32)(dAcPy_c::getCurrentBeetleType() >= HOOK_BEETLE) == FALSE) {
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
        getSoundSource()->vt_0x40(5);
    }

    if (dAcPy_c::GetLink2()) {
        dAcPy_c::GetLink2()->onModelUpdateFlag(0x10000000);
    }
}

void dAcBoomerang_c::retrieve() {
    if (mFader.isStatus(mFaderBase_c::FADED_OUT)) {
        mFader.setFrame(5);
        mFader.fadeIn();
        playSound(SE_BE_COME_BACK);
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
    mWorldMtx.getTranslation(position);
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
    if (checkField_0x8CC(0x1000) && GetLinkage().checkFlag(0x80)) {}
}

// ...

void dAcBoomerang_c::bonk() {
    if (checkField_0x8CC(FLAG_BOOMERANG_RUMBLE_ACTIVE)) {
        return;
    }
    onField_0x8CC(FLAG_BOOMERANG_RUMBLE_ACTIVE);
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

// ...

bool dAcBoomerangProc_c::create(m3d::mdl_c *mdl, mColor clr, int prioOpa, mAllocator_c *alloc) {}
