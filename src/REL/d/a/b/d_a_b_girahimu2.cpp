#include "d/a/b/d_a_b_girahimu2.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/b/d_a_b_girahimu_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_girahimu_knife.h"
#include "d/a/obj/d_a_obj_girahimu_sword_link.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d_lyt_boss_caption.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_sword_battle_game.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/MTX/mtx.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"

STATE_DEFINE(dAcGirahimu2_c, CreateSpinKnife);
STATE_DEFINE(dAcGirahimu2_c, WaitKnifeAttack);
STATE_DEFINE(dAcGirahimu2_c, KnifeAttack);
STATE_DEFINE(dAcGirahimu2_c, KnifePreAttack);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, CatchDamage);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Walk);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Counter);
STATE_DEFINE(dAcGirahimu2_c, G_SwordDemo);
STATE_DEFINE(dAcGirahimu2_c, G_SwordWait);
STATE_DEFINE(dAcGirahimu2_c, G_SwordPiyori);
STATE_DEFINE(dAcGirahimu2_c, Death);
STATE_DEFINE(dAcGirahimu2_c, DeathMiniGame);
STATE_DEFINE(dAcGirahimu2_c, UpWarp);
STATE_DEFINE(dAcGirahimu2_c, UpAttack);
STATE_DEFINE(dAcGirahimu2_c, CreateKnife);
STATE_DEFINE(dAcGirahimu2_c, ComeOn);
STATE_DEFINE(dAcGirahimu2_c, ComeOnGuard);
STATE_DEFINE(dAcGirahimu2_c, GuardJustCounter);
STATE_DEFINE(dAcGirahimu2_c, G_SwordDamage); // Non Virtual Define??
STATE_DEFINE(dAcGirahimu2_c, TwoSwordAttack);
STATE_DEFINE(dAcGirahimu2_c, HitAction);
STATE_VIRTUAL_DEFINE(dAcGirahimu2_c, Catch);

SPECIAL_ACTOR_PROFILE(B_GIRAHIMU2, dAcGirahimu2_c, fProfile::B_GIRAHIMU2, 0x10F, 0, 0);

struct dAcGirahimu2_HIO_c {
    f32 field_0x00;
    f32 field_0x04;
    f32 field_0x08;
    f32 field_0x0C;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1C;
    s16 field_0x20;
    s16 field_0x22;
    s16 field_0x24;
    s16 field_0x26;

    static const dAcGirahimu2_HIO_c sHIO;
};

const dAcGirahimu2_HIO_c dAcGirahimu2_HIO_c::sHIO = {1.05f, 1.05f, -5.0f, -2.0f, 15.0f, 150.0f,
                                                     3.0f,  40.0f, 1,     0x5DC, 0x14,  0x1000};

void dAcGirahimu2_c::initializeState_G_SwordWait() {
    mbFirstSwordWaitEntered = true;
    field_0xD6C = 6;
    mCcBody.OnTgSet();
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    mSubstateTimer = 30;
    if (isNearHalfHealth()) {
        field_0x186C = 60.0f;
        mSubstateTimer = 5;
    }
    if (cM::rndInt(100) > 50) {
        field_0xD7E = true;
    } else {
        field_0xD7E = false;
    }
}
void dAcGirahimu2_c::executeState_G_SwordWait() {
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    if (checkObjectProperty(OBJ_PROP_0x2)) {
        return;
    }

    mVec3_c distFromHome = mPosition - mStartingPos;
    mVec3_c v0(0.0f, 0.0f, 1.0f);
    v0.rotY(mRotation.y);
    if (mDistanceFromStart >= 1500.0f && distFromHome.dot(v0) < 0.0f && mDistanceToPlayer < 500.0f) {
        changeState(StateID_HomeWarp);
        return;
    }

    if (mAng(mAngleToPlayer - mRotation.y).abs() < 0x3000) {
        if (mDistanceToPlayer <= 400.0f && field_0xD7E) {
            mbShortStun = true;
            vt_0x1F8();
            changeState(StateID_FrontAttack);
            return;
        }
        if (mSubstateTimer <= 0) {
            mSubstateTimer = 60;
            if (fn_228_5110()) {
                return;
            }
        }
    }

    if (mDistanceToPlayer <= 400.0f && fn_228_5110()) {
        return;
    }

    mAng diff = mAngleToPlayer - mRotation.y;
    if (mAnmIDBody != ANM_WaitA && mRotation.y == mAngleToPlayer && mDistanceToPlayer <= 250.f) {
        mSpeedTarget = 0.0f;
        mSpeed = 0.0f;
        setAnm("WaitBtD", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else if (mDistanceToPlayer >= 300.0f) {
        if (mAnmIDBody != ANM_Walk) {
            setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
        }
    } else if (mAng(diff).abs() > 0x4000) {
        changeState(StateID_BackWalk);
        return;
    } else {
        turn(mAngleToPlayer);
    }

    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    fn_226_B960(6.0f);
}
void dAcGirahimu2_c::finalizeState_G_SwordWait() {}

void dAcGirahimu2_c::initializeState_CreateKnife() {
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    setAnm("WaitBtD", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    mSubstateTimer = 15;
    mCallback.field_0x088 = true;
    mCcBody.SetTgShieldType(~AT_TYPE_DAMAGE);
    field_0xD6C = 6;
    field_0x2220 = 0;
    mSubstate = 0;
}
void dAcGirahimu2_c::executeState_CreateKnife() {
    mCyl0.SetR(150.0f);
    switch (mSubstate) {
        case 0: {
            if (mSubstateTimer <= 0) {
                if (!turn(mAngleToPlayer)) {
                    correctPoseHip();
                }
                field_0xD6C = 0;
                if (mDistanceToPlayer <= 1000.0f) {
                    setAnm("CallWalk", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                    setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
                    mSpeedTarget = -5.0f;
                } else {
                    mSpeed = 0.0f;
                    mSpeedTarget = 0.0f;
                    setAnm("Call", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
                }
                mSubstate++;
            }
        } break;
        case 1: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
                int rnd = cM::rndInt(100);
                if (rnd >= 60) {
                    startSound(SE_OGhKf_GROUP_APPEAR);
                    mKnifeVolleyState = 2;
                    spawnKnifeVolley(0, 5);
                    spawnKnifeVolley(5, 10);
                    spawnKnifeVolley(10, 15);
                } else if (rnd >= 30) {
                    mKnifeVolleyState = 3;
                    spawnKnifeSurrond(false);
                } else {
                    mKnifeVolleyState = 3;
                    spawnKnifeSurrond(true);
                }
            }
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                changeState(StateID_KnifePreAttack);
                return;
            }
        } break;
    }

    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    fn_226_B960(6.0f);
}
void dAcGirahimu2_c::finalizeState_CreateKnife() {}

void dAcGirahimu2_c::initializeState_KnifePreAttack() {
    mCallback.field_0x088 = true;
    clearAnms();
    if (mDistanceToPlayer <= 1000.0f) {
        setAnm("CallWalk", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
        setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
        mSpeedTarget = -5.0f;
    } else {
        mSpeed = 0.0f;
        mSpeedTarget = 0.0f;
        setAnm("CallWalk", ANM_Call, m3d::PLAY_MODE_4, 5.0f, 1.5f);
    }

    mSubstate = 0;
    field_0xD6C = 6;
    if (mKnifeVolleyIdx > 10) {
        mKnifeVolleyIdx = 0;
    }

    for (int i = mKnifeVolleyIdx; i < mKnifeVolleyIdx + 5; ++i) {
        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
            return;
        }
    }
    mKnifeVolleyIdx += 5;
    if (mKnifeVolleyIdx > 10) {
        mKnifeVolleyIdx = 0;
    }
    for (int i = mKnifeVolleyIdx; i < mKnifeVolleyIdx + 5; ++i) {
        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
            return;
        }
    }
    mKnifeVolleyIdx += 5;
    if (mKnifeVolleyIdx > 10) {
        mKnifeVolleyIdx = 0;
    }
    mSubstate = 0;
}
void dAcGirahimu2_c::executeState_KnifePreAttack() {
    if (mAnmIDHip != ANM_Walk && mDistanceToPlayer <= 1000.0f) {
        setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
        mSpeedTarget = -5.0f;
    }
    if (mAnmIDHip != ANM_Walk) {
        adjustLegAngle(mAngleToPlayer);
    } else {
        fn_226_B960(6.0f);
    }

    mCyl0.SetR(150.0f);

    bool hasKnife = false;
    for (int i = 0; i < 15; ++i) {
        if (mKnives[i].isLinked()) {
            hasKnife = true;
        }
    }

    if (!hasKnife) {
        vt_0x220();
        return;
    }
    switch (mSubstate) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
                if (mKnifeVolleyState == 3) {
                    for (int i = 0; i < 15; ++i) {
                        if (mKnives[i].isLinked()) {
                            mKnives[i].get()->field_0x792 = false;
                        }
                    }
                } else {
                    if (mKnifeVolleyIdx > 10) {
                        mKnifeVolleyIdx = 0;
                    }
                    for (int i = mKnifeVolleyIdx; i < mKnifeVolleyIdx + 5; i++) {
                        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                            mKnives[i].get()->setWait();
                        }
                    }
                }
            }
            if (mAnmChrs[ANMIDX_Body].checkFrame(16.0f)) {
                startSound(SE_OGhKf_GROUP_DIVIDE);
            }
            if (mAnmIDBody == ANM_Call && mAnmChrs[ANMIDX_Body].isStop()) {
                mSubstateTimer = 20;
                mSubstate++;
            }
        } break;
        case 1: {
            if (mAnmIDHip == ANM_Walk) {
                setAnm("WalkBt", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.0f);
            }
            if (mAnmIDHip != ANM_Walk || mSubstateTimer <= 0) {
                if (mKnifeVolleyState == 3) {
                    for (int i = 0; i < 15; ++i) {
                        if (mKnives[i].isLinked() && !mKnives[i].get()->isWaiting()) {
                            vt_0x220();
                            return;
                        }
                    }
                }
                changeState(StateID_KnifeAttack);
                return;
            }
        } break;
    }

    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
}
void dAcGirahimu2_c::finalizeState_KnifePreAttack() {}

void dAcGirahimu2_c::initializeState_KnifeAttack() {
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    field_0xD6C = 6;
    setAnm("AttackKnife", ANM_AttackKnife, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
    mCallback.field_0x088 = true;
}
void dAcGirahimu2_c::executeState_KnifeAttack() {
    mCyl0.SetR(150.0f);
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);

    if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        if (mKnifeVolleyState == 3) {
            startSound(SE_OGhKf_GROUP_SHOT);
            for (int i = 0; i < 15; ++i) {
                if (mKnives[i].isLinked()) {
                    mKnives[i].get()->fn_239_2460(true, true, 40.0f);
                }
            }
        } else {
            for (int i = mKnifeVolleyIdx; i < mKnifeVolleyIdx + 5; ++i) {
                if (mKnives[i].isLinked()) {
                    startSound(SE_OGhKf_SHOT);
                    mKnives[i].get()->fn_239_2360(true, true, 50.0f);
                }
            }
            mKnifeVolleyIdx += 5;
        }
        field_0xD6C = 6;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (mKnifeVolleyState == 3) {
            vt_0x220();
            return;
        }
        if (mKnifeVolleyIdx >= 15) {
            vt_0x220();
            return;
        }
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                changeState(StateID_KnifePreAttack);
                return;
            }
        }
        vt_0x220();
        return;
    }
}
void dAcGirahimu2_c::finalizeState_KnifeAttack() {}

