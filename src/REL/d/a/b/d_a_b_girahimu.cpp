#include "d/a/b/d_a_b_girahimu.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/b/d_a_b_girahimu_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_girahimu_knife.h"
#include "d/a/obj/d_a_obj_girahimu_sword_link.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/flag/sceneflag_manager.h"
#include "d/lyt/d_lyt_boss_caption.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_sword_battle_game.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_anmchr.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"

STATE_DEFINE(dAcGirahimu_c, G_SwordDemo);
STATE_DEFINE(dAcGirahimu_c, G_SwordWait);
STATE_VIRTUAL_DEFINE(dAcGirahimu_c, G_SwordDamage);
STATE_DEFINE(dAcGirahimu_c, G_SwordPiyori);
STATE_DEFINE(dAcGirahimu_c, Knife);
STATE_DEFINE(dAcGirahimu_c, KnifeAttack);
STATE_DEFINE(dAcGirahimu_c, Death);
STATE_DEFINE(dAcGirahimu_c, DeathMiniGame);
STATE_VIRTUAL_DEFINE(dAcGirahimu_c, CatchDamage);

SPECIAL_ACTOR_PROFILE(B_GIRAHIMU, dAcGirahimu_c, fProfile::B_GIRAHIMU, 0x10E, 0, 0);

struct dAcGirahimu_HIO_c {
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

    static const dAcGirahimu_HIO_c sHIO;
};

const dAcGirahimu_HIO_c dAcGirahimu_HIO_c::sHIO = {1.05f, 1.05f, -5.0f, -2.0f, 15.0f, 150.0f,
                                                   3.0f,  50.0f, 1,     0x5DC, 0x14,  0x1000};

void dAcGirahimu_c::initializeState_G_SwordDemo() {
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->setFlag(mRoomID, getFromParams(8, 0xFF));
    }

    mCallback.field_0x2D4 = 1.0f;
    field_0xD44 = 0;
    mCallback.field_0x2D0 = 1.0f;
    mCyl0.SetR(50.0f);
    mCyl2.OnTgSet();
    mSph0.ClrTgSet();
    mMdlSwordA.mProc.setColor1(mColor(0, 0, 0, 0xFF));
    mMdlSwordA.mProc.setColor2(mColor(0, 0, 0, 0xFF));
    field_0xD71 = false;
    field_0xD70 = true;
    mHealth = 200;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    mCallback.field_0x04E = false;
    if ((s32)getFromParams(0, 0xFF) != 0xFF) {
        SceneflagManager::sInstance->unsetFlag(mRoomID, getFromParams(0, 0xFF) + 1);
    }
}
void dAcGirahimu_c::executeState_G_SwordDemo() {
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
                fn_226_D070();
                setAnm("WaitA", ANM_WaitA, "WaitA", ANM_WaitA, "FaceTongue", ANM_NONE, m3d::PLAY_MODE_4, 15.0f, 1.0f);
                field_0xD46 = 50;
                field_0xD44++;
            }
        } break;
        case 2: {
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            if (field_0xD46 <= 0) {
                field_0xD44++;
                setAnm("SwordAppear", ANM_SwordAppear, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
            }
        } break;
        case 3: {
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].checkFrame(40.0f)) {
                field_0xD7B = true;
                field_0xD44++;
            }
        } break;
        case 4: {
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 100.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);

            if (sLib::chase(&field_0x12DC, 1.0f, 0.1f) && mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("WaitBtC", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
                field_0xD46 = 50;
                field_0xD44++;
            }
        } break;
        case 5: {
            v1.set(0.0f, 200.0f, 500.0f);
            v1.rotY(mRotation.y);
            v1 += mPosition;
            cLib::addCalcPos2(&field_0x20EC, v1, 0.2f, 10.0f);
            cLib::addCalcPos2(&field_0x20F8, midpoint, 0.2f, 10.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 35.0f, 0.0f);
            if (field_0xD46 <= 0) {
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                changeState(StateID_G_SwordWait);
            }
        } break;
    }
}
void dAcGirahimu_c::finalizeState_G_SwordDemo() {
    mCyl2.OnTgSet();
    field_0xD44 = 0;
}

