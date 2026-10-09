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
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
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
    field_0xD8C = true;
    field_0xD6C = 6;
    mCyl2.OnTgSet();
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
    field_0xD46 = 30;
    if (vt_0x218()) {
        field_0x186C = 60.0f;
        field_0xD46 = 5;
    }
    if (cM::rndInt(100) > 50) {
        field_0xD7E = true;
    } else {
        field_0xD7E = false;
    }
}
void dAcGirahimu2_c::executeState_G_SwordWait() {
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    if (checkObjectProperty(OBJ_PROP_0x2)) {
        return;
    }

    mVec3_c distFromHome = mPosition - mStartingPos;
    mVec3_c v0(0.0f, 0.0f, 1.0f);
    v0.rotY(mRotation.y);
    if (field_0x1868 >= 1500.0f && distFromHome.dot(v0) < 0.0f && field_0x1838 < 500.0f) {
        changeState(StateID_HomeWarp);
        return;
    }

    if (mAng(field_0x183E - mRotation.y).abs() < 0x3000) {
        if (field_0x1838 <= 400.0f && field_0xD7E) {
            field_0xD82 = true;
            vt_0x1F8();
            changeState(StateID_FrontAttack);
            return;
        }
        if (field_0xD46 <= 0) {
            field_0xD46 = 60;
            if (fn_228_5110()) {
                return;
            }
        }
    }

    if (field_0x1838 <= 400.0f && fn_228_5110()) {
        return;
    }

    mAng diff = field_0x183E - mRotation.y;
    if (mAnmIDBody != ANM_WaitA && mRotation.y == field_0x183E && field_0x1838 <= 250.f) {
        field_0x1834 = 0.0f;
        mSpeed = 0.0f;
        setAnm("WaitBtD", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    } else if (field_0x1838 >= 300.0f) {
        if (mAnmIDBody != ANM_Walk) {
            setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.2f);
        }
    } else if (mAng(diff).abs() > 0x4000) {
        changeState(StateID_BackWalk);
        return;
    } else {
        turn(field_0x183E);
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    fn_226_B960(6.0f);
}
void dAcGirahimu2_c::finalizeState_G_SwordWait() {}

void dAcGirahimu2_c::initializeState_CreateKnife() {
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    setAnm("WaitBtD", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    field_0xD46 = 15;
    mCallback.field_0x088 = true;
    mCyl2.SetTgShieldType(~AT_TYPE_DAMAGE);
    field_0xD6C = 6;
    field_0x2220 = 0;
    field_0xD44 = 0;
}
void dAcGirahimu2_c::executeState_CreateKnife() {
    mCyl0.SetR(150.0f);
    switch (field_0xD44) {
        case 0: {
            if (field_0xD46 <= 0) {
                if (!turn(field_0x183E)) {
                    vt_0x228();
                }
                field_0xD6C = 0;
                if (field_0x1838 <= 1000.0f) {
                    setAnm("CallWalk", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                    setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
                    field_0x1834 = -5.0f;
                } else {
                    mSpeed = 0.0f;
                    field_0x1834 = 0.0f;
                    setAnm("Call", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
                }
                field_0xD44++;
            }
        } break;
        case 1: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
                int rnd = cM::rndInt(100);
                if (rnd >= 60) {
                    startSound(SE_OGhKf_GROUP_APPEAR);
                    field_0x2214 = 2;
                    fn_228_4550(0, 5);
                    fn_228_4550(5, 10);
                    fn_228_4550(10, 15);
                } else if (rnd >= 30) {
                    field_0x2214 = 3;
                    fn_228_47F0(false);
                } else {
                    field_0x2214 = 3;
                    fn_228_47F0(true);
                }
            }
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                changeState(StateID_KnifePreAttack);
                return;
            }
        } break;
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    fn_226_B960(6.0f);
}
void dAcGirahimu2_c::finalizeState_CreateKnife() {}

void dAcGirahimu2_c::initializeState_KnifePreAttack() {
    mCallback.field_0x088 = true;
    fn_226_D070();
    if (field_0x1838 <= 1000.0f) {
        setAnm("CallWalk", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
        setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
        field_0x1834 = -5.0f;
    } else {
        mSpeed = 0.0f;
        field_0x1834 = 0.0f;
        setAnm("CallWalk", ANM_Call, m3d::PLAY_MODE_4, 5.0f, 1.5f);
    }

    field_0xD44 = 0;
    field_0xD6C = 6;
    if (field_0x2218 > 10) {
        field_0x2218 = 0;
    }

    for (int i = field_0x2218; i < field_0x2218 + 5; ++i) {
        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
            return;
        }
    }
    field_0x2218 += 5;
    if (field_0x2218 > 10) {
        field_0x2218 = 0;
    }
    for (int i = field_0x2218; i < field_0x2218 + 5; ++i) {
        if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
            return;
        }
    }
    field_0x2218 += 5;
    if (field_0x2218 > 10) {
        field_0x2218 = 0;
    }
    field_0xD44 = 0;
}
void dAcGirahimu2_c::executeState_KnifePreAttack() {
    if (mAnmIDHip != ANM_Walk && field_0x1838 <= 1000.0f) {
        setAnmHip("Walk", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.2f);
        field_0x1834 = -5.0f;
    }
    if (mAnmIDHip != ANM_Walk) {
        fn_226_A280(field_0x183E);
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
    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
                if (field_0x2214 == 3) {
                    for (int i = 0; i < 15; ++i) {
                        if (mKnives[i].isLinked()) {
                            mKnives[i].get()->field_0x792 = false;
                        }
                    }
                } else {
                    if (field_0x2218 > 10) {
                        field_0x2218 = 0;
                    }
                    for (int i = field_0x2218; i < field_0x2218 + 5; i++) {
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
                field_0xD46 = 20;
                field_0xD44++;
            }
        } break;
        case 1: {
            if (mAnmIDHip == ANM_Walk) {
                setAnm("WalkBt", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.0f);
            }
            if (mAnmIDHip != ANM_Walk || field_0xD46 <= 0) {
                if (field_0x2214 == 3) {
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

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
void dAcGirahimu2_c::finalizeState_KnifePreAttack() {}

void dAcGirahimu2_c::initializeState_KnifeAttack() {
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 6;
    setAnm("AttackKnife", ANM_AttackKnife, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.5f);
    mCallback.field_0x088 = true;
}
void dAcGirahimu2_c::executeState_KnifeAttack() {
    mCyl0.SetR(150.0f);
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);

    if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        if (field_0x2214 == 3) {
            startSound(SE_OGhKf_GROUP_SHOT);
            for (int i = 0; i < 15; ++i) {
                if (mKnives[i].isLinked()) {
                    mKnives[i].get()->fn_239_2460(true, true, 40.0f);
                }
            }
        } else {
            for (int i = field_0x2218; i < field_0x2218 + 5; ++i) {
                if (mKnives[i].isLinked()) {
                    startSound(SE_OGhKf_SHOT);
                    mKnives[i].get()->fn_239_2360(true, true, 50.0f);
                }
            }
            field_0x2218 += 5;
        }
        field_0xD6C = 6;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (field_0x2214 == 3) {
            vt_0x220();
            return;
        }
        if (field_0x2218 >= 15) {
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
    field_0xD80 = false;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    setAnm("PoseFree", ANM_PoseFree, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 9;
    mCallback.field_0x088 = false;
    field_0x221C = false;
}
void dAcGirahimu2_c::executeState_HitAction() {
    mCallback.field_0x088 = false;
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    fn_226_B960(5.0f);
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (field_0x1838 < 300.0f) {
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
    field_0x221C = false;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    fn_226_D070();
    setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    setAnm("PoseFree", ANM_PoseFree, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 9;
    mCallback.field_0x088 = false;
    field_0xD46 = 60;
}
void dAcGirahimu2_c::executeState_ComeOn() {
    mCallback.field_0x088 = false;
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    fn_226_B960(5.0f);
    if (mAnmChrs[ANMIDX_Body].isStop()) {
        if (field_0x1838 <= 400.0f) {
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
    if (vt_0x218()) {
        field_0xD46 = 30;
    } else {
        field_0xD46 = 50;
    }
    field_0xD48 = 5;
    field_0xD6C = 9;
    f32 f0 = 1.0f;
    setAnmHead("RarmU", ANM_PoseC, m3d::PLAY_MODE_4, f0, 1.0f);
    setAnmBody("RarmU", ANM_PoseC, m3d::PLAY_MODE_4, f0, 1.0f);
    if (!fn_228_5E90()) {
        setAnmShoulderR("WaitBtD", ANM_WaitBtC, m3d::PLAY_MODE_4, f0, 1.0f);
    } else if (!fn_228_5EC0()) {
        setAnmShoulderL("WaitBtD", ANM_WaitBtC, m3d::PLAY_MODE_4, f0, 1.0f);
    }
    setAnmHip("WalkBt", ANM_Walk, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mMdlBody.play();
    if (fn_228_5E90()) {
        fn_228_5EC0();
    }
}
void dAcGirahimu2_c::executeState_ComeOnGuard() {
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    if (field_0xD46 <= 0) {
        if (field_0x1838 <= 350.0f) {
            changeState(StateID_Counter);
            return;
        } else {
            vt_0x220();
            return;
        }
    } else if (field_0x1838 >= 300.0f) {
        vt_0x220();
        return;
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    fn_226_B960(5.0f);
}
void dAcGirahimu2_c::finalizeState_ComeOnGuard() {}

void dAcGirahimu2_c::initializeState_G_SwordDemo() {
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->setFlag(mRoomID, getFromParams(8, 0xFF));
    }

    mCallback.field_0x2D4 = 1.0f;
    field_0xD44 = 0;
    mCallback.field_0x2D0 = 1.0f;
    mCyl0.SetR(30.0f);
    mCyl2.OnTgSet();
    mSph0.ClrTgSet();
    mMdlSwordA.mProc.setColor1(mColor(0, 0, 0, 0xFF));
    mMdlSwordA.mProc.setColor2(mColor(0, 0, 0, 0xFF));
    mMdlSwordB.mProc.setColor1(mColor(0, 0, 0, 0xFF));
    mMdlSwordB.mProc.setColor2(mColor(0, 0, 0, 0xFF));
    field_0xD71 = false;
    field_0xD70 = true;
    mHealth = 600;
    field_0x1834 = 0.0f;
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

    switch (field_0xD44) {
        case 0: {
            {
                Event event("GirahimuSwordDemo", 100, 0x100001, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
            }

            if (EventManager::isInEvent(this, "GirahimuSwordDemo")) {
                setAnm("WaitA", ANM_WaitA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;
                field_0x20F8.set(headTranslation);
                v1.set(0.0f, 0.0f, 150.0f);
                v1.rotY(mRotation.y);
                v1 += headTranslation;
                field_0x20EC.set(v1);
                pCamera->setEventCamView(field_0x20F8, field_0x20EC, 70.0f, 0.0f);
                field_0xD46 = 30;
            }
        } break;
        case 1: {
            mRotation.y = cLib::targetAngleY(mPosition, mStartingPos);

            mVec3_c v(0.0f, 0.0f, 500.0f);
            v.rotY(mRotation.y);
            v += mPosition;
            mAng3_c a(0, mRotation.y + 0x8000, 0);
            dAcPy_c::GetLinkM()->setPosRot(&v, &a, false, 0, 0);

            field_0x20F8.set(headTranslation);
            v1.set(0.0f, 0.0f, 150.0f);
            v1.rotY(mRotation.y);
            v1 += headTranslation;
            field_0x20EC.set(v1);

            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (field_0xD46 <= 0) {
                mAnmIDBody = ANM_NONE;
                setAnm(
                    "WaitA", ANM_WaitA, "WaitA", ANM_WaitA, "FaceTongue", ANM_FaceTongue, m3d::PLAY_MODE_4, 15.0f, 1.0f
                );
                field_0xD46 = 50;
                field_0xD44++;
            }
        } break;
        case 2: {
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            if (field_0xD46 <= 0) {
                field_0xD44++;
                setAnm("TwoSwordAppear", ANM_SwordAppear, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } break;
        case 3: {
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
                field_0xD7B = true;
                field_0x12E0 = true;
                field_0xD44++;
            }
        } break;
        case 4: {
            sLib::chase(&field_0x12DC, 1.0f, 0.1f);
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].checkFrame(53.0f)) {
                field_0xD44++;
            }
        } break;
        case 5: {
            v1.set(0.0f, 200.0f, 500.0f);
            v1.rotY(mRotation.y);
            v1 += mPosition;
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (sLib::chase(&field_0x12DC, 1.0f, 0.1f) && mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("WaitBtC", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
                field_0xD46 = 50;
                field_0xD44++;
            }
        } break;
        case 6: {
            v1.set(0.0f, 200.0f, 500.0f);
            v1.rotY(mRotation.y);
            v1 += mPosition;
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 10.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 100.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (field_0xD46 <= 0) {
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                changeState(StateID_G_SwordWait);
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_G_SwordDemo() {
    mCyl2.OnTgSet();
    field_0xD44 = 0;
}

void dAcGirahimu2_c::initializeState_G_SwordDamage() {
    mCyl2.OnTgSet();
    setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = false;
    field_0xD44 = 0;
    field_0xD62 = 150;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 7;
    field_0xD8E = 0;
    vt_0x1FC();
}
void dAcGirahimu2_c::executeState_G_SwordDamage() {
    field_0xD8D = true;
    if (--field_0xD62 <= 0) {
        field_0xD8E = 0;
        if (field_0x1838 < 500.0f) {
            changeState(StateID_BackStep);
        } else {
            vt_0x220();
        }
        return;
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        setAnm("DamageWait", ANM_DamageWait, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    if (mCyl2.ChkTgHit() && mCyl2.GetTgAtHitType() == AT_TYPE_SWORD) {
        changeState(StateID_G_SwordPiyori);
    }
}
void dAcGirahimu2_c::finalizeState_G_SwordDamage() {
    field_0xD8E = 0;
}

void dAcGirahimu2_c::initializeState_UpWarp() {
    setAnm("Jump", ANM_StepStart, nullptr, m3d::PLAY_MODE_4, 2.0f, 1.0f);
    setFrame(6.0f);
    field_0xD46 = 3;
    mCallback.field_0x088 = true;
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
    field_0xD7E = 0;
}
void dAcGirahimu2_c::executeState_UpWarp() {
    mCyl2.ClrTgSet();
    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].isStop() && sLib::chase(&field_0x8B0.x, 0.0f, 0.5f)) {
                mMtx_c hipTransform;
                nw4r::g3d::ResMdl mdl = mMdlBody.getResMdl();
                mMdlBody.getNodeWorldMtx(mdl.GetResNode("Hip").GetID(), hipTransform);
                mVec3_c hipPos;
                hipTransform.multVec(hipPos, hipPos);
                hipPos.y += 100.0f;
                if (field_0xD70 && getHealth() == 0) {
                    startSound(SE_Girahim_MAGIC_DISAPPEAR_STAGE);
                } else {
                    startSound(SE_Girahim_MAGIC_DISAPPEAR);
                }
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_684_, hipPos, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
                field_0xD44++;

                field_0x8B0.set(0.0f, 0.0f, 0.0f);
                mVec3_c position = dAcPy_c::GetLink()->mPosition;
                mVec3_c adj(0.0f, 600.0f, 50.0f);
                adj.rotY(dAcPy_c::GetLink()->mRotation.y);
                position += adj;
                setPosition(position);
                setOldPosition(position);
                mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
                field_0xD46 = 40;
                setAnm("PoseUpper", ANM_PoseUpper, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
            }
        } break;
        case 1: {
            if (dAcPy_c::GetLink()->checkCurrentAction(98 /* BACKFLIP */) ||
                dAcPy_c::GetLink()->checkFlags0x350(0x80000)) {
                if (!field_0xD7E) {
                    field_0x1840.set(dAcPy_c::GetLink()->mPosition);
                    field_0xD7E = true;
                }
            }
            mVelocity.set(0.0f, 0.0f, 0.0f);
            if (field_0xD46 <= 0) {
                field_0x8B0.set(0.0f, 1.05f, 1.05f);
                field_0xD44++;
                changeState(StateID_UpAttack);
                return;
            }
        } break;
    }
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
void dAcGirahimu2_c::finalizeState_UpWarp() {
    field_0xD44 = 0;
    mCyl2.OnTgSet();
}

void dAcGirahimu2_c::initializeState_UpAttack() {
    field_0xD44 = 0;
    setAnm("PoseUpper", ANM_PoseUpper, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD7D = false;
    mVec3_c pos = dAcPy_c::GetLink()->mPosition;
    if (field_0xD7E) {
        pos.set(field_0x1840);
    }
    mVec3_c adj(0.0f, 600.0f, 0.0f);
    adj.rotY(dAcPy_c::GetLink()->mRotation.y);
    pos += adj;
    setPosition(pos);
    setOldPosition(pos);
    mRotation.y = cLib::targetAngleY(mPosition, dAcPy_c::GetLink()->mPosition);
    field_0xD6C = 6;
    mCyl2.ClrTgSet();
    mSph0.ClrTgSet();
    mCallback.field_0x04E = false;
    mCallback.field_0x088 = false;
    field_0xD46 = 2;
    mAcceleration = -8.0f;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    mMdlSwordA.fn_8006B7A0(0x210000);
    mMdlSwordB.fn_8006B7A0(0x210000);
}
void dAcGirahimu2_c::executeState_UpAttack() {
    field_0xD8D = true;
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    switch (field_0xD44) {
        case 0: {
            if (field_0xD46 > 0) {
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
            mCyl2.ClrTgSet();
            mSph0.ClrTgSet();
            field_0xD7D = false;
            if (mAcch.ChkGndHit()) {
                mVelocity.set(0.0f, 0.0f, 0.0f);
                setAnm("PoseUpperAttack", ANM_PoseUpperAttack, nullptr, m3d::PLAY_MODE_4, 1.0f, 1.0f);
                field_0xD6C = 5;
                field_0xD44++;
            }
        } break;
        case 1: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
                mCallback.field_0x088 = true;
            }

            if (mAnmChrs[ANMIDX_Body].getFrame() >= 60.0f) {
                field_0xD6C = 6;
                mCyl2.OnTgSet();
                mCyl0.OnTgSet();
            } else {
                mCyl2.OnTgSet();
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
    field_0xD44 = 0;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
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
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x088 = true;
}
void dAcGirahimu2_c::executeState_Counter() {
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
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
    mCyl2.OnTgSet();
    field_0xD82 = false;
}

void dAcGirahimu2_c::initializeState_GuardJustCounter() {}
void dAcGirahimu2_c::executeState_GuardJustCounter() {}
void dAcGirahimu2_c::finalizeState_GuardJustCounter() {}

void dAcGirahimu2_c::initializeState_TwoSwordAttack() {}
void dAcGirahimu2_c::executeState_TwoSwordAttack() {}
void dAcGirahimu2_c::finalizeState_TwoSwordAttack() {}

void dAcGirahimu2_c::initializeState_G_SwordPiyori() {}
void dAcGirahimu2_c::executeState_G_SwordPiyori() {}
void dAcGirahimu2_c::finalizeState_G_SwordPiyori() {}

void dAcGirahimu2_c::initializeState_Death() {}
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

    switch (field_0xD44) {
        case 0: {
            {
                Event event("GirahimuDeathDemo", 100, 0x100001, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
            }
            if (EventManager::isInEvent(this, "GirahimuDeathDemo")) {
                // ...Enough of this foolishness...
                mFlowMgr.triggerEntryPoint(304, 19, 0, 0);
                setAnm("End2A", ANM_EndA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;

                v0.set(mStartingPos);
                v0.y = 163.0f;
                field_0x20F8.set(v0);
                field_0x20EC.set(v1);
                pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
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
            field_0xD44++;
            v0.set(mStartingPos);
            v0.y = 163.0f;
            field_0x20F8 = v0;
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 2: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 200.0f);
            field_0x20EC.set(v1);
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                field_0x20F8.set(109.0f, 124.0f, -637.0f);
                field_0x20EC.set(-54.0f, 155.0f, 50.0f);
                setAnm("End2BLoop", ANM_EndBLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 3: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                setAnm("End2C", ANM_EndC, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0x20F8.set(0.0f, 131.0f, -359.0f);
                field_0x20EC.set(0.0f, 153.0f, 92.0f);
                dStageMgr_c::GetInstance()->procfn_800192F0(0xDC, mMtx_c::fromIdentity(), 0x82);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 4: {
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("End2DLoop", ANM_EndDLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                // It shouldn't matter how powerful your sword is, you are still nothing...
                mFlowMgr.triggerEntryPoint(304, 20, 0, 0);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 5: {
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                // You filthy scamp! You have awakened a wrath that will burn for eons! ...
                mFlowMgr.triggerEntryPoint(304, 21, 0, 0);
                field_0xD44++;
            }
        } break;
        case 6: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                field_0xD44++;
                setAnm("End2E", ANM_EndE, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } break;
        case 7: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(35.0f)) {
                mMdlSwordB.setProcActive(true);
            }
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].getFrame() >= 90.0f) {
                sLib::chase(&field_0x8B0.x, 0.0f, 0.2);
            }
            if (mAnmChrs[ANMIDX_Body].isStop() && field_0x8B0.x <= 0.0f) {
                mMdlSwordB.setProcActive(false);
                field_0xD44++;
                field_0xD46 = 40;
            }

        } break;
        case 8: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (field_0xD46 <= 0) {
                field_0xD44++;
            }
        } break;
        case 9: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (sLib::chase(&dLightEnv_c::GetInstance().GetOverrideSpf().mRatio, 0.0f, 0.012f)) {
                DungeonflagManager::sInstance->setFlag(3);
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                deleteRequest();
            }
        } break;
    }
}
void dAcGirahimu2_c::finalizeState_Death() {}

void dAcGirahimu2_c::fn_228_4550(int idxStart, int idxEnd) {}

void dAcGirahimu2_c::fn_228_47F0(bool) {}

void dAcGirahimu2_c::initializeState_DeathMiniGame() {}
void dAcGirahimu2_c::executeState_DeathMiniGame() {}
void dAcGirahimu2_c::finalizeState_DeathMiniGame() {}

void dAcGirahimu2_c::fn_228_4A30() {
    mMdlSwordA.mCcList.findAtHit();
}

void dAcGirahimu2_c::fn_228_4C20() {}

bool dAcGirahimu2_c::fn_228_4CD0(s32 cutDir) {}

void dAcGirahimu2_c::vt_0x220() {
    field_0x221D = false;
    if (field_0xD70) {
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

        if (field_0xD80 && cM::rndInt(100) > 80) {
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
                field_0xD46 = 0;
                return;
            }
        } else {
            if ((waitingCount == 2 && field_0x220C == 0) || (waitingCount == 4 && field_0x220C == 1)) {
                changeState(StateID_Wait);
                return;
            }
            if (getStateID().isEqual(StateID_Catch)) {
                field_0xD52 = 20;
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

bool dAcGirahimu2_c::fn_228_5110() {}

void dAcGirahimu2_c::fn_228_5400() {}

void dAcGirahimu2_c::fn_228_5740() {}

bool dAcGirahimu2_c::fn_228_5770(s32 cutDir) {
    bool bRightArm = false;
    f32 rate = 1.0f;
    switch (cutDir) {
        case CUT_DIR_U:
            if (mAnmIDShoulderR == ANM_PoseC || mAnmIDShoulderL == ANM_PoseC) {
                return false;
            }
            if (!fn_228_5E90()) {
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
            if (!fn_228_5E90()) {
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
            if (!fn_228_5E90()) {
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
            if (!fn_228_5E90()) {
                setAnmShoulderL("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            }
            break;
        case CUT_DIR_L:
            if (mAnmIDShoulderR == ANM_PoseR || mAnmIDShoulderL == ANM_PoseR) {
                return false;
            }
            if (!fn_228_5E90()) {
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
            if (!fn_228_5E90()) {
                setAnmShoulderL("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
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
            if (!fn_228_5E90()) {
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
            if (!fn_228_5E90()) {
                setAnmShoulderL("RarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderL].play();
            } else {
                setAnmShoulderR("LarmL", ANM_PoseL, m3d::PLAY_MODE_4, rate, rate);
                mAnmChrs[ANMIDX_ShoulderR].play();
            }
            break;
    }

    if (bRightArm) {
        switch (mAnmIDShoulderR) {
            case ANM_PoseC:
                mCallback.field_0x245 = true;
                mCallback.field_0x23C = mAng(0x1000);
                mCallback.field_0x240 = 0;
                break;
            case ANM_PoseL:
                mCallback.field_0x245 = true;
                mCallback.field_0x23C = mAng(0x1000);
                mCallback.field_0x240 = 0;
                break;
            case ANM_PoseR:
                mCallback.field_0x245 = false;
                mCallback.field_0x23C = mAng(0x1000);
                mCallback.field_0x240 = 0;
                break;
            case ANM_PoseC2:
                mCallback.field_0x245 = true;
                mCallback.field_0x23C = mAng(0x1000);
                mCallback.field_0x240 = 0;
                break;
        }
    } else {
        switch (mAnmIDShoulderL) {
            case ANM_PoseC:
                mCallback.field_0x244 = true;
                mCallback.field_0x23E = -0x1000;
                mCallback.field_0x242 = 0;
                break;
            case ANM_PoseL:

                mCallback.field_0x244 = false;
                mCallback.field_0x23E = -0x1000;
                mCallback.field_0x242 = 0;
                break;
            case ANM_PoseR:

                mCallback.field_0x244 = true;
                mCallback.field_0x23E = -0x1000;
                mCallback.field_0x242 = 0;
                break;
            case ANM_PoseC2:

                mCallback.field_0x244 = true;
                mCallback.field_0x23E = -0x1000;
                mCallback.field_0x242 = 0;
                break;
        }
    }
    mMdlBody.play();

    return true;
}

bool dAcGirahimu2_c::fn_228_5E90() {}

bool dAcGirahimu2_c::fn_228_5EC0() {}

void dAcGirahimu2_c::vt_0x1F4() {}

void dAcGirahimu2_c::vt_0x1F8() {}

void dAcGirahimu2_c::vt_0x20C() {}

bool dAcGirahimu2_c::createHeap() {}

int dAcGirahimu2_c::actorCreate() {
    MinigameManager::GetInstance()->checkInBossRush();
}

int dAcGirahimu2_c::doDelete() {}

void dAcGirahimu2_c::vt_0x1DC() {}

int dAcGirahimu2_c::actorExecute() {
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        dTgSwordBattleGame_c *pSwordBattleGame =
            static_cast<dTgSwordBattleGame_c *>(fManager_c::searchBaseByProfName(fProfile::TAG_SWORD_BATTLE_GAME));
        if (!pSwordBattleGame->checkFightStarted()) {
            field_0x88C.set(getPlayerHeadOffset());
            mAcch.CrrPos(*dBgS::GetInstance());
            for (int i = 0; i < 5; ++i) {
                mAnmChrs[i].play();
            }
            mMdlBody.play();
            if (pSoundIface != nullptr) {
                pSoundIface->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
            }
            mWorldMtx.transS(mPosition.x, mPosition.y + getYOffset(), mPosition.z);
            mWorldMtx.ZXYrotM(mRotation);
            mWorldMtx.scaleM(field_0x8B0.x, field_0x8B0.y, field_0x8B0.z);
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
    if (!vt_0x218()) {
        b0 = true;
    }
    vt_0x1DC();
    fn_228_80E0();
    field_0xD81 = false;
    executeState();
    fn_228_80A0();
    if (field_0xD7C) {
        mAngle.y = mRotation.y;
    }

    mCallback.field_0x0A8.set(1.0f, 0.0f, 0.0f);
    mCallback.field_0x0A8.rotY(mRotation.y);
    mCallback.field_0x084 = mRotation.y;
    mCallback.fn_226_3130();

    mSwordLink.get()->setAngleCopy(mRotation);
    mSwordLink.get()->setGhirahimPosition(mPosition);

    field_0x8BC.set(field_0x8B0);
    sLib::addCalcScaledDiff(&mSpeed, field_0x1834, field_0x1858, 100.0f);
    mVec3_c v0(0.0f, 0.0f, 0.0f);
    cLib::chasePos(field_0x185C, v0, 10.0f);
    if (field_0xD7D) {
        calcVelocity();
    } else {
        mVelocity.y += mAcceleration;
    }

    mPosition += field_0x185C;
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    mAcch.CrrPos(*dBgS::GetInstance());

    mWorldMtx.transS(mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.ZXYrotM(mRotation);
    mWorldMtx.scaleM(field_0x8B0.x, field_0x8B0.y, field_0x8B0.z);
    mMdlBody.setLocalMtx(mWorldMtx);
    for (int i = 0; i < 5; ++i) {
        mAnmChrs[i].play();
    }
    mMdlBody.play();

    if (pSoundIface != nullptr) {
        pSoundIface->setFrame(mAnmChrs[ANMIDX_Body].getFrame());
    }
    mMdlBody.calc(false);
    if (!isState(StateID_UpWarp)) {
        fn_226_D090();
    }
    fn_226_D2D0();
    mAnmTexPatWink.play();
    fn_226_9690();
    fn_226_9620();
    fn_256_9C90();
    if (--field_0xD5A <= 0) {
        field_0xD5A = 0;
    }

    if (mCallback.field_0x04E || field_0xD71 || getStateID().isEqual(StateID_Catch)) {
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
    fn_256_99F0();
    field_0xD74 = 0;
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
        mKnives[i].get()->field_0x6E4.set(mCallback.field_0x260);

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
    if (b0 && vt_0x218()) {
        dSndBgmMgr_c::GetInstance()->fn_80372D70(0);
    }

    return SUCCEEDED;
}

void dAcGirahimu2_c::initializeState_Walk() {}
void dAcGirahimu2_c::executeState_Walk() {}
void dAcGirahimu2_c::finalizeState_Walk() {}

void dAcGirahimu2_c::initializeState_CreateSpinKnife() {}
void dAcGirahimu2_c::executeState_CreateSpinKnife() {}
void dAcGirahimu2_c::finalizeState_CreateSpinKnife() {}

void dAcGirahimu2_c::initializeState_CatchDamage() {}
void dAcGirahimu2_c::executeState_CatchDamage() {}
void dAcGirahimu2_c::finalizeState_CatchDamage() {}

void dAcGirahimu2_c::initializeState_WaitKnifeAttack() {}
void dAcGirahimu2_c::executeState_WaitKnifeAttack() {}
void dAcGirahimu2_c::finalizeState_WaitKnifeAttack() {}

void dAcGirahimu2_c::initializeState_Catch() {}
void dAcGirahimu2_c::executeState_Catch() {}
void dAcGirahimu2_c::finalizeState_Catch() {}

void dAcGirahimu2_c::fn_228_80A0() {}

void dAcGirahimu2_c::fn_228_80E0() {}

void dAcGirahimu2_c::fn_228_82C0() {
    mVec3_c v0 = mPosition;
    mVec3_c v1(0.0f, 100.0f, 200.0f);
    v1.rotY(mRotation.y);
    v0 += v1;
    v0.y = mCallback.field_0x21C.y;

    int rnd = cM::rndInt(90);
    mAng a0;
    mAng a1;
    mVec3_c spacing(0.0f, 0.0f, 0.0f);
    dAcObjGirahimuKnife_c::Pattern_e pattern;
    switch (field_0x220C) {
        case 0: {
            mAng a0;
            a1 = 0x8000;
            if (rnd >= 60 && field_0x2214 == 2) {
                a0 = 0;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_U_D;
                field_0x2210 = 8;
                field_0x2214 = 2;
            } else if (rnd >= 30 && field_0x2214 == 1) {
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
                field_0x2214 = 1;
            } else if (field_0x2214 != 0) {
                a0 = 0x4000;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_R_L;
                field_0x2210 = 1;
                field_0x2214 = 0;
            } else {
                a0 = 0;
                spacing.set(100.0f, 65.0f, 0.0f);
                pattern = dAcObjGirahimuKnife_c::PATTERN_U_D;
                field_0x2210 = 8;
                field_0x2214 = 2;
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
            if (rnd >= 50 && field_0x2214 != 2) {
                field_0x2210 = 9;
                a0 = 0;
                spacing.set(0.0f, 65.0f, 100.0f);
                field_0x2214 = 2;
            } else if (field_0x2214 != 1) {
                field_0x2210 = 6;
                a0 = 0x2000;
                spacing.set(0.0f, 65.0f, 100.0f);
                field_0x2214 = 1;
            } else {
                field_0x2210 = 9;
                a0 = 0;
                spacing.set(0.0f, 65.0f, 100.0f);
                field_0x2214 = 2;
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
                    if (field_0x2214 != 1) {
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

void dAcGirahimu2_c::vt_0x1FC() {}

void dAcGirahimu2_c::vt_0x200() {}

void dAcGirahimu2_c::vt_0x204() {}

void dAcGirahimu2_c::vt_0x228() {}

void dAcGirahimu2_c::vt_0x22C() {}

bool dAcGirahimu2_c::vt_0x210() {}

void dAcGirahimu2_c::vt_0x230() {}

void dAcGirahimu2_c::vt_0x214() {}

bool dAcGirahimu2_c::vt_0x218() {}

void dAcGirahimu2_c::vt_0x234() {}
