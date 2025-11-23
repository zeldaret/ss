#include "d/a/obj/d_a_obj_boomerang.h"

#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_item.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_bg_s_poly_info.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_pad.h"
#include "d/d_room.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/snd/d_snd_wzsound.h"
#include "egg/gfx/eggDrawGX.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_fader_base.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/g3d_anmchr.h"
#include "nw4r/g3d/g3d_obj.h"
#include "nw4r/g3d/res/g3d_resanmchr.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resshp.h"
#include "nw4r/math/math_triangular.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

// copy from d_a_obj_fairy - TODO move it to a shared file
inline static void vecCylCalc(mVec3_c &target, const mAng &rot, f32 factor) {
    target.x += factor * rot.sin();
    target.z += factor * rot.cos();
}

dBgS_BeetleLinChk dAcBoomerang_c::sLinChk;
const dCcD_SrcSph dAcBoomerang_c::sSphSrc = {
    {/* mObjAt */ {AT_TYPE_BEETLE, dAcBoomerang_c::getSrcFlags(), {4, 0, 0}, 0, 1, 0, 0, 0},
     /* mObjTg */ {~AT_TYPE_COMMON0, 0x291, {0, 0, 0x407}, 0, 0},
     /* mObjCo */ {0x28}},
    {15.f}
};

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
    if (i_actorB != nullptr && i_actorB->getLinkage().checkFlag(0x80)) {
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
                mAnmChr[BOOMERANG_ANIM_PINCERS].setFrameOnly(i_actorB->getLinkage().field_0x24);
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
            field_0x8D8.set(-mAngle.y.sin(), 0.f, -mAngle.y.cos());
        }
        field_0x8D8 *= mVelocity.absXZ();
        field_0x8D8.y = -mVelocity.y;

        field_0x8D8.normalize();

        setField_0x8CC(FLAG_CANCEL);
        mStateMgr.changeState(StateID_MoveCancelWait);
    } else {
        // Play the animation to move the pincers
        setChrAnimation(RB_CUT);

        // I think this plays the vibration and drops the object
        startRumble();
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

    TRY_CREATE(mMdl.create(mdl, &mAllocator, 0x120, 1));

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

    TRY_CREATE(mAnmChrBlend.create(mdl, 2, &mAllocator));

    m3d::anmChr_c *pAnmChr = mAnmChr;
    nw4r::g3d::AnmObjChr *pAnimChr;
    nw4r::g3d::ResAnmChr resAnmChr = mResFile.GetResAnmChr("RB_Set");
    for (s32 i = 0; i < 2; ++i, ++pAnmChr) {
        TRY_CREATE(pAnmChr->create2(mdl, resAnmChr, &mAllocator));

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
    TRY_CREATE(mProc.create2(&mMdl, mColor(0x00, 0x10, 0x14, 0xFF), 0x27, &mAllocator));
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
    if (mAcch.GetGroundH() != -1e9f) {
        mRoomID = dBgS::GetInstance()->GetRoomId(mAcch.GetGnd());
    } else if (mRoomID == -1) {
        mRoomID = dAcPy_c::GetLink2()->mRoomID;
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

void dAcBoomerang_c::fn_80261E10() {
    mWorldMtx.transS(mPosition);
    if (checkField_0x8CC(FLAG_CANCEL_1)) {
        mWorldMtx.YrotM(field_0x8C4.y);
        mWorldMtx.XrotM(field_0x8C4.x);
        mWorldMtx.YrotM(-field_0x8C4.y);
    }
    if (!cM::isZero(field_0x8D0)) {
        mVec3_c z = mVec3_c::Ez * field_0x8D0;
        z.rotY(field_0x8BE.z);
        z += mVec3_c::Ey;

        mMtx_c m;
        m.makeRotationFromVecs(mVec3_c::Ey, z, 1.f);
        field_0x8D0 *= dAcPy_c::GetLink2()->getBeetle_8021B290();

        field_0x8BE.addZ(6372.f + cM::rndF(2731.f));

        mWorldMtx += m;
    }

    mWorldMtx.ZXYrotM(mRotation);
    mMdl.setLocalMtx(mWorldMtx);

    mSph1.OffAt_0x40();
    if (mSph1.ChkAtHit() || mSph0.ChkAtHit()) {
        dLightEnv_c::GetPInstance()->setBPM8_Type6(&mPosition);
    }

    dAcObjBase_c *pActor = mGrabbedActor.get();
    if (pActor != nullptr) {
        mWorldMtx.multVec(mVec3_c(0.f, 0.f, pActor->getLinkage().field_0x20), pActor->mPosition);

        f32 f = pActor->getLinkage().field_0x1C; // TODO Maybe Inline
        pActor->mPosition.y -= f;
        pActor->mRotation.y = mRotation.y + field_0x8BE.y;

        mMtx_c m;
        m.ZXYrotS(mRotation);
        m.YrotM(-mRotation.y);
        pActor->getLinkage().carryTransMtx = m;

        if (pActor->mProfileName == fProfile::BOMB) {
            if (mSph1.ChkAtHit()) {
                static_cast<dAcBomb_c *>(pActor)->setField0xA3C(0x4);
                static_cast<dAcBomb_c *>(pActor)->unsetField0xA3C(0x800000);
            }
            mSph1.OnAt_0x40();
        }
    }
}

void dAcBoomerang_c::fn_80262150() {
    unsetField_0x8CC(FLAG_CONTROLLABLE | FLAG_0x80000 | FLAG_0x20000);
    mRemainingFlightTime = 0;
    mFlashTimer = 0;
    deleteCheck();
    executeTimeWarning();
}

void dAcBoomerang_c::calcVelocity() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    f32 speed;
    if (checkField_0x8CC(FLAG_SPEED_UP)) {
        speed = player->getBeetleQuickSpeed();
    } else {
        speed = player->getBeetleNormalSpeed();
    }

    if (!EventManager::isInEvent() && player->hasBeetleVariantOrBetter(HOOK_BEETLE) && dPad::getDownZ()) {
        speed *= 0.7f;
    }

    sLib::addCalc(&mSpeed, speed, 0.5f, 30.f, 3.f);

    field_0x8E4.copyFrom(mVelocity); // Idk why this inline was used
    mVelocity.fromXY(mAngle.x, mAngle.y, mSpeed);

    if (mVelocity.y > 0.f) {
        mVelocity *= 0.7f;
    }
}

void dAcBoomerang_c::playFlySound() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    f32 param = cM::minMaxLimit(
        (mSpeed - player->getBeetleNormalSpeed()) / (player->getBeetleQuickSpeed() - player->getBeetleNormalSpeed()),
        0.f, 1.f
    );

    holdSoundWithFloatParam(SE_BE_FLY_LV, param);
}

void dAcBoomerang_c::startHitEffect(const mVec3_c &position, const cBgS_PolyInfo &polyInfo) {
    dAcPy_c::spawnItemHitEffect(position, polyInfo, 0.8f);
    startBgHitSound(SE_BE_HIT, polyInfo, &position);
}

bool dAcBoomerang_c::fn_80262400() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    mVec3_c collidingPosition;
    cBgS_PolyInfo *pCollidingPolyInfo = getCollidingPolyInfo(collidingPosition);

    s16 warningTimeAmount = player->getBeetleWarningTimeLeft();
    if (!checkField_0x8CC(FLAG_STOP_TIMER_ACTIVE) || (checkField_0x8CC(FLAG_0x80) && mRemainingFlightTime > 60)) {
        if (checkField_0x8CC(FLAG_0x20 | FLAG_0x80)) {
            field_0x8B4++;

            s16 time;
            if (checkField_0x8CC(FLAG_0x20)) {
                time = 30;
                mRemainingFlightTime -= 5;
            } else {
                time = 60;
            }

            if (field_0x8B4 >= player->getBeetleUnkTimeLeft()) {
                mRemainingFlightTime = time;
                mFlashTimer = time;
            }

        } else {
            field_0x8B4 = 0;
        }
    }

    // My assumption here is checking to see if it enters an unloaded room.
    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(mRoomID);
    if (pRoom != nullptr && pRoom->checkFlag(0x4)) {
        if (mRemainingFlightTime > 60) {
            mRemainingFlightTime = 60;
            mFlashTimer = 60;
        }
    }

    // Deal Stage collision Check
    if (pCollidingPolyInfo != nullptr && dBgS::GetInstance()->ChkPolySafe(*pCollidingPolyInfo)) {
        bool through = dBgS::GetInstance()->GetPolyObjectThrough(*pCollidingPolyInfo);

        mVec3_c normal = dBgS_GetN(*pCollidingPolyInfo);
        mVec3_c invVel = mVelocity * -1.f;
        invVel.normalize();

        if (through) {
            if (mRemainingFlightTime > warningTimeAmount) {
                mRemainingFlightTime -= 5;
                if (mRemainingFlightTime < warningTimeAmount) {
                    mRemainingFlightTime = warningTimeAmount;
                }
            }
        } else if (invVel.dot(normal) < nw4r::math::CosIdx(0x2000) /* 45 deg*/ &&
                   dBgS::GetInstance()->GetSpecialCode(*pCollidingPolyInfo) != POLY_ATTR_LAVA &&
                   (!mAcch.ChkGndHit() || !mAcch.ChkRoofHit())) {
            if (field_0x8D0 < 0.1f) {
                // 0.35f
                field_0x8D0 = player->getBeetle_8021B280();

                startHitEffect(collidingPosition, *pCollidingPolyInfo);

                mBounceVelocity = normal * 8.f;
                field_0x8D4 = 1.f;

                startRumble();
                if (mRemainingFlightTime > 200) {
                    mRemainingFlightTime -= 200;
                } else {
                    mRemainingFlightTime = 0;
                }
            }
        } else {
            field_0x8D8 = normal;
            startHitEffect(collidingPosition, *pCollidingPolyInfo);
            setField_0x8CC(FLAG_CANCEL);

            mStateMgr.changeState(StateID_MoveCancelWait);
            // Ends the Flight
            return true;
        }
    }

    // Deal with Object Collision ?
    if (mSph1.ChkTgHit()) {
        mVec3_c &hitPos = mSph1.GetTgHitPos();

        // Calculate Object Hit direction
        field_0x8D8 = mOldPosition - hitPos;
        field_0x8D8.y = 0.f;

        // Use Rotation if just a tap
        if (cM::isZero(field_0x8D8.normalize())) {
            field_0x8D8.set(-mAngle.y.sin(), 0.f, -mAngle.y.cos());
        }

        field_0x8D8 *= mVelocity.absXZ();
        field_0x8D8.y = -mVelocity.y;
        field_0x8D8.normalize();

        setField_0x8CC(FLAG_CANCEL);

        mStateMgr.changeState(StateID_MoveCancelWait);
        // Ends the Flight
        return true;
    }

    // Do not End the flight
    return false;
}

