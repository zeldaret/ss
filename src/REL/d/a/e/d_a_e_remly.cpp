#include "d/a/e/d_a_e_remly.h"

#include "c/c_counter.h"
#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/npc/d_a_npc_shinkan2.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_linkage.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_smdl.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event_manager.h"

SPECIAL_ACTOR_PROFILE(E_REMLY, dAcEremly_c, fProfile::E_REMLY, 0xE1, 0, 3);

static dCcD_SrcSph sSphSrc = {
    {{
         AT_TYPE_DAMAGE,
         0xD,
         {0, 0, 0},
         2,
         0,
         0,
         0,
         0,
     }, {
         ~AT_TYPE_COMMON0,
         0x303,
         {0, 0, 0x407},
         8,
         0,
     }, {
         0x28,
     }},
    {
     50.f, },
};

STATE_DEFINE(dAcEremly_c, Wait);
STATE_DEFINE(dAcEremly_c, Walk);
STATE_DEFINE(dAcEremly_c, Run);
STATE_DEFINE(dAcEremly_c, Escape);
STATE_DEFINE(dAcEremly_c, EscapeDash);
STATE_DEFINE(dAcEremly_c, Wind);
STATE_DEFINE(dAcEremly_c, Hold);
STATE_DEFINE(dAcEremly_c, Jump);
STATE_DEFINE(dAcEremly_c, Fly);
STATE_DEFINE(dAcEremly_c, Damage);
STATE_DEFINE(dAcEremly_c, Sleep);
STATE_DEFINE(dAcEremly_c, Scared);
STATE_DEFINE(dAcEremly_c, Stun);
STATE_DEFINE(dAcEremly_c, Water);
STATE_DEFINE(dAcEremly_c, Hear);
STATE_DEFINE(dAcEremly_c, NightSleepDemo);
STATE_DEFINE(dAcEremly_c, NightFoo);
STATE_DEFINE(dAcEremly_c, NightReflectionFoo);
STATE_DEFINE(dAcEremly_c, NightWait);
STATE_DEFINE(dAcEremly_c, NightWalk);
STATE_DEFINE(dAcEremly_c, NightRun);
STATE_DEFINE(dAcEremly_c, NightRet);
STATE_DEFINE(dAcEremly_c, NightJumpAttack);
STATE_DEFINE(dAcEremly_c, BirthWait);

void dAcEremly_c::callback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *result, nw4r::g3d::ResMdl resMdl) {
    static u32 headNode = resMdl.GetResNode("head").GetID();

    if (nodeId == headNode) {
        mMtx_c m;
        result->GetMtx(m);
        m.ZXYrotM(mAng);
        result->SetMtx(m);
    }
}

bool dAcEremly_c::createHeap() {
    TRY_CREATE(mMdl.create(*this, getOarcResFile("Remly"), "Remly", "RemlyWalk", 0x133));
    fn_80030980(mMdl.getModel(), 1, false);

    mMdl.getModel().setCallback(&mMdlCallback);

    nw4r::g3d::ResFile res = mMdl.getMdlFile();
    nw4r::g3d::ResMdl resMdl = mMdl.getModel().getResMdl();
    nw4r::g3d::ResAnmTexPat resTexPat = res.GetResAnmTexPat("RemlyWink");

    bool result;
    TRY_CREATE2(mTexPat.create(resMdl, resTexPat, &mAllocator, nullptr, 1));
    mMdl.getModel().setAnm(mTexPat);

    return true;
}