void dAcGirahimu2_c::initializeState_HitAction() {
    mbSwordAtHit = false;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    setAnm("PoseFree", ANM_PoseFree, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 9;
    mCallback.field_0x088 = false;
    mConsecutiveSwordHitCount = 0;
}
void dAcGirahimu2_c::executeState_HitAction() {
    mCallback.field_0x088 = false;
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    fn_226_B960(5.0f);
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (mDistanceToPlayer < 300.0f) {
            changeState(StateID_Counter);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimu2_c::finalizeState_HitAction() {
    mCallback.field_0x088 = true;
}

void dAcGirahimu2_c::initializeState_ComeOn() {
    mConsecutiveSwordHitCount = 0;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    clearAnms();
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    setAnm("PoseFree", ANM_PoseFree, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 9;
    mCallback.field_0x088 = false;
    mSubstateTimer = 60;
}
void dAcGirahimu2_c::executeState_ComeOn() {
    mCallback.field_0x088 = false;
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    fn_226_B960(5.0f);
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (mDistanceToPlayer <= 400.0f) {
            changeState(StateID_Counter);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimu2_c::finalizeState_ComeOn() {
    mCallback.field_0x088 = true;
}

void dAcGirahimu2_c::initializeState_ComeOnGuard() {
    if (isNearHalfHealth()) {
        mSubstateTimer = 30;
    } else {
        mSubstateTimer = 50;
    }
    mMovePoseTimer = 5;
    field_0xD6C = 9;
    f32 f0 = 1.0f;
    setAnmHead("RarmU", ANM_PoseC, m3d::PLAY_MODE_4, f0, 1.0f);
    setAnmBody("RarmU", ANM_PoseC, m3d::PLAY_MODE_4, f0, 1.0f);
    if (!isRightArmPose()) {
        setAnmShoulderR("WaitBtD", ANM_WaitBtC, m3d::PLAY_MODE_4, f0, 1.0f);
    } else if (!isLeftArmPose()) {
        setAnmShoulderL("WaitBtD", ANM_WaitBtC, m3d::PLAY_MODE_4, f0, 1.0f);
    }
    setAnmHip("WalkBt", ANM_Walk, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mMdlBody.play();
    if (isRightArmPose()) {
        isLeftArmPose();
    }
}
void dAcGirahimu2_c::executeState_ComeOnGuard() {
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    if (mSubstateTimer <= 0) {
        if (mDistanceToPlayer <= 350.0f) {
            changeState(StateID_Counter);
            return;
        } else {
            vt_0x220();
            return;
        }
    } else if (mDistanceToPlayer >= 300.0f) {
        vt_0x220();
        return;
    }
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    fn_226_B960(5.0f);
}
void dAcGirahimu2_c::finalizeState_ComeOnGuard() {}

void dAcGirahimu2_c::initializeState_G_SwordDemo() {
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->setFlag(mRoomID, getFromParams(8, 0xFF));
    }

    mCallback.mSlerpFactorTarget = 1.0f;
    mSubstate = 0;
    mCallback.mSlerpFactor = 1.0f;
    mCyl0.SetR(30.0f);
    mCcBody.OnTgSet();
    mCcHand.ClrTgSet();
    mMdlSwordA.mProc.setColor1(mColor(0, 0, 0, 0xFF));
    mMdlSwordA.mProc.setColor2(mColor(0, 0, 0, 0xFF));
    mMdlSwordB.mProc.setColor1(mColor(0, 0, 0, 0xFF));
    mMdlSwordB.mProc.setColor2(mColor(0, 0, 0, 0xFF));
    mbCatchingPlayerSword = false;
    mbSpawnedSwords = true;
    mHealth = 600;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x04E = false;
    vt_0x1FC();
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, getFromParams(0, 0xFF) + 1);
    }
}
void dAcGirahimu2_c::executeState_G_SwordDemo() {
    dCamera_c *pCamera = dScGame_c::getCamera();
    mVec3_c v0 = mPosition;
    v0.y += 150.f;
    mVec3_c v1(0.0f, 200.0f, 300.0f);
    v1.rotY(mRotation.y);
    v1 += mPosition;

    mMtx_c transform;
    mVec3_c headTranslation(0.0f, 0.0f, 0.0f);
    nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("Head").GetID(), transform);
    transform.getTranslation(headTranslation);

    mVec3_c handRTranslation;
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("HandR").GetID(), transform);
    transform.getTranslation(handRTranslation);

    mVec3_c handLTranslation;
    mMdlBody.getNodeWorldMtx(mdl.GetResNode("HandL").GetID(), transform);
    transform.getTranslation(handLTranslation);

    mVec3_c midpoint = handRTranslation + handLTranslation;
    midpoint *= 0.5f;

    switch (mSubstate) {
        case 0: {
            {
                Event event("GirahimuSwordDemo", 100, 0x100001, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
            }

            if (EventManager::isInEvent(this, "GirahimuSwordDemo")) {
                setAnm("WaitA", ANM_WaitA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                mSubstate++;
                mCameraTarget.set(headTranslation);
                v1.set(0.0f, 0.0f, 150.0f);
                v1.rotY(mRotation.y);
                v1 += headTranslation;
                mCameraPosition.set(v1);
                pCamera->setEventCamView(mCameraTarget, mCameraPosition, 70.0f, 0.0f);
                mSubstateTimer = 30;
            }
        } break;
        case 1: {
            mRotation.y = cLib::targetAngleY(mPosition, mStartingPos);

            mVec3_c v(0.0f, 0.0f, 500.0f);
            v.rotY(mRotation.y);
            v += mPosition;
            mAng3_c a(0, mRotation.y + 0x8000, 0);
            dAcPy_c::GetLinkM()->setPosRot(&v, &a, false, 0, 0);

            mCameraTarget.set(headTranslation);
            v1.set(0.0f, 0.0f, 150.0f);
            v1.rotY(mRotation.y);
            v1 += headTranslation;
            mCameraPosition.set(v1);

            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 35.0f, 0.0f);
            if (mSubstateTimer <= 0) {
                mAnmIDBody = ANM_NONE;
                setAnm(
                    "WaitA", ANM_WaitA, "WaitA", ANM_WaitA, "FaceTongue", ANM_FaceTongue, m3d::PLAY_MODE_4, 15.0f, 1.0f
                );
                mSubstateTimer = 50;
                mSubstate++;
            }
        } break;
        case 2: {
            cLib::addCalcPos2(&mCameraPosition, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&mCameraTarget, midpoint, 0.2f, 10.0f);
            if (mSubstateTimer <= 0) {
                mSubstate++;
                setAnm("TwoSwordAppear", ANM_SwordAppear, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } break;
        case 3: {
            cLib::addCalcPos2(&mCameraPosition, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&mCameraTarget, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 35.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
                mbHoldingSwordA = true;
                mbHoldingSwordB = true;
                mSubstate++;
            }
        } break;
        case 4: {
            sLib::chase(&mSwordScaleY, 1.0f, 0.1f);
            cLib::addCalcPos2(&mCameraPosition, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&mCameraTarget, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 35.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].checkFrame(53.0f)) {
                mSubstate++;
            }
        } break;
        case 5: {
            v1.set(0.0f, 200.0f, 500.0f);
            v1.rotY(mRotation.y);
            v1 += mPosition;
            cLib::addCalcPos2(&mCameraTarget, midpoint, 0.2f, 10.0f);
            cLib::addCalcPos2(&mCameraPosition, v1, 0.2f, 100.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 35.0f, 0.0f);
            if (sLib::chase(&mSwordScaleY, 1.0f, 0.1f) && mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("WaitBtC", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
                mSubstateTimer = 50;
                mSubstate++;
            }
        } break;
        case 6: {
            v1.set(0.0f, 200.0f, 500.0f);
            v1.rotY(mRotation.y);
            v1 += mPosition;
            cLib::addCalcPos2(&mCameraPosition, v1, 0.2f, 10.0f);
            cLib::addCalcPos2(&mCameraTarget, midpoint, 0.2f, 100.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 35.0f, 0.0f);
            if (mSubstateTimer <= 0) {
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                changeState(StateID_G_SwordWait);
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_G_SwordDemo() {
    mCcBody.OnTgSet();
    mSubstate = 0;
}

void dAcGirahimu2_c::initializeState_G_SwordDamage() {
    mCcBody.OnTgSet();
    setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = false;
    mSubstate = 0;
    mRecoverDamageTimer = 150;
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    field_0xD6C = 7;
    mConsecutiveHitCount = 0;
    vt_0x1FC();
}
void dAcGirahimu2_c::executeState_G_SwordDamage() {
    mbStandardDamage = true;
    if (--mRecoverDamageTimer <= 0) {
        mConsecutiveHitCount = 0;
        if (mDistanceToPlayer < 500.0f) {
            changeState(StateID_BackStep);
        } else {
            vt_0x220();
        }
        return;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        setAnm("DamageWait", ANM_DamageWait, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    if (mCcBody.ChkTgHit() && mCcBody.GetTgAtHitType() == AT_TYPE_SWORD) {
        changeState(StateID_G_SwordPiyori);
    }
}
void dAcGirahimu2_c::finalizeState_G_SwordDamage() {
    mConsecutiveHitCount = 0;
}

void dAcGirahimu2_c::initializeState_UpWarp() {
    setAnm("Jump", ANM_StepStart, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
    setFrame(6.0f);
    mSubstateTimer = 3;
    mCallback.field_0x088 = true;
    mSubstate = 0;
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    field_0xD6C = 0;
    field_0xD7E = false;
}
void dAcGirahimu2_c::executeState_UpWarp() {
    mCcBody.ClrTgSet();
    switch (mSubstate) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].isStop() && sLib::chase(&mWarpScale.x, 0.0f, 0.5f)) {
                mMtx_c hipTransform;
                nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
                mMdlBody.getNodeWorldMtx(mdl.GetResNode("Hip").GetID(), hipTransform);
                mVec3_c hipPos;
                hipTransform.multVec(hipPos, hipPos);
                hipPos.y += 100.0f;
                if (mbSpawnedSwords && getHealth() == 0) {
                    startSound(SE_Girahim_MAGIC_DISAPPEAR_STAGE);
                } else {
                    startSound(SE_Girahim_MAGIC_DISAPPEAR);
                }
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_684_, hipPos, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
                mSubstate++;

                mWarpScale.set(0.0f, 0.0f, 0.0f);
                mVec3_c position = dAcPy_c::GetLink()->mPosition;
                mVec3_c adj(0.0f, 600.0f, 50.0f);
                adj.rotY(dAcPy_c::GetLink()->mRotation.y);
                position += adj;
                setPosition(position);
                setOldPosition(position);
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                mSubstateTimer = 40;
                setAnm("PoseUpper", ANM_PoseUpper, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
            }
        } break;
        case 1: {
            if (dAcPy_c::GetLink()->checkCurrentAction(98 /* BACKFLIP */) ||
                dAcPy_c::GetLink()->checkFlags0x350(0x80000)) {
                if (!field_0xD7E) {
                    mTargetPlayerPos.set(dAcPy_c::GetLink()->mPosition);
                    field_0xD7E = true;
                }
            }
            mVelocity.set(0.0f, 0.0f, 0.0f);
            if (mSubstateTimer <= 0) {
                mWarpScale.set(0.0f, 1.05f, 1.05f);
                mSubstate++;
                changeState(StateID_UpAttack);
                return;
            }
        } break;
    }
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
}
void dAcGirahimu2_c::finalizeState_UpWarp() {
    mSubstate = 0;
    mCcBody.OnTgSet();
}

void dAcGirahimu2_c::initializeState_UpAttack() {
    mSubstate = 0;
    setAnm("PoseUpper", ANM_PoseUpper, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mbOnGround = false;
    mVec3_c pos = dAcPy_c::GetLink()->mPosition;
    if (field_0xD7E) {
        pos.set(mTargetPlayerPos);
    }
    mVec3_c adj(0.0f, 600.0f, 0.0f);
    adj.rotY(dAcPy_c::GetLink()->mRotation.y);
    pos += adj;
    setPosition(pos);
    setOldPosition(pos);
    mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
    field_0xD6C = 6;
    mCcBody.ClrTgSet();
    mCcHand.ClrTgSet();
    mCallback.field_0x04E = false;
    mCallback.field_0x088 = false;
    mSubstateTimer = 2;
    mAcceleration = -8.0f;
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    mMdlSwordA.fn_8006B7A0(0x210000);
    mMdlSwordB.fn_8006B7A0(0x210000);
}
void dAcGirahimu2_c::executeState_UpAttack() {
    mbStandardDamage = true;
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    switch (mSubstate) {
        case 0: {
            if (mSubstateTimer > 0) {
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
            }
            if (dAcPy_c::GetLink()->checkCurrentAction(98 /* BACKFLIP */) ||
                dAcPy_c::GetLink()->checkFlags0x350(0x80000) || dAcPy_c::GetLink()->mSpeed >= 24.0f) {
                field_0xD7E = true;
            }
            if (!field_0xD7E) {
                sLib::chase(&mPosition.x, dAcPy_c::GetLink()->mPosition.x, 20.0f);
                sLib::chase(&mPosition.z, dAcPy_c::GetLink()->mPosition.z, 20.0f);
            }
            mMdlSwordA.enable();
            mMdlSwordB.enable();
            mCcBody.ClrTgSet();
            mCcHand.ClrTgSet();
            mbOnGround = false;
            if (mAcch.ChkGndHit()) {
                mVelocity.set(0.0f, 0.0f, 0.0f);
                setAnm("PoseUpperAttack", ANM_PoseUpperAttack, nullptr, m3d::PLAY_MODE_4, 1.0f, 1.0f);
                field_0xD6C = 5;
                mSubstate++;
            }
        } break;
        case 1: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
                mCallback.field_0x088 = true;
            }

            if (mAnmChrs[ANMIDX_Body].getFrame() >= 60.0f) {
                field_0xD6C = 6;
                mCcBody.OnTgSet();
                mCyl0.OnTgSet();
            } else {
                mCcBody.OnTgSet();
                mCyl0.ClrTgSet();
            }
            mMdlSwordA.disable();
            mMdlSwordB.disable();
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                vt_0x220();
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_UpAttack() {
    mMdlSwordA.disable();
    mMdlSwordB.disable();
    mAcceleration = -6.0f;
    mSubstate = 0;
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    field_0xD7C = true;
    mMdlSwordA.fn_8006B7A0(0x10000);
    mMdlSwordB.fn_8006B7A0(0x10000);
}

void dAcGirahimu2_c::initializeState_Counter() {
    field_0xD7E = false;
    if (mAnmIDShoulderL == ANM_PoseL && mAnmIDShoulderR == ANM_PoseR) {
        setAnm("PoseLRAttack", ANM_PoseLRAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDShoulderL == ANM_PoseC2 && mAnmIDShoulderR == ANM_PoseC) {
        setAnm("PoseUDAttack", ANM_PoseUDAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDShoulderL == ANM_PoseL && mAnmIDShoulderR == ANM_PoseC) {
        setAnm("PoseLUAttack", ANM_PoseLUAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDShoulderL == ANM_PoseC && mAnmIDShoulderR == ANM_PoseR) {
        setAnm("PoseRUAttack", ANM_PoseRUAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        field_0xD6C = 6;
        int rnd = cM::rndInt(100);
        if (rnd >= 75) {
            setAnm("PoseLRAttack", ANM_PoseLRAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);

        } else if (rnd >= 50) {
            setAnm("PoseUDAttack", ANM_PoseUDAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);

        } else if (rnd >= 25) {
            setAnm("PoseLUAttack", ANM_PoseLUAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);

        } else {
            setAnm("PoseRUAttack", ANM_PoseRUAttack, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        }
    }
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
}
void dAcGirahimu2_c::executeState_Counter() {
    sLib::chase(&mWarpScale.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    if (mAnmChrs[ANMIDX_Body].getFrame() >= 16.0f && mAnmChrs[ANMIDX_Body].getFrame() <= 20.0f) {
        mMdlSwordA.enable();
        mMdlSwordB.enable();
    } else {
        mMdlSwordA.disable();
        mMdlSwordB.disable();
    }

    if (mAnmChrs[ANMIDX_Body].getFrame() <= 20.0f) {
        field_0xD6C = 5;
    } else {
        field_0xD6C = 6;
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimu2_c::finalizeState_Counter() {
    mMdlSwordA.disable();
    mMdlSwordB.disable();
    mCcBody.OnTgSet();
    mbShortStun = false;
}

void dAcGirahimu2_c::initializeState_GuardJustCounter() {
    field_0xD7E = false;
    field_0xD6C = 6;
    setAnm("GuardTwoSword", ANM_GuardTwoSword, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
}
void dAcGirahimu2_c::executeState_GuardJustCounter() {
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    if (mAnmChrs[ANMIDX_Body].getFrame() >= 42.0f && mAnmChrs[ANMIDX_Body].getFrame() <= 49.0f) {
        mMdlSwordA.enable();
    } else {
        mMdlSwordA.disable();
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimu2_c::finalizeState_GuardJustCounter() {
    mMdlSwordA.disable();
    mMdlSwordB.disable();
    mCcBody.OnTgSet();
    mbShortStun = false;
}

void dAcGirahimu2_c::initializeState_TwoSwordAttack() {
    field_0xD7E = false;
    field_0xD6C = 11;
    setAnm("AttackTwoSword", ANM_AttackTwoSword, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    mbShortStun = true;
}
void dAcGirahimu2_c::executeState_TwoSwordAttack() {
    s32 _weird_zero = 0;
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);

    if (mAnmChrs[ANMIDX_Body].checkFrame(30.0f)) {
        mSpeed = _weird_zero + 100.0f;
    }
    if (mAnmChrs[ANMIDX_Body].getFrame() >= 31.0f && mAnmChrs[ANMIDX_Body].getFrame() <= 35.0f) {
        mMdlSwordA.enable();
    } else {
        mMdlSwordA.disable();
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
        mSpeed = _weird_zero + 100.0f;
    }
    if (mAnmChrs[ANMIDX_Body].getFrame() >= 40.0f && mAnmChrs[ANMIDX_Body].getFrame() <= 46.0f) {
        mMdlSwordB.enable();
    } else {
        mMdlSwordB.disable();
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(47.0f)) {
        field_0xD6C = 6;
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimu2_c::finalizeState_TwoSwordAttack() {
    mMdlSwordA.disable();
    mMdlSwordB.disable();
    mCcBody.OnTgSet();
    mbShortStun = false;
    mSpeed = 0.0f;
}

void dAcGirahimu2_c::initializeState_G_SwordPiyori() {
    mCcBody.OnTgSet();
    setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = false;
    mSubstate = 0;
    mSubstateTimer = 60;
    if (mbShortStun || isNearHalfHealth()) {
        mSubstateTimer = 30;
        mbShortStun = false;
    }
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
    field_0xD6C = 7;
    mConsecutiveHitCount = 0;
}
void dAcGirahimu2_c::executeState_G_SwordPiyori() {
    if (mSubstateTimer <= 0 || mConsecutiveHitCount >= 4) {
        mConsecutiveHitCount = 0;
        if (mDistanceToPlayer < 500.0f) {
            changeState(StateID_BackStep);
        } else {
            vt_0x220();
        }
    } else if (mAnmChrs[ANMIDX_Body].isStop()) {
        setAnm("DamageWait", ANM_DamageWait, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        mCallback.field_0x088 = false;
    }
}
void dAcGirahimu2_c::finalizeState_G_SwordPiyori() {
    mConsecutiveHitCount = 0;
}

void dAcGirahimu2_c::initializeState_Death() {
    mSubstate = 0;
    mCallback.mSlerpFactorTarget = 1.0f;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    field_0x1830 = 200.0f;
    mCallback.field_0x088 = false;
    field_0xD6C = 0;
    mCollider.ClrTg();
    for (int i = 0; i < 15; ++i) {
        if (mKnives[i].isLinked()) {
            mKnives[i].get()->field_0x78E = true;
        }
    }
}
void dAcGirahimu2_c::executeState_Death() {
    mCallback.field_0x088 = false;
    dCamera_c *pCamera = dScGame_c::getCamera();
    mVec3_c v0(mPosition);
    v0.y = 116.0f;
    mVec3_c v1(0.0f, 148.0f, 506.0f);
    mVec3_c v2(0.0f, 77.0f, 600.0f);

    mMtx_c headTransform;
    mVec3_c headTranslation(0.0f, 0.0f, 0.0f);
    mMdlBody.getNodeWorldMtx(mMdlBody.getResMdl().GetResNode("Head").GetID(), headTransform);
    headTransform.getTranslation(headTranslation);
    headTranslation.y = 135.0f;

    switch (mSubstate) {
        case 0: {
            {
                // ISSUE 1: The event needs to be pushed on the stack and switch with the ident matrix in case 3
                // This can be fixed by making the event be generated within an inline, but then that the load for the
                // Event storage(r3) and the event name(r4) are switched
                Event event("GirahimuDeathDemo", 100, 0x100001, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
            }
            if (EventManager::isInEvent(this, "GirahimuDeathDemo")) {
                // ...Enough of this foolishness...
                mFlowMgr.triggerEntryPoint(304, 19, 0, 0);
                setAnm("End2A", ANM_EndA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                mSubstate++;

                v0.set(mStartingPos);
                v0.y = 163.0f;
                mCameraTarget.set(v0);
                mCameraPosition.set(v1);

                // ISSUE 2: the mCameraTarget load gets optimized in combination with v0.
                // changing mCameraTarget -> v0 here matches other code gen, but isnt the right parameter
                pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
            }
        } break;
        case 1: {
            setPosition(mStartingPos);
            setOldPosition(mStartingPos);
            mRotation.y = 0;
            mVec3_c v5(0.0f, 0.0f, 1300.0f);
            v5.rotY(mRotation.y);
            v5 += mPosition;
            mAng3_c a0(0, mRotation.y + 0x8000, 0);
            dAcPy_c::GetLinkM()->setPosRot(&v5, &a0, false, 0, 0);
            mSubstate++;
            v0.set(mStartingPos);
            v0.y = 163.0f;
            mCameraTarget.set(v0);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
        } break;
        case 2: {
            cLib::addCalcPos2(&mCameraTarget, headTranslation, 0.2f, 200.0f);
            mCameraPosition.set(v1);
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                mCameraTarget.set(109.0f, 124.0f, -637.0f);
                mCameraPosition.set(-54.0f, 155.0f, 50.0f);
                setAnm("End2BLoop", ANM_EndBLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                mSubstate++;
            }
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
        } break;
        case 3: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                setAnm("End2C", ANM_EndC, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                mCameraTarget.set(0.0f, 131.0f, -359.0f);
                mCameraPosition.set(0.0f, 153.0f, 92.0f);
                mMtx_c ident;
                MTXIdentity(ident);
                dStageMgr_c::GetInstance()->procfn_800192F0(0xDC, ident, 0x82);
                mSubstate++;
            }
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
        } break;
        case 4: {
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("End2DLoop", ANM_EndDLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                // It shouldn't matter how powerful your sword is, you are still nothing...
                mFlowMgr.triggerEntryPoint(304, 20, 0, 0);
                mSubstate++;
            }
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
        } break;
        case 5: {
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                // You filthy scamp! You have awakened a wrath that will burn for eons! ...
                mFlowMgr.triggerEntryPoint(304, 21, 0, 0);
                mSubstate++;
            }
        } break;
        case 6: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                mSubstate++;
                setAnm("End2E", ANM_EndE, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } break;
        case 7: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(35.0f)) {
                mMdlSwordB.setProcActive(true);
            }
            cLib::addCalcPos2(&mCameraTarget, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&mCameraPosition, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 90.0f) {
                sLib::chase(&mWarpScale.x, 0.0f, 0.2);
            }
            if (mAnmChrs[ANMIDX_Body].isStop() && mWarpScale.x <= 0.0f) {
                mMdlSwordB.setProcActive(false);
                mSubstate++;
                mSubstateTimer = 40;
            }

        } break;
        case 8: {
            cLib::addCalcPos2(&mCameraTarget, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&mCameraPosition, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
            if (mSubstateTimer <= 0) {
                mSubstate++;
            }
        } break;
        case 9: {
            cLib::addCalcPos2(&mCameraTarget, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&mCameraPosition, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(mCameraTarget, mCameraPosition, 20.0f, 0.0f);
            if (sLib::chase(&dLightEnv_c::GetInstance().GetOverrideSpf().mRatio, 0.0f, 0.012f)) {
                DungeonflagManager::sInstance->setFlag(3);
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                deleteRequest();
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_Death() {
    mCcBody.OnTgSet();
    mSubstate = 0;
}

void dAcGirahimu2_c::spawnKnifeVolley(int idxStart, int idxEnd) {
    startSound(SE_OGhKf_GROUP_APPEAR);
    dAcObjGirahimuKnife_c::Pattern_e pattern;
    mVec3_c knifeOffset;
    mVec3_c spacing;
    int rnd = cM::rndInt(90);
    f32 baseYOffset = 120.0f;
    if (rnd >= 60) {
        pattern = dAcObjGirahimuKnife_c::PATTERN_R_L;
        knifeOffset.set(-100.0f, baseYOffset, 0.0f);
        spacing.set(50.0f, 0.0f, 0.0f);
    } else if (rnd >= 30) {
        pattern = dAcObjGirahimuKnife_c::PATTERN_U_D;
        knifeOffset.set(0.0f, baseYOffset + 100.0f + 10.0f, 0.0f);
        spacing.set(0.0f, -50.0f, 0.0f);
    } else {
        if (cM::rndInt(100) > 50) {
            pattern = dAcObjGirahimuKnife_c::PATTERN_RU_LD;
            knifeOffset.set(100.0f, baseYOffset + 100.0f + 10.0f, 0.0f);
            spacing.set(-50.0f, -50.0f, 0.0f);
        } else {
            pattern = dAcObjGirahimuKnife_c::PATTERN_RD_LU;
            knifeOffset.set(-100.0f, baseYOffset + 100.0f + 10.0f, 0.0f);
            spacing.set(50.0f, -50.0f, 0.0f);
        }
    }
    knifeOffset.z = 80.0f;
    for (int i = idxStart; i < idxEnd; ++i) {
        mVec3_c spawnPos = mPosition;
        mVec3_c adjustedKnifeOffset = knifeOffset;
        adjustedKnifeOffset.rotY(mRotation.y);
        spawnPos += adjustedKnifeOffset;
        dAcObjGirahimuKnife_c *pKnife;
        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
            pKnife = mKnives[i].get();
        } else {
            pKnife = static_cast<dAcObjGirahimuKnife_c *>(
                create(fProfile::OBJ_GH_KNIFE, mRoomID, 0, &spawnPos, nullptr, nullptr, 0xFFFFFFFF)
            );
            mKnives[i].link(pKnife);
        }
        if (pKnife != nullptr) {
            mKnives[i].get()->setTarget(knifeOffset);
            knifeOffset += spacing;
            mKnives[i].get()->setPattern(pattern);
            mKnives[i].get()->setState(5);
        }
    }
}

void dAcGirahimu2_c::spawnKnifeSurrond(bool bVertical) {
    dAcObjGirahimuKnife_c::Pattern_e pattern = dAcObjGirahimuKnife_c::PATTERN_SURROUND_H;
    dAcPy_c::GetLinkM()->startSound(SE_OGhKf_GROUP_APPEAR);
    if (bVertical) {
        pattern = dAcObjGirahimuKnife_c::PATTERN_SURROUND_V;
    }

    for (int i = 0; i < 15; ++i) {
        mVec3_c spawnPos = dAcPy_c::GetLink()->mPosition;
        spawnPos.y += 400.0f;
        dAcObjGirahimuKnife_c *pKnife;
        pKnife = static_cast<dAcObjGirahimuKnife_c *>(
            create(fProfile::OBJ_GH_KNIFE, mRoomID, 0, &spawnPos, nullptr, nullptr, 0xFFFFFFFF)
        );
        mKnives[i].link(pKnife);

        if (pKnife != nullptr) {
            mKnives[i].get()->setPattern(pattern);
            mKnives[i].get()->setState(6);
        }
    }
}

void dAcGirahimu2_c::initializeState_DeathMiniGame() {
    mSubstate = 0;
    mCallback.mSlerpFactorTarget = 1.0f;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    field_0x1830 = 200.0f;
    mCallback.field_0x088 = false;
    field_0xD6C = 0;
    mCollider.ClrTg();
    notifyTgSwordBattleGame();
    setAnm("End2A", ANM_EndA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
}
void dAcGirahimu2_c::executeState_DeathMiniGame() {
    switch (mSubstate) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("End2BLoop", ANM_EndE, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                mSubstate++; // lol this will keep going up
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_DeathMiniGame() {
    mCcBody.OnTgSet();
    mSubstate = 0;
}

void dAcGirahimu2_c::fn_228_4A30() {
    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);

    cCcD_Obj *pCc = mMdlSwordA.mCcList.findAtHit();
    if (pCc != nullptr) {
        if (pCc->ChkAtShieldReflect()) {
            if (mAnmIDBody == ANM_GuardTwoSword) {
                changeState(StateID_G_SwordPiyori);
            } else {
                changeState(StateID_GuardJustCounter);
            }
            return;
        }
    } else {
        pCc = mMdlSwordB.mCcList.findAtHit();
        if (pCc != nullptr) {
            if (pCc->ChkAtShieldReflect()) {
                if (mAnmIDBody == ANM_GuardTwoSword) {
                    changeState(StateID_G_SwordPiyori);
                } else {
                    changeState(StateID_GuardJustCounter);
                }
                return;
            }
        }
    }
    pCc = mCollider.findTgHit();
    if (pCc != nullptr) {
        if (pCc->GetTgAtHitType() == AT_TYPE_SWORD) {
            adjustSpineHitRotation(pCc);
        }
    }
}

void dAcGirahimu2_c::fn_228_4C20() {
    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);

    cCcD_Obj *pCc = mCollider.findTgHit();
    if (pCc != nullptr) {
        if (pCc->GetTgAtHitType() == AT_TYPE_DAMAGE) {
            u16 health = getHealth();
            if (health != 0) {
                health--;
            }
            mHealth = health;
            changeState(StateID_KnifeDamage);
        }
    }
}

bool dAcGirahimu2_c::fn_228_4CD0(s32 cutDir) {
    switch (cutDir) {
        case CUT_DIR_U:
        case CUT_DIR_D:
            if ((field_0x2210 & ~8) == 8) {
                return true;
            }
            break;
        case CUT_DIR_RD:
        case CUT_DIR_LU:
            if ((field_0x2210 & ~4) == 4) {
                return true;
            }
            break;
        case CUT_DIR_R:
        case CUT_DIR_L:
            if ((field_0x2210 & ~1) == 1) {
                return true;
            }
            break;
        case CUT_DIR_RU:
        case CUT_DIR_LD:
            if ((field_0x2210 & ~2) == 2) {
                return true;
            }
            break;
    }
    return false;
}

void dAcGirahimu2_c::vt_0x220() {
    mbPoseFreeRequest = false;
    if (mbSpawnedSwords) {
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isCircleWait()) {
                changeState(StateID_KnifeAttack);
                return;
            }
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                changeState(StateID_KnifePreAttack);
                return;
            }
        }

        if (getStateID().isEqual(StateID_BackStep)) {
            changeState(StateID_G_SwordWait);
            return;
        }

        if (mbSwordAtHit && cM::rndInt(100) > 80) {
            changeState(StateID_HitAction);
            return;
        } else {
            changeState(StateID_G_SwordWait);
            return;
        }
    } else if (getHealth() > 0) {
        u16 waitingCount = 0;
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                waitingCount++;
            }
        }

        if (getStateID().isEqual(StateID_BackStep)) {
            if (waitingCount != 0) {
                changeState(StateID_WaitKnifeAttack);
                return;
            } else {
                changeState(StateID_Walk);
                mSubstateTimer = 0;
                return;
            }
        } else {
            if ((waitingCount == 2 && field_0x220C == 0) || (waitingCount == 4 && field_0x220C == 1)) {
                changeState(StateID_Wait);
                return;
            }
            if (getStateID().isEqual(StateID_Catch)) {
                mCreateKnifeTimer = 20;
                changeState(StateID_Wait);
                return;
            } else {
                changeState(StateID_CreateSpinKnife);
            }
        }
    } else {
        if (getStateID().isEqual(StateID_BackStep)) {
            changeState(StateID_G_SwordDemo);
            return;
        } else {
            changeState(StateID_BackStep);
            return;
        }
    }
}

bool dAcGirahimu2_c::fn_228_5110() {
    mbPoseFreeRequest = false;
    int rnd = cM::rndInt(100);

    if (mDistanceToPlayer >= 400.0f) {
        if (rnd >= 70 && mCurrentAttack != 0) {
            mCurrentAttack = 0;
            if (cM::rndInt(90) > 50) {
                if (cM::rndInt(100) > 50) {
                    changeState(StateID_FrontWarp);
                } else {
                    changeState(StateID_BackWarp);
                }
            } else {
                changeState(StateID_UpWarp);
            }
            return true;
        } else if (rnd >= 30) {
            if (mCurrentAttack != 3) {
                mCurrentAttack = 3;
                changeState(StateID_CreateKnife);
                return true;
            }
        } else {
            if (mCurrentAttack != 5 && mDistanceToPlayer <= 300.0f) {
                mCurrentAttack = 5;
                changeState(StateID_TwoSwordAttack);
                return true;
            } else {
                mCurrentAttack = 4;
                if (mAnmIDBody != ANM_Walk) {
                    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
                }
            }
        }
    } else {
        if (rnd >= 75 && mCurrentAttack != 2) {
            mCurrentAttack = 2;
            if (cM::rndInt(100) > 50) {
                mbPoseFreeRequest = true;
                changeState(StateID_ComeOn);

            } else {
                changeState(StateID_FrontAttack);
            }
            return true;
        }
        if (rnd >= 60 && mCurrentAttack != 0) {
            mCurrentAttack = 0;
            changeState(StateID_UpWarp);
            return true;
        }
        if (rnd >= 25 && mCurrentAttack != 3) {
            mCurrentAttack = 3;
            changeState(StateID_CreateKnife);
            return true;
        }
        mCurrentAttack = 5;
        changeState(StateID_TwoSwordAttack);
        return true;
    }
    return false;
}

void dAcGirahimu2_c::fn_228_5400() {
    if (mConsecutiveSwordHitCount >= 2) {
        mCcBody.SetR(15.0f);
    } else {
        mCcBody.SetR(45.0f);
    }

    someEnemyDamageCollisionStuffMaybe(mCollider, nullptr);
    fn_228_5740();
    vt_0x1F4();

    cCcD_Obj *pCc = mCollider.findTgHit();
    if (pCc != nullptr && pCc->GetTgAtHitType() == AT_TYPE_SWORD) {
        if (pCc->GetTgAtCutDir() == CUT_DIR_STAB) {
            changeState(StateID_TwoSwordAttack);
        } else if (mConsecutiveSwordHitCount >= 2) {
            if (pCc == &mCcSwordR || pCc == &mCcSwordL) {
                getSoundSource()->startSoundAtPosition(SE_Girahim_SWORD_HIT_SWORD, pCc->GetTgHitPos());
                changeState(StateID_Counter);
            } else {
                adjustSpineHitRotation(pCc);
                changeState(StateID_G_SwordDamage);
            }
        } else if (mConsecutiveSwordHitCount == 1) {
            if (pCc == &mCcSwordR || pCc == &mCcSwordL) {
                getSoundSource()->startSoundAtPosition(SE_Girahim_SWORD_HIT_SWORD, pCc->GetTgHitPos());
                changeState(StateID_Counter);
            } else if (pCc->GetTgShieldType() != AT_TYPE_NONE) {
                getSoundSource()->startSoundAtPosition(SE_Girahim_SWORD_HIT_SWORD, pCc->GetTgHitPos());
                fn_228_5770(pCc->GetTgAtCutDir());
                changeState(StateID_ComeOnGuard);
                mConsecutiveSwordHitCount++;
            } else {
                adjustSpineHitRotation(pCc);
                changeState(StateID_G_SwordDamage);
            }
        } else if (pCc->GetTgShieldType() != AT_TYPE_NONE) {
            getSoundSource()->startSoundAtPosition(SE_Girahim_SWORD_HIT_SWORD, pCc->GetTgHitPos());
            fn_228_5770(pCc->GetTgAtCutDir());
            changeState(StateID_ComeOnGuard);
            mConsecutiveSwordHitCount++;
        } else {
            adjustSpineHitRotation(pCc);
            changeState(StateID_G_SwordDamage);
        }
    }
}

void dAcGirahimu2_c::fn_228_5740() {
    s32 cutDir = mPlayerAttackDir;
    if (mConsecutiveSwordHitCount < 2 && cutDir != CUT_DIR_STAB) {
        mCcBody.SetTgShieldType(~AT_TYPE_DAMAGE);
    }
}

bool dAcGirahimu2_c::fn_228_5770(s32 cutDir) {
    bool bRightArm = false;
    f32 rate = 1.0f;
    switch (cutDir) {
        case CUT_DIR_U:
            if (mAnmIDShoulderR == ANM_PoseC || mAnmIDShoulderL == ANM_PoseC) {
                return false;
            }
            if (!isRightArmPose()) {
                bRightArm = true;
                setAnmShoulderR("RarmU", ANM_PoseC, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            } else {
                setAnmShoulderL("LarmU", ANM_PoseC, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            }
            break;
        case CUT_DIR_D:
            if (mAnmIDShoulderR == ANM_PoseC2 || mAnmIDShoulderL == ANM_PoseC2) {
                return false;
            }
            if (!isRightArmPose()) {
                bRightArm = true;
                setAnmShoulderR("RarmD", ANM_PoseC2, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            } else {
                setAnmShoulderL("LarmD", ANM_PoseC2, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            }
            break;
        case CUT_DIR_LU:
            if (mAnmIDShoulderR == ANM_PoseC || mAnmIDShoulderL == ANM_PoseC) {
                return false;
            }
            if (mAnmIDShoulderR == ANM_PoseR || mAnmIDShoulderL == ANM_PoseR) {
                return false;
            }
            if (!isRightArmPose()) {
                bRightArm = true;
                setAnmShoulderR("RarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            } else {
                setAnmShoulderL("LarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            }
            break;
        case CUT_DIR_RD:
            if (mAnmIDShoulderR == ANM_PoseC2 || mAnmIDShoulderL == ANM_PoseC2) {
                return false;
            }
            if (mAnmIDShoulderR == ANM_PoseL || mAnmIDShoulderL == ANM_PoseL) {
                return false;
            }
            if (!isRightArmPose()) {
                setAnmShoulderL("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            }
            break;
        case CUT_DIR_L:
            if (mAnmIDShoulderR == ANM_PoseR || mAnmIDShoulderL == ANM_PoseR) {
                return false;
            }
            if (!isRightArmPose()) {
                bRightArm = true;
                setAnmShoulderR("RarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            } else {
                setAnmShoulderL("LarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            }
            break;
        case CUT_DIR_R:
            if (mAnmIDShoulderR == ANM_PoseL || mAnmIDShoulderL == ANM_PoseL) {
                return false;
            }
            if (!isRightArmPose()) {
                setAnmShoulderL("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            }
            break;
        case CUT_DIR_LD:
            if (mAnmIDShoulderR == ANM_PoseC2 || mAnmIDShoulderL == ANM_PoseC2) {
                return false;
            }
            if (mAnmIDShoulderR == ANM_PoseR || mAnmIDShoulderL == ANM_PoseR) {
                return false;
            }
            if (!isRightArmPose()) {
                bRightArm = true;
                setAnmShoulderR("RarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            } else {
                setAnmShoulderL("LarmR", ANM_PoseR, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            }
            break;
        case CUT_DIR_RU:
            if (mAnmIDShoulderR == ANM_PoseC || mAnmIDShoulderL == ANM_PoseC) {
                return false;
            }
            if (mAnmIDShoulderR == ANM_PoseL || mAnmIDShoulderL == ANM_PoseL) {
                return false;
            }
            if (!isRightArmPose()) {
                setAnmShoulderL("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            }
            break;
    }

    if (bRightArm) {
        switch (mAnmIDShoulderR) {
            case ANM_PoseC:
                mCallback.field_0x245 = true;
                mCallback.mElbowRRotationYAmplitude = mAng(0x1000);
                mCallback.mElbowRRotationYPhase = 0;
                break;
            case ANM_PoseL:
                mCallback.field_0x245 = true;
                mCallback.mElbowRRotationYAmplitude = mAng(0x1000);
                mCallback.mElbowRRotationYPhase = 0;
                break;
            case ANM_PoseR:
                mCallback.field_0x245 = false;
                mCallback.mElbowRRotationYAmplitude = mAng(0x1000);
                mCallback.mElbowRRotationYPhase = 0;
                break;
            case ANM_PoseC2:
                mCallback.field_0x245 = true;
                mCallback.mElbowRRotationYAmplitude = mAng(0x1000);
                mCallback.mElbowRRotationYPhase = 0;
                break;
        }
    } else {
        switch (mAnmIDShoulderL) {
            case ANM_PoseC:
                mCallback.field_0x244 = true;
                mCallback.mElbowLRotationYAmplitude = -0x1000;
                mCallback.mElbowLRotationYPhase = 0;
                break;
            case ANM_PoseL:
                mCallback.field_0x244 = false;
                mCallback.mElbowLRotationYAmplitude = -0x1000;
                mCallback.mElbowLRotationYPhase = 0;
                break;
            case ANM_PoseR:
                mCallback.field_0x244 = true;
                mCallback.mElbowLRotationYAmplitude = -0x1000;
                mCallback.mElbowLRotationYPhase = 0;
                break;
            case ANM_PoseC2:
                mCallback.field_0x244 = true;
                mCallback.mElbowLRotationYAmplitude = -0x1000;
                mCallback.mElbowLRotationYPhase = 0;
                break;
        }
    }
    mMdlBody.play();

    return true;
}

bool dAcGirahimu2_c::isRightArmPose() {
    if (mAnmIDShoulderR == ANM_PoseL || mAnmIDShoulderR == ANM_PoseR || mAnmIDShoulderR == ANM_PoseC ||
        mAnmIDShoulderR == ANM_PoseC2) {
        return true;
    }
    return false;
}

bool dAcGirahimu2_c::isLeftArmPose() {
    if (mAnmIDShoulderL == ANM_PoseL || mAnmIDShoulderL == ANM_PoseR || mAnmIDShoulderL == ANM_PoseC ||
        mAnmIDShoulderL == ANM_PoseC2) {
        return true;
    }
    return false;
}

void dAcGirahimu2_c::vt_0x1F4() {
    if (mPlayerAttackDir == CUT_DIR_STAB) {
        mCcSwordR.ClrTgSet();
        mCcSwordL.ClrTgSet();

    } else {
        configureSwordShield(mCcSwordR, mAnmIDShoulderR);
        configureSwordShield(mCcSwordL, mAnmIDShoulderL);
        if (mCcSwordR.ChkTgSet()) {
            mCcBody.ClrTgSet();
        }
        if (mCcSwordL.ChkTgSet()) {
            mCcBody.ClrTgSet();
        }
    }
}

void dAcGirahimu2_c::vt_0x1F8() {
    if (mbPoseFreeRequest) {
        setAnm("PoseFree", ANM_PoseFree, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        mbPoseFreeRequest = false;
        return;
    }

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        int rnd = cM::rndInt(100);
        mVec3_c v0 = dAcPy_c::GetLink()->getSwordPos() - mPosition;
        mVec3_c v1 = dAcPy_c::GetLink()->getSwordPos() - dAcPy_c::GetLink()->vt_0x278();
        if (v1.y >= 0.5f) {
            s16 a = v0.atan2sX_Z();
            if (a - mRotation.y > 0) {
                setAnm("PoseLU", ANM_PoseLU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            } else {
                setAnm("PoseRU", ANM_PoseRU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } else if (v1.y <= -0.8f) {
            setAnm("PoseUD", ANM_PoseUD, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else {
            s16 a = v0.atan2sX_Z();
            if (a - mRotation.y > 0) {
                if (rnd >= 50) {
                    setAnm("PoseLR", ANM_PoseLR, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                } else {
                    setAnm("PoseLU", ANM_PoseLU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                }
            } else {
                if (rnd >= 75) {
                    setAnm("PoseLR", ANM_PoseLR, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                } else {
                    setAnm("PoseRU", ANM_PoseRU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                }
            }
        }
    } else {
        int rnd = cM::rndInt(100);
        if (rnd >= 75) {
            setAnm("PoseLR", ANM_PoseLR, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else if (rnd >= 50) {
            setAnm("PoseLU", ANM_PoseLU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else if (rnd >= 25) {
            setAnm("PoseRU", ANM_PoseRU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        } else {
            setAnm("PoseUD", ANM_PoseUD, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        }
    }
}

void dAcGirahimu2_c::vt_0x20C() {
    if (mAnmIDBody == ANM_PoseFree) {
        changeState(StateID_ComeOn);
    } else {
        changeState(StateID_FrontAttack);
    }
}

// Largely copied from dAcGirahimuBase_c::createHeap
bool dAcGirahimu2_c::createHeap() {
    nw4r::g3d::ResFile bodyRes;
    void *rawRes = getOarcResFile("GirahimBt02");
    mResAnmChr0 = nw4r::g3d::ResFile(rawRes);
    loadMainResFile();
    nw4r::g3d::ResMdl resMdl = mResAnmChr0.GetResMdl("GirahimBt02");
    if (!mMdlBody.create(resMdl, &mAllocator, 0x137, 1)) {
        return false;
    }
    TRY_CREATE(mAnmChrBlend.create(resMdl, 10, &mAllocator));

    nw4r::g3d::ResAnmChr resAnm = mResAnmChr1.GetResAnmChr("WaitA");
    if (!resAnm.IsValid()) {
        resAnm = mResAnmChr0.GetResAnmChr("WaitA");
    }
    for (int i = 0; i < 5; ++i) {
        if (!mAnmChrs[i].create2(resMdl, resAnm, &mAllocator)) {
            mMdlBody.remove();
            return false;
        }
    }

    for (int i = 0; i < 5; ++i) {
        mAnmChrBlend.attach(i, &mAnmChrs[i], 1.0f);
    }

    nw4r::g3d::ResAnmTexPat pat = mResAnmChr0.GetResAnmTexPat("GirahimBt02Wink");
    TRY_CREATE(mAnmTexPatWink.create(resMdl, pat, &mAllocator, nullptr, 1));

    mMdlBody.setAnm(mAnmTexPatWink);
    mMdlBody.setCallback(&mCallback);
    mMdlBody.setAnm(mAnmChrBlend, 5.0f);

    initNodeData();

    bodyRes = nw4r::g3d::ResFile(rawRes);
    mMdlPias.create(bodyRes.GetResMdl("GirahimBt02Pias"), &mAllocator, 0x20);

    rawRes = getOarcResFile("GirahimBt");
    mVec3_c v0(0.0f, -200.0f, 0.0f);
    TRY_CREATE(
        mMdlSwordA.create(&mAllocator, rawRes, "GirahimSwordA", 0x130, mVec3_c::Zero, v0, mStts, nullptr, 1, nullptr)
    );

    mVec3_c v1(0.0f, -200.0f, 0.0f);
    TRY_CREATE(
        mMdlSwordB.create(&mAllocator, rawRes, "GirahimSwordB", 0x130, mVec3_c::Zero, v1, mStts, nullptr, 1, nullptr)
    );

    int id = m3d::getMatID(mMdlBody.getResMdl(), "m_Eyeball");
    mEyeMdlCallback.create(mMdlBody, id, id, 1, 1, nullptr);
    mEyeMdlCallback.mBlendWeight = 1;
    mpSound = getSoundSource();

    return true;
}

int dAcGirahimu2_c::actorCreate() {
    CREATE_ALLOCATOR(dAcGirahimu2_c);
    if (!MinigameManager::GetInstance()->checkInBossRush()) {
        if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8)) {
            return FAILED;
        }
    }
    createCollision();
    createBase(fProfile::LYT_BOSS_CAPTION, this, 0, OTHER);
    mHealth = 100;
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        mCreateKnifeTimer = 2;
        changeState(StateID_Wait);
    } else {
        changeState(StateID_CreateSpinKnife);
    }
    mPosition.set(0.0f, 0.0f, 0.0f);
    mStartingPos.set(0.0f, 0.0f, 0.0f);
    dLightEnv_c::GetInstance().GetOverrideSpf().mRatio = 1.0f;

    return SUCCEEDED;
}

int dAcGirahimu2_c::doDelete() {
    return SUCCEEDED;
}

void dAcGirahimu2_c::vt_0x1DC() {
    dAcGirahimuBase_c::vt_0x1DC();

    if (!getStateID().isEqual(StateID_Death) && !getStateID().isEqual(StateID_DeathMiniGame)) {
        if (mbSpawnedSwords && getHealth() == 0) {
            if (MinigameManager::GetInstance()->checkInBossRush()) {
                changeState(StateID_DeathMiniGame);
            } else {
                changeState(StateID_Death);
            }
            return;
        }
    }

    if (--field_0x2222 <= 0) {
        field_0x2222 = 0;
    }
    if (mHealth > 50) {
        field_0x220C = 0;
    } else {
        field_0x220C = 1;
    }

    if (mbSpawnedSwords) {
        mCallback.field_0x04E = false;
        mCyl0.OnTgSet();
        mCcHand.ClrTgSet();
        mCcBody.OnTgSet();
    } else {
        mCcBody.SetTgType(~(AT_TYPE_COMMON0 | AT_TYPE_SWORD));
    }
}

int dAcGirahimu2_c::actorExecute() {
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        dTgSwordBattleGame_c *pSwordBattleGame =
            static_cast<dTgSwordBattleGame_c *>(fManager_c::searchBaseByProfName(fProfile::TAG_SWORD_BATTLE_GAME));
        if (!pSwordBattleGame->checkFightStarted()) {
            mLookAtTarget.set(getPlayerHeadOffset());
            mAcch.CrrPos(*dBgS::GetInstance());
            for (int i = 0; i < 5; ++i) {
                mAnmChrs[i].play();
            }
            mMdlBody.play();
            if (mpSound != nullptr) {
                mpSound->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
            }
            mWorldMtx.transS(mPosition.x, mPosition.y + getYOffset(), mPosition.z);
            mWorldMtx.ZXYrotM(mRotation);
            mWorldMtx.scaleM(mWarpScale.x, mWarpScale.y, mWarpScale.z);
            mMdlBody.setLocalMtx(mWorldMtx);
            mMdlBody.calc(false);
            return SUCCEEDED;
        }
    } else {
        if (!mbShownBossCaption) {
            mbShownBossCaption = true;
            const char *label2 = "BOSS_06_caption";
            const char *label = "BOSS_06";
            dLytBossCaption_c::GetInstance()->show(label, label2);
            mBossCaptionTimer = 100;
        }
        if (mBossCaptionTimer == 1) {
            dLytBossCaption_c::GetInstance()->unk_inline();
        }
    }

    bool b0 = false;
    if (!isNearHalfHealth()) {
        b0 = true;
    }
    vt_0x1DC();
    fn_228_80E0();
    mbVulnerable = false;
    executeState();
    fn_228_80A0();
    if (field_0xD7C) {
        mAngle.y = mRotation.y;
    }

    mCallback.field_0x0A8.set(1.0f, 0.0f, 0.0f);
    mCallback.field_0x0A8.rotY(mRotation.y);
    mCallback.mHeadRotationYCorrection = mRotation.y;
    mCallback.fn_226_3130();

    mSwordLink.get()->setAngleCopy(mRotation);
    mSwordLink.get()->setGhirahimPosition(mPosition);

    mWarpScaleSnapshot.set(mWarpScale);
    sLib::addCalcScaledDiff(&mSpeed, mSpeedTarget, mSpeedChangeFactor, 100.0f);
    mVec3_c v0(0.0f, 0.0f, 0.0f);
    cLib::chasePos(mTeleportDodgeAmount, v0, 10.0f);
    if (mbOnGround) {
        calcVelocity();
    } else {
        mVelocity.y += mAcceleration;
    }

    mPosition += mTeleportDodgeAmount;
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    mAcch.CrrPos(*dBgS::GetInstance());

    mWorldMtx.transS(mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.ZXYrotM(mRotation);
    mWorldMtx.scaleM(mWarpScale.x, mWarpScale.y, mWarpScale.z);
    mMdlBody.setLocalMtx(mWorldMtx);
    for (int i = 0; i < 5; ++i) {
        mAnmChrs[i].play();
    }
    mMdlBody.play();

    if (mpSound != nullptr) {
        mpSound->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
    }
    mMdlBody.calc(false);
    if (!isState(StateID_UpWarp)) {
        handleEnterExitEffect();
    }
    executeHeadMovement();
    mAnmTexPatWink.play();
    updateHeadMovement();
    updateSpineMovement();
    updateCollision();
    if (--field_0xD5A <= 0) {
        field_0xD5A = 0;
    }

    if (mCallback.field_0x04E || mbCatchingPlayerSword || getStateID().isEqual(StateID_Catch)) {
        field_0xD5A = 6;
    } else if (field_0xD5A >= 5) {
        mEmitter.setFading(5);
    }

    if (mEmitter.isFadeComplete()) {
        mEmitter.remove(true);
    }

    if (field_0xD5A > 0) {
        mMtx_c middleR1Transform;
        mMdlBody.getNodeWorldMtx(mMdlBody.getResMdl().GetResNode("MiddleR1").GetID(), middleR1Transform);
        mEmitter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_973_, middleR1Transform, nullptr, nullptr);
    }

    setSwordLinkTransform();
    updateSwordLocations();
    mSwordGrabType = 0;
    executeEarringTransform();

    int i0 = 0;
    bool playerSound0 = false;
    bool playerSound1 = false;
    for (int i = 0; i < 15; ++i) {
        if (!mKnives[i].isLinked()) {
            continue;
        }

        mKnives[i].get()->field_0x6F0.set(mRotation.x, mRotation.y, mRotation.z);
        mKnives[i].get()->field_0x6D8.set(mPosition);
        mKnives[i].get()->field_0x6E4.set(mCallback.mHandRTranslation);

        if (mKnives[i].get()->isWaiting()) {
            playerSound1 = true;
            i0++;
        }

        if (mKnives[i].get()->isCirclingPlayer()) {
            playerSound0 = true;
        }
    }

    if (i0 > 0) {
        if (playerSound0) {
            if (playerSound1) {
                dAcPy_c::GetLinkM()->holdSoundWithParams(SE_OGhKf_GROUP_FLY_LV, 1280.0f, i0);
            } else {
                dAcPy_c::GetLinkM()->holdSoundWithParams(SE_OGhKf_GROUP_FLY_LV, 0.0f, i0);
            }
        } else {
            holdSound(SE_OGhKf_GROUP_FLY_LV);
        }
    }
    if (b0 && isNearHalfHealth()) {
        dSndBgmMgr_c::GetInstance()->fn_80372D70(0);
    }

    return SUCCEEDED;
}

void dAcGirahimu2_c::initializeState_Walk() {
    field_0xD6C = 1;
    setAnm("Walk", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    mCallback.field_0x088 = true;
    mCallback.field_0x04E = false;
    mCallback.mSlerpFactorTarget = 1.0f;

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        mCallback.field_0x04E = true;
        mCallback.mSlerpFactorTarget = 0.5f;
        mCallback.mSlerpFactor = 0.0f;
    } else {
        mCallback.field_0x04E = false;
        mCallback.mSlerpFactorTarget = 1.0f;
    }
    mSubstateTimer = 100;
}
void dAcGirahimu2_c::executeState_Walk() {
    if (mSwordLink.get()->isStick()) {
        changeState(StateID_Wait);
        return;
    }

    fn_226_BC00();
    if (mCallback.field_0x04E) {
        setAnmShoulderR("WaitA", ANM_WaitA, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        mCallback.field_0x2BC = 0;
        mCallback.field_0x098 = 10.0f;
        setAnmShoulderR("Walk", ANM_Walk, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }

    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        if (mDistanceToPlayer <= 200.0f) {
            changeState(StateID_Wait);
            return;
        }
    } else {
        if (mDistanceToPlayer <= 150.0f) {
            changeState(StateID_Panch);
            return;
        }
    }
    if (mAnmIDBody == ANM_CallWalk && mAnmChrs[ANMIDX_Body].isStop()) {
        setAnm("Walk", ANM_Walk, m3d::PLAY_MODE_4, 10.0f, 1.0f);
        mSubstateTimer = 100;
    }
    if (mAnmIDBody == ANM_AttackKnife) {
        if (mAnmChrs[ANMIDX_Body].checkFrame(9.0f)) {
            for (int i = 0; i < 15; ++i) {
                if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                    startSound(SE_OGhKf_SHOT);
                    mKnives[i].get()->fn_239_2460(true, false, 60.0f);
                    break;
                }
            }
        } else if (mAnmChrs[ANMIDX_Body].isStop()) {
            setAnm("Walk", ANM_Walk, m3d::PLAY_MODE_4, 10.0f, 1.0f);
            mSubstateTimer = 100;
        }
    }

    if (mSubstateTimer <= 0 && !mCallback.field_0x04E) {
        bool bKnifeWaiting = false;
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                bKnifeWaiting = true;
            }
        }
        if (bKnifeWaiting) {
            if (mAnmIDBody != ANM_AttackKnife) {
                setAnm("WalkKnife", ANM_AttackKnife, m3d::PLAY_MODE_4, 2.0f, 1.0f);
            }
        } else {
            if (mAnmIDBody != ANM_CallWalk) {
                if (mAnmIDBody != ANM_AttackKnife) {
                    setAnm("CallWalk", ANM_CallWalk, m3d::PLAY_MODE_4, 2.0f, 1.0f);
                }
            } else if (mAnmChrs[ANMIDX_Body].checkFrame(9.0f)) {
                fn_228_82C0();
                mSubstateTimer = 100;
            }
        }
    }
    fn_226_B960(6.0f);
}
void dAcGirahimu2_c::finalizeState_Walk() {}

void dAcGirahimu2_c::initializeState_CreateSpinKnife() {
    setAnm("Call", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 1;
    mCallback.field_0x088 = true;
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    mbVulnerable = true;
    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        mCallback.field_0x04E = true;
        mCallback.mSlerpFactorTarget = 0.2f;
        mCallback.mSlerpFactor = 0.2f;
    } else {
        mCallback.field_0x04E = false;
        mCallback.mSlerpFactorTarget = 1.0f;
    }
}
void dAcGirahimu2_c::executeState_CreateSpinKnife() {
    if (dAcPy_c::GetLinkM()->isUsingSword() && mDistanceToPlayer <= 370.0f) {
        fn_226_BC00();
    } else {
        mCallback.field_0x04F = false;
        mCallback.field_0x04E = false;
    }

    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        fn_228_82C0();
    }
    if (mAnmChrs[ANMIDX_Body].checkFrame(27.0f)) {
        changeState(StateID_Wait);
    } else if (mAnmChrs[ANMIDX_Body].isStop()) {
        changeState(StateID_Wait);
    }
}
void dAcGirahimu2_c::finalizeState_CreateSpinKnife() {
    field_0xD66 = 0;
    mCallback.field_0x2BC = 0;
    mbVulnerable = false;
}

void dAcGirahimu2_c::initializeState_CatchDamage() {
    mCallback.mSlerpFactorTarget = 0.5f;
    clearAnms();
    setAnm("Damage", ANM_Damage, nullptr, m3d::PLAY_MODE_4, 5.0f, 0.5f);
    field_0xD6C = 1;
    mCallback.field_0x088 = false;
    mConsecutiveCatchDamageCount++;
    mSpeed = 0.0f;
    mSpeedTarget = 0.0f;
}
void dAcGirahimu2_c::executeState_CatchDamage() {
    if (mConsecutiveCatchDamageCount >= 3 || getHealth() <= 0) {
        mbVulnerable = true;
    }
    mCallback.field_0x04E = false;

    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);

    if (mbVulnerable) {
        mCcBody.ClrTgSet();
        mCcHand.ClrTgSet();
        setTgVulnerable();
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (getHealth() > 0) {
            vt_0x220();
        } else {
            changeState(StateID_BackStep);
        }
        mConsecutiveCatchDamageCount = 0;
    }
}
void dAcGirahimu2_c::finalizeState_CatchDamage() {
    field_0xD58 = 0;
    mCallback.field_0x2BC = 0;
    mbVulnerable = false;
    field_0xD66 = 0;
}

void dAcGirahimu2_c::initializeState_WaitKnifeAttack() {
    setAnm("AttackKnife", ANM_AttackKnife, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 0;
}
void dAcGirahimu2_c::executeState_WaitKnifeAttack() {
    if (dAcPy_c::GetLinkM()->isUsingSword()) {
        mLookAtTarget += dAcPy_c::GetLink()->getSwordPos();
        mLookAtTarget *= 0.5f;
    }
    sLib::chaseAngle(mRotation.y.ref(), mAngleToPlayer, 0x500);
    mCyl0.ClrTgSet();
    if (mAnmChrs[ANMIDX_Body].getFrame() < 15.0f) {
        mCcBody.ClrTgSet();
    } else {
        field_0xD6C = 1;
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                startSound(SE_OGhKf_SHOT);
                mKnives[i].get()->fn_239_2460(true, false, 60.0f);
            }
        }
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimu2_c::finalizeState_WaitKnifeAttack() {}

void dAcGirahimu2_c::initializeState_Catch() {
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->setFlag(mRoomID, getFromParams(0, 0xFF));
    }
    mCollider.ClrCo();
    mSpeedTarget = 0.0f;
    mSpeed = 0.0f;
    field_0xD7E = false;
    if (cM::rndInt(100) > 50) {
        field_0xD7E = true;
        setAnm("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceTongue", ANM_FaceTongue, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnm("Catch", ANM_Catch, "Catch", ANM_Catch, "FaceSmaile", ANM_FaceSmaile, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    mCallback.field_0x088 = true;
    dAcPy_c::GetLinkM()->set0x439F(this, 7);
    mCallback.field_0x014 = 1.0f;
    mCallback.mSlerpFactorTarget = 0.5f;
    mSubstateTimer = 4;
    field_0xD5E = 100;
}
void dAcGirahimu2_c::executeState_Catch() {
    if (field_0xD5E == 70) {
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                mKnives[i].get()->setSpinWaitPreAttack();
            }
        }
    }
    if (field_0xD5E < 70) {
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked()) {
                mKnives[i].get()->holdSound(SE_OGhKf_FLOAT_LV);
            }
        }
    }
    mLookAtTarget += dAcPy_c::GetLink()->getSwordPos();
    mLookAtTarget *= 0.5f;
    if (mSubstateTimer == 1) {
        bool mbNoWait = true;
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                mbNoWait = false;
            }
        }

        if (mbNoWait) {
            fn_228_82C0();
        }
    }
    if (mSubstateTimer <= 0) {
        mbCatchingPlayerSword = true;
        mCallback.field_0x04E = true;
        if (updateSwordGrabAnim()) {
            return;
        }
    }
    if (mSubstateTimer <= 0 && !dAcPy_c::GetLinkM()->IfCurrentActionToActor(this, 169 /* ??? */)) {
        vt_0x220();
        return;
    }

    field_0xD5E--;
    if (mAnmIDShoulderL != ANM_Call && field_0xD5E <= 0) {
        setAnm("Call", ANM_Call, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
    if (mAnmIDShoulderL == ANM_Call && mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        for (int i = 0; i < 15; ++i) {
            if (mKnives[i].isLinked()) {
                startSound(SE_OGhKf_SHOT);
                mKnives[i].get()->fn_239_2460(false, false, 60.0f);
            }
        }
    }
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        field_0xD5E = 100;
        if (field_0xD7E) {
            if (mAnmIDShoulderL != ANM_Call) {
                setAnm("CatchLoop", ANM_CatchLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            } else {
                setAnmBody("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmHip("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmShoulderR("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmHead("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } else {
            if (mAnmIDShoulderL != ANM_Call) {
                setAnm("CatchLoop", ANM_CatchLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            } else {
                setAnmBody("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmHip("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmShoulderR("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                setAnmHead("CatchLoop", ANM_CatchLoop, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        }
    }

    sLib::chaseAngle(mRotation.y.ref(), cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition), 0x500);
}
void dAcGirahimu2_c::finalizeState_Catch() {
    mCallback.field_0x04E = false;
    mbCatchingPlayerSword = false;
    mCollider.CoSet();
    mCallback.mSlerpFactorTarget = 1.0f;
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, getFromParams(0, 0xFF));
    }
    field_0xD66 = 0;
    mCallback.field_0x2BC = 0;
    vt_0x1FC();
}

void dAcGirahimu2_c::fn_228_80A0() {
    if (field_0xD6C == 6 || field_0xD6C == 8) {
        mbVulnerable = true;
    }

    if (field_0xD6C == 6) {
        mCcBody.SetTgType(~(AT_TYPE_COMMON0 | AT_TYPE_SWORD));
    }
}

void dAcGirahimu2_c::fn_228_80E0() {
    if (field_0xD6C == 0) {
        return;
    }

    if (field_0xD6C == 6 || field_0xD6C == 8) {
        mbVulnerable = true;
    }

    mCcBody.SetR(30.0f);
    if (vt_0x1E0()) {
        return;
    }

    switch (field_0xD6C) {
        case 1:
            if (fn_228_4CD0(mPlayerAttackDir)) {
                mCcHand.ClrTgSet();
            }
            vt_0x1E4();
            break;
        case 5:  vt_0x234(); break;
        case 6:  fn_228_4C20(); break;
        case 11: fn_228_4A30(); break;
        case 7:  vt_0x1EC(); break;
        case 8:  vt_0x1F0(); break;
        case 9:  fn_228_5400(); break;
        default: mCcBody.ClrTgSet(); break;
    }
    if (mMdlSwordA.getCollider().findAtHit()) {
        mbSwordAtHit = true;
    } else if (mMdlSwordB.getCollider().findAtHit()) {
        mbSwordAtHit = true;
    }
}

void dAcGirahimu2_c::fn_228_82C0() {
    mVec3_c v0 = mPosition;
    mVec3_c v1(0.0f, 100.0f, 200.0f);
    v1.rotY(mRotation.y);
    v0 += v1;
    v0.y = mCallback.mArmRTranslation.y;

    int rnd = cM::rndInt(90);
    mAng a0;
    mAng a1;
    mVec3_c spacing(0.0f, 0.0f, 0.0f);
    dAcObjGirahimuKnife_c::Pattern_e pattern;
    switch (field_0x220C) {
        case 0: {
            mAng a0;
            a1 = 0x8000;
            if (rnd >= 60 && mKnifeVolleyState == 2) {
                a0 = 0;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_U_D;
                field_0x2210 = 8;
                mKnifeVolleyState = 2;
            } else if (rnd >= 30 && mKnifeVolleyState == 1) {
                if (cM::rndInt(100) >= 50) {
                    a0 = 0x2000;
                    spacing.set(100.0f, 65.0f, 0.0f);
                    pattern = dAcObjGirahimuKnife_c::PATTERN_RD_LU;
                    field_0x2210 = 4;
                } else {
                    a0 = -0x2000;
                    spacing.set(100.0f, 65.0f, 0.0f);
                    pattern = dAcObjGirahimuKnife_c::PATTERN_RU_LD;
                    field_0x2210 = 2;
                }
                mKnifeVolleyState = 1;
            } else if (mKnifeVolleyState != 0) {
                a0 = 0x4000;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_R_L;
                field_0x2210 = 1;
                mKnifeVolleyState = 0;
            } else {
                a0 = 0;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_U_D;
                field_0x2210 = 8;
                mKnifeVolleyState = 2;
            }
            startSound(SE_Girahim_MAGIC_KNIFE_APPEAR);

            for (int i = 0; i < 2; ++i) {
                mVec3_c spawnPos = v0;
                mVec3_c adjustedSpacing = spacing;
                adjustedSpacing.rotZ(a0 + i * a1);
                mVec3_c adjustedSpacing1 = adjustedSpacing;
                adjustedSpacing1.rotY(mRotation.y);
                spawnPos += adjustedSpacing1;

                dAcObjGirahimuKnife_c *pKnife;
                if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                    pKnife = mKnives[i].get();
                } else {
                    spawnPos.x = mPosition.x;
                    spawnPos.z = mPosition.z;
                    pKnife = static_cast<dAcObjGirahimuKnife_c *>(
                        create(fProfile::OBJ_GH_KNIFE, mRoomID, 0, &spawnPos, nullptr, nullptr, 0xFFFFFFFF)
                    );
                    mKnives[i].link(pKnife);
                }

                if (pKnife != nullptr) {
                    spawnPos.y += v0.y;
                    mKnives[i].get()->setTarget(spawnPos);
                    pKnife->setState(4);
                }
            }
        } break;
        case 1: {
            mAng a0, a1;
            a1 = 0x4000;
            if (rnd >= 50 && mKnifeVolleyState != 2) {
                field_0x2210 = 9;
                a0 = 0;
                spacing.set(0.0f, 65.0f, 100.0f);
                mKnifeVolleyState = 2;
            } else if (mKnifeVolleyState != 1) {
                field_0x2210 = 6;
                a0 = 0x2000;
                spacing.set(0.0f, 65.0f, 100.0f);
                mKnifeVolleyState = 1;
            } else {
                field_0x2210 = 9;
                a0 = 0;
                spacing.set(0.0f, 65.0f, 100.0f);
                mKnifeVolleyState = 2;
            }
            startSound(SE_Girahim_MAGIC_KNIFE_APPEAR);

            for (int i = 0; i < 4; ++i) {
                mVec3_c spawnPos = v0;
                mVec3_c adjustedSpacing = spacing;

                adjustedSpacing.rotZ(a0 + a1 * i);
                spawnPos += adjustedSpacing;
                spawnPos.x = mPosition.x;
                spawnPos.z = mPosition.z;
                dAcObjGirahimuKnife_c *pKnife;
                if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                    pKnife = mKnives[i].get();
                } else {
                    pKnife = static_cast<dAcObjGirahimuKnife_c *>(
                        create(fProfile::OBJ_GH_KNIFE, mRoomID, 0, &spawnPos, nullptr, nullptr, 0xFFFFFFFF)
                    );
                    mKnives[i].link(pKnife);
                }

                if (pKnife != nullptr) {
                    spawnPos.y += v0.y;
                    mKnives[i].get()->setTarget(spawnPos);
                    if (mKnifeVolleyState != 1) {
                        if (i % 2 == 0) {
                            pKnife->setPattern(dAcObjGirahimuKnife_c::PATTERN_U_D);
                        } else {
                            pKnife->setPattern(dAcObjGirahimuKnife_c::PATTERN_R_L);
                        }
                    } else {
                        if (i % 2 == 0) {
                            pKnife->setPattern(dAcObjGirahimuKnife_c::PATTERN_RU_LD);
                        } else {
                            pKnife->setPattern(dAcObjGirahimuKnife_c::PATTERN_RD_LU);
                        }
                    }
                    pKnife->setState(4);
                }
            }
        } break;
    }
}

void dAcGirahimu2_c::vt_0x1FC() {
    for (int i = 0; i < 15; ++i) {
        if (mKnives[i].isLinked()) {
            mKnives[i].get()->field_0x78E = true;
        }
    }
}

void dAcGirahimu2_c::loadMainResFile() {
    mResAnmChr1 = nw4r::g3d::ResFile(getOarcResFile("GirahimBt"));
}

void dAcGirahimu2_c::setStateIdleAttack() {
    changeState(StateID_KnifeAttack);
}

void dAcGirahimu2_c::correctPoseHip() {
    if (mAnmIDBody == ANM_PoseLR) {
        setAnmHip("PoseLR", ANM_PoseLR, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseLU) {
        setAnmHip("PoseLU", ANM_PoseLU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseRU) {
        setAnmHip("PoseRU", ANM_PoseRU, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseUD) {
        setAnmHip("PoseUD", ANM_PoseUD, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else if (mAnmIDBody == ANM_PoseFree) {
        setAnmHip("PoseFree", ANM_PoseFree, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    } else {
        setAnmHip("WaitBtD", ANM_WaitBtC, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
}

void dAcGirahimu2_c::changeStateGetPlayerSword() {
    changeState(StateID_GetSword);
}

bool dAcGirahimu2_c::vt_0x210() {
    if (--mCreateKnifeTimer == 1) {
        changeState(StateID_CreateKnife);
        mCallback.field_0x2BC = 0;
        return true;
    }
    return false;
}

void dAcGirahimu2_c::vt_0x230() {
    if (mbSpawnedSwords) {
        setAnm("WalkBtB", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else {
        setAnm("Walk", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }
}

void dAcGirahimu2_c::changeStateEscape() {
    if (mbSpawnedSwords) {
        if (isState(StateID_UpAttack)) {
            changeState(StateID_Escape);
        } else {
            if (cM::rndInt(100) > 50) {
                changeState(StateID_Escape);
            } else {
                changeState(StateID_UpWarp);
            }
        }
    } else {
        changeState(StateID_Escape);
    }
}

bool dAcGirahimu2_c::isNearHalfHealth() {
    return mbSpawnedSwords && getHealth() <= 400;
}

void dAcGirahimu2_c::vt_0x234() {
    vt_0x1F4();
    cCcD_Obj *pCc = mCollider.findTgHit();
    if (pCc != nullptr && pCc->GetTgAtHitType() == AT_TYPE_SWORD) {
        if (pCc == &mCcSwordR || pCc == &mCcSwordL) {
            getSoundSource()->startSoundAtPosition(SE_Girahim_SWORD_HIT_SWORD, pCc->GetTgHitPos());
            if (!getStateID().isEqual(StateID_Counter)) {
                changeState(StateID_Counter);
                setFrame(5.0f);
                return;
            }
        } else if (pCc == &mCcBody) {
            adjustSpineHitRotation(pCc);
            if (pCc->GetTgAtCutDir() == CUT_DIR_STAB) {
                changeState(StateID_BackStep);
            } else {
                changeState(StateID_G_SwordDamage);
            }
            return;
        }
    }

    pCc = mMdlSwordA.getCollider().findAtHit();
    if (pCc != nullptr && pCc->ChkAtShieldReflect()) {
        changeState(StateID_GuardJustCounter);
        return;
    }

    pCc = mMdlSwordB.getCollider().findAtHit();
    if (pCc != nullptr && pCc->ChkAtShieldReflect()) {
        changeState(StateID_GuardJustCounter);
        return;
    }
}