cBgS_PolyInfo *dAcBoomerang_c::getCollidingPolyInfo(mVec3_c &outPos) {
    if (mAcch.ChkWallHit(nullptr)) {
        mVec3_c dir = mVec3_c::Zero;

        vecCylCalc(dir, mAcchCir.GetWallAngleY(), 1.f);

        mVec3_c end = mPosition;
        end -= dir * 100.f;

        sLinChk.Set(&mPosition, &end, nullptr);

        if (dBgS::GetInstance()->LineCross(&sLinChk)) {
            outPos = sLinChk.GetLinEnd();
            return &sLinChk;
        } else {
            outPos = mPosition;
            return &mAcchCir;
        }
    }

    if (mAcch.ChkGndHit()) {
        outPos.set(mPosition.x, mAcch.GetGroundH(), mPosition.z);
        return &mAcch.mGnd;
    }

    if (mAcch.ChkRoofHit()) {
        outPos.set(mPosition.x, mAcch.GetRoofHeight(), mPosition.z);
        return &mAcch.mRoof;
    }

    if (mAcch.field_0x1A8.field_0x00 != 0) {
        const mVec3_c &v = mAcch.field_0x1A8.GetField_0x1C();
        outPos.set(v.x, v.y, v.z);
        return &mAcch.mSph;
    }

    return nullptr;
}