int dAcEremly_c::actorCreate() {
    CREATE_ALLOCATOR(dAcEremly_c);

    u8 p = getFromParams(0, 0xF);
    field_0xB61 = p != 15 ? p : 0;
    mSleepDemoPlayedSceneflag = getFromParams(4, 0xFF);

    s32 f = getFromParams(12, 0xFF);
    field_0xB08 = f != 0xFF ? f * 100.f : 1000.f;

    mAcch.Set(this, 1, &mAcchCir);

    mBoundingBox.Set(mVec3_c(-150.f, -700.f, -150.f), mVec3_c(150.f, 150.f, 150.f));

    mAcceleration = -3.f;
    mMaxSpeed = -60.f;
    mScale.set(1.f, 1.f, 1.f);

    field_0xB10 = 1.f;
    field_0xB1E.clear();
    mStts.SetRank(5);
    mSph.Set(sSphSrc);
    mSph.SetStts(mStts);

    mMdlCallback.mAng.clear();

    mMdl.getModel().setScale(mScale);
    mMdl.play();

    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);

    mSph.ClrAtSet();
    mLinkage.set(0, 0.f, 0.f, 0.f, nullptr);

    mStartingPos.set(mPosition.x, mPosition.y, mPosition.z);
    field_0xA44.set(mPosition.x, mPosition.y, mPosition.z);
    field_0xB38 = mPosition.y;
    mStartingRot = mRotation;

    mRef1.unlink();
    mNearbyBombRef.unlink();

    field_0xB64 = 1;
    mSph.ClrAtSet(); // Duplicated clear

    mEmitters[0].init(this);
    mEmitters[1].init(this);

    mWaterEffect.init(this, 100.f, mScale.x * 0.7f, 0.f);
    mWaterEffect.setIsSmall(true);

    field_0xA50.x = mPosition.x;
    field_0xA50.y = mPosition.y;
    field_0xA50.z = mPosition.z;
    mAcchCir.SetWall(field_0xB10 * 10.f, field_0xB10 * 60.f);

    mAcch.SetField_0xD4(100.f + mAng(0));
    field_0xB04 = 60.f;

    if (field_0xB61 == 0) {
        mAcch.SetGndThinCellingOff();
        if (dScGame_c::currentSpawnInfo.isNight()) {
            mTargetFiTextID = 1;
        } else {
            mTargetFiTextID = 0;
        }

        if (!fn_177_7330()) {
            setBattleBgmRelated(0);
            changeState(StateID_Sleep);
            mMdl.setAnm("RemlySleep", m3d::PLAY_MODE_4, 0.f);
            mAcchCir.SetWall(30.f, 50.f);
            return SUCCEEDED;
        }

        mScale.set(1.3f, 1.3f, 1.3f);

        field_0xB10 = 1.3f;
        field_0xB40 = 1.3f;

        field_0xB64 = 2;

        (void)mMdl.getModel().getResMdl();
        fn_177_78D0();

        field_0xB48 = 24.f + cM::rndF(24.f);
        if (mSleepDemoPlayedSceneflag == 0xFF ||
            SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSleepDemoPlayedSceneflag)) {
            changeState(StateID_NightWait);
        } else {
            changeState(StateID_NightSleepDemo);
        }
    } else if (field_0xB61 == 1) {
        setBattleBgmRelated(0);
        changeState(StateID_Sleep);
    } else {
        changeState(StateID_Run);
    }
    return SUCCEEDED;
}

int dAcEremly_c::actorPostCreate() {
    if (field_0xB61 == 1) {
        fBase_c *pActor = fManager_c::searchBaseByProfName(fProfile::NPC_SKN2);
        if (pActor != nullptr) {
            dAcNpcSkn2_c *pSkn2 = static_cast<dAcNpcSkn2_c *>(pActor);
            mRef1.link(pSkn2);

            mAcch.ClrRoofNone();
            if (fn_177_75E0()) {
                fn_177_79D0(false);

                mRotation.y = mAngle.y = cLib::targetAngleY(mPosition, mStartingPos);

                updateMatrix();
                mMdl.getModel().setLocalMtx(mWorldMtx);
                mMdl.getModel().calc(false);
                changeState(StateID_Wait);
                return SUCCEEDED;
            }

            changeState(StateID_Sleep);
            return SUCCEEDED;
        }

        mRef1.unlink();
        field_0xB61 = 0;
    }
    return SUCCEEDED;
}

int dAcEremly_c::doDelete() {
    return SUCCEEDED;
}

