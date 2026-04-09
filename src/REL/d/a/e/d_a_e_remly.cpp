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
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_wtr_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "d/t/d_t_stream.h"
#include "f/f_base.h"
#include "f/f_manager.h"
#include "f/f_profile_name.h"
#include "m/m3d/m3d.h"
#include "m/m3d/m_fanm.h"
#include "m/m3d/m_smdl.h"
#include "m/m_angle.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/attention.h"
#include "toBeSorted/d_d3d.h"
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
         ~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x8000 | AT_TYPE_WIND),
         0x303,
         {0, 0, 0x40F},
         8,
         0,
     }, {
         0xE5,
     }},
    {
     50.f, },
};

struct dAcEremly_HIO_c {
    f32 _0x00;
    f32 _0x04;
    f32 _0x08;
    f32 _0x0C;
    f32 _0x10;
    f32 _0x14;
    f32 _0x18;
    f32 _0x1C;
    f32 _0x20;
    f32 _0x24;
    f32 _0x28;
    f32 _0x2C;
    f32 _0x30;
    f32 _0x34;
    f32 _0x38;
    f32 _0x3C;
    f32 _0x40;
    f32 _0x44;
    s16 _0x48;
    f32 _0x4C;
    f32 _0x50;
    f32 _0x54;
    f32 _0x58;
    s16 _0x5C;
    s16 _0x5E;
    s16 _0x60;
    s16 _0x62;
    s16 _0x64;
    f32 _0x68;
    f32 _0x6C;
    static const dAcEremly_HIO_c sInstance;
};
const dAcEremly_HIO_c dAcEremly_HIO_c::sInstance = {
    1.f,  1000.f, 500.0f, 200.0f, 120.0f, 250.0f, 400.0f, 400.0f, 600.0f, 0.8f, 5.0f, 20.0f, 10.0f, -8.0f, 20.0f,
    7.0f, 15.0f,  50.0f,  30,     1000.f, 300.f,  8.f,    1.3f,   300,    300,  30,   90,    90,    40.f,  0.f,
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
        m.ZXYrotM(mHeadRotation);
        result->SetMtx(m);
    }
}