bool dAcBoomerang_c::tryGrabObject(dAcObjBase_c *pObj) {
    dAcPy_c *player = dAcPy_c::GetLink2();

    // Check Controllable and some other flag
    if ((checkField_0x8CC(FLAG_CONTROLLABLE) && pObj->getLinkage().checkFlag(0x80))
        // Check Active status - Not active or has Connection 9
        && (!pObj->getLinkage().checkState(dLinkage_c::STATE_ACTIVE) ||
            pObj->getLinkage().checkConnection(dLinkage_c::CONNECTION_9))
        // Check to make sure there is no obnject linked and is not related to player
        && (!mGrabbedActor.isLinked() && pObj != player->getActorInActorRef1()) &&
        pObj->getLinkage().tryAttach(pObj, this, &mGrabbedActor, dLinkage_c::CONNECTION_5, true)) {
        field_0x8BE.y = pObj->mRotation.y - mRotation.y;
        player->setGuide(pObj);
        unsetField_0x8CC(FLAG_SPEED_UP);
        setField_0x8CC(FLAG_DROP_ITEM);
        startSound(SE_BE_CATCH_OBJECT);
        setFlyChrAnimation(RB_FLY);
        return true;
    }

    return false;
}

void dAcBoomerang_c::deleteCheck() {
    if (checkField_0x8CC(FLAG_DROP_ITEM)) {
        setField_0x8CC(FLAG_REQUEST_0x400);
        return;
    }

    dAcObjBase_c *pObj = mGrabbedActor.get();
    if (pObj == nullptr) {
        return;
    }

    dAcPy_c *player = dAcPy_c::GetLink2();
    player->getOwnedObjects().Regist(pObj);
    if (dBgS_ObjLinChk::LineCross(&mOldPosition, &pObj->mPosition, nullptr)) {
        mVec3_c diff = mOldPosition - pObj->mPosition;
        diff.normalize();

        pObj->mPosition = dBgS_ObjLinChk::GetInstance().GetLinEnd() + diff * 15.f;
        pObj->mOldPosition = pObj->mPosition + diff * 15.f;
    } else {
        pObj->mOldPosition.x = mOldPosition.x;
        pObj->mOldPosition.z = mOldPosition.z;
    }

    pObj->getLinkage().fn_80050DC0(pObj, 20.f, 0.f, mRotation.y);
    dRoom_c *pRoom = dStage_c::GetInstance()->getRoom(mRoomID);
    if (pRoom && pRoom->checkFlag(0x4) && pObj->getConnectParent() == pRoom) {
        pObj->deleteRequest();
    }
}