int dAcEremly_c::actorExecute() {
    mHealth = 100;
    nw4r::g3d::ResMdl resMdl = mMdl.getModel().getResMdl();

    mStateMgr.executeState();
    if (mStateMgr.isState(StateID_BirthWait)) {
        return SUCCEEDED;
    }

    if (field_0xB60 == 32) {
        mSpeed = 0.f;

        if (mMdl.getAnm().isStop()) {
            mMdl.setAnm("RemlyWaitStand", m3d::PLAY_MODE_4, 4.f);
        }
        field_0xB60 = 0;
        field_0xB66 = 1;
        field_0xB48 = 48.f + cM::rndF(48.f);
        field_0xB6A = 1;
        changeState(StateID_Wait);
    }

    if (!isState(StateID_Hold)) {
        mSph.OnCoSet();
    }

    if ((cCounter_c::GetGameFrame() & 0xF) == 0) {
        dAcBomb_c *pBomb = dAcPy_c::getBombWithinRadius(this, mPosition, 600.f);
        if (pBomb != nullptr) {
            mNearbyBombRef.link(pBomb);
            field_0xB6C = 1;
        } else {
            mNearbyBombRef.unlink();
        }
    }

    if (isState(StateID_EscapeDash) || isState(StateID_Water) || isState(StateID_Scared)) {
        if (fn_177_7650(false)) {
            if (fn_177_7330()) {
                deleteRequest();
                return SUCCEEDED;
            }
            changeState(StateID_BirthWait);
        }
    }

    if (0 == sLib::calcTimer(&field_0xB4E)) {
        s32 damageTypeMaybe = fn_8002fde0(mSph, nullptr);
        switch (damageTypeMaybe) {
            case 7: {
                if (isState(StateID_Fly) && field_0xB60 != 20) {
                    if (field_0xB60 == 16) {
                        mMdl.setAnm("RemlyFlyDamage", m3d::PLAY_MODE_4, 4.f);
                        field_0xB60 = 17;
                        field_0xB4E = 48;
                    }
                } else {
                    if (field_0xB60 == 6) {
                        mMdl.setAnm("RemlySwimDamage", m3d::PLAY_MODE_4, 4.f);
                        field_0xB60 = 8;
                    } else {
                        mSph.ClrAtSet();
                        changeState(StateID_Wind);
                    }
                }
            } break;
            case 12: {
                if (!isState(StateID_Fly) && field_0xB60 != 6 && field_0xB60 != 11) {
                    if (mSph.ChkCoHit() && mSph.ChkTgAtHitType(AT_TYPE_0x80000 | AT_TYPE_SLINGSHOT)) {
                        mAngle.y = getXZAngleToPlayer();
                        mSpeed = -15.f;
                        changeState(StateID_Stun);
                        break;
                    }
                }
            }
            // fallthrough
            default: {
                if (damageTypeMaybe != 0) {
                    field_0xB52 = 8;
                    field_0xB4E = 8;
                    dJEffManager_c::spawnHitMarkEffect(6, mSph, nullptr, true);

                    if (isState(StateID_Fly) && field_0xB60 != 20) {
                        if (field_0xB60 == 16) {
                            mMdl.setAnm("RemlyFlyDamage", m3d::PLAY_MODE_4, 4.f);
                            field_0xB60 = 17;
                        }
                    } else if (field_0xB60 == 6 || field_0xB60 == 8) {
                        mMdl.setAnm("RemlySwimDamage", m3d::PLAY_MODE_4, 4.f);
                        field_0xB60 = 8;
                    } else {
                        if (field_0xB64 != 0) {
                            field_0xB64--;
                            if (mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_BOMB | AT_TYPE_ARROW)) {
                                field_0xB64 = 0;
                            }
                        }
                        field_0xB6A = 0;
                        mSph.ClrAtSet();

                        field_0xB68 = 1;
                        field_0xB6B = 0;

                        if (isState(StateID_Scared)) {
                            field_0xB6B = 1;
                        }

                        changeState(StateID_Damage);
                    }
                }
            } break;
        }
    }

    if (!isState(StateID_Hold)) {
        field_0xAA4 = mPosition;
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
        mPosition += field_0xAF8;
        field_0xAF8 *= 0.7f;
    }

    if (fn_177_7330()) {
        if (field_0xB64 == 0 || fn_177_8AC0()) {
            if (field_0xB40 != 1.0f) {
                field_0xB54 = 7;
                field_0xB40 = 1.f;
            }
        } else {
            if (field_0xB40 != 1.3f) {
                field_0xB54 = 7;
                field_0xB40 = 1.3f;
            }
        }

        if (0 != sLib::calcTimer(&field_0xB54)) {
            sLib::addCalcScaledDiff(&field_0xB10, field_0xB40, 1.f, 0.05f);
        }
    }

    mScale.set(field_0xB10, field_0xB10, field_0xB10);

    mMdl.getModel().setScale(mScale);
    mMdl.play();
    if (!isState(StateID_Hold)) {
        mAcch.CrrPos(*dBgS::GetInstance());
        mWaterEffect.execute(mAcch.GetWtrGroundH(), mAcch.GetGroundH());
    } else {
        mLinkage.bushTpFunc(mAcch);
    }

    if (!isState(StateID_Hold) && !isState(StateID_NightSleepDemo) && !isState(StateID_Damage) &&
        !isState(StateID_Fly) && !isState(StateID_Jump)) {
        sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);
    }

    if (!isState(StateID_Fly) && !isState(StateID_Hold) && !isState(StateID_BirthWait) && !isState(StateID_Water)) {
        if (!mAcch.ChkGndHit()) {
            if (fn_177_8980(100.f) && field_0xB58 == 0) {
                field_0xB58 = 1;
            }
            if (field_0xB58 != 0) {
                if (field_0xB61 == 1 && !fn_177_75E0() && EventManager::isInEvent()) {
                    setActorProperty(AC_PROP_0x1);
                    mPosition.set(mStartingPos.x, mStartingPos.y, mStartingPos.z);
                    mRotation.y = mStartingRot.y;
                    mAngle.y = mStartingRot.y;
                    changeState(StateID_Sleep);
                }
                field_0xB58++;
                // Why cast?
                if ((s32)field_0xB58 > 30) {
                    changeState(StateID_Fly);
                }
            }

            field_0xB38 = mPosition.y;
        } else {
            sLib::chase(&field_0xB38, mPosition.y, 10.f);
            fn_177_8520(false);

            if (field_0xB24 != 0 || field_0xB26 != 0) {
                field_0xB38 = mPosition.y;
            }
            field_0xB58 = 0;
        }
    } else {
        field_0xB26 = 0;
        field_0xB24 = 0;
        field_0xB38 = mPosition.y + field_0xB3C;
    }

    if (!fn_177_7B10() && isState(StateID_Water)) {
        field_0xB6B = 1;
    }

    sLib::addCalcAngle(field_0xB1E.x.ref(), field_0xB24, 2, 0x1000);
    sLib::addCalcAngle(field_0xB1E.z.ref(), field_0xB26, 2, 0x1000);

    mWorldMtx.transS(mPosition.x, field_0xB38, mPosition.z);

    mWorldMtx.XrotM(field_0xB1E.x);
    mWorldMtx.ZrotM(field_0xB1E.z);
    mWorldMtx.ZXYrotM(mRotation);

    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);
    fn_177_6A40();

    f32 radius = 40.f + mAng(0);
    mVec3_c center;

    center.x = mPosition.x;
    center.y = mPosition.y;
    center.z = mPosition.z;

    center.y += (30.f + mAng(0));

    mSph.SetC(center);
    if (isState(StateID_Fly)) {
        mMtx_c m;
        mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone3").GetID(), m);
        m.getTranslation(center);
        mSph.SetC(center);
    }
    mSph.SetR(radius * field_0xB10);

    dCcS::GetInstance()->Set(&mSph);

    if (!(fn_177_7330() && field_0xB64)                 // Check Batreaux Human
        && !(isState(StateID_Hold) || field_0xB60 == 6) // Hold
        && !isState(StateID_Stun) && !isState(StateID_Jump)) {
        AttentionManager::GetInstance()->addPickUpTarget(*this, 3.f * radius * field_0xB10);
        if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
            changeState(StateID_Hold);
        }
    }

    if (field_0xB60 == 7) {
        mMtx_c m;
        mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone3").GetID(), m);
        m.getTranslation(center);

        mEmitters[1].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_859_, center, nullptr, nullptr, nullptr, nullptr);
    }

    mPositionCopy2 = mPosition;
    mPositionCopy2.y += 30.f;

    mPositionCopy3 = mPositionCopy2;
    mPositionCopy3.y += 50.f;

    if (isState(StateID_Fly)) {
        mPositionCopy3.y += 50.f;
    }

    if (!isState(StateID_Hold)) {
        if (!fn_177_7330()) {
            setInteractionFlags(INTERACT_0x2000);
        }
        fn_80030c20(3, 600.f, radius, -400.f, 400.f);
    }

    sLib::calcTimer(&field_0xB52);
    fn_80030400(mMdl.getModel(), 100, false, field_0xB52);
    return SUCCEEDED;
}