bool dAcEremly_c::createHeap() {
    TRY_CREATE(mMdl.create3(*this, getOarcResFile("Remly"), "Remly", "RemlyWalk", 0x133));
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
    s32 _weird_zero = 0;

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

    mMdlCallback.mHeadRotation.clear();

    mMdl.getModel().setScale(mScale);
    mMdl.play();

    updateMatrix();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);

    mSph.ClrAtSet();
    mLinkage.set(0, 0.f, 0.f, 0.f, nullptr);

    mStartingPos.set(mPosition.x, mPosition.y, mPosition.z);
    mTargetPosition.set(mPosition.x, mPosition.y, mPosition.z);
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

    mAcch.SetField_0xD4(100.f + _weird_zero);
    field_0xB04 = 60.f;

    if (field_0xB61 == 0) {
        mAcch.SetGndThinCellingOff();
        if (dScGame_c::currentSpawnInfo.isNight()) {
            mTargetFiTextID = 1;
        } else {
            mTargetFiTextID = 0;
        }

        if (!isScary()) {
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
    s32 _weird_zero = 0;

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
            field_0xB60 = 0;
            field_0xB66 = 1;
            field_0xB48 = 48.f + cM::rndF(48.f);
            field_0xB6A = 1;
            changeState(StateID_Wait);
        }
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
            if (isScary()) {
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
                        field_0xB4E = 90;
                        field_0xB60 = 8;
                    } else {
                        mSph.ClrAtSet();
                        changeState(StateID_Wind);
                    }
                }
            } break;
            case 12: {
                if (!isState(StateID_Fly) && field_0xB60 != 6 && field_0xB60 != 8) {
                    if (mSph.ChkTgHit() && mSph.ChkTgAtHitType(AT_TYPE_0x80000 | AT_TYPE_SLINGSHOT)) {
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
        field_0xAA4.set(mPosition);
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
        mPosition += field_0xAF8;
        field_0xAF8 *= 0.7f;
    }

    if (isScary()) {
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
                if ((s32)++field_0xB58 > 30) {
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
    playWink();

    f32 radius = 40.f + _weird_zero;
    mVec3_c center;

    center.x = mPosition.x;
    center.y = mPosition.y;
    center.z = mPosition.z;

    center.y += 30.f + _weird_zero;

    mMtx_c m;
    mSph.SetC(center);
    if (isState(StateID_Fly)) {
        mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone3").GetID(), m);
        m.getTranslation(center);
        mSph.SetC(center);
    }
    mSph.SetR(radius * field_0xB10);

    dCcS::GetInstance()->Set(&mSph);

    if (!(isScary() && field_0xB64)                     // Check Batreaux Human
        && !(isState(StateID_Hold) || field_0xB60 == 6) // Hold
        && !isState(StateID_Stun) && !isState(StateID_Jump)) {
        AttentionManager::GetInstance()->addPickUpTarget(*this, 3.f * radius * field_0xB10);
        if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
            changeState(StateID_Hold);
        }
    }

    if (field_0xB60 == 7) {
        mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone02").GetID(), m);
        m.getTranslation(center);

        mEmitters[1].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_859_, center, nullptr, nullptr, nullptr, nullptr);
    }

    mPositionCopy2.set(mPosition);
    mPositionCopy2.y += 30.f;

    mPositionCopy3.set(mPositionCopy2);
    mPositionCopy3.y += 50.f;

    if (isState(StateID_Fly)) {
        mPositionCopy3.y += 50.f;
    }

    if (!isState(StateID_Hold)) {
        if (!isScary()) {
            setInteractionFlags(INTERACT_0x2000);
        }
        fn_80030c20(3, 600.f, radius, -400.f, 400.f);
    }

    sLib::calcTimer(&field_0xB52);
    fn_80030400(mMdl.getModel(), 100, false, field_0xB52);
    return SUCCEEDED;
}

int dAcEremly_c::draw() {
    s32 _weird_zero = 0;

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

    // elp
    static mQuat_c shadow(0.f, 70.f + _weird_zero, 0.f, 290.f + _weird_zero);
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
        mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 10.f);
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

    if (isScary()) {
        changeState(StateID_EscapeDash);
        return;
    }

    if (fn_177_86C0()) {
        return;
    }

    if (field_0xB60 == 2) {
        if (mMdl.getAnm().isStop()) {
            mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 10.f);
            field_0xB60 = 1;
        }
        return;
    }

    if (shouldBeScared()) {
        if (field_0xB6E == 0) {
            if (fn_800301b0(mPositionCopy2, mRotation.y + 0x8000, true, 140.f) != 0) {
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
        } else if (fn_177_6B10(false, 0)) {
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
    fn_177_6B10(false, 0);
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
        if (mAng::absDiff2(getXZAngleToPlayer(), mAng(pPlayer->mRotation.y + 0x8000)) > 0x2000) {
            field_0xB69 = 1;
            field_0xB67 = 0;

            changeState(StateID_Walk);
            return;
        }
    }

    if (field_0xB60 == 1) {
        mMdl.setAnm("RemlyWaitSitCry", m3d::PLAY_MODE_4, 10.f);
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

    // These two values somehow need to be before the RemlyCryWalk data
    field_0xB32 = 0;
    field_0xB34 = 0;

    if (field_0xB69 != 0) {
        fn_177_77C0();
    }
    field_0xB14 = 0;
    field_0xB66 = 1;
}
void dAcEremly_c::executeState_Walk() {
    s32 _weird_zero = 0;

    bool b0 = false;
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    adjustHeadRotation(true);

    if (fn_177_86C0()) {
        return;
    }

    if (!field_0xB6A && mAng::absDiff2(getXZAngleToPlayer(), mRotation.y) < 0x4000 && shouldBeScared() &&
        field_0xB6E == 0) {
        changeState(StateID_Escape);
        field_0xB6A = 1;
        return;
    }

    if (field_0xB69 != 0) {
        mMtx_c m;
        m.YrotS(field_0xB32);
        mVec3_c in(0.f, 0.f, 180.f), out;
        m.multVec(in, out);
        out += pPlayer->mPosition;
        mTargetPosition.set(out);

        if (fn_177_7040(2, 0.8f)) {
            field_0xB32 += field_0xB34;
        }
        if (mSph.ChkCoHit()) {
            b0 = true;
            if (mSph.GetCoActor()->isActorPlayer() &&
                mAng::absDiff2<mAng>(getXZAngleToPlayer(), pPlayer->mRotation.y + 0x8000) >
                    mAng(1024.f + cM::rndF(1024.f))) {
                b0 = false;
            }
        }

        if (b0 ||
            mAng::absDiff2<mAng>(getXZAngleToPlayer(), pPlayer->mRotation.y + 0x8000) <
                mAng(1024.f + cM::rndF(1024.f)) ||
            fn_177_8C20(mRotation.y)) {
            field_0xB69 = 0;
            changeState(StateID_Wait);
            return;
        }
    } else if (field_0xB6A == 0 && !fn_177_8C20(mRotation.y)) {
        fn_177_6B10(false, 0);

        if (fn_177_75E0()) {
            mTargetPosition.set(mRef1.get()->mPosition);
            fn_177_7040(2, 0.8f);
        } else {
            fn_177_7040(0, 0.8f);
        }
    } else {
        sLib::addCalcScaled(&mSpeed, 0.7f, 3.f + _weird_zero);
        fn_177_7040(0, 0.f);
        if (mAng::absDiff2<mAng>(getXZAngleToPlayer(), mRotation.y) < 0x400 && !fn_177_6B10(false, -1000)) {
            changeState(StateID_Wait);
            mMdl.setAnm("RemlyWaitSitCry", m3d::PLAY_MODE_4, 4.f);
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
    adjustHeadRotation(true);
    fn_177_6B10(false, 0);
    if (fn_177_86C0()) {
        return;
    }
    if (shouldBeScared()) {
        if (isWithinPlayerRadius(600.f)) {
            changeState(StateID_Escape);
            field_0xB6A = 1;
        } else {
            changeState(StateID_Walk);
        }
        return;
    }
    if (fn_177_75E0()) {
        mTargetPosition.set(mRef1.get()->mPosition);
        fn_177_7040(2, 5.f);
    } else {
        fn_177_7040(0, 5.f);
    }
    if (fn_177_8F90()) {
        return;
    }
}
void dAcEremly_c::finalizeState_Run() {}

void dAcEremly_c::initializeState_Escape() {
    field_0xB6E = 0;
    mAcceleration = -3.f;
    mMdl.setAnm("RemlyRun", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 5;
    mMdl.setRate(2.f);
    field_0xB66 = 1;
    field_0xB6A = 0;
    field_0xB69 = 0;
    field_0xB48 = 0;
    field_0xB18 = 0;
}
void dAcEremly_c::executeState_Escape() {
    s32 _weird_zero = 0;

    adjustHeadRotation(false);
    if (0 == sLib::calcTimer(&field_0xB48)) {
        fn_177_7040(1, 20.f);
    } else {
        sLib::addCalcScaledDiff(&mSpeed, 20.f, 0.7f, 3.f);
    }

    if (fn_177_8C20(mRotation.y) || fn_177_8980(140.f)) {
        field_0xB6E = 1;
        if (field_0xB64 == 0) {
            changeState(StateID_Scared);
        } else {
            field_0xB69 = 0;
            changeState(StateID_Walk);
        }
        return;
    }

    if (field_0xB6A != 0) {
        f32 f = (_weird_zero + 400.f);
        field_0xB6C = 1;
        if (!fn_177_7510(600.f + f) && !mNearbyBombRef.isLinked()) {
            field_0xB69 = 0;
            changeState(StateID_Walk);
        }
    }
}
void dAcEremly_c::finalizeState_Escape() {}

void dAcEremly_c::initializeState_EscapeDash() {
    mMdl.setAnm("RemlyRun", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 5;
    mMdl.setRate(2.f);
    field_0xB66 = 1;
    field_0xB6A = 0;
    field_0xB6B = 0;
    field_0xB18 = 0;
}
void dAcEremly_c::executeState_EscapeDash() {
    adjustHeadRotation(false);

    // Idk
    s32 a = getXZAngleToPlayer() + 0x8000;
    if (field_0xB6B == 0) {
        if (mAng::absDiff2<mAng>(a, mAngle.y) > 0x1000) {
            sLib::addCalcAngle(mAngle.y.ref(), a, 1, 0x1000);
        } else {
            field_0xB6B = 1;
            mAngle.y = a;
        }
    }

    if (field_0xB6B != 0) {
        mVec3_c v(getPosition());
        sLib::addCalcScaledDiff(&mSpeed, 30.f, 0.7f, 3.f);

        if (fn_177_8C20(mRotation.y)) {
            changeState(StateID_Scared);
        } else if (fn_177_8980(200.f)) {
            changeState(StateID_Scared);
        }
    }
}
void dAcEremly_c::finalizeState_EscapeDash() {}

void dAcEremly_c::initializeState_Wind() {
    if (field_0xB60 != 10 && field_0xB60 != 9) {
        if (field_0xB60 == 21 || mAng::absDiff2<mAng>(getXZAngleToPlayer(), mRotation.y + 0x8000) < 0x6000) {
            mMdl.setAnm("RemlyWindBack", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 10;
        } else {
            mMdl.setAnm("RemlyWind", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 9;
        }
        field_0xB4E = 11;
    }
    field_0xB48 = 11;
}
void dAcEremly_c::executeState_Wind() {
    adjustHeadRotation(false);
    sLib::addCalcScaled(&mSpeed, 0.7f, 5.f);
    if (field_0xB60 == 9) {
        fn_177_7040(0, 0.f);
        sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);
    }

    if (0 != sLib::calcTimer(&field_0xB48)) {
        return;
    }

    if (field_0xB64 == 0) {
        changeState(StateID_EscapeDash);
        return;
    }

    if (!isScary()) {
        field_0xB66 = 1;
        changeState(StateID_Wait);

        mMdl.setAnm("RemlyWaitStand", m3d::PLAY_MODE_4, 10.f);
        field_0xB60 = 0;
        return;
    }

    if (fn_177_9370(100.f)) {
        changeState(StateID_NightRun);
    } else {
        changeState(StateID_NightRet);
    }
}
void dAcEremly_c::finalizeState_Wind() {}

void dAcEremly_c::initializeState_Hold() {
    s32 _weird_zero = 0;

    mMdl.setAnm("RemlyHold", m3d::PLAY_MODE_4, 4.f);

    field_0xB60 = 11;
    mSph.ClrCoSet();
    field_0xB3C = 0.f;
    field_0xB38 = mPosition.y;

    field_0xB6A = 0;
    field_0xB6C = 0;
    field_0xB56 = 0;
    field_0xB6E = 0;

    mStts.SetRank(5);
    field_0xB1E.clear();
    field_0xB26 = 0;
    field_0xB24 = 0;
    if (isScary() || field_0xB68) {
        mMdl.setAnm("RemlyHoldBata", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 13;
    }

    if (isScary()) {
        field_0xB64 = 2;
        field_0xB68 = 0;
    }

    mLinkage.field_0x90 = 13.f + _weird_zero;
    mLinkage.field_0x8C = 13.f + _weird_zero;
    if (field_0xB61 == 1) {
        mLinkage.field_0x90 = 26.f + _weird_zero;
        mLinkage.field_0x8C = 13.f + _weird_zero;
    }

    mAcceleration = -1.75f + _weird_zero;
    field_0xB48 = 64.f + cM::rndF(64.f);
    field_0xB4E = 90;
}
void dAcEremly_c::executeState_Hold() {
    adjustHeadRotation(false);
    field_0xB4E = 90;

    mRotation.y = mAngle.y = getXZAngleToPlayer();

    if (field_0xB60 == 12 && mMdl.getAnm().isStop()) {
        mMdl.setAnm("RemlyHold", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 11;
    }

    if (0 == sLib::calcTimer(&field_0xB48)) {
        if (field_0xB60 == 11) {
            mMdl.setAnm("RemlyHoldMozo", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 12;
        }
        field_0xB48 = 64.f + cM::rndF(64.f);
    }

    if (mLinkage.checkConnection(dLinkage_c::CONNECTION_1)) {
        return;
    }

    if (mLinkage.checkState(dLinkage_c::STATE_2)) {
        mAngle.y = getXZAngleToPlayer();
        mAngle.y += 0x8000;
        changeState(StateID_Jump);
        return;
    }

    if (isScary()) {
        mAcceleration = -3.f;
        changeState(StateID_NightFoo);
    } else if (field_0xB68 == 0) {
        mAcceleration = -3.f;
        field_0xB48 = 0;
        changeState(StateID_Wait);
    } else {
        mAcceleration = -3.f;
        changeState(StateID_EscapeDash);
    }
}
void dAcEremly_c::finalizeState_Hold() {}

void dAcEremly_c::initializeState_Jump() {
    field_0xB14 = 0;
    unsetActorProperty(AC_PROP_0x1);
    field_0xB50 = 90;
    mMdl.setAnm("RemlyHoldJumpStart", m3d::PLAY_MODE_4, 0.f);
    field_0xB60 = 14;
    field_0xB4E = 90;
}
void dAcEremly_c::executeState_Jump() {
    s32 _weird_zero = 0;

    field_0xB4E = 90;
    if (field_0xB60 == 14 && !fn_177_8980(100.f) && (mAcch.ChkGndHit() || mVelocity.y < 0.f)) {
        mMdl.setAnm("RemlyHoldJumpEnd", m3d::PLAY_MODE_4, 0.f + _weird_zero);
        field_0xB60 = 15;
        return;
    }

    sLib::addCalcScaled(&mSpeed, 0.3f, 0.2f);
    if (mSpeed && mAcch.ChkGndHit()) {
        fn_177_8600();
        mSpeed = 0.f;
    }

    if (mAcch.ChkGndHit() && mMdl.getAnm().isStop()) {
        mAngle.y = mRotation.y;
        setActorProperty(AC_PROP_0x1);
        mAcceleration = -3.f;

        if (isScary()) {
            changeState(StateID_NightFoo);
        } else if (field_0xB68 == 0) {
            changeState(StateID_Wait);
        } else {
            changeState(StateID_EscapeDash);
        }
    }
}
void dAcEremly_c::finalizeState_Jump() {}

void dAcEremly_c::initializeState_Fly() {
    s32 _weird_zero = 0;

    mAcch.ClrGndThinCellingOff();
    field_0xB5C = 0;
    field_0xB5E = 0;
    field_0xB58 = 0;
    unsetActorProperty(AC_PROP_0x1);

    field_0xB6B = 0;
    mMdl.setAnm("RemlyFly", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 16;
    mMdl.setRate(1.3f);
    field_0xB66 = 1;
    field_0xB6A = 0;
    field_0xB14 = 0;
    field_0xB68 = 0;

    if (isScary()) {
        field_0xB64 = 2;
    } else {
        field_0xB64 = 1;
    }

    mAcch.ClrRoofNone();

    f32 f = 200.f + _weird_zero;

    field_0xB3C = 0.f;
    field_0xB65 = 0;

    mAcch.SetField_0xD4(f);

    mVelocity.x = mVelocity.z = 0.f;
    mSpeed = 0.f;
}
void dAcEremly_c::executeState_Fly() {
    s32 _weird_zero = 0;

    if (field_0xB61 == 1 && !fn_177_75E0() && EventManager::isInEvent()) {
        setActorProperty(AC_PROP_0x1);
        mPosition.set(mStartingPos.x, mStartingPos.y, mStartingPos.z);
        mAngle.y = mRotation.y = mStartingRot.y;
        changeState(StateID_Sleep);
        return;
    }

    adjustHeadRotation(false);

    if (field_0xB60 == 17 && mMdl.getAnm().isStop()) {
        mMdl.setAnm("RemlyFly", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 16;
        mMdl.setRate(1.3f);
    }

    if (mAcch.ChkWallHit(nullptr) || mAcch.ChkRoofHit()) {
        mAng a;
        if (mAcch.ChkWallHit(nullptr)) {
            a = mAcchCir.GetWallAngleY() + 0x8000;
        } else {
            a = mAngle.y + 0x8000;
        }

        field_0xB14++;
        mVec3_c v(0.f, 0.f, 0.f);

        v.offsetWithAngle(a, -10.f + _weird_zero);

        field_0xAF8.set(v);
    }

    mAng toStartingPosAngle = cLib::targetAngleY(mPosition, mStartingPos);

    switch (field_0xB65) {
        default: {
            sLib::addCalcScaledDiff(&mVelocity.y, 8.f, 0.7f, 2.f);
            sLib::addCalcScaled(&mAcceleration, 0.7f, 2.f);
            sLib::addCalcAngle(mAngle.y.ref(), getXZAngleToPlayer(), 2, 0x800);
            sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);
            if (mStartingPos.y + 300.f < mPosition.y) {
                mAcceleration = 0.f;
                mVelocity.y = 0.f;
                field_0xB65++;
            }
        } break;
        case 1: {
            if (field_0xB14 < 120 && fn_177_7650(false)) {
                changeState(StateID_BirthWait);
                return;
            }

            f32 targetSpeed;
            if (field_0xB14 < 120 && mStartingPos.squareDistanceToXZ(mPosition) > 2500.f) {
                u32 angle = ++field_0xB5C * 1000;
                targetSpeed = mAng(angle).sin();
                targetSpeed *= _weird_zero + 30.f;
            } else {
                targetSpeed = 0.f;
            }
            sLib::addCalcScaledDiff(&field_0xB3C, targetSpeed, 0.5f, 10.f);

            if (field_0xB14 < 120 && mStartingPos.squareDistanceToXZ(mPosition) > 2500.f) {
                s32 angle = ++field_0xB5E * 300;
                f32 tmp = mAng(angle).cos();
                tmp *= 10000.f + _weird_zero;

                sLib::addCalcAngle(mAngle.y.ref(), toStartingPosAngle + tmp, 2, 0x800);
                sLib::addCalcAngle(mRotation.y.ref(), toStartingPosAngle, 2, 0x800);

                if (mAcch.ChkWallHit(nullptr)) {
                    sLib::addCalcScaledDiff(&mVelocity.y, 8.f, 0.7f, 2.f);
                    if (std::abs(mPosition.y - field_0xAA4.y) < 4.f) {
                        field_0xB14++;
                    }
                } else {
                    mAcceleration = 0.f;
                    mVelocity.y = 0.f;
                    field_0xB14 = 0;
                }
                targetSpeed = 8.f;
            } else {
                sLib::addCalcScaledDiff(&mVelocity.y, -8.f, 0.7f, 2.f);

                targetSpeed = 0.f;

                if (field_0xB14 >= 120 && fn_177_8980(100.f) && !d3d::isOnScreen(mPosition)) {
                    changeState(StateID_BirthWait);
                    return;
                }
                if (mAcch.ChkGndHit()) {
                    field_0xB3C = 0.f;
                    mAcceleration = -3.f;
                    mSpeed = 0.f;
                    field_0xB65++;
                    mAcch.SetRoofNone();

                    setActorProperty(AC_PROP_0x1);
                    if (field_0xB14 >= 120) {
                        field_0xB64 = 0;
                        changeState(StateID_Scared);
                    } else {
                        if (isScary()) {
                            changeState(StateID_NightWait);
                        } else {
                            mMdl.setAnm("RemlyWakeUp", m3d::PLAY_MODE_4, 4.f);
                            mMdl.setFrame(mMdl.getAnm().getEndFrame() - 1.f);
                            mMdl.setRate(-1.f);
                            field_0xB60 = 20;
                        }
                    }
                    return;
                }
            }
            sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 0.7f, 3.f);
        } break;
        case 2: {
            if (mMdl.getAnm().checkFrame(0.f)) {
                setActorProperty(AC_PROP_0x1);
                mAngle.y = mRotation.y;
                changeState(StateID_Sleep);
            }

        } break;
    }
}
void dAcEremly_c::finalizeState_Fly() {}

void dAcEremly_c::initializeState_Damage() {
    field_0xB14 = 0;
    mAcceleration = -3.f;
    if (field_0xB60 == 21) {
        field_0xB14 = 1;
    }

    if (!isScary()) {
        mSpeed = 7.f;
    } else {
        mSpeed = 15.f;
    }

    f32 blend = 10.f;
    if (field_0xB6B != 0) {
        blend = 0.f;
        mSpeed = 0.f;
    }
    mStts.SetRank(5);
    field_0xB6B = 0;
    if (mAcch.ChkGndHit()) {
        field_0xB6B = 1;
    }
    mMdl.setAnm("RemlyDamage", m3d::PLAY_MODE_4, blend);
    field_0xB60 = 18;
    mAngle.y = getXZAngleToPlayer() + 0x8000;
}
void dAcEremly_c::executeState_Damage() {
    if (mAcch.ChkGndHit()) {
        sLib::addCalcScaled(&mSpeed, 0.5f, 1.f);
        if (field_0xB6B == 0) {
            fn_177_8600();
            field_0xB6B = 1;
        }
    }

    if (field_0xB14 != 0) {
        field_0xB4E = 8;
        s32 targetAngle = getXZAngleToPlayer() + 0x8000;
        if (mAng::absDiff2<mAng>(targetAngle, mAngle.y) > 0x1000) {
            sLib::addCalcAngle(mRotation.y.ref(), targetAngle, 1, 0x1000);
        } else {
            mRotation.y = targetAngle;
        }
    }

    if (mSpeed < 0.1f && mMdl.getAnm().isStop()) {
        if (field_0xB14) {
            changeState(StateID_EscapeDash);
            field_0xB6B = 1;
        } else {
            mAngle.y = mRotation.y;
            if (isScary() && field_0xB64 != 0) {
                if (fn_177_9370(100.f)) {
                    changeState(StateID_NightRun);
                } else {
                    changeState(StateID_NightRet);
                }
            } else {
                changeState(StateID_EscapeDash);
            }
        }
    }
}
void dAcEremly_c::finalizeState_Damage() {}

void dAcEremly_c::initializeState_Sleep() {
    mMdl.setAnm("RemlySleep", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 19;
    mTexPat.setFrame(2.f, 0);
    mMdlCallback.mHeadRotation.clear();
}
void dAcEremly_c::executeState_Sleep() {
    if (field_0xB60 == 20) {
        if (mMdl.getAnm().isStop()) {
            changeState(StateID_Wait);
        }
    } else if (fn_177_86C0() || fn_177_8F90() || mSph.ChkCoHit()) {
        if (field_0xB60 != 20) {
            mMdl.setAnm("RemlyWakeUp", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 20;
        }
    }
}
void dAcEremly_c::finalizeState_Sleep() {}

void dAcEremly_c::initializeState_Scared() {
    mMdl.setAnm("RemlyScared", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 21;
    field_0xB66 = 1;
    mSpeed = 0.f;
    field_0xB14 = 0;
    mStts.SetRank(11);
}
void dAcEremly_c::executeState_Scared() {
    if (isScary()) {
        return;
    }

    fn_177_86C0();
    if (mMdl.getAnm().getRate()) {
        adjustHeadRotation(false);
    } else if (field_0xB64) {
        sLib::addCalcAngle(mMdlCallback.mHeadRotation.z.ref(), 8000, 20, 0x100);
        if (field_0xB14 > 80 && !shouldBeScared()) {
            field_0xB68 = 0;
            field_0xB66 = 1;
            changeState(StateID_Wait);
        }
    }

    sLib::addCalcScaled(&mSpeed, 0.7f, 3.f);

    if (!field_0xB64) {
        return;
    }

    if (!shouldBeScared()) {
        if (++field_0xB14 > 30) {
            mMdl.setRate(0.f);
        }
    } else {
        field_0xB14 = 0;
        mMdl.setRate(1.f);
    }
}
void dAcEremly_c::finalizeState_Scared() {}

void dAcEremly_c::initializeState_Stun() {
    field_0xB6B = 0;
    mAcceleration = -3.f;
    if (field_0xB60 != 22 && field_0xB60 != 23) {
        mMdl.setAnm("RemlyPiyo1", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 22;
    } else {
        mMdl.setAnm("RemlyPiyo1", m3d::PLAY_MODE_4, 4.f);
        field_0xB60 = 22;
        mMdl.setFrame(11.f);
        field_0xB6B = 1;
    }

    if (!isScary()) {
        field_0xB64 = 0;
    }

    mStts.SetRank(11);
    fn_177_8520(true);
    field_0xB6A = 0;
    mSph.ClrAtSet();
    field_0xB68 = 1;
}
void dAcEremly_c::executeState_Stun() {
    switch (field_0xB60) {
        default: {
            if (mMdl.getAnm().isStop()) {
                mMdl.setAnm("RemlyPiyo2", m3d::PLAY_MODE_4, 4.f);
                field_0xB60 = 23;
                field_0xB48 = 300;
                fn_177_8520(true);
            }
        } break;
        case 23: {
            if (sLib::calcTimer(&field_0xB48)) {
                field_0xB6B = 1;
            } else {
                mMdl.setAnm("RemlyPiyo3", m3d::PLAY_MODE_4, 4.f);
                field_0xB60 = 24;
            }
        } break;
        case 24: {
            if (mMdl.getAnm().checkFrame(36.f)) {
                mSpeed = 15.f;
            }
            if (mMdl.getAnm().isStop()) {
                mStts.SetRank(5);
                if (isScary()) {
                    if (fn_177_9370(100.f)) {
                        changeState(StateID_NightRun);
                    } else {
                        changeState(StateID_NightRet);
                    }
                } else {
                    changeState(StateID_Escape);
                }
            }
        } break;
    }
    if (field_0xB60 != 23 && field_0xB60 != 24) {}

    sLib::addCalcScaled(&mSpeed, 0.5f, 1.f);
    if (field_0xB6B) {
        mVec3_c pos, scale;
        pos.set(mPosition);
        pos.y += 50.f;

        scale.set(mScale);
        scale *= 0.5f;
        mEmitters[0].holdEffect(PARTICLE_RESOURCE_ID_MAPPING_309_, pos, &mRotation, &scale, nullptr, nullptr);
        holdSound(SE_E_PIYORI_LV);
    }
}
void dAcEremly_c::finalizeState_Stun() {}

void dAcEremly_c::initializeState_Water() {
    mMdl.setAnm("RemlySwim", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 6;
    field_0xB6B = 0;
    mAcceleration = 0.f;
    mSpeed = 0.f;
    mSph.ClrAtSet();
    field_0xB68 = 0;

    if (!isScary()) {
        field_0xB64 = 1;
    } else {
        field_0xB64 = 2;
    }

    mStts.SetRank(11);
    field_0xB6E = 0;
    field_0xB6A = 0;
    startSound(SE_ERemly_DIVE_WATER);
    field_0xB6F = 0;
    field_0xB66 = 1;
}
void dAcEremly_c::executeState_Water() {
    s32 _weird_zero = 0;

    adjustHeadRotation(false);
    if (field_0xB6B) {
        mVec3_c out(mVec3_c::Zero);
        mVec3_c pos;
        pos.set(mPosition);
        pos.y = field_0xB0C;
        if (dTgStream_c::getForce(pos, out)) {
            mAngle.y = out.atan2sX_Z();
            sLib::addCalcScaledDiff(&mSpeed, out.absXZ(), 0.2f, 0.2f);
            field_0xB6F = 1;
        } else if (field_0xB6F) {
            mStts.SetRank(5);
            mAcceleration = -3.f;

            if ((s32)++field_0xB58 > 30) {
                changeState(StateID_Fly);
            }
        } else {
            mSpeed = 0.f;
            mAcceleration = -3.f;
            mStts.SetRank(5);

            if (field_0xB60 != 8 && field_0xB60 != 7) {
                mMdl.setAnm("RemlyWater", m3d::PLAY_MODE_4, 4.f);
                field_0xB60 = 7;

            } else if (field_0xB60 == 8 || mMdl.getAnm().isStop()) {
                if (isScary()) {
                    field_0xB6E = 1;
                    changeState(StateID_Fly);
                } else {
                    changeState(StateID_Walk);
                }
            }
        }
    } else {
        if (field_0xB60 == 8 && mMdl.getAnm().isStop()) {
            mMdl.setAnm("RemlySwim", m3d::PLAY_MODE_4, 10.f);
            field_0xB60 = 6;
        }
        field_0xB5A++;
        f32 yOffset = -35.f + _weird_zero;
        if (field_0xB4E != 0) {
            yOffset -= 10.f + _weird_zero;
        }
        fn_177_7040(0, 0.8f);
        (void)dAcPy_c::GetLink()->mPosition.absXZTo(mPosition);
        if (fn_177_7510(220.f)) {
            mSpeed = 0.f;
        }
        f32 yTarget = field_0xB0C + yOffset + mAng(field_0xB5A * 2000).sin() * 3.f;
        sLib::addCalcScaledDiff(&mPosition.y, yTarget, 0.7f, 3.f);
        sLib::addCalcScaled(&mVelocity.y, 0.8f, 10.f);
    }
}
void dAcEremly_c::finalizeState_Water() {}

void dAcEremly_c::initializeState_Hear() {
    field_0xB14 = 0;
    field_0xB65 = 0;
    switch (field_0xB60) {
        default: {
            mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 1;
            field_0xB66 = 0;
        } // fallthrough
        case 1: {
            field_0xB65 = 1;
        } break;
        case 3:
        case 4:
        case 5: {
            mMdl.setAnm("RemlyWaitStand", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 0;
            field_0xB66 = 1;
            field_0xB48 = 24.f + cM::rndF(24.f);
        }
    }
}
void dAcEremly_c::executeState_Hear() {
    sLib::addCalcScaled(&mSpeed, 0.7f, 5.f);
    switch (field_0xB65) {
        case 0: {
            if (mSpeed < 0.2f && 0 == sLib::calcTimer(&field_0xB48)) {
                mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 10.f);
                field_0xB60 = 1;
                field_0xB66 = 0;
                field_0xB65++;
            }
        } break;
        case 1: {
            if (!fn_177_6B10(false, 0) && fn_177_86C0()) {
                if (field_0xB60 == 2 && mMdl.getAnm().isStop()) {
                    mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 20.f);
                    field_0xB60 = 1;
                } else {
                    if (field_0xB4C > 50) {
                        field_0xB4C = 50;
                        if (field_0xB60 == 1) {
                            mMdl.setAnm("RemlyWaitSitCry", m3d::PLAY_MODE_4, 10.f);
                            field_0xB60 = 2;
                        }
                    }
                    fn_177_6EA0(false);
                }
            } else if (shouldBeScared()) {
                changeState(StateID_Escape);
                field_0xB6A = 1;
            } else {
                if (field_0xB60 != 1) {
                    mMdl.setAnm("RemlyWaitSit", m3d::PLAY_MODE_4, 10.f);
                    field_0xB60 = 1;
                }
                fn_177_8F90();
                adjustHeadRotation(true);
            }
        } break;
    }
}
void dAcEremly_c::finalizeState_Hear() {}

void dAcEremly_c::initializeState_NightSleepDemo() {
    mMdl.setAnm("RemlySleep", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 25;
    mSph.ClrAtSet();
    mSph.ClrTgSet();
    mSph.ClrCoSet();
}
void dAcEremly_c::executeState_NightSleepDemo() {
    nightSleepDemoImpl();
}
void dAcEremly_c::finalizeState_NightSleepDemo() {}

void dAcEremly_c::initializeState_NightFoo() {
    mMdl.setAnm("RemlyFoo", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 28;
    field_0xB48 = 12.f + cM::rndF(12.f);
    field_0xB6D = 0;
    field_0xB6E = 0;
    field_0xB6B = 0;
    mSph.ClrAtSet();
}
void dAcEremly_c::executeState_NightFoo() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    adjustHeadRotation(false);
    sLib::addCalcScaled(&mSpeed, 0.7f, 5.f);
    sLib::addCalcAngle(mAngle.y.ref(), getXZAngleToPlayer(), 2, 0x1000);
    sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x1000);
    if (field_0xB6D == 0) {
        if (0 != sLib::calcTimer(&field_0xB48)) {
            return;
        }
        if (mAng::absDiff2<s32>(getXZAngleToPlayer(), mRotation.y) > 0x400) {
            return;
        }
    } else {
        if (!mNearbyBombRef.isLinked()) {
            changeState(StateID_NightWait);
            return;
        }
        if (pPlayer->getPosition().absXZTo(mPosition) > mNearbyBombRef.get()->mPosition.absXZTo(mPosition)) {
            sLib::addCalcAngle(
                mAngle.y.ref(), cLib::targetAngleY(mPosition, mNearbyBombRef.get()->mPosition), 2, 0x800
            );
            return;
        }
    }
    if (field_0xB6E != 0) {
        changeState(StateID_Fly);
        return;
    }

    if (field_0xB6B != 0 || pPlayer->mPosition.absXZTo(mStartingPos) > field_0xB08 + 500.f) {
        changeState(StateID_NightRet);
        return;
    }

    if (mAng::absDiff2<s32>(getXZAngleToPlayer(), mRotation.y) < 0x1000) {
        changeState(StateID_NightJumpAttack);
        if (!mAcch.ChkWallHit(nullptr) && fn_800301b0(mPositionCopy2, mRotation.y, true, 120.f) == 0) {
            return;
        }
        field_0xB6E = 1;
    }
}
void dAcEremly_c::finalizeState_NightFoo() {}

void dAcEremly_c::initializeState_NightReflectionFoo() {
    mMdl.setAnm("RemlyFoo", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 28;
    mMdl.setRate(0.f);
    mSpeed *= -1.f;
    field_0xB48 = 24.f + cM::rndF(24.f);
    field_0xB6B = 0;
    if (mAcch.ChkGndHit()) {
        field_0xB6B = 1;
    }
    mSph.ClrAtSet();
}
void dAcEremly_c::executeState_NightReflectionFoo() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    adjustHeadRotation(false);
    if (mAcch.ChkGndHit()) {
        sLib::addCalcScaled(&mSpeed, 0.5f, 2.f);
        if (field_0xB6B == 0) {
            fn_177_8600();
            field_0xB6B = 1;
        }
    }

    if (!mMdl.getAnm().getRate()) {
        if (mSpeed < 0.1f) {
            mMdl.setRate(1.f);
        }
        return;
    }

    if (pPlayer->isRecovering() || sLib::calcTimer(&field_0xB48)) {
        return;
    }

    if (pPlayer->mPosition.absXZTo(mStartingPos) > field_0xB08 + 500.f || !fn_177_9370(100.f)) {
        changeState(StateID_NightRet);
    } else {
        changeState(StateID_NightRun);
        field_0xB48 = 24.f + cM::rndF(24.f);
    }
}
void dAcEremly_c::finalizeState_NightReflectionFoo() {}

void dAcEremly_c::initializeState_NightWait() {
    fn_177_8520(true);
    mMdl.setAnm("RemlyWaitStandNight", m3d::PLAY_MODE_4, 4.f);
    field_0xB48 = 24.f + cM::rndF(24.f);
    mSph.ClrAtSet();
}
void dAcEremly_c::executeState_NightWait() {
    if (mNearbyBombRef.isLinked()) {
        changeState(StateID_NightRet);
        return;
    }

    if (fn_177_9370(0.f)) {
        changeState(StateID_NightRun);
        return;
    }

    if (fn_177_86C0()) {
        changeState(StateID_NightFoo);
        field_0xB6D = 1;
        return;
    }

    if (0 == sLib::calcTimer(&field_0xB48)) {
        changeState(StateID_NightWalk);
        return;
    }

    sLib::addCalcScaled(&mSpeed, 0.5f, 5.f);
}
void dAcEremly_c::finalizeState_NightWait() {}

// ???
inline bool compareDistance(const mVec3_c &v0, const mVec3_c &v1, f32 f) {
    return v0.squareDistanceToXZ(v1) > f * f;
}
void dAcEremly_c::initializeState_NightWalk() {
    mMdl.setAnm("RemlyWalkNight", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 29;

    mTargetPosition.set(mStartingPos);

    // odd
    if (!compareDistance(mPosition, mStartingPos, field_0xB08)) {
        mTargetPosition.x += cM::rndFX(field_0xB08 * 0.5f);
        mTargetPosition.z += cM::rndFX(field_0xB08 * 0.5f);
    }
}
void dAcEremly_c::executeState_NightWalk() {
    fn_177_6B10(true, 0);
    if (mNearbyBombRef.isLinked()) {
        changeState(StateID_NightRet);
        return;
    }

    s32 idk = fn_800301b0(mPositionCopy2, mRotation.y, true, 120.f);
    if (idk != 0) {
        mTargetPosition.set(getStartingPosition());
        if (idk == 1) {
            mAng a = cLib::targetAngleY(mPosition, getStartingPosition());
            if (mAng::absDiff2<mAng>(mRotation.y, a) < 0x400) {
                field_0xB6E = 1;
                changeState(StateID_Fly);
                return;
            }
        }
    }

    if (fn_177_7040(2, 0.8f)) {
        mTargetPosition.set(mStartingPos);
        if (idk == 0) {
            mTargetPosition.x += cM::rndFX(field_0xB08 * 0.5f);
            mTargetPosition.z += cM::rndFX(field_0xB08 * 0.5f);
        }
    }

    if (fn_177_9370(0.f)) {
        changeState(StateID_NightRun);
    } else if (fn_177_86C0()) {
        changeState(StateID_NightFoo);
        field_0xB6D = 1;
    }
}
void dAcEremly_c::finalizeState_NightWalk() {}

void dAcEremly_c::initializeState_NightRun() {
    mMdl.setAnm("RemlyRunFast", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 30;
    field_0xB18 = 0;
    field_0xB6B = 0;
    mSph.ClrAtSet();
    mAcceleration = -3.f;
    mMdl.setRate(1.5f);
    field_0xB48 = 0;
    fn_177_77C0();
}
void dAcEremly_c::executeState_NightRun() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    fn_177_6B10(false, 0);

    mMtx_c m;
    mVec3_c in, out;
    mVec3_c stuffs0, stuffs1;

    if (field_0xB6B == 0) {
        // What...
        const mVec3_c &target = dScGame_c::getCamera()->getTarget();
        f32 z = target.z;
        f32 y = target.y;
        f32 x = target.x;
        stuffs0.x = x;
        stuffs0.y = y;
        stuffs1.z = z;

        if (fn_177_7040(2, 10.f)) {
            field_0xB32 += field_0xB34;
        }

        if (fn_177_8C20(mRotation.y)) {
            changeState(StateID_NightFoo);
            return;
        }

        m.YrotS(field_0xB32);
        in.set(0.f, 0.f, 320.f);
        m.multVec(in, out);
        out += pPlayer->mPosition;
        mTargetPosition.set(out);
    }

    if (mNearbyBombRef.isLinked()) {
        changeState(StateID_NightRet);
        return;
    }

    if (!fn_177_9370(100.f)) {
        changeState(StateID_NightFoo);
        field_0xB6B = 1;
        return;
    }

    if (0 == sLib::calcTimer(&field_0xB48) && isWithinPlayerRadius(400.f) && d3d::isOnScreen(mPosition)) {
        field_0xB6B = 1;
    }

    if (field_0xB6B) {
        fn_177_7040(0, 5.0f);
        if (field_0xB48 == 0) {
            if (mAng::absDiff2<mAng>(getXZAngleToPlayer(), mRotation.y) < mAng(4608.f + cM::rndF(4096.f))) {
                changeState(StateID_NightFoo);
            }
        }
    }
}
void dAcEremly_c::finalizeState_NightRun() {}

void dAcEremly_c::initializeState_NightRet() {
    mMdl.setAnm("RemlyRunFast", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 30;
    mTargetPosition.set(mStartingPos);
    field_0xB6B = 0;
    field_0xB18 = 0;
}
void dAcEremly_c::executeState_NightRet() {
    adjustHeadRotation(true);
    if (mNearbyBombRef.isLinked()) {
        mSpeed = 10.f;
        f32 radius = 500.f;
        mAngle.y = cLib::targetAngleY(mNearbyBombRef.get()->mPosition, mPosition);
        sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);

        if (mNearbyBombRef.get()->mPosition.squareDistanceToXZ(mPosition) > radius * radius) {
            changeState(StateID_NightFoo);
            field_0xB6D = 1;
        }
        return;
    }

    if (fn_177_8C20(mRotation.y)) {
        changeState(StateID_NightFoo);
        field_0xB6E = 1;
        return;
    }

    if (fn_177_7040(2, 10.f)) {
        changeState(StateID_NightWait);
    }
}
void dAcEremly_c::finalizeState_NightRet() {}

void dAcEremly_c::initializeState_NightJumpAttack() {
    mSpeed = 20.f;
    mVelocity.y = 27.f + cM::rndF(3.f);
    mSph.OnAtSet();
    mMdl.setAnm("RemlyJump", m3d::PLAY_MODE_4, 0.f);
    field_0xB60 = 31;
    field_0xB6E = 0;
    field_0xB32 = 0;
    field_0xB6B = 0;
}
void dAcEremly_c::executeState_NightJumpAttack() {
    adjustHeadRotation(false);
    if (mSph.ChkAtHit()) {
        if (mSph.GetAtFlag0x8()) {
            setBattleBgmRelated(0);
            mSpeed = -20.f;
            changeState(StateID_Stun);
            return;
        }

        if (mSph.GetAtFlag0x2()) {
            changeState(StateID_NightReflectionFoo);
            return;
        }
    }

    if (mAcch.ChkGndHit()) {
        fn_177_8600();
        if (field_0xB6E != 0 &&
            (mAcch.ChkWallHit(nullptr) || fn_800301b0(mPositionCopy2, mRotation.y, true, 120.f) != 0)) {
            changeState(StateID_NightFoo);
            field_0xB6B = 1;
            return;
        }
        changeState(StateID_NightRun);
        field_0xB48 = 24.f + cM::rndF(24.f);
    }
}
void dAcEremly_c::finalizeState_NightJumpAttack() {}

void dAcEremly_c::initializeState_BirthWait() {
    field_0xB48 = 300;
    unsetActorProperty(AC_PROP_0x1);
    field_0xB64 = 2;
    field_0xB68 = 0;
    mMdl.setAnm("RemlyWaitStand", m3d::PLAY_MODE_4, 4.f);
    field_0xB60 = 0;
}
void dAcEremly_c::executeState_BirthWait() {
    if (!sLib::calcTimer(&field_0xB48) && fn_177_7650(true)) {
        mPosition.set(mStartingPos.x, mStartingPos.y, mStartingPos.z);
        mOldPosition.set(mStartingPos.x, mStartingPos.y, mStartingPos.z);
        mPosition.y += 10.f;
        mVelocity.set(0.f, 0.f, 0.f);
        mSpeed = 0.f;
        mAcceleration = -3.f;
        setActorProperty(AC_PROP_0x1);

        if (isScary()) {
            changeState(StateID_NightWait);
        } else if (field_0xB61 == 1 && fn_177_75E0()) {
            changeState(StateID_Wait);
        } else {
            changeState(StateID_Sleep);
        }
    }
}
void dAcEremly_c::finalizeState_BirthWait() {}

void dAcEremly_c::playWink() {
    if (isState(StateID_Sleep)) {
        return;
    }

    if (sLib::calcTimer(&mWinkTimer)) {
        mTexPat.play();
    } else {
        mWinkTimer = 40.f + cM::rndF(120.f);
        mTexPat.setFrame(0.f, 0);
    }
}

// NONMATCHING
bool dAcEremly_c::fn_177_6B10(bool lookAtTarget, const mAng &range) {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    mAng angleTargetX, angleTargetY;
    if (lookAtTarget) {
        angleTargetX = cLib::targetAngleX(mPosition, mTargetPosition) * 0.7f;

        angleTargetY = (mAng)cLib::targetAngleY(mPosition, mTargetPosition);
        angleTargetY -= mRotation.y;
    } else {
        angleTargetX = cLib::targetAngleX(mPosition, pPlayer->getHeadTranslation()) * 0.7f;

        angleTargetY = getXZAngleToPlayer();
        angleTargetY -= mRotation.y;
    }

    mVec3_c outpos;
    if (!dLightEnv_c::GetPInstance()->check_BPM8(&mPosition, &outpos)) {
        // TODO - Return type pPlayer->vt_0x1C0()
        dAcBase_c *pActor = static_cast<dAcBase_c *>(pPlayer->vt_0x1C0());
        if (pActor) {
            field_0xA50.set(pActor->mPosition);
            field_0xB56 = 30;
        }
    }

    if (sLib::calcTimer(&field_0xB56)) {
        angleTargetX = cLib::targetAngleX(mPosition, field_0xA50) * 0.7f;
        angleTargetY = (mAng)cLib::targetAngleY(mPosition, field_0xA50);
        angleTargetY -= mRotation.y;
    }

    if (fn_177_75E0()) {
        outpos.set(mRef1.get()->mPosition);
        outpos.y += 100.f;

        angleTargetX = cLib::targetAngleX(mPosition, outpos) * 0.7f;
        angleTargetY = (mAng)cLib::targetAngleY(mPosition, outpos);
        angleTargetY -= mRotation.y;
        fn_177_79D0(true);
    }

    if (angleTargetX < 0) {
        angleTargetY = 0;
    } else if (angleTargetX > 2500) {
        angleTargetX = 2500;
    }
    sLib::addCalcAngle(mMdlCallback.mHeadRotation.z.ref(), angleTargetX, 2, 0x800);
    if (field_0xB60 == 2) {
        return false;
    }

    bool b = false;
    s32 t0 = range + 8000;
    if (isState(StateID_Wait) || isState(StateID_Walk)) {
        t0 = range + 6000;
    }
    s16 t = t0;

    if (angleTargetY < -t) {
        angleTargetY = -t;
        b = true;
    } else if (angleTargetY > t) {
        angleTargetY = t;
        b = true;
    }
    sLib::addCalcAngle(mMdlCallback.mHeadRotation.y.ref(), angleTargetY, 2, 0x800);
    return b;
}

void dAcEremly_c::fn_177_6EA0(bool b) {
    if (!b && shouldBeScared()) {
        return;
    }

    if (!sLib::calcTimer(&field_0xB4C)) {
        field_0xB4C = 100.f + cM::rndF(100.f);
        if (cM::rnd() < 0.3f) {
            field_0xB30 = 0;
        } else if (field_0xB30 == 0) {
            if (cM::rnd() < 0.5f) {
                field_0xB30 = 5000;
            } else {
                field_0xB30 = -5000;
            }
        } else {
            field_0xB30 *= -1;
            field_0xB4C = 50.f + cM::rndF(100.f);
        }
    }
    sLib::addCalcAngle(mMdlCallback.mHeadRotation.x.ref(), field_0xB30, 10, 0x200);
}

void dAcEremly_c::adjustHeadRotation(bool bOnlyRotX) {
    sLib::addCalcAngle(mMdlCallback.mHeadRotation.x.ref(), 0, 2, 0x400);
    if (!bOnlyRotX) {
        sLib::addCalcAngle(mMdlCallback.mHeadRotation.y.ref(), 0, 2, 0x400);
        sLib::addCalcAngle(mMdlCallback.mHeadRotation.z.ref(), 0, 2, 0x400);
    }
}

// I do not like how this is written and somehow it matches
bool dAcEremly_c::fn_177_7040(u32 param0, f32 f) {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    f32 f0;
    s16 targetAngle;
    s16 targetRatio;
    s16 targetStepSize;

    targetAngle = getXZAngleToPlayer();
    if (param0 != 2) {
        mTargetPosition.set(pPlayer->mPosition);
    }

    if (param0 == 1) {
        targetAngle += 0x8000;
    } else if (param0 == 2) {
        targetAngle = cLib::targetAngleY(mPosition, mTargetPosition);
    }

    f0 = mAng(mAng::abs(mAngle.y - targetAngle)).sin();
    targetRatio = 4.f - std::abs(f0 * 2.f);

    f0 = mAng(mAng::abs(mAngle.y - targetAngle)).sin();
    targetStepSize = 2048.f + std::abs(f0 * 512.f);

    if (field_0xB60 == 29 || field_0xB60 == 4) {
        f32 f = mAng(mAng::abs(mAngle.y - targetAngle)).sin();
        mMdl.setRate(f * 4.f + 1.f);
        targetRatio = 10;
        targetStepSize = 0x400;
    }

    if (field_0xB60 == 4) {
        f32 f = mAng(mAng::abs(mAngle.y - targetAngle)).sin();
        mMdl.setRate(f + 1.f);
        targetRatio = 5;
        targetStepSize = 0x400;
    }

    if (field_0xB60 == 9 || mNearbyBombRef.isLinked()) {
        targetRatio = 1;
        targetStepSize = 0x2000;
    }

    sLib::addCalcAngle(mAngle.y.ref(), targetAngle, targetRatio, targetStepSize);
    f0 = 1.5f;
    f0 -= std::abs(mAng(mAng::abs(mAngle.y - targetAngle)).sin());
    sLib::addCalcScaledDiff(&mSpeed, f * f0, 0.7f, 3.f);
    if (mTargetPosition.squareDistanceToXZ(mPosition) < 10000.f) {
        return true;
    } else {
        return false;
    }
}

bool dAcEremly_c::isScary() {
    if (StoryflagManager::sInstance->getFlag(STORYFLAG_GRATITUDE_QUEST_FINISHED) &&
        (dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("D000"))) {
        return false;
    }

    if (dScGame_c::currentSpawnInfo.isNight()) {
        return true;
    }

    return false;
}

bool dAcEremly_c::shouldBeScared() {
    if (isState(StateID_Sleep) || isScary() || field_0xB68) {
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

bool dAcEremly_c::fn_177_7510(f32 radius) {
    if (fn_177_75E0()) {
        mTargetPosition.set(mRef1.get()->getPosition());
        if (mTargetPosition.squareDistanceToXZ(mPosition) < radius * radius) {
            return true;
        }
    }

    if (isWithinPlayerRadius(radius)) {
        return true;
    }

    return false;
}

bool dAcEremly_c::fn_177_75E0() {
    if (mRef1.isLinked() && mSleepDemoPlayedSceneflag != 0xFF) {
        if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mSleepDemoPlayedSceneflag)) {
            return true;
        }
    }
    return false;
}

bool dAcEremly_c::fn_177_7650(bool comparePlayer) const {
    f32 radius = 0.f;
    mVec3_c pos = mPosition;

    if (comparePlayer) {
        pos.set(mStartingPos);
    }

    if (d3d::isOnScreen(pos)) {
        return false;
    }

    radius = 40.f;
    radius *= 100.f;
    if (comparePlayer) {
        const dAcPy_c *pPlayer = dAcPy_c::GetLink();
        return pPlayer->mPosition.squareDistanceToXZ(pos) > radius * radius;
    } else {
        mVec3_c camPos = dScGame_c::getCamera()->getPosition();
        return camPos.squareDistanceToXZ(pos) > radius * radius;
    }
}

// NONMATCHING
void dAcEremly_c::fn_177_77C0() {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();
    mAng a0 = getXZAngleToPlayer() + -0x7800;
    mAng a1 = getXZAngleToPlayer() + 0x7800;

    field_0xB32 = getXZAngleToPlayer() + 0x8000;
    field_0xB32 += cM::rndFX(12288.f);

    if (mAng::abs(a0 - pPlayer->mRotation.y) < mAng::abs(a1 - pPlayer->mRotation.y)) {
        field_0xB34 = mAng(0x800);
    } else {
        field_0xB34 = mAng(0xf800);
    }

    field_0xB32 += field_0xB34;
}

void dAcEremly_c::fn_177_78D0() {
    mWinkTimer = 0;
    nw4r::g3d::ResFile fil = mMdl.getMdlFile();
    nw4r::g3d::ResMdl mdl = fil.GetResMdl("Remly");
    mMdl.getModel().setCullMode(m3d::getMatID(mdl, "mouth"), GX_CULL_BACK, false);
    mMdl.getModel().setCullMode(m3d::getMatID(mdl, "eye_scary"), GX_CULL_BACK, false);
    mMdl.getModel().setCullMode(m3d::getMatID(mdl, "night_set"), GX_CULL_BACK, false);

    mTexPat.setAnm(mMdl.getModel(), fil.GetResAnmTexPat("RemlyScaryFace"), 0, m3d::PLAY_MODE_4);
}

// NONMATCHING
void dAcEremly_c::fn_177_79D0(bool b) {
    mMtx_c m;
    mVec3_c in, out;
    if (mRef1.isLinked()) {
        m.YrotS(mRef1.get()->mRotationCopy.y + 0x5000);
        in.set(0.f, 0.f, 120.f);
        m.multVec(in, out);
        out += mRef1.get()->getPosition();

        if (!b) {
            setPosition(out);
            setOldPosition(getStartingPosition());
            field_0xB38 = out.y;
            setStartingRotation(mRotation);
        }
        setStartingPosition(out);
        mTargetPosition.set(out);
    }
}

bool dAcEremly_c::fn_177_7B10() {
    s32 _weird_zero = 0;

    if (dBgS_WtrChk::CheckPos(&mPosition, true, 600.f + _weird_zero, -100.f + _weird_zero)) {
        field_0xB0C = dBgS_WtrChk::GetWaterHeight();

        if (mPosition.y < dBgS_WtrChk::GetWaterHeight() && mAcch.GetGroundH() < dBgS_WtrChk::GetWaterHeight()) {
            if (mAcch.GetGroundH() < -8.f + field_0xB0C) {
                if (isState(StateID_Hold)) {
                    mLinkage.fn_80050EA0(this);
                }
                if (isState(StateID_Jump)) {
                    mAngle.y = mRotation.y;
                }
                if (!isState(StateID_Water)) {
                    changeState(StateID_Water);
                }
            }
            return true;
        }
    }
    return false;
}

void dAcEremly_c::nightSleepDemoImpl() {}

void dAcEremly_c::fn_177_8520(bool b) {
    s32 _weird_zero = 0;
    if (!b && !mSpeed) {
        return;
    }

    if ((++field_0xB28 & 3) == 0) {
        fn_80030030(mPosition, field_0xB24, field_0xB26, 4.f + _weird_zero);
        if ((u16)(field_0xB24 + 0x1800) > 0x3000U) {
            field_0xB24 = 0;
        } else if ((u16)(field_0xB26 + 0x1800) > 0x3000U) {
            field_0xB26 = 0;
        }
    }
}

void dAcEremly_c::fn_177_8600() {
    mMtx_c m;
    mVec3_c in, out;
    m.YrotS(mRotation.y);
    in.set(0.f, 0.f, 15.f);
    m.multVec(in, out);
    out += mPosition;

    dJEffManager_c::spawnGroundEffect(out, mPolyAttr0, mPolyAttr1, field_0x1B4, 1, mScale.x * 0.7f, field_0x1B0);
}

bool dAcEremly_c::fn_177_86C0() {
    mVec3_c outpos;
    if (dLightEnv_c::GetPInstance()->check_BPM8(&mPosition, &outpos)) {
        field_0xA50.set(outpos);

        if (!isScary()) {
            if (isState(StateID_Hear) || isState(StateID_Sleep)) {
                return true;
            }

            if (!dAcPy_c::GetLinkM()->isUsingHarp()) {
                if (!isState(StateID_Scared) && field_0xB60 != 32) {
                    mMdl.setAnm("RemlySound", m3d::PLAY_MODE_4, 4.f);
                    field_0xB60 = 32;
                    mSpeed = 0.f;
                }
            } else if (isState(StateID_Scared) ||
                       (isState(StateID_Wait) && mAng::absDiff2(getXZAngleToPlayer(), mRotation.y) < 0x400)) {
                if (field_0xB60 == 21) {
                    if (++field_0xB2C > 40) {
                        field_0xB64 = 1;
                        field_0xB14++;
                    }
                    return true;
                }
                changeState(StateID_Hear);
                return true;
            }
            return false;
        }
    }

    if (field_0xB2C > 0) {
        field_0xB2C--;
    }
    return false;
}

// TODO - Have its own location
inline void offsetWithAngle(mVec3_c &v, const mAng &a, f32 f) {
    v.x += f * a.sin();
    v.z += f * a.cos();
}

bool dAcEremly_c::fn_177_8980(f32 f) {
    dBgS_ObjGndChk gndChk;
    mVec3_c v(mPosition);

    v.y += 100.f;

    offsetWithAngle(v, mRotation.y, f);

    gndChk.SetPos(v);

    f32 cross = dBgS::GetInstance()->GroundCross(&gndChk);
    if (cross < mPosition.y - 1000.f || cross == -1e9f || dBgS::GetInstance()->GetGroundCode(gndChk) == 4) {
        return true;
    }
    return false;
}

// NONMATCHING
bool dAcEremly_c::fn_177_8AC0() {
    if ((field_0xB60 != STATE_16) && (field_0xB60 != STATE_17) && (field_0xB60 != STATE_13) &&
        (field_0xB60 != STATE_14) && (field_0xB60 != STATE_15) && (field_0xB60 != STATE_5) &&
        (field_0xB60 != STATE_21) && (field_0xB60 != STATE_6) && (field_0xB60 != STATE_8) && (field_0xB60 != STATE_7) &&
        (field_0xB60 != STATE_10) && (field_0xB60 != STATE_22) && (field_0xB60 != STATE_23) &&
        (field_0xB60 != STATE_24)) {
        return false;
    } else {
        return true;
    }
}

bool dAcEremly_c::fn_177_8C20(mAng a) {
    s32 _weird_zero = 0;
    if (fn_800301b0(mPositionCopy2, a, true, mSpeed + (120.f + _weird_zero)) == 2) {
        if (std::abs(mPosition.y - field_0xAA4.y) < 4.f) {
            field_0xB14++;
        }
        mStts.SetRank(11);
        return true;
    }

    if (field_0xB6C || field_0xB6A || isState(StateID_Escape) || isState(StateID_EscapeDash)) {
        mVec3_c v(mPosition);
        f32 f = 150.f + _weird_zero;
        offsetWithAngle(v, mAngle.y, f);
        if (dBgS_WtrChk::CheckPos(&v, true, 600.f + _weird_zero, -100.f + _weird_zero)) {
            if (mPosition.y < dBgS_WtrChk::GetWaterHeight() && mAcch.GetGroundH() < dBgS_WtrChk::GetWaterHeight()) {
                return true;
            }
        }
    }

    if (fn_800301b0(mPositionCopy2, mRotation.y, true, 120.f) == 1 ||
        (mAcch.ChkWallHit(nullptr) && (mAng::abs(mAcchCir.GetWallAngleY() - mAng(mRotation.y + 0x8000)) < 0x800 ||
                                       (mSpeed && std::abs(mPosition.y - field_0xAA4.y) < 4.f)))) {
        if (mAng::absDiff2<mAng>(mRotation.y, cLib::targetAngleY(mPosition, mTargetPosition) < 0x200)) {
            if ((++field_0xB18 & 7) == 0) {
                field_0xB18 = 1;
                return true;
            }
        }
    } else {
        field_0xB18 = 0;
    }
    return false;
}

bool dAcEremly_c::fn_177_8F90() {
    if (field_0xB6A != 0) {
        return false;
    }
    if (sLib::calcTimer(&field_0xB50)) {
        return false;
    }
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    if (isState(StateID_Sleep)) {
        if (fn_177_7510(400.f) && std::abs(mPosition.y - pPlayer->mPosition.y) < 100.f) {
            return true;
        }
    } else if (isState(StateID_Walk)) {
        if (!fn_177_7510(300.f)) {
            changeState(StateID_Run);
            return true;
        }

        if (!fn_177_6B10(false, -1000) && field_0xB69 == 0 && fn_177_7510(120.f)) {
            sLib::addCalcScaled(&mSpeed, 0.7f, 5.f);
            if (++field_0xB14 < 20) {
                return false;
            }
            changeState(StateID_Wait);
            return true;
        }
    } else if (isState(StateID_Run)) {
        if (fn_177_7510(200.f)) {
            field_0xB67 = 1;
            changeState(StateID_Walk);
            return true;
        }

        if (fn_177_8C20(mRotation.y)) {
            changeState(StateID_Wait);
            mMdl.setAnm("RemlyWaitSitCry", m3d::PLAY_MODE_4, 4.f);
            field_0xB60 = 2;
            return true;
        }
    } else if (field_0xB6E == 0 && field_0xB6C == 0) {
        if (fn_177_8C20(getXZAngleToPlayer())) {
            return false;
        }

        if (field_0xB18 == 0) {
            (void)pPlayer->mPosition.absXZTo(mPosition);
            if (!fn_177_7510(220.f)) {
                field_0xB67 = 1;
                changeState(StateID_Walk);
                return true;
            }
        }

        if (fn_177_6B10(false, 0)) {
            field_0xB67 = 1;
            changeState(StateID_Walk);
            return true;
        }
    }
    return false;
}

bool dAcEremly_c::fn_177_9370(f32 f) {
    const dAcPy_c *pPlayer = dAcPy_c::GetLink();

    if (!pPlayer->isRecovering() && pPlayer->mPosition.absXZTo(mStartingPos) < field_0xB08 &&
        std::abs(mPosition.y - pPlayer->mPosition.y) < f + 2000.f) {
        return true;
    }

    return false;
}