void dAcBoomerang_c::fn_80262DC0() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    if (EventManager::isInEvent()) {
        mStateMgr.changeState(StateID_EventReturnWait);
    } else if (player->retrieveBeetle()) {
        fn_80261E10();
        if (mStateMgr.isState(StateID_Move)) {
            startSound(SE_BE_CATCH);
            mStateMgr.changeState(StateID_Wait);
        } else {
            mStateMgr.changeState(StateID_ReturnWait);
        }
    } else {
        deleteRequest();
    }

    player->unlinkActorRef1();
    deleteCheck();
}

void dAcBoomerang_c::startRumble() {
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

void dAcBoomerang_c::setFlyChrAnimation(FlyAnimation_e requestedAnimation) {
    static const char *sFlyChrAnims[dAcBoomerang_c::RB_FLY_MAX] = {"RB_Fly", "RB_FlyFast"};
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
    mSpeed = 0.f;
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
    mSpeed = 0.f;
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
    field_0x8C4.z = 0;

    dAcPy_c *player = dAcPy_c::GetLink2();
    mSpeed = player->getBeetleNormalSpeed();

    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();

    mRemainingFlightTime = player->getBeetleFlightTime();
    mFlashTimer = player->getBeetleWarningTimeLeft();
    unsetField_0x8CC(FLAG_REQUEST_MOVE | FLAG_SPEED_UP | FLAG_0x10000);
    setField_0x8CC(FLAG_CONTROLLABLE | FLAG_0x80000);

    mAngle.x = 2000 - player->vt_0x198();
    mRotation.set(-mAngle.x, mAngle.y, mAng(0)); // wtf?
    field_0x8B2 = 0;

    mOldPosition = player->mPosition;
    mOldPosition.y += 60.f;

    field_0x8B8.z = dPad::ex_c::getInstance()->mMPLS.getVerticalAngle();
    field_0x8B8.y = dPad::ex_c::getInstance()->mMPLS.getHorizontalAngle();
    field_0x8B8.x = dAcPy_c::fn_8005BAA0();

    field_0x8BE.x = (mAngle.x - dPad::ex_c::getInstance()->mMPLS.getVerticalAngle());
    field_0x8E4.fromXY(mAngle.x, mAngle.y, mSpeed);

    startSound(SE_BE_THROW);
    dJEffManager_c::spawnUIEffect(PARTICLE_RESOURCE_ID_MAPPING_3_, mVec3_c::Zero, nullptr, nullptr, nullptr, nullptr);
    mBounceVelocity = mVec3_c::Zero;
    field_0x8D4 = 0.0f;
}

void dAcBoomerang_c::executeState_Move() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    // Check for collision and adjust flight time as needed
    // If it collides with something and stops, the function will return true
    if (fn_80262400()) {
        return;
    }

    // Reset the Pincer Animation when complete
    if (mAnmChr[BOOMERANG_ANIM_PINCERS].isStop() && (mCurrentAnimation == RB_HOLD_ED || mCurrentAnimation == RB_CUT)) {
        setChrAnimation(RB_DEFAULT);
    }

    // Reset the Animation in the case of no longer holding an object
    if (mCurrentAnimation == RB_HOLD && !mGrabbedActor.isLinked()) {
        setChrAnimation(RB_DEFAULT);
    }

    // Reduce the flught time (1 per frame). The Collision check may reduce further
    if (mRemainingFlightTime != 0) {
        mRemainingFlightTime--;
    }

    if (field_0x8B1 != 0) {
        if (--field_0x8B1 == 0) {
            field_0x8B4 = 0;
        }
    }

    if (checkField_0x8CC(FLAG_CONTROLLABLE)) {
        if (!EventManager::isInEvent()) {
            // Check for dropping item -> No longer moving
            if (mGrabbedActor.isLinked() && dPad::getDownZ() && dPad::getDownTrigA()) {
                setField_0x8CC(FLAG_RELEASE_OBJECT);
                player->setBeetleReleasedObject(mGrabbedActor.get());
                startSound(SE_BE_RELEASE_OBJECT);
                mStateMgr.changeState(StateID_MoveCancelWait);
                return;
            }

            setField_0x8CC(FLAG_0x40000);
            if (checkField_0x8CC(FLAG_SPEED_UP)) {
                // Any condition that will prevent the beetle from speeding up
                if (!dPad::getDownA() || mGrabbedActor.isLinked() || dPad::getDownZ()) {
                    startSound(SE_BE_SPEED_DOWN);
                    unsetField_0x8CC(FLAG_SPEED_UP);
                    setFlyChrAnimation(RB_FLY);
                }
            } else {
                if (player->hasBeetleVariantOrBetter(QUICK_BEETLE) && !mGrabbedActor.isLinked() && !dPad::getDownZ() &&
                    dScGame_c::getCamera(0)->fn_8019E3C0() && dPad::getDownA()) {
                    startSound(SE_BE_SPEED_UP);
                    setField_0x8CC(FLAG_SPEED_UP);
                    setFlyChrAnimation(RB_FLY_FAST);
                    mEff1.startEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_4_, mVec3_c::Zero, nullptr, nullptr, nullptr, nullptr
                    );
                }
            }
        }
    } else {
        // In the case of no longer being controlled Stop speed up
        unsetField_0x8CC(FLAG_SPEED_UP);
    }

    const f32 maxRange = player->getBeetleMaxRange();
    const f32 heightDiff = mPosition.y - player->mPosition.y;

    // In range is within collection range for beetle to return to player
    bool inRange = field_0x8B1 == 0;
    if (inRange) {
        inRange = player->checkWithinRadius(mPosition, mSph0.GetR());
    }
    if (inRange) {
        f32 r = mSph0.GetR();
        inRange = heightDiff > -r;
    }
    if (inRange) {
        f32 r = mSph0.GetR();
        inRange = heightDiff < r + 3500.f;
    }

    if (checkField_0x8CC(FLAG_CONTROLLABLE)) {
        if (
            mRemainingFlightTime == 0                                              // Check End of Flight
            || inRange                                                             // Change In Range of player
            || !player->hasvt_0x1C0()                                              // ??
            || checkField_0x8CC(FLAG_0x20000)                                      // ??
            || (EventManager::isInEvent() && !checkField_0x8CC(FLAG_0x40))         // ??
            || (mPosition.squareDistance(player->mPosition) > maxRange * maxRange) // Outside of Max Range
            || (!EventManager::isInEvent() && dPad::getDownTrigB())                // Pressing B while not in Event
        ) {
            if (inRange || EventManager::isInEvent() || checkField_0x8CC(FLAG_0x20000)) {
                if (inRange) {
                    dAcObjBase_c *pObj = mGrabbedActor.get();
                    if (pObj != nullptr) {
                        if (pObj->mProfileName == fProfile::ITEM) {
                            static_cast<dAcItem_c *>(pObj)->getItemFromBWheelItem();
                        } else if (pObj->getLinkage().checkFlag(0x8000)) {
                            player->unkBeetle_8021B040(pObj);
                        }
                    }
                }
                fn_80262150();
                fn_80262DC0();
            } else {
                if (!EventManager::isInEvent() && dPad::getDownTrigB()) {
                    startSound(SE_BE_CALL_BACK);
                }
                setField_0x8CC(FLAG_CANCEL_2);
                mStateMgr.changeState(StateID_MoveCancelWait);
            }
            return;
        }
    }

    if (checkField_0x8CC(FLAG_CONTROLLABLE)) {
        mAng ang0Min, ang0Max;
        mAng ang1Min, ang1Max;
        mAng3_c angTmp;
        u32 maxFlightTime;
        s32 usedTime;

        ang0Min = ang0Max = player->getBeetleAngle0();
        ang1Min = ang1Max = player->getBeetleAngle1();
        maxFlightTime = player->getBeetleFlightTime();

        usedTime = (maxFlightTime - mRemainingFlightTime) - 5;

        mAng angle, rotation;
        if (usedTime <= 0) {
            setField_0x8CC(FLAG_0x80000);
        } else {
            unsetField_0x8CC(FLAG_0x80000);
            f32 f = nw4r::ut::Min(usedTime * (1.f / 60.f), 1.f);

            if (EventManager::isInEvent()) {
                angle = mAngle.x;
                rotation = mRotation.z;
            } else {
                // HELPPPPP
                mAng a = -player->fn_8023BA40(field_0x8B8.x, field_0x8B8.y);
                mAng b = player->getBeetleSmallAngle0();
                if (mAng::abs(a) < b) {
                    a = 0;
                } else if (a > 0) {
                    a = b - a;
                } else {
                    a = b + a;
                }
                rotation = a;

                a = angTmp.x = dPad::ex_c::getInstance()->mMPLS.mZ.atan2sY_XZ();
                a += field_0x8BE.x;
                b = player->getBeetleSmallAngle1();
                if (mAng::abs(a) < b) {
                    a = 0;
                } else if (a > 0) {
                    a = b - a;
                } else {
                    a = b + a;
                }
                angle = a;
            }

            sLib::addCalcAngle(
                mAngle.x.ref(), cM::minMaxLimit<s16>(angle, -ang0Min, ang0Max), 5, f * 2000.f, f * 100.f
            );
            sLib::addCalcAngle(
                mRotation.z.ref(), cM::minMaxLimit<s16>(rotation, -ang1Min, ang1Max), 5, f * 2000.f, f * 100.f
            );

            f32 f2;
            if (checkField_0x8CC(FLAG_SPEED_UP)) {
                f2 = player->getBeetleWingAngleActive();
            } else {
                f2 = player->getBeetleWingAngleInactive();
            }
            mAngle.y -= f2 * mRotation.z;

            mRotation.x = -mAngle.x;
            mRotation.y = mAngle.y;

            s32 tmp;
            tmp = rotation.abs();
            if (tmp > 0x400) {
                sLib::addCalcAngle(field_0x8B8.x.ref(), 0, 5, tmp >> 2, tmp >> 8);
                sLib::addCalcAngle(field_0x8B8.y.ref(), 0, 5, tmp >> 2, tmp >> 8);
            }
            tmp = mAng(field_0x8B8.z - angTmp.x).abs();
            if (tmp > 0x400) {
                sLib::addCalcAngle(field_0x8BE.x.ref(), 0, 5, tmp >> 2, tmp >> 8);
            }
        }
        executeTimeWarning();
    }

    calcVelocity();
    playFlySound();

    mPosition += (mVelocity * (1.f - field_0x8D4)) + (mBounceVelocity * field_0x8D4);

    if (mPosition.y > player->mPosition.y + player->getBeetleMaxHeightOffset()) {
        setRemainingFlightTime(-1);
    }

    fn_80261E10();

    if (!EventManager::isInEvent()) {
        mSph0.SetR(player->getBeetleLargeRadius());
        mVec3_c dir = mPosition - mOldPosition;
        mSph0.setCenterAndAtVec(mPosition, dir);
        mSph1.setCenterAndAtVec(mPosition, dir);
        if (mRemainingFlightTime < player->getBeetleFlightTime() - 2) {
            if (!mGrabbedActor.isLinked()) {
                dCcS::GetInstance()->Set(&mSph0);
            } else {
                mSph0.ClrAtHit();
            }
            dCcS::GetInstance()->GetMassMng().SetObj(&mSph0, 1);
            dCcS::GetInstance()->GetMassMng().SetArea(&mSph0, 1, dAcBoomerang_areaCallback);
        }
        player->registMassObj(&mSph1, 1);
    } else {
        mSph0.ClrAtHit();
        mSph1.ClrAtHit();
    }
}
void dAcBoomerang_c::finalizeState_Move() {}