void dAcGirahimu_c::initializeState_G_SwordWait() {
    field_0xD6C = 6;
    setAnm("WaitBtC", ANM_WaitBtC, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    mCallback.field_0x088 = true;
    field_0xD46 = 30;
    if (vt_0x218()) {
        field_0x186C = 75.0f;
        field_0xD46 = 5;
    }
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    if (cM::rndInt(100) > 50) {
        field_0xD7E = true;
    } else {
        field_0xD7E = false;
    }
}
void dAcGirahimu_c::executeState_G_SwordWait() {
    sLib::chase(&field_0x8B0.x, 1.05f, 0.2f);
    if (checkObjectProperty(OBJ_PROP_0x2)) {
        return;
    }

    if (field_0x1868 >= 1500.0f) {
        changeState(StateID_HomeWarp);
        return;
    }

    if (mAng(field_0x183E - mRotation.y).abs() < 0x3000) {
        if (field_0x1838 <= 300.0f && field_0xD7E) {
            field_0xD82 = true;
            vt_0x1F8();
            changeState(StateID_FrontAttack);
            field_0xD46 = 20;
            return;
        }

        if (field_0xD46 <= 0) {
            field_0xD46 = 30;
            determineNextMove();
            return;
        }
    }

    if (field_0x8DC != 4) {
        field_0x1834 = 0.0f;
        if (!turn(field_0x183E) && mAnmIDHip != ANM_WaitBtC) {
            setAnmHip("WaitBtC", ANM_WaitBtC, m3d::PLAY_MODE_4, 15.0f, 1.0f);
        }
    } else {
        field_0x1834 = 5.0f;
        setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_4, 15.0f, 1.0f);
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
}
void dAcGirahimu_c::finalizeState_G_SwordWait() {}

void dAcGirahimu_c::initializeState_G_SwordDamage() {
    mCyl2.OnTgSet();
    setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = false;
    field_0xD44 = 0;
    field_0xD62 = 50;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 7;
    field_0xD8E = 0;
}
void dAcGirahimu_c::executeState_G_SwordDamage() {
    field_0xD8D = true;
    field_0xD62--;
    if (getHealth() != 0) {
        if (field_0xD8E > 0) {
            changeState(StateID_G_SwordPiyori);
            return;
        }

        if (mAnmIDBody != ANM_DamageHit) {
            if (vt_0x218()) {
                changeState(StateID_BackStep);
                return;
            }

            if (field_0xD62 <= 0 || field_0xD8E >= 4) {
                field_0xD8E = 0;
                if (field_0x1838 < 500.0f) {
                    changeState(StateID_BackStep);
                    return;
                } else {
                    vt_0x220();
                    return;
                }
            }
        }
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        mCallback.field_0x088 = false;
        setAnm("DamageWait", ANM_DamageWait, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }
}
void dAcGirahimu_c::finalizeState_G_SwordDamage() {
    field_0xD8E = 0;
}

void dAcGirahimu_c::initializeState_G_SwordPiyori() {
    mCyl2.OnTgSet();
    setAnm("DamageWait", ANM_DamageWait, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mCallback.field_0x088 = false;
    field_0xD44 = 0;
    field_0xD46 = 30;

    if (field_0xD82) {
        field_0xD46 = 30;
        field_0xD82 = false;
    }

    mSpeed = 0.0f;
    field_0x1834 = 0.0f;

    field_0xD6C = 7;
    field_0xD8E = 0;
}
void dAcGirahimu_c::executeState_G_SwordPiyori() {
    if (getHealth() == 0) {
        return;
    }
    if (field_0xD8E > 0 && vt_0x218()) {
        changeState(StateID_BackStep);
        return;
    }

    if (field_0xD46 <= 0 || field_0xD8E >= 4) {
        field_0xD8E = 0;
        if (field_0x1838 < 500.0f) {
            changeState(StateID_BackStep);
        } else {
            vt_0x220();
        }
    }
}
void dAcGirahimu_c::finalizeState_G_SwordPiyori() {
    field_0xD8E = 0;
}

void dAcGirahimu_c::determineNextMove() {
    if (field_0xD84) {
        field_0xD84 = false;
        changeState(StateID_Run);
        return;
    }

    if (field_0xD85) {
        field_0xD85 = false;
        changeState(StateID_Knife);
        return;
    }

    for (int i = 0; i <= 10; ++i) {
        int rnd = cM::rndInt(90);
        if (rnd >= 60) {
            if (field_0x8DC != 1) {
                if (getStateID().isEqual(StateID_BackStep)) {
                    field_0x8DC = 1;
                    changeState(StateID_Run);
                    return;
                }
                if (field_0x8DC != 1 && field_0x1838 >= 600.0f) {
                    field_0x8DC = 1;
                    changeState(StateID_Run);
                    return;
                }
                if (!getStateID().isEqual(StateID_BackStep)) {
                    field_0xD84 = true;
                    changeState(StateID_BackStep);
                    return;
                }
            }
        } else if (rnd >= 30) {
            if (field_0x8DC != 0) {
                field_0x8DC = 0;
                if (field_0x1838 <= 300.0f) {
                    vt_0x1F8();
                    changeState(StateID_FrontAttack);
                    return;
                }
                if (cM::rndInt(100) > 50) {
                    changeState(StateID_FrontWarp);
                    return;
                } else {
                    changeState(StateID_BackWarp);
                    return;
                }
            }
        } else {
            if (field_0x8DC != 3) {
                if (!getStateID().isEqual(StateID_BackStep) && field_0x1838 <= 300.0f) {
                    field_0xD85 = true;
                    changeState(StateID_BackStep);
                    return;
                } else {
                    field_0x8DC = 3;
                    changeState(StateID_Knife);
                    return;
                }
            }
        }
    }

    field_0x8DC = 0;
    changeState(StateID_BackWarp);
}

void dAcGirahimu_c::fn_227_1EA0() {
    cCcD_Obj *pCc = mMdlSwordA.mCcList.findAtHit();
    if (pCc != nullptr && pCc->ChkAtShieldReflect()) {
        changeState(StateID_G_SwordDamage);
        fn_226_D070();
        setAnm("GuardLSwordA", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        field_0xD62 = 40;
        mCallback.field_0x088 = true;
        mRotation.y = field_0x183E;
        return;
    }
    pCc = mCollider.findTgHit();
    if (pCc != nullptr && pCc->GetTgAtHitType() == AT_TYPE_DAMAGE) {
        u16 health = getHealth();
        if (health != 0) {
            health--;
        }
        mHealth = health;
        changeState(StateID_KnifeDamage);
    }
}

void dAcGirahimu_c::initializeState_KnifeAttack() {
    setAnm("AttackKnife", ANM_AttackKnife, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD6C = 0;
}
void dAcGirahimu_c::executeState_KnifeAttack() {
    s32 _weird_zero = 0;

    fn_226_A280(field_0x183E);
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (mAnmChrs[ANMIDX_Body].getFrame() < 15.0f) {
        mCyl0.ClrTgSet();
        mCyl2.ClrTgSet();
    } else {
        mCyl0.OnTgSet();
        field_0xD81 = true;
        field_0xD6C = 6;
    }

    if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
        for (int i = 0; i < 5; ++i) {
            startSound(SE_OGhKf_SHOT);
            if (mKnives[i].isLinked()) {
                mKnives[i].get()->fn_239_2360(true, false, 50.0f + _weird_zero);
            }
        }
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        vt_0x220();
    }
}
void dAcGirahimu_c::finalizeState_KnifeAttack() {}

void dAcGirahimu_c::initializeState_Knife() {
    if (field_0x1838 <= 600.0f) {
        setAnm("WalkBt", ANM_Walk, nullptr, m3d::PLAY_MODE_2, 15.0f, 1.0f);
        setAnm("Call", ANM_Call, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        f32 rate = 1.0f;
        setAnmRate(rate);
        if (mAnmChrs[ANMIDX_Body].getPlayMode() == m3d::PLAY_MODE_2) {
            rate = -rate;
        }
        if (pSoundIface != nullptr) {
            pSoundIface->setRate(rate);
        }
        mSpeed = 0.0f;
        field_0x1834 = -8.0f * mAnmChrs[ANMIDX_Body].getRate();
    } else {
        mSpeed = 0.0f;
        field_0x1834 = 0.0f;
        setAnm("Call", ANM_Call, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    }

    mCyl2.SetTgShieldType(~AT_TYPE_NONE);
    field_0xD6C = 6;
    field_0xD44 = 0;
}
void dAcGirahimu_c::executeState_Knife() {
    if (mAnmIDHip != ANM_Walk && field_0x1838 <= 700.0f) {
        setAnmHip("WalkBt", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.0f);
        field_0x1834 = -8.0f * mAnmChrs[ANMIDX_Body].getRate();
    }
    if (mAnmIDHip != ANM_Walk) {
        fn_226_A280(field_0x183E);
    } else {
        fn_226_B960(6.0f);
    }

    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    mCyl2.ClrTgSet();

    switch (field_0xD44) {
        case 0: {
            if (mAnmChrs[ANMIDX_Body].checkFrame(15.0f)) {
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
                        knifeOffset.set(100.0f, baseYOffset + 100.0f, 0.0f);
                        spacing.set(-50.0f, -50.0f, 0.0f);
                    } else {
                        pattern = dAcObjGirahimuKnife_c::PATTERN_RD_LU;
                        knifeOffset.set(-100.0f, baseYOffset + 100.0f, 0.0f);
                        spacing.set(50.0f, -50.0f, 0.0f);
                    }
                }
                knifeOffset.z = 80.0f;
                for (int i = 0; i < 5; ++i) {
                    mVec3_c spawnPos = mPosition;
                    mVec3_c adjustedKnifeOffset = knifeOffset;
                    adjustedKnifeOffset.rotY(mRotation.y);
                    spawnPos += adjustedKnifeOffset;
                    dAcObjGirahimuKnife_c *pKnife = static_cast<dAcObjGirahimuKnife_c *>(
                        create(fProfile::OBJ_GH_KNIFE, mRoomID, 0, &spawnPos, nullptr, nullptr, 0xFFFFFFFF)
                    );
                    mKnives[i].link(pKnife);
                    if (pKnife != nullptr) {
                        mKnives[i].get()->setTarget(knifeOffset);
                        knifeOffset += spacing;
                        mKnives[i].get()->setPattern(pattern);
                    }
                }
            }
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                field_0xD46 = 25;
                field_0xD44++;
            }
        } break;
        case 1: {
            if (mAnmIDHip == ANM_Walk) {
                setAnm("WalkBt", ANM_Walk, m3d::PLAY_MODE_2, 15.0f, 1.0f);
            }
            if (mAnmIDHip != ANM_Walk || field_0xD46 <= 0) {
                bool waiting = false;
                for (int i = 0; i < 5; ++i) {
                    if (mKnives[i].isLinked() && mKnives[i].get()->isWaiting()) {
                        waiting = true;
                    }
                }
                if (waiting) {
                    changeState(StateID_KnifeAttack);
                } else {
                    vt_0x220();
                }
            }
        } break;
    }
}
void dAcGirahimu_c::finalizeState_Knife() {}

void dAcGirahimu_c::initializeState_Death() {
    field_0xD44 = 0;
    mCallback.field_0x2D4 = 1.0f;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    field_0x1830 = 200.0f;
    mCallback.field_0x088 = 0;
    field_0xD6C = 0;
    mCollider.ClrTg();
    mMdlSwordA.setField_0x080(mVec3_c(0.0f, -150.0f, 0.0f));
}
void dAcGirahimu_c::executeState_Death() {
    mCallback.field_0x088 = false;
    dCamera_c *pCamera = dScGame_c::getCamera();
    mVec3_c v0(mPosition);
    v0.y = 163.0f;
    mVec3_c v1(0.0f, 212.0f, -1361.0f);
    mVec3_c v2(0.0f, 86.0f, -1000.0f);
    mVec3_c v3(0.0f, 131.0f, -3356.0f);
    mVec3_c v4(72.0f, 178.0f, -1558.0f);

    mMtx_c headTransform;
    mVec3_c headTranslation(0.0f, 0.0f, 0.0f);
    mMdlBody.getNodeWorldMtx(mMdlBody.getResMdl().GetResNode("Head").GetID(), headTransform);
    headTransform.getTranslation(headTranslation);
    headTranslation.y = 118.0f;

    switch (field_0xD44) {
        case 0: {
            {
                Event event("GirahimuDeathDemo", 100, 0x100001, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &event, nullptr);
            }
            if (EventManager::isInEvent(this, "GirahimuDeathDemo")) {
                // Well... You put up more of a fight than I would have thought possible out of such a soft boy...
                mFlowMgr.triggerEntryPoint(201, 55, 0, 0);
                setAnm("End1A", ANM_EndA, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
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
                field_0x20F8.set(0.0f, 158.0f, -2438.0f);
                field_0x20EC.set(0.0f, 192.0f, -1727.0f);
                setAnm("End1BLoop", ANM_EndBLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 3: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                setAnm("End1C", ANM_EndC, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 4: {
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                setAnm("End1DLoop", ANM_EndDLoop, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD46 = 20;
                // I fear I spent far too long teasing and toying with you ...
                mFlowMgr.triggerEntryPoint(201, 56, 0, 0);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 5: {
            if (field_0xD46 <= 0) {
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 6: {
            mFlowMgr.checkFinished();
            if (mFlowMgr.fn_800C40F0()) {
                setAnm("End1C", ANM_EndC, nullptr, m3d::PLAY_MODE_3, 5.0f, 1.0f);
                field_0xD44++;
            }
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
        } break;
        case 7: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                mMdlSwordA.setProcActive(true);
                setAnm("End1E", ANM_EndE, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
                field_0xD44++;
            }

        } break;
        case 8: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (mAnmChrs[ANMIDX_Body].isStop()) {
                field_0xD44++;
            }
        } break;
        case 9: {
            cLib::addCalcPos2(&field_0x20F8, headTranslation, 0.2f, 20.0f);
            cLib::addCalcPos2(&field_0x20EC, v2, 0.5f, 20.0f);
            pCamera->setEventCamView(field_0x20F8, field_0x20EC, 20.0f, 0.0f);
            if (sLib::chase(&field_0x8B0.x, 0.0f, 0.2f)) {
                field_0xD46 = 40;
                field_0xD44++;
            }
        } break;
        case 10: {
            if (field_0xD46 <= 0) {
                field_0xD44++;
            }
        } break;
        case 11: {
            if (sLib::chase(&dLightEnv_c::GetInstance().GetOverrideSpf().mRatio, 0.0f, 0.012f)) {
                DungeonflagManager::sInstance->setFlag(3);
                pCamera->fn_8019EA70(false);
                EventManager::finishEvent(this, nullptr);
                deleteRequest();
            }

        } break;
    }
}
void dAcGirahimu_c::finalizeState_Death() {
    mCyl2.OnTgSet();
    field_0xD44 = 0;
}

void dAcGirahimu_c::initializeState_DeathMiniGame() {
    mCollider.ClrTg();
    mCallback.field_0x2D4 = 1.0f;
    field_0xD44 = 0;
    field_0x1834 = 0.0f;
    mSpeed = 0.0f;
    field_0x1830 = 200.0f;
    mCallback.field_0x088 = false;
    fn_226_D5A0();
    setAnm("DamageHit", ANM_DamageHit, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
    field_0xD6C = 0;
}
void dAcGirahimu_c::executeState_DeathMiniGame() {
    if (field_0xD44 == 0 && mAnmChrs[ANMIDX_Body].isStop()) {
        setAnm("DamageWait", ANM_EndE, nullptr, m3d::PLAY_MODE_4, 5.0f, 1.0f);
        field_0xD44++;
    }
}
void dAcGirahimu_c::finalizeState_DeathMiniGame() {
    mCyl2.OnTgSet();
    field_0xD44 = 0;
}

bool dAcGirahimu_c::createHeap() {
    return dAcGirahimuBase_c::createHeap();
}

int dAcGirahimu_c::actorCreate() {
    CREATE_ALLOCATOR(dAcGirahimu_c);
    if (!MinigameManager::GetInstance()->checkInBossRush()) {
        if (DungeonflagManager::sInstance->getCounterOrFlag(3, 8)) {
            return FAILED;
        }
    }

    dLightEnv_c::GetInstance().GetOverrideSpf().mRatio = 1.0f;
    createCollision();
    mHealth = 120;
    createBase(fProfile::LYT_BOSS_CAPTION, this, 0, OTHER);
    changeState(StateID_Wait);

    return SUCCEEDED;
}

int dAcGirahimu_c::doDelete() {
    return SUCCEEDED;
}

void dAcGirahimu_c::vt_0x1DC() {
    dAcGirahimuBase_c::vt_0x1DC();

    if (!getStateID().isEqual(StateID_Death) && !getStateID().isEqual(StateID_DeathMiniGame)) {
        if (field_0xD70 && getHealth() == 0) {
            if (MinigameManager::GetInstance()->checkInBossRush()) {
                changeState(StateID_DeathMiniGame);
            } else {
                changeState(StateID_Death);
            }
            return;
        }
    }

    if (field_0xD70) {
        mCyl2.SetTgType(~AT_TYPE_COMMON0);
        mCallback.field_0x04E = false;
        mCyl0.OnTgSet();
        mSph0.ClrTgSet();
        mCyl2.OnTgSet();
    } else {
        mCyl2.SetTgType(~(AT_TYPE_COMMON0 | AT_TYPE_SWORD));
    }
}

int dAcGirahimu_c::actorExecute() {
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        dTgSwordBattleGame_c *pSwordBattleGame =
            static_cast<dTgSwordBattleGame_c *>(fManager_c::searchBaseByProfName(fProfile::TAG_SWORD_BATTLE_GAME));
        if (!pSwordBattleGame->checkFightStarted()) {
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
            const char *label2 = "BOSS_00_caption";
            const char *label = "BOSS_00";
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
    fn_227_3DF0();
    field_0xD81 = false;
    executeState();
    fn_227_3DB0();
    if (field_0xD7C) {
        mAngle.y = mRotation.y;
    }

    mCallback.field_0x0A8.set(1.0f, 0.0f, 0.0f);
    mCallback.field_0x0A8.rotY(mRotation.y);
    mCallback.field_0x084 = mRotation.y;
    mCallback.fn_226_3130();

    mSwordLink.get()->setAngleCopy(mRotation);
    mSwordLink.get()->setGhirahimPosition(mPosition);

    mVec3_c scale = field_0x8B0;
    if (scale.x >= 1.0f) {
        scale.x = 1.0f;
    }
    if (scale.y >= 1.0f) {
        scale.y = 1.0f;
    }
    if (scale.z >= 1.0f) {
        scale.z = 1.0f;
    }
    field_0x8BC.set(scale);
    sLib::addCalcScaledDiff(&mSpeed, field_0x1834, field_0x1858, 100.0f);
    mVec3_c v0(0.0f, 0.0f, 0.0f);
    cLib::chasePos(field_0x185C, v0, 10.0f);
    if (field_0xD7D) {
        calcVelocity();
    } else {
        mVelocity.y += mAcceleration;
    }

    mPosition += field_0x185C;
    if (mPosition.y <= 0) {
        mPosition.y = 0.0f;
    }
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

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
    fn_226_D090();
    fn_226_D2D0();
    mAnmTexPatWink.play();
    fn_226_9690();
    fn_226_9620();
    fn_226_D2C0();
    fn_256_9C90();
    if (--field_0xD5A <= 0) {
        field_0xD5A = 0;
    }

    if (mCallback.field_0x04E || getStateID().isEqual(StateID_Catch) || field_0xD71) {
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

    bool b2 = false;
    for (int i = 0; i < 5; ++i) {
        if (mKnives[i].isLinked() && mKnives[i].get()->field_0x78D) {
            b2 = true;
        }
    }

    int i0 = 0;
    bool playerSound = false;
    for (int i = 0; i < 5; ++i) {
        if (!mKnives[i].isLinked()) {
            break;
        }

        if (b2) {
            mKnives[i].get()->mSph1.ClrAtSet();
        }

        if (mKnives[i].get()->isWaiting() && mKnives[i].get()->field_0x792) {
            i0++;
        }

        if (mKnives[i].get()->isCirclingPlayer()) {
            playerSound = true;
        }

        mKnives[i].get()->field_0x6F0.set(mRotation.x, mRotation.y, mRotation.z);
        mKnives[i].get()->field_0x6D8.set(mPosition);
    }

    if (i0 > 0) {
        if (playerSound) {
            dAcPy_c::GetLinkM()->holdSound(SE_OGhKf_GROUP_FLY_LV);
        } else {
            holdSound(SE_OGhKf_GROUP_FLY_LV);
        }
    }

    if (b0 && vt_0x218()) {
        dSndBgmMgr_c::GetInstance()->fn_80372D70(0);
    }

    return true;
}

void dAcGirahimu_c::initializeState_CatchDamage() {
    mCallback.field_0x2D4 = 0.5f;
    fn_226_D070();
    setAnm("Damage", ANM_Damage, nullptr, m3d::PLAY_MODE_4, 5.0f, 0.5f);
    field_0xD6C = 1;
    mCallback.field_0x088 = false;
    field_0xD4E++;
    mSpeed = 0.0f;
    field_0x1834 = 0.0f;
    field_0xD66 = 0;
}
void dAcGirahimu_c::executeState_CatchDamage() {
    if (field_0xD4E >= 3 || getHealth() <= 60) {
        field_0xD81 = true;
    }

    mCallback.field_0x04E = false;
    sLib::chaseAngle(mRotation.y.ref(), field_0x183E, 0x500);
    if (field_0xD81) {
        mCyl2.ClrTgSet();
        mSph0.ClrTgSet();
        fn_226_CC40();
    }

    if (mAnmChrs[ANMIDX_Body].isStop()) {
        field_0xD58 = 60;
        if (getHealth() > 60) {
            changeState(StateID_Wait);
        } else {
            changeState(StateID_BackStep);
        }
        field_0xD4E = 0;
    }
}
void dAcGirahimu_c::finalizeState_CatchDamage() {
    field_0xD66 = 0;
    field_0xD58 = 0;
    mCallback.field_0x2BC = 0;
    field_0xD81 = false;
}

void dAcGirahimu_c::fn_227_3DB0() {
    if (field_0xD6C == 6 || field_0xD6C == 8) {
        field_0xD81 = true;
    }
    if (field_0xD6C == 6) {
        mCyl2.SetTgType(~(AT_TYPE_COMMON0 | AT_TYPE_SWORD));
    }
}

void dAcGirahimu_c::fn_227_3DF0() {
    if (field_0xD6C == 0) {
        return;
    }

    if (field_0xD6C == 6 || field_0xD6C == 8) {
        field_0xD81 = true;
    }

    if (vt_0x1E0()) {
        return;
    }

    switch (field_0xD6C) {
        case 1: vt_0x1E4(); break;
        case 2: vt_0x1E8(); break;
        case 3: fn_226_C060(); break;
        case 4: fn_226_C1C0(); break;
        case 5: fn_226_C320(); break;
        case 6: fn_227_1EA0(); break;
        case 7: vt_0x1EC(); break;
        case 8: vt_0x1F0(); break;
    }

    if (mMdlSwordA.mCcList.findAtHit() != nullptr) {
        field_0xD80 = true;
    } else if (mMdlSwordB.mCcList.findAtHit() != nullptr) {
        field_0xD80 = true;
    }
}

void dAcGirahimu_c::vt_0x220() {
    if (field_0xD70) {
        for (int i = 0; i < 5; ++i) {
            if (mKnives[i].get() != nullptr && mKnives[i].get()->isWaiting()) {
                changeState(StateID_KnifeAttack);
                return;
            }
        }

        mVec3_c v0 = mPosition - mStartingPos;
        mVec3_c v1(0.0f, 0.0f, 1.0f);
        v1.rotY(mRotation.y);

        if (field_0x1868 >= 1500.0f && v0.dot(v1) < 0.0f && field_0x1838 < 500.0f) {
            field_0x8DC = 0;
            changeState(StateID_HomeWarp);
            return;
        }

        if (getStateID().isEqual(StateID_BackStep)) {
            determineNextMove();
        } else if (field_0xD80) {
            changeState(StateID_BackWalk);
        } else {
            changeState(StateID_G_SwordWait);
        }
    } else if (getHealth() > 60) {
        changeState(StateID_Wait);
    } else if (getStateID().isEqual(StateID_BackStep)) {
        changeState(StateID_G_SwordDemo);
    } else {
        changeState(StateID_BackStep);
    }
}

bool dAcGirahimu_c::vt_0x218() {
    const f32 unused[] = {200.0f, 20.0f, 200.0f};
    return field_0xD70 && getHealth() <= 120;
}