int dAcEremly_c::draw() {
    if (mStateMgr.isState(StateID_BirthWait)) {
        return SUCCEEDED;
    }

    if (mStateMgr.isState(StateID_Fly)) {
        if (EventManager::isInEvent()) {
            if (EventManager::isCurrentEvent("RescuePlayer") || EventManager::isCurrentEvent("RescuePlayerToUpward")) {
                return SUCCEEDED;
            }
        }
    }

    m3d::smdl_c &mdl = mMdl.getModel();
    drawModelType1(&mdl);

    static mQuat_c shadow = mQuat_c(mVec3_c(0, 70.f + mAng(0), 0), 290.f + mAng(0));
    fn_8002edb0(mShadow, mdl, &shadow, -1, -1, mPosition.y - mAcch.GetGroundH());

    return SUCCEEDED;
}

void dAcEremly_c::initializeState_Wait() {
    field_0xB14 = 0;
    mAcceleration = -3.f;

    fn_177_8520(true);

    if (field_0xB66 || field_0xB6A) {
        mMdl.setAnm("RemlyWaitStand", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 0;

        field_0xB48 = cM::rndF(48.f) + 48.f;
    } else {
        mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 1;

        field_0xB44 = cM::rndF(128.f) + 128.f;
    }
}
void dAcEremly_c::executeState_Wait() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    if (field_0xB6A == 0) {
        fn_177_6EA0(false);
    }
    sLib::addCalcScaled(&mSpeed, 0.7f, 5.f);

    if (fn_177_7330()) {
        changeState(StateID_EscapeDash);
        return;
    }

    if (fn_177_86C0()) {
        return;
    }

    if (field_0xB60 == 2) {
        if (mMdl.getAnm().isStop()) {
            mMdl.setAnm("RemlytWaitSit", m3d::PLAY_MODE_4, 10.f);
            field_0xB60 = 1;
        }
        return;
    }

    if (fn_177_73C0()) {
        if (field_0xB6E == 0) {
            if (fn_800301b0(mPositionCopy2, mRotation.y + 0x8000, true, 140.f) != 0 /* TODO: Enum?*/) {
                field_0xB6E = 1;
            } else if (isWithinPlayerRadius(600.f) || mNearbyBombRef.isLinked()) {
                changeState(StateID_Escape);
                field_0xB6A = 1;
            } else {
                fn_177_6B10(false, 0);
                field_0xB48 = 48.f + cM::rndF(48.f);
                field_0xB44 = 0x80;
                if (isWithinPlayerRadius(250.f)) {
                    changeState(StateID_Scared);
                }
            }
        } else if (fn_177_6B10(0, 0)) {
            changeState(StateID_Escape);
            field_0xB6A = 1;
            field_0xB48 = 20;
            field_0xB44 = 128;
        } else if (isWithinPlayerRadius(250.f)) {
            changeState(StateID_Scared);
        }
        return;
    }

    field_0xB6E = 0;
    fn_177_6B10(0, 0);
    if (field_0xB56 != 0 || fn_177_8F90()) {
        return;
    }

    if (0 == sLib::calcTimer(&field_0xB48) && field_0xB66 != 0) {
        field_0xB66 = 0;
        field_0xB6A = 0;
        field_0xB6E = 0;
        field_0xB44 = 128.f + cM::rndF(128.f);

        changeState(StateID_Wait);
        return;
    }

    if (field_0xB61 == 1) {
        return;
    }
    if (sLib::calcTimer(&field_0xB44)) {
        return;
    }

    // Maybe Inline
    bool coPlayer = false;
    if (mSph.ChkCoHit()) {
        coPlayer = true;
        if (mSph.GetCoActor()->isActorPlayer()) {
            coPlayer = false;
        }
    }

    if (field_0xB50 == 0 && !coPlayer && !fn_177_8C20(getXZAngleToPlayer())) {
        mAng a = (pPlayer->mRotation.y + 0x8000);
        if (mAng::abs(getXZAngleToPlayer() - a) > 0x2000) {
            field_0xB69 = 1;
            field_0xB67 = 0;

            changeState(StateID_Walk);
            return;
        }
    }

    if (field_0xB60 == 1) {
        mMdl.setAnm("RemlytWaitSitCry", m3d::PLAY_MODE_4, 10.f);
        field_0xB60 = 2;
    }
    field_0xB44 = 128.f + cM::rndF(128.f);
}
void dAcEremly_c::finalizeState_Wait() {}