void dAcBoomerang_c::initializeState_MoveCancelWait() {
    dAcPy_c *player = dAcPy_c::GetLink2();

    playFlySound();
    unsetField_0x8CC(FLAG_SPEED_UP);
    setChrAnimation(RB_HOLD_ED);

    // If canceling due to bonk
    if (checkField_0x8CC(FLAG_CANCEL_1)) {
        f32 mult = (0.5f * mVelocity.mag());
        mVelocity = field_0x8D8 * mult;
        field_0x8C4.y = mVelocity.atan2sX_Z();
        field_0x8C4.x = field_0x8C4.z = player->getBeetleBonkRecoilAngle();

        startRumble();
    }

    // I think the below function is responsible for giving the item to link
    deleteCheck();

    // Set Fly Animation
    if (checkField_0x8CC(FLAG_CANCEL)) {
        setFlyChrAnimation(RB_FLY_FAST);
    } else {
        setFlyChrAnimation(RB_FLY);
    }

    if (field_0x8B2 == 0) {
        if (mAcch.ChkGndHit() && mAcch.ChkRoofHit() &&
            (dBgS::GetInstance()->ChkMoveBG(mAcch.GetGnd(), true) ||
             dBgS::GetInstance()->ChkMoveBG(mAcch.GetRoof(), true))) {
            field_0x8B2 = 0;
        } else {
            field_0x8B2 = player->getBeetle_8021B2A0();
        }
    }
    if (checkField_0x8CC(FLAG_CANCEL_2)) {
        // 0.35f
        field_0x8D0 = player->getBeetle_8021B280();
        field_0x8BE.z = mVelocity.atan2sX_Z();
    }

    fn_80261E10();
}
void dAcBoomerang_c::executeState_MoveCancelWait() {
    executeTimeWarning();

    dAcPy_c *player = dAcPy_c::GetLink2();
    if (mAnmChr[BOOMERANG_ANIM_PINCERS].isStop()) {
        setChrAnimation(RB_DEFAULT);
    }

    mPosition += (1.f - field_0x8D4) * mVelocity + field_0x8D4 * mBounceVelocity;

    if (checkField_0x8CC(FLAG_CANCEL_2)) {
        mVelocity *= 0.9f;
        holdSound(SE_BE_FLY_TOTTER_LV);
    } else {
        playFlySound();
        s32 r;
        if (mRotation.z >= 0) {
            r = player->getBeetleAngle1();
        } else {
            r = player->getBeetleAngle1();
            r = -r; // Ok
        }

        sLib::addCalcAngle(mRotation.z.ref(), r, 5, 2000, 100);

        mAngle.y -= mRotation.z * player->getBeetleWingAngleActive();

        sLib::addCalc(&mSpeed, player->getBeetleNormalSpeed(), 0.5f, 30.f, 3.f);
        calcVelocity();

        if (fn_80262400()) {
            return;
        }
    }

    if (field_0x8C4.z > player->getBeetleSmallAngle2()) {
        field_0x8C4.z -= player->getBeetleSmallAngle2();
    } else {
        field_0x8C4.z = 0;
    }

    field_0x8C4.x += field_0x8C4.z;
    fn_80261E10();

    bool fadedOut = false;
    if (field_0x8B2 == 0 || (!player->doesActorRef1Exist() && checkField_0x8CC(FLAG_RELEASE_OBJECT))) {
        if (mFader.isStatus(mFaderBase_c::FADED_OUT)) {
            fadedOut = true;
        } else if (!mFader.isActive()) {
            if (EventManager::isInEvent()) {
                fn_80262150();
                fn_80262DC0();
            } else {
                mFader.setFrame(5);
                mFader.fadeOut();
            }
        }
    }

    if (fadedOut || !player->hasvt_0x1C0() || checkField_0x8CC(FLAG_0x20000)) {
        fn_80262150();
        fn_80262DC0();
        return;
    }

    if (checkField_0x8CC(FLAG_0x10 | FLAG_CANCEL_1) || !checkField_0x8CC(FLAG_RELEASE_OBJECT)) {
        if (field_0x8B2 != 0) {
            field_0x8B2--;
        }
    }
}
void dAcBoomerang_c::finalizeState_MoveCancelWait() {}

