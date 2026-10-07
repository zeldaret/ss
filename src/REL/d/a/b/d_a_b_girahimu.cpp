#include "d/a/b/d_a_b_girahimu.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/b/d_a_b_girahimu_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_girahimu_knife.h"
#include "d/a/obj/d_a_obj_girahimu_sword_link.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_light_env.h"
#include "d/flag/dungeonflag_manager.h"
#include "d/lyt/d_lyt_boss_caption.h"
#include "d/snd/d_snd_bgm_mgr.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_sword_battle_game.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "s/s_Math.h"
#include "s/s_State.hpp"
#include "toBeSorted/d_emitter.h"
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

void dAcGirahimu_c::initializeState_G_SwordDemo() {}
void dAcGirahimu_c::executeState_G_SwordDemo() {}
void dAcGirahimu_c::finalizeState_G_SwordDemo() {}

void dAcGirahimu_c::initializeState_G_SwordWait() {}
void dAcGirahimu_c::executeState_G_SwordWait() {}
void dAcGirahimu_c::finalizeState_G_SwordWait() {}

void dAcGirahimu_c::initializeState_G_SwordDamage() {}
void dAcGirahimu_c::executeState_G_SwordDamage() {}
void dAcGirahimu_c::finalizeState_G_SwordDamage() {}

void dAcGirahimu_c::initializeState_G_SwordPiyori() {}
void dAcGirahimu_c::executeState_G_SwordPiyori() {}
void dAcGirahimu_c::finalizeState_G_SwordPiyori() {}

void dAcGirahimu_c::determineNextMove() {}
void dAcGirahimu_c::fn_227_1EA0() {
    mCollider.findAtHit();
    mCollider.findTgHit();
}

void dAcGirahimu_c::initializeState_KnifeAttack() {}
void dAcGirahimu_c::executeState_KnifeAttack() {}
void dAcGirahimu_c::finalizeState_KnifeAttack() {}

void dAcGirahimu_c::initializeState_Knife() {}
void dAcGirahimu_c::executeState_Knife() {}
void dAcGirahimu_c::finalizeState_Knife() {}

void dAcGirahimu_c::initializeState_Death() {}
void dAcGirahimu_c::executeState_Death() {}
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
                mWorldMtx.transS(mPosition);
                mWorldMtx.ZXYrotM(mRotation);
                mWorldMtx.scaleM(field_0x8B0);
                mMdlBody.setLocalMtx(mWorldMtx);
                mMdlBody.calc(false);
                return SUCCEEDED;
            }
        }
    } else {
        if (!mbShownBossCaption) {
            mbShownBossCaption = true;
            dLytBossCaption_c::GetInstance()->show("BOSS_00", "BOSS_00_caption");
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
    executeState();
    fn_227_3DB0();
    if (field_0xD7C) {
        mAngle.y = mRotation.y;
    }

    mCallback.field_0x0A8.set(1.0f, 0.0f, 0.0f);
    mCallback.field_0x0A8.rotY(mRotation.y);
    mCallback.field_0x084 = mRotation.y;
    mCallback.fn_226_3130();

    mSwordLink.get()->mAngleCopy = mRotation;
    mSwordLink.get()->mGhirahimPos = mPosition;

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
    field_0x8BC = scale;
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
    mWorldMtx.scaleM(field_0x8B0);
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
            mKnives[i].get()->mSph0.ClrAtSet();
        }

        if (mKnives[i].get()->isWaiting() && mKnives[i].get()->field_0x792) {
            i0++;
        }

        // TODO: Probaly inline
        bool b = false;
        if (mKnives[i].get()->field_0x784 != 6 && mKnives[i].get()->field_0x784 != 7) {
            b = true;
        }
        if (b) {
            playerSound = true;
        }

        mKnives[i].get()->field_0x6F0.set(mRotation);
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
    return field_0xD70 && getHealth() <= 120;
}