void dAcEremly_c::initializeState_Walk() {
    if (field_0xB67 != 0 && field_0xB6A == 0) {
        mMdl.setAnm("RemlyCryWalk", m3d::PLAY_MODE_4, 10.f);
        field_0xB60 = 3;
    } else {
        mMdl.setAnm("RemlyWalk", m3d::PLAY_MODE_4, 10.f);
        field_0xB60 = 4;
    }

    field_0xB32 = 0;
    field_0xB34 = 0;

    if (field_0xB69 != 0) {
        fn_177_77C0();
    }
    field_0xB14 = 0;
    field_0xB66 = 1;
}
void dAcEremly_c::executeState_Walk() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    fn_177_6FC0(true);

    if (fn_177_86C0()) {
        return;
    }

    if (!field_0xB6A && mAng::abs(getXZAngleToPlayer() - mRotation.y) < 0x4000 && fn_177_73C0() && field_0xB6E == 0) {
        changeState(StateID_Escape);
        field_0xB6A = 1;
        return;
    }

    if (field_0xB69 != 0) {
        mMtx_c m;
        m.YrotS(field_0xB32);
        mVec3_c in(0.f, 0.f, 180.f), out;
        m.multVec(in, out);
        out += mPosition;
        field_0xA44.set(out);

        if (fn_177_7040(2, 0.8f)) {
            field_0xB32 += field_0xB34;
        }
        bool b0 = false;
        if (mSph.ChkCoHit()) {
            b0 = true;
            if (mSph.GetCoActor()->isActorPlayer()) {
                mAng a = (pPlayer->mRotation.y + 0x8000);
                if (mAng::abs(getXZAngleToPlayer() - a) > mAng(1024.f + cM::rndF(1024.f))) {
                    b0 = false;
                }
            }
        }

        if (!b0) {
            mAng a = (pPlayer->mRotation.y + 0x8000);
            if (mAng::abs(getXZAngleToPlayer() - a) >= mAng(1024.f + cM::rndF(1024.f))) {
                if (fn_177_8C20(mRotation.y)) {
                    field_0xB69 = 0;
                    changeState(StateID_Walk);
                }
            }
            return;
        }
    }

    if (field_0xB6A == 0) {
        if (!fn_177_8C20(mRotation.y)) {
            fn_177_6B10(false, 0);

            if (fn_177_75E0()) {
                field_0xA44.set(mNearbyBombRef.get()->mPosition);
                fn_177_7040(2, 0.8f);
            } else {
                fn_177_7040(0, 0.8f);
            }
        }
        return;
    }
    sLib::addCalcScaled(&mSpeed, 0.7f, 3.f + mAng(0));
    fn_177_7040(0, 0.f);
    if (mAng::abs(getXZAngleToPlayer() - mRotation.y) < 0x400) {
        if (!fn_177_6B10(false, -1000)) {
            changeState(StateID_Wait);
            mMdl.setAnm("RemlyWaitSitCry", m3d::PLAY_MODE_4, 2.f);
            field_0xB60 = 2;
        }
        return;
    }

    if (field_0xB60 == 3 && mMdl.getAnm().isStop()) {
        field_0xB67 = 0;
        mMdl.setAnm("RemlyWalk", m3d::PLAY_MODE_4, 2.f);
        field_0xB60 = 4;
    }

    if (fn_177_8F90()) {
        return;
    }
}
void dAcEremly_c::finalizeState_Walk() {}