void dAcBoomerang_c::initializeState_EventReturnWait() {
    mSph0.ClrCoHit();
    mSph0.ClrAtHit();
    mSph1.ClrTgHit();
    mSph1.ClrAtHit();

    unsetField_0x8CC(FLAG_COMMON_INIT);
}
void dAcBoomerang_c::executeState_EventReturnWait() {
    if (EventManager::isInEvent()) {
        return;
    }
    fn_80262DC0();
}
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
        deleteCheck();
    }

    if (player->checkModelUpdateFlag(0x10000 | 0x80)) {
        deleteRequest();
        unsetField_0x8CC(FLAG_DROP_ITEM);
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
    mMdl.getNodeWorldMtxMultVecZero(0, mPositionCopy2);
    mPositionCopy3 = mPositionCopy2;

    if (checkField_0x8CC(FLAG_SPEED_UP)) {
        mEff0.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_6_, mWorldMtx, nullptr, nullptr);
        mEff1.setTransform(mWorldMtx);

        f32 ang = field_0x8E4.angle(mVelocity);
        ang = 10.f - ((f32)mAng::fromRad(ang) * (7.f / 256.f));
        ang = nw4r::ut::Max(ang, 3.f);
        mEff1.setAwayFromCenterSpeed(ang);
    } else {
        mEff0.remove(true);
    }

    if (field_0x8B1 == 0 && (mStateMgr.isState(StateID_Move) || mStateMgr.isState(StateID_MoveCancelWait))) {
        mEff2.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_5_, player->getPosition(), nullptr, nullptr, nullptr, nullptr);
    } else {
        mEff2.remove(true);
    }

    if (checkField_0x8CC(FLAG_0x20) && !checkField_0x8CC(FLAG_CANCEL)) {
        if (mStateMgr.isState(StateID_Move) || mStateMgr.isState(StateID_MoveCancelWait)) {
            mEff3.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_7_, mWorldMtx, nullptr, nullptr);
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

void dAcBoomerangProc_c::drawOpa() {
    EGG::DrawGX::ResetMaterial(EGG::DrawGX::COLORCHAN_1);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
    GXSetChanMatColor(GX_COLOR0A0, mClr);

    u32 info = nw4r::g3d::RESMDL_DRAWMODE_IGNORE_MATERIAL | nw4r::g3d::RESMDL_DRAWMODE_FORCE_LIGHTOFF;
    mpMdl->getG3dObject()->G3dProc(nw4r::g3d::G3dObj::G3DPROC_DRAW_OPA, 0, &info);
}