void dAcEremly_c::initializeState_Run() {
    mMdl.setAnm("RemlyRun", m3d::PLAY_MODE_4, 2.f);
    field_0xB60 = 5;
    mMdl.setRate(1.5f);
    field_0xB66 = 1;
}
void dAcEremly_c::executeState_Run() {
    fn_177_6FC0(true);
    fn_177_6B10(false, 0);
    if (fn_177_86C0()) {
        return;
    }
    if (fn_177_73C0()) {
        if (isWithinPlayerRadius(600.f)) {
            changeState(StateID_Escape);
            field_0xB6A = 1;
        } else {
            changeState(StateID_Walk);
        }
        return;
    }
    if (fn_177_75E0()) {
        field_0xA44.set(mNearbyBombRef.get()->mPosition);
        fn_177_7040(2, 5.f);
    } else {
        fn_177_7040(0, 5.f);
    }
    if (fn_177_8F90()) {
        return;
    }
}
void dAcEremly_c::finalizeState_Run() {}

void dAcEremly_c::initializeState_Escape() {}
void dAcEremly_c::executeState_Escape() {}
void dAcEremly_c::finalizeState_Escape() {}
void dAcEremly_c::initializeState_EscapeDash() {}
void dAcEremly_c::executeState_EscapeDash() {}
void dAcEremly_c::finalizeState_EscapeDash() {}
void dAcEremly_c::initializeState_Wind() {}
void dAcEremly_c::executeState_Wind() {}
void dAcEremly_c::finalizeState_Wind() {}
void dAcEremly_c::initializeState_Hold() {}
void dAcEremly_c::executeState_Hold() {}
void dAcEremly_c::finalizeState_Hold() {}
void dAcEremly_c::initializeState_Jump() {}
void dAcEremly_c::executeState_Jump() {}
void dAcEremly_c::finalizeState_Jump() {}
void dAcEremly_c::initializeState_Fly() {}
void dAcEremly_c::executeState_Fly() {}
void dAcEremly_c::finalizeState_Fly() {}
void dAcEremly_c::initializeState_Damage() {}
void dAcEremly_c::executeState_Damage() {}
void dAcEremly_c::finalizeState_Damage() {}
void dAcEremly_c::initializeState_Sleep() {}
void dAcEremly_c::executeState_Sleep() {}
void dAcEremly_c::finalizeState_Sleep() {}
void dAcEremly_c::initializeState_Scared() {}
void dAcEremly_c::executeState_Scared() {}
void dAcEremly_c::finalizeState_Scared() {}
void dAcEremly_c::initializeState_Stun() {}
void dAcEremly_c::executeState_Stun() {}
void dAcEremly_c::finalizeState_Stun() {}
void dAcEremly_c::initializeState_Water() {}
void dAcEremly_c::executeState_Water() {}
void dAcEremly_c::finalizeState_Water() {}
void dAcEremly_c::initializeState_Hear() {}
void dAcEremly_c::executeState_Hear() {}
void dAcEremly_c::finalizeState_Hear() {}
void dAcEremly_c::initializeState_NightSleepDemo() {}
void dAcEremly_c::executeState_NightSleepDemo() {}
void dAcEremly_c::finalizeState_NightSleepDemo() {}
void dAcEremly_c::initializeState_NightFoo() {}
void dAcEremly_c::executeState_NightFoo() {}
void dAcEremly_c::finalizeState_NightFoo() {}
void dAcEremly_c::initializeState_NightReflectionFoo() {}
void dAcEremly_c::executeState_NightReflectionFoo() {}
void dAcEremly_c::finalizeState_NightReflectionFoo() {}
void dAcEremly_c::initializeState_NightWait() {}
void dAcEremly_c::executeState_NightWait() {}
void dAcEremly_c::finalizeState_NightWait() {}
void dAcEremly_c::initializeState_NightWalk() {}
void dAcEremly_c::executeState_NightWalk() {}
void dAcEremly_c::finalizeState_NightWalk() {}
void dAcEremly_c::initializeState_NightRun() {}
void dAcEremly_c::executeState_NightRun() {}
void dAcEremly_c::finalizeState_NightRun() {}
void dAcEremly_c::initializeState_NightRet() {}
void dAcEremly_c::executeState_NightRet() {}
void dAcEremly_c::finalizeState_NightRet() {}
void dAcEremly_c::initializeState_NightJumpAttack() {}
void dAcEremly_c::executeState_NightJumpAttack() {}
void dAcEremly_c::finalizeState_NightJumpAttack() {}
void dAcEremly_c::initializeState_BirthWait() {}
void dAcEremly_c::executeState_BirthWait() {}
void dAcEremly_c::finalizeState_BirthWait() {}

bool dAcEremly_c::fn_177_73C0() {
    if (isState(StateID_Sleep) || fn_177_7330() || field_0xB68) {
        return false;
    }

    dAcPy_c *pPlayer = dAcPy_c::GetLinkM();
    if (pPlayer->isUsingSword() || pPlayer->checkActionFlagsCont(0x2) || pPlayer->checkActionFlagsCont(0x80) ||
        pPlayer->isCarryingBomb() || pPlayer->isUsingWhip() || pPlayer->checkActionFlagsCont(0x4) ||
        pPlayer->checkActionFlagsCont(0x100) || pPlayer->checkActionFlagsCont(0x10)) {
        return true;
    }

    if (mNearbyBombRef.isLinked()) {
        return true;
    }

    field_0xB6C = 0;
    return false;
}
