#include "d/a/b/d_a_b_lastboss.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_base.h"
#include "d/d_camera.h"
#include "d/d_cc.h"
#include "d/d_light_env.h"
#include "d/d_player_act.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/d_stage_mgr.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
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
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/math/math_arithmetic.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/event.h"
#include "toBeSorted/event_manager.h"
#include "toBeSorted/minigame_mgr.h"

SPECIAL_ACTOR_PROFILE(B_LASTBOSS, dAcBlastboss_c, fProfile::B_LASTBOSS, 0xBA, 0, 0);

STATE_DEFINE(dAcBlastboss_c, Fight);
STATE_DEFINE(dAcBlastboss_c, Guard);
STATE_DEFINE(dAcBlastboss_c, GuardBreak);
STATE_DEFINE(dAcBlastboss_c, Attack);
STATE_DEFINE(dAcBlastboss_c, CounterAttack);
STATE_DEFINE(dAcBlastboss_c, PunchAttack);
STATE_DEFINE(dAcBlastboss_c, DashAttack);
STATE_DEFINE(dAcBlastboss_c, SmallAttack);
STATE_DEFINE(dAcBlastboss_c, ThunderAttack);
STATE_DEFINE(dAcBlastboss_c, Damage);
STATE_DEFINE(dAcBlastboss_c, SitDamage);
STATE_DEFINE(dAcBlastboss_c, Down);
STATE_DEFINE(dAcBlastboss_c, Stun);
STATE_DEFINE(dAcBlastboss_c, ThunderWait);

static dCcD_SrcCyl sSrcCyl1 = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {~(AT_TYPE_BUGNET | AT_TYPE_0x80000 | AT_TYPE_BELLOWS | AT_TYPE_BEETLE | AT_TYPE_WIND | AT_TYPE_0x8000),
      0x4103,
      {0, 0x4, 0x407},
      0x0,
      0x0},
     /* mObjCo */ {0x0}},
    /* mCylInf */
    {40.f, 300.f}
};

static dCcD_SrcCyl sSrcCyl2 = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_BELLOWS, 0x103, {0, 0x4, 0x407}, 0x0, 0x0},
     /* mObjCo */ {0x0}},
    /* mCylInf */
    {70.f, 30.f}
};

static dCcD_SrcCyl sSrcCyl3 = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {0x0, 0x0, {0, 0x0, 0x407}, 0x0, 0x0},
     /* mObjCo */ {0xE5}},
    /* mCylInf */
    {100.f, 250.f}
};

static dCcD_SrcSph sSrcSph1 = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_DAMAGE, 0x2008D, {0, 0, 0}, 4, 0, 0, 0, 0, 0},
     /* mObjTg */
     {0, 0, {0, 0x0, 0x407}, 0, 0},
     /* mObjCo */ {0}},
    /* mSphInf */
    {100.0f}
};

static dCcD_SrcSph sSrcSph2 = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_DAMAGE, 0x22008D, {0, 0, 4}, 8, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_0x800000 | AT_TYPE_BUGNET, 0x103, {0, 0x19, 0x407}, 0, 0},
     /* mObjCo */ {0}},
    /* mSphInf */
    {100.0f}
};

static dCcD_SrcSph sSrcSph = {
    /* mObjInf */
    {/* mObjAt */ {0, 0, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */
     {AT_TYPE_BELLOWS, 0x4103, {0, 0x0, 0x40F}, 0, 0},
     /* mObjCo */ {0}},
    /* mSphInf */
    {30.0f}
};

s32 useUnused() {
    static const f32 a1[] = {
        0.95f,
        1.05f,
        0.0f,
    };
    return (s32)a1;
}

bool dAcBlastboss_c::createHeap() {
    bool result;
    void *pData = getOarcResFile("BLastBoss");

    TRY_CREATE2(mMdl.create3(*this, pData, "LastBoss", "Wait", 0x334));
    mScnCallback.attach(mMdl.getModel());

    nw4r::g3d::ResFile res(pData);

    TRY_CREATE2(mAnmTexSrt.create(mMdl.getModel().getResMdl(), res.GetResAnmTexSrt("Hair"), &mAllocator, nullptr, 1));
    mMdl.getModel().setAnm(mAnmTexSrt);

    TRY_CREATE2(mAnmMatClr1.create(mMdl.getModel().getResMdl(), res.GetResAnmClr("Wait"), &mAllocator, nullptr, 2));
    mAnmMatClr1.setAnm(mMdl.getModel(), res.GetResAnmClr("Hair"), 1, m3d::PLAY_MODE_0);
    mMdl.getModel().setAnm(mAnmMatClr1);

    mMdl.getModel().setCallback(&mMdlCallback);
    mMdlCallback.mpOwner = this;

    TRY_CREATE2(mSwordMdl.create(
        &mAllocator, pData, "LastBossSword", 0x130, mVec3_c(0.0f, 0.0f, 0.0f), mVec3_c(0.0f, -230.0f, 0.0f), mStts,
        nullptr, 1, nullptr
    ));
    // TODO I'm not going to try and make sense of this for now
    mSwordMdl.fn_8006B660(4, 1, 0, 0x10000, 0, 0, 0, 0x400, 0xc, 60.0f);
    mSwordMdl.mProc.setColor1(mColor(0xFF, 0xFF, 0xFF, 0x14));
    mSwordMdl.mProc.setColor2(mColor(0xFF, 0xFF, 0xFF, 0x14));

    TRY_CREATE2(mThunderMdl.create(res.GetResMdl("FX_LBThunder"), &mAllocator, 0x120));

    TRY_CREATE2(
        mThunderAnmMatClr.create(mThunderMdl.getResMdl(), res.GetResAnmClr("FX_LBThunder"), &mAllocator, nullptr, 1)
    );
    mThunderMdl.setAnm(mThunderAnmMatClr);

    dBase_c::createBase(fProfile::LYT_BOSS_CAPTION, this, 0, OTHER);

    return result;
}

int dAcBlastboss_c::create() {
    field_0x1130 = getFromParams(0, 0xFF);
    if (MinigameManager::GetInstance()->checkInBossRush()) {
        field_0x114D = 1;
    }
    CREATE_ALLOCATOR(dAcBlastboss_c);

    mStts.SetRank(10);
    mCcList.addCc(mCc1, sSrcCyl1);
    mCcList.addCc(mCc2, sSrcCyl2);
    mCcList.addCc(mCc3, sSrcCyl3);

    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x1268); i++) {
        field_0x1268[i].field_0x000 = i;
        field_0x1268[i].field_0x002 = cM::rndF(65536.0f);
        mCcList.addCc(field_0x1268[i].mCc, sSrcSph);
        for (int j = 0; j < (int)ARRAY_LENGTH(field_0x1268[j].field_0x014); j++) {
            field_0x1268[i].field_0x014[j] = mPosition;
        }
        field_0x1268[i].field_0x008 = i * 15000;
    }

    // TODO loop of length 1 might not exist
    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x2948); i++) {
        mCcList.addCc(field_0x2948[i].mCc, sSrcSph);
        for (int j = 0; j < (int)ARRAY_LENGTH(field_0x2948[j].field_0x008); j++) {
            field_0x2948[i].field_0x008[j] = mPosition;
        }
    }

    mCcList.SetStts(mStts);

    mCc4.Set(sSrcSph1);
    mCc4.SetStts(mStts);

    mCc5.Set(sSrcSph2);
    mCc5.mTg.Set_0x4C(-1);
    mCc5.SetStts(mStts);

    mAcceleration = -5.0f;
    mMaxSpeed = -80.0f;

    mBoundingBox.Set(mVec3_c(-200.0f, -300.0f, -200.0f), mVec3_c(200.0f, 300.0f, 200.0f));
    mStateMgr.changeState(StateID_Fight);

    field_0x1132 = 1;
    mCounter = cM::rndF(65536.0f);

    mHealth = 400;
    mPositionCopy3 = mPosition;

    mLastAttackDir = mSecondLastAttackDirection = -1;

    mWaterEmitter.init(this);
    mEmitter2.init(this);
    // mEmitter3.init(this); // TODO why not this one...
    mEmitter4.init(this);
    mEmitter5.init(this);
    mEmitter6.init(this);
    mEmitter7.init(this);

    mLightInfo.SetPosition(mPosition);
    mLightInfo.mClr.Set(100, 150, 255, 255);
    dLightEnv_c::GetPInstance()->plight_set(&mLightInfo);
    setLightningTimerMaybe(100);

    if (!field_0x114D) {
        mDemoState = DEMO_STATE_200;
        setActorProperty(AC_PROP_0x4);
    }

    return SUCCEEDED;
}

// why are these up here?
static const s16 moreAngs[] = {0xA000, 0xC000, 0xE000, 0xF000, 0x0000, 0x1000, 0x2000, 0x4000};

int dAcBlastboss_c::doDelete() {
    dLightEnv_c::GetPInstance()->plight_cut(&mLightInfo);
    return SUCCEEDED;
}

int dAcBlastboss_c::actorExecute() {
    if (field_0x114D && mpSwordBattleGame == nullptr) {
        mpSwordBattleGame = static_cast<dTgSwordBattleGame_c *>(
            fManager_c::searchBaseByProfName(fProfile::TAG_SWORD_BATTLE_GAME, mpSwordBattleGame)
        );
    }
    dAcPy_c *link = dAcPy_c::GetLinkM();

    mCounter++;

    field_0x1133 = 0;
    field_0x1131 = 0;
    field_0x1141 = 0;
    field_0x114E = 0;

    field_0x11A4 = 0.0f;
    mLightInfo.mScale = 0.0f;
    field_0x11B0 = 0.7f;

    for (int i = 0; i < TIMER_MAX; i++) {
        if (mTimers[i] != 0) {
            mTimers[i]--;
        }
    }

    if (mMdlCallback.field_0x04.mVal != 0) {
        mMdlCallback.field_0x04.mVal--;
    }

    if (field_0x117C != 0) {
        field_0x117C--;
    }

    if (field_0x113D != 0) {
        field_0x113D--;
    }

    if (field_0x113F != 0) {
        field_0x113F--;
    }

    if (field_0x113E != 0) {
        field_0x113E--;
    }

    mVec3_c vEff;
    mVec3_c linkPos = link->getPosition();
    mYAngleToLink = cLib::targetAngleY(mPosition, linkPos);
    mXZDistanceToLink = link->getPosition().absXZTo(mPosition);

    mYRotationRelativeToLink = mYAngleToLink - mRotation.y;
    mSwordMdl.disable();

    mCc4.ClrAtSet();
    mCc5.ClrAtSet();

    checkDamage();
    if (mpSwordBattleGame == nullptr || mpSwordBattleGame->checkFightStarted() == true) {
        mStateMgr.executeState();
    }

    mRotation.x = mAngle.x;
    mRotation.z = mAngle.z;
    sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);

    mMdl.play();
    if (!field_0x114F) {
        mAnmMatClr1.play();
    }

    calcVelocity();
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();
    mPosition.x += field_0x119C;
    mPosition.z += field_0x11A0;

    field_0x119C *= 0.6f;
    field_0x11A0 *= 0.6f;

    f32 f5 = 0.0f;
    if (mRotation.x != 0) {
        f5 += field_0x11C0 * mRotation.x.sin();
    }

    field_0x1140 = 0;
    if (mPosition.y <= f5) {
        mPosition.y = f5;
        field_0x11BC = mVelocity.y;
        mVelocity.y = 0.0f;
        field_0x1140 = 1;
    }

    updateMainNodeTransforms();

    mWorldMtx.transS(mPosition);
    mWorldMtx.ZXYrotM(mRotation);
    mScale.set(0.95f, 0.95f, 0.95f);
    mMdl.getModel().setScale(mScale);
    mMdl.getModel().setLocalMtx(mWorldMtx);
    if (!field_0x1141) {
        mAnmTexSrt.play();
    }

    mMdl.getModel().calc(false);

    nw4r::g3d::ResMdl resMdl = mMdl.getModel().getResMdl();
    // TODO: These very deliberately placed matrices might work better with
    // inlines but I haven't found an inline that places rotMtx where it needs
    // to be.
    mMtx_c nodeMtx;
    mMtx_c rotMtx;
    mMtx_c scaleMtx;
    mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("loc_sword01").GetID(), nodeMtx);
    scaleMtx.scaleS(1.05f, 1.05f, 1.05f);
    MTXConcat(nodeMtx, scaleMtx, nodeMtx);

    mSwordMdl.calc(nodeMtx, mRotation.y, false);

    vEff.set(0.0f, -120.0f, 0.0f);
    if (mGuardDirection == GUARD_BT) {
        vEff.y = -80.0f;
        if (mXZDistanceToLink > 300.0f) {
            vEff.y -= (mXZDistanceToLink - 300.0f);
            if (vEff.y < -280.0f) {
                vEff.y = -280.0f;
            }
        }
    }
    MTXMultVec(nodeMtx, vEff, field_0x1208);

    if (mIsSwordEmpowered) {
        mEmitter4.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_619_, nodeMtx, nullptr, nullptr);
        mLightInfo.mPos = field_0x1208;
        if (dScGame_c::getUpdateFrameCount() & 2) {
            mLightInfo.SetScale(300.0f);
        } else {
            mLightInfo.SetScale(200.0f);
        }
        holdSound(SE_BLasBos_SwordEnpowered);
    }

    if (field_0x114B != 0) {
        if (field_0x114B == 1) {
            mEmitter7.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_852_, nodeMtx, nullptr, nullptr);
            mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone2").GetID(), nodeMtx);
            mEmitter6.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_853_, nodeMtx, nullptr, nullptr);
            holdSound(SE_BLasBos_UnderElecShock);
        } else if (field_0x114B == 2) {
            mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("backbone2").GetID(), nodeMtx);
            dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_851_, nodeMtx, nullptr, nullptr, 0, 0);
            mMtx_c transMtx;
            transMtx.transS(0.0f, -100000.0f, 0.0f);
            MTXConcat(nodeMtx, transMtx, nodeMtx);
            mEmitter7.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_852_, nodeMtx, nullptr, nullptr);
            mEmitter6.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_853_, nodeMtx, nullptr, nullptr);
            holdSound(SE_BLasBos_KillOffElecShock);
        }
        mLightInfo.mPos = field_0x1208;
        if (dScGame_c::getUpdateFrameCount() & 2) {
            mLightInfo.SetScale(400.0f);
        } else {
            mLightInfo.SetScale(300.0f);
        }
        field_0x114B = 0;
    }

    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_thumbL2, nodeMtx);
    nodeMtx.getTranslation(mThumbL2Translation);
    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_backbone1, nodeMtx);
    nodeMtx.getTranslation(mBackbone1Translation);
    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_chest, nodeMtx);
    nodeMtx.getTranslation(mChestTranslation);
    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_toeL, nodeMtx);
    nodeMtx.getTranslation(mToeTranslation[0]);
    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_toeR, nodeMtx);
    nodeMtx.getTranslation(mToeTranslation[1]);

    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_head, nodeMtx);
    mVec3_c tmp1(0.0f, 0.0f, 0.0f);
    MTXMultVec(nodeMtx, tmp1, mPositionCopy2);
    mVec3_c v3;
    v3.x = mPositionCopy2.x;
    v3.y = mPositionCopy2.y + 50.0f;
    v3.z = mPositionCopy2.z;
    mPositionCopy3 = v3;

    if (field_0x1132) {
        fn_80030c20(0, 1500.0f, 50.0f, -200.0f, 200.0f);
    }

    nodeMtx.YrotS(mRotation.y);

    mVec3_c tmpCc;
    if (link->isAttacking() && mStateMgr.isState(StateID_Guard)) {
        if (mGuardDirection == GUARD_BT) {
            tmpCc.x = 0.0f;
            tmpCc.y = 0.0f;
            tmpCc.z = 100.0f;
            if (mXZDistanceToLink > 300.0f) {
                tmpCc.z = (mXZDistanceToLink - 300.0f) + 100.0f;
                if (tmpCc.z > 300) {
                    tmpCc.z = 300.0f;
                }
            }
        } else {
            tmpCc.x = 0.0f;
            tmpCc.y = 0.0f;
            tmpCc.z = 130.0f;
        }
        mCc1.SetR(20.0f);
    } else if (mStateMgr.isState(StateID_ThunderWait)) {
        tmpCc.set(0.0f, 0.0f, 0.0f);
        mCc1.SetR(150.0f);
    } else {
        tmpCc.set(0.0f, 0.0f, 80.0f);
        mCc1.SetR(55.0f);
    }

    MTXMultVec(nodeMtx, tmpCc, tmpCc);
    tmpCc += mPosition;

    if (link->isAttacking() && mXZDistanceToLink < 400.0f) {
        tmpCc.y += 100.0f;
        mCc1.SetH(100.0f);
    } else {
        mCc1.SetH(250.0f);
    }
    mCc1.SetC(tmpCc);

    tmpCc.x = mChestTranslation.x;
    tmpCc.y = mChestTranslation.y;
    tmpCc.z = mChestTranslation.z;
    tmpCc.y = mPosition.y;

    if (!field_0x1141) {
        mCc3.SetR(100.0f);
    } else {
        mCc3.SetR(200.0f);
    }
    mCc3.SetC(tmpCc);

    tmpCc.x = mPosition.x;
    tmpCc.y = mPosition.y + 150.0f;
    tmpCc.z = mPosition.z;

    mCc2.SetC(tmpCc);

    field_0x1250[0] = mToeTranslation[0] + (mToeTranslation[1] - mToeTranslation[0]) * 0.5f;
    // UB: this should use string comparison functions for robustness, but
    // probably works fine in MWCC
    if ((mpCurrentAnm == "CatchThunderEnd" || mpCurrentAnm == "Stan") && mMdl.getAnm().getFrame() <= 3.0f) {
        field_0x11A4 = 25.0f;
    }

    updateSkirtHairTransforms();

    if (!link->isAttacking()) {
        mInvulnerabilityTimerMaybe = 1;
    }

    if (mInvulnerabilityTimerMaybe == 0) {
        mCc1.SetTg_0x4B(1);
        // TODO
        mCc1.mTg.field_0x6C = field_0x1208;
    } else {
        mInvulnerabilityTimerMaybe--;
        tmpCc.set(0.0f, -10000.0f, 0.0);
        mCc1.SetTg_0x4B(1);
        // TODO
        mCc1.mTg.field_0x6C = tmpCc;
    }

    mCc1.ClrTg_0x10000();

    if (field_0x1142 != 0) {
        field_0x1142--;
        mCcList.ClrTg();
    } else {
        mCcList.TgSet();
    }
    mCcList.registerColliders();

    if (field_0x1139 != 0) {
        mCc4.OnAtSet();
        if (field_0x1139 == 1) {
            mCc4.setCenter(mThumbL2Translation);
            field_0x1139 = 2;
        } else {
            mCc4.moveCenter(mThumbL2Translation);
        }
        dCcS::GetInstance()->Set(&mCc4);
    }

    static mAng3_c sEffRot(0, 0, 0);
    static mVec3_c sEffScale(1.5f, 1.5f, 1.5f);
    mVec3_c effPos(mPosition.x, 10.0f, mPosition.z);
    // Basically the same code as in dWaterEffect_c::execute
    mWaterEmitter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_127_, effPos, &sEffRot, &sEffScale, nullptr, nullptr);
    f32 rate = nw4r::math::FAbs(mSpeed) * 0.02f;
    rate = rate > 0.95f ? 0.95f : rate;
    mWaterEmitter.setRate(rate + 0.05f);

    bool footSplash[2] = {false, false};
    // UB: this should use string comparison functions for robustness, but
    // probably works fine in MWCC
    if (mpCurrentAnm == "Walk" || mpCurrentAnm == "WalkBt") {
        if (mMdl.getAnm().checkFrame(22.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(48.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "TurnR") {
        if (mMdl.getAnm().checkFrame(31.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(19.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "TurnL") {
        if (mMdl.getAnm().checkFrame(31.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(19.0f)) {
            footSplash[0] = true;
        }
    } else if (mpCurrentAnm == "AttackBreak") {
        if (mMdl.getAnm().checkFrame(16.0f)) {
            footSplash[0] = true;
        }
    } else if (mpCurrentAnm == "AttackL") {
        if (mMdl.getAnm().checkFrame(26.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "AttackR") {
        if (mMdl.getAnm().checkFrame(24.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "AttackU") {
        if (mMdl.getAnm().checkFrame(10.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(24.0f)) {
            footSplash[0] = true;
        }
    } else if (mpCurrentAnm == "Counter") {
        if (mMdl.getAnm().checkFrame(21.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(45.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "AttackJumpEnd") {
        if (mMdl.getAnm().checkFrame(6.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "DownGetUp") {
        if (mMdl.getAnm().checkFrame(137.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "DownEscapeL") {
        if (mMdl.getAnm().checkFrame(15.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(17.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "DownEscapeR") {
        if (mMdl.getAnm().checkFrame(15.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(17.0f)) {
            footSplash[0] = true;
        }
    } else if (mpCurrentAnm == "ThunderDemo") {
        if (mMdl.getAnm().checkFrame(18.0f)) {
            footSplash[1] = true;
        }
    } else if (mpCurrentAnm == "AttackThunder") {
        if (mMdl.getAnm().checkFrame(25.0f)) {
            footSplash[0] = true;
        }
    }

    // Spawn splash effects
    for (int i = 0; i < 2; i++) {
        if (footSplash[i]) {
            mToeTranslation[i].y = 10.0f;
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_855_, mToeTranslation[i], nullptr, nullptr, nullptr, nullptr, 0, 0
            );
        }
    }

    if (field_0x1149 != 0) {
        if (!link->checkSwordAndMoreStates(0x400000) && (field_0x1149 < 0 || !mIsSwordEmpowered)) {
            if (mLightningTimerMaybe == 0) {
                if (dLightEnv_c::GetPInstance()->getField_0x38DC() == 0) {
                    dLightEnv_c::GetPInstance()->setField_0x38DC(1);
                    setLightningTimerMaybe(100);
                }
            } else if (!mIsSwordEmpowered || (dScGame_c::getUpdateFrameCount() & 0b11) == 0) {
                mLightningTimerMaybe--;
            }
        } else {
            setLightningTimerMaybe(100);
        }
        sLib::addCalcScaledDiff(&field_0x11CC, 1.0f, 1.0f, 0.01f);
    }

    // TODO: inline doesn't work :(
    dLightEnv_c::GetInstance().GetOverrideSpf().mIdxStart = 1;
    dLightEnv_c::GetInstance().GetOverrideSpf().mIdxEnd = 2;
    dLightEnv_c::GetInstance().GetOverrideSpf().mRatio = field_0x11CC;

    if (field_0x1147 && field_0x1148 != 0) {
        field_0x1148--;
        field_0x1220 += field_0x122C;
        nodeMtx.transS(field_0x1220);
        nodeMtx.ZXYrotM(field_0x11DC);

        if (!field_0x1150) {
            if (mCc5.ChkAtHit() || mCc5.ChkTgHit()) {
                dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_850_, nodeMtx, nullptr, nullptr, 0, 0);
                if (mCc5.ChkTgHit() && mCc5.ChkTgAtHitType(AT_TYPE_BUGNET)) {
                    getSoundSource()->startSoundAtPosition2(SE_BLasBos_EmitThunderBeam, field_0x1220);
                    field_0x1148 = 0x3C;
                    field_0x122C *= -1.0f;
                    dRumble_c::start(dRumble_c::sRumblePreset2, dRumble_c::FLAG_SLOT0);
                    dStageMgr_c::GetInstance()->fn_80199B60(3);
                    field_0x1150 = 1;
                } else {
                    field_0x1147 = 0;
                    mMtx_c mtx;
                    mtx.transS(0.0f, -100000.0f, 0.0f);
                    MTXConcat(nodeMtx, mtx, nodeMtx);
                    getSoundSource()->startSoundAtPosition2(SE_BLasBos_BeamCrossCrush, field_0x1220);
                }
                mCc5.SetTgRpm(0);
                mCc5.ClrTgActorInfo();
                mCc5.SubtractTgEffCounter();
                mCc5.SetAtRpm(0);
                mCc5.ClrAtActorInfo();
                mCc5.SubtractAtEffCounter();
            }
        } else {
            if ((field_0x1220 - mPosition).absXZ() < 150.0f) {
                dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_850_, nodeMtx, nullptr, nullptr, 0, 0);
                field_0x1147 = false;
                mMtx_c mtx;
                mtx.transS(0.0f, -100000.0f, 0.0f);
                MTXConcat(nodeMtx, mtx, nodeMtx);
                getSoundSource()->startSoundAtPosition2(SE_BLasBos_BeamCrossCrush, field_0x1220);
                v3.set(0.0f, 0.0f, 20.0f);
                rotMtx.YrotS(link->mRotation.y);
                MTXMultVec(rotMtx, v3, v3);
                field_0x119C = v3.x;
                field_0x11A0 = v3.z;
                mStateMgr.changeState(StateID_Stun);
            }
        }
        mLightInfo.mPos = field_0x1220;
        if ((dScGame_c::getUpdateFrameCount() & 2) != 0) {
            mLightInfo.SetScale(300.0f);
        } else {
            mLightInfo.SetScale(200.0f);
        }

        if (field_0x1148 <= 10) {
            // TODO: lightning timer?
            if (field_0x1148 == 10) {
                mEmitter5.setFading(10);
            }
            if (field_0x1148 == 0) {
                field_0x1147 = 0;
            }
            mCc5.ClrAtSet();
        } else {
            if (!field_0x1150) {
                mCc5.OnAtSet();
                mCc5.SetR(100.0f);
                mCc5.moveCenter(field_0x1220);
                dCcS::GetInstance()->Set(&mCc5);
            }
            getSoundSource()->holdSoundAtPosition(SE_BLasBos_ThunderBeamFly, field_0x1220);
        }
        mEmitter5.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_618_, nodeMtx, nullptr, nullptr);
    }

    if (field_0x114A != 0) {
        field_0x114A--;
        mCc5.OnAtSet();
        mCc5.SetC(mPosition);
        mCc5.SetR(300.0f - field_0x114A * 30.0f);
        mCc5.SetAtVec(link->getPosition() - mPosition);
        dCcS::GetInstance()->Set(&mCc5);
        if (field_0x114A == 8) {
            vEff.x = mPosition.x;
            vEff.y = 10.0f;
            vEff.z = mPosition.z;
            dJEffManager_c::spawnEffect(
                PARTICLE_RESOURCE_ID_MAPPING_842_, vEff, nullptr, nullptr, nullptr, nullptr, 0, 0
            );
        }
    }
    executeDemo();

    SceneflagManager *mgr = SceneflagManager::sInstance;
    if (mgr != nullptr) {
        if (mStateMgr.isState(StateID_ThunderWait)) {
            mgr->setFlag(getRoomId(), field_0x1130);
        } else {
            mgr->unsetFlag(getRoomId(), field_0x1130);
        }
    }

    if (field_0x1149 != 0) {
        // "Target lock: Demise" (Phase 2)
        mTargetFiTextID = 0x38;
    } else {
        // "Target lock: Demise" (Phase 1)
        mTargetFiTextID = 0x37;
    }

    return SUCCEEDED;
}

struct dCcD_Check : public cCcD_Obj {
    bool ChkAtHit();
    bool ChkTgHit();
};

bool dCcD_Check::ChkAtHit() {
    if (cCcD_Obj::ChkAtHit()) {
        return true;
    }
    return false;
}

bool dCcD_Check::ChkTgHit() {
    if (cCcD_Obj::ChkTgHit()) {
        return true;
    }
    return false;
}

void dAcBlastboss_c::callback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *manip, nw4r::g3d::ResMdl mdl) {
    u32 baseNodeIdSkirt = 0;
    u32 baseNodeIdHair = 0;
    int r27;
    int param_4;

    if (nodeId >= B_LAST_BOSS_NODE_skirtA1 && nodeId < B_LAST_BOSS_NODE_skirtAU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtA1;
        param_4 = 0;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtB1 && nodeId < B_LAST_BOSS_NODE_skirtBU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtB1;
        param_4 = 1;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtC1 && nodeId < B_LAST_BOSS_NODE_skirtCU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtC1;
        param_4 = 2;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtD1 && nodeId < B_LAST_BOSS_NODE_skirtDU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtD1;
        param_4 = 3;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtE1 && nodeId < B_LAST_BOSS_NODE_skirtEU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtE1;
        param_4 = 4;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtF1 && nodeId < B_LAST_BOSS_NODE_skirtFU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtF1;
        param_4 = 5;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtG1 && nodeId < B_LAST_BOSS_NODE_skirtGU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtG1;
        param_4 = 6;
    } else if (nodeId >= B_LAST_BOSS_NODE_skirtH1 && nodeId < B_LAST_BOSS_NODE_skirtHU1) {
        baseNodeIdSkirt = B_LAST_BOSS_NODE_skirtH1;
        param_4 = 7;
    }

    if (baseNodeIdSkirt != 0) {
        u32 nodeOffset = nodeId - baseNodeIdSkirt;

        mMtx_c mtx;
        mtx.transS(mpOwner->field_0x1268[param_4].field_0x014[nodeOffset]);
        mtx.XrotM(mpOwner->field_0x1268[param_4].field_0x08C[nodeOffset].x);
        mtx.YrotM(mpOwner->field_0x1268[param_4].field_0x08C[nodeOffset].y);
        mtx.ZrotM(-mpOwner->field_0x1268[param_4].field_0x08C[nodeOffset].z);
        mtx.scaleM(0.95f, 0.95f, 1.0f);
        mtx.YrotM(-0x4000);
        manip->SetMtx(mtx);
    }

    if (nodeId >= B_LAST_BOSS_NODE_hairA1 && nodeId < B_LAST_BOSS_NODE_hairBL1) {
        baseNodeIdHair = B_LAST_BOSS_NODE_hairA1;
        r27 = 0;
    }

    if (baseNodeIdHair != 0) {
        u32 nodeOffset = nodeId - baseNodeIdHair;
        mMtx_c mtx;
        mtx.transS(mpOwner->field_0x2948[r27].field_0x008[nodeOffset]);
        mtx.YrotM(mpOwner->field_0x2948[r27].field_0x044[nodeOffset].y);
        mtx.XrotM(mpOwner->field_0x2948[r27].field_0x044[nodeOffset].x);
        mtx.ZrotM(-mpOwner->field_0x2948[r27].field_0x044[nodeOffset].z);
        mtx.scaleM(0.95f, 0.95f, 1.0f);
        mtx.YrotM(-0x4000);
        manip->SetMtx(mtx);
    }

    if (nodeId == B_LAST_BOSS_NODE_center) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.XrotM(-field_0x0E.mVal / 2);
        mtx.ZrotM(-field_0x10.mVal / 2);
        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_backbone1 || nodeId == B_LAST_BOSS_NODE_backbone2) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.YrotM(field_0x0E);
        mtx.XrotM(field_0x2E - field_0x0E);
        mtx.ZrotM(-field_0x10);
        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_head) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.YrotM(field_0x0E * 2 + field_0x08 + field_0x06);
        mtx.ZrotM(field_0x0A - field_0x10 * 2);
        mtx.XrotM(field_0x0C);
        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_chin) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.ZrotM(field_0x12);
        manip->SetMtx(mtx);
    } else if (nodeId >= B_LAST_BOSS_NODE_hairBL1 && nodeId <= B_LAST_BOSS_NODE_hairC3) {
        mMtx_c mtx;
        manip->GetMtx(mtx);

        if (nodeId >= B_LAST_BOSS_NODE_hairC1) {
            mtx.XrotM(field_0x1C[nodeId - B_LAST_BOSS_NODE_hairBL1]);
            mtx.ZrotM(field_0x1C[nodeId - B_LAST_BOSS_NODE_hairBL1]);
        } else {
            mtx.YrotM(field_0x1C[nodeId - B_LAST_BOSS_NODE_hairBL1]);
            mtx.XrotM(field_0x1C[nodeId - B_LAST_BOSS_NODE_hairBL1]);
        }

        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_armR1) {
        int blah = field_0x04 * mAng(field_0x04 * 0x4300).sin() * 80.0f;

        mMtx_c mtx;
        manip->GetMtx(mtx);

        mtx.XrotM(blah + (int)(300.0f * field_0x30));
        mtx.ZrotM(blah + (int)(200.0f * field_0x30));

        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_armR2) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.ZrotM(field_0x30 * 200.0f);
        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_wristR) {
        // unused
        int blah = field_0x04 * (f32)mAng(field_0x04 * 0x4300).sin();

        mMtx_c mtx;
        manip->GetMtx(mtx);

        int i2 = 0;
        if (field_0x30 > 0.0f) {
            i2 = field_0x30 * -200.0f;
        }

        mtx.XrotM(i2 + field_0x18);
        mtx.ZrotM(i2 + field_0x1A);

        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_legR1 || nodeId == B_LAST_BOSS_NODE_legR2) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.XrotM(field_0x14);
        if (nodeId == B_LAST_BOSS_NODE_legR2) {
            mtx.XrotM(field_0x14);
        }
        manip->SetMtx(mtx);
    } else if (nodeId == B_LAST_BOSS_NODE_legL1 || nodeId == B_LAST_BOSS_NODE_legL2) {
        mMtx_c mtx;
        manip->GetMtx(mtx);
        mtx.XrotM(field_0x16);
        if (nodeId == B_LAST_BOSS_NODE_legL2) {
            mtx.XrotM(field_0x16);
        }
        manip->SetMtx(mtx);
    }
}

void dAcBlastboss_c::initializeState_Fight() {
    setAnm("WaitBt", 20.0f);
    mAnmRate = 1.0f;
    mSubState = SUB_STATE_0;
    field_0x118A = 0;
    if (field_0x1149 == 0) {
        mTimers[TIMER_2] = cM::rndF(50.0f) + 50.0f;
    } else {
        mTimers[TIMER_2] = cM::rndF(20.0f) + 10.0f;
    }
    mGuardDirection = GUARD_BT;
}
void dAcBlastboss_c::executeState_Fight() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    field_0x1131 = 1;
    f32 targetSpeed = 0.0f;
    f32 targetAnmRate = 1.5f;
    f32 puVar2 = 0.0f;

    bool b = false;
    s16 diff = mAngle.y - mYAngleToLink;

    if (mDemoState >= DEMO_STATE_200 && mDemoState <= DEMO_STATE_2XX_LAST) {
        puVar2 = -100.0f;
        mTimers[TIMER_2] = cM::rndF(50.0f) + 100.0f;
    }

    switch (mSubState) {
        case SUB_STATE_0: {
            b = true;
            field_0x118C = 0;
            targetAnmRate = 1.0f;
            field_0x117E = 0;
            if (mXZDistanceToLink > puVar2 + 450.0f) {
                mSubState = SUB_STATE_2;
                break;
            } else if (mXZDistanceToLink < puVar2 + 250.0f + 100.0f) {
                mSubState = SUB_STATE_1;
                mTimers[TIMER_0] = 8;
                mAnmRate = -1.0f;
                field_0x1180 = 0;
                // fall-through...
            } else {
                setAnm("WaitBt", 20.0f);
                mAnmRate = 1.0f;
                if (diff < -0x1000 || diff > 0x1000) {
                    mSubState = SUB_STATE_4;
                    if (diff < 0) {
                        setAnm("TurnL", 10.0f);
                    } else {
                        setAnm("TurnR", 10.0f);
                    }
                }
                break;
            }
        }
        case SUB_STATE_1: {
            b = true;
            setAnm("WalkBt", 20.0f);
            targetSpeed = -5.0f;
            targetAnmRate = -2.5f;
            if (mXZDistanceToLink < 200.0f) {
                field_0x1180++;
                if (field_0x1180 > 40) {
                    mStateMgr.changeState(StateID_PunchAttack);
                    return;
                }
            } else if (mXZDistanceToLink >= puVar2 + 200.0f && mTimers[TIMER_0] == 0) {
                mSubState = SUB_STATE_0;
            }
            break;
        }
        case SUB_STATE_2: {
            b = true;
            field_0x117E = 0;
            if (mXZDistanceToLink > puVar2 + 850.0f) {
                mSubState = SUB_STATE_3;
            } else {
                setAnm("WalkBt", 20.0f);
                if (mMdl.getAnm().getFrame() >= 25.0f && mMdl.getAnm().getFrame() <= 50.0f) {
                    targetSpeed = 5.0f;
                }
                field_0x11A4 = 5.0f;
                targetAnmRate = 1.5f;
            }
            if (mXZDistanceToLink < puVar2 + 400.0f) {
                mSubState = SUB_STATE_0;
                if (field_0x1149 == 0) {
                    mTimers[TIMER_2] = cM::rndF(50.0f) + 50.0f;
                } else {
                    mTimers[TIMER_2] = cM::rndF(20.0f) + 10.0f;
                }
            }
            break;
        }
        case SUB_STATE_3: {
            setAnm("Walk", 20.0f);
            targetSpeed = 7.5f;
            field_0x11A4 = 7.5f;
            targetAnmRate = 1.0f;
            if (mXZDistanceToLink < puVar2 + 750.0f) {
                mSubState = SUB_STATE_2;
            } else if (!link->isRecovering()) {
                field_0x117E++;
                if (field_0x117E > 70) {
                    mStateMgr.changeState(StateID_DashAttack);
                    return;
                }
            } else {
                field_0x117E = 0;
            }
            break;
        }
        case SUB_STATE_4: {
            field_0x117E = 0;
            b = true;
            if (mMdl.getAnm().isStop()) {
                mSubState = SUB_STATE_0;
            }
            break;
        }
    }

    sLib::addCalcScaledDiff(&mAnmRate, targetAnmRate, 1.0f, 0.5f);
    setAnmRate(mAnmRate);
    sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 1.0f, 2.0f);

    if (mSubState != SUB_STATE_0) {
        sLib::addCalcAngle(field_0x118C.ref(), 0x800, 1, 100);
        sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, field_0x118C);
    }

    if (b) {
        if (diff > 0x1000) {
            diff = 0x1000;
        } else if (diff < -0x1000) {
            diff = -0x1000;
        }
        field_0x1172 = diff / 2;
        // TODO what is this constant
        sLib::addCalcScaledDiff(&mMdlCallback.field_0x30, diff * (1.0f / 409.59937f), 0.5f, 1.0f);
        if (mDemoState == DEMO_STATE_NONE && !link->isUsingShield() && link->isUsingSword()) {
            mMtx_c rotMtx;
            rotMtx.YrotS(-mYAngleToLink);
            mVec3_c v = mVec3_c(link->getSwordPos() - mPosition);
            MTXMultVec(rotMtx, v, v);
            if (v.x > 150.0f) {
                v.x = 150.0f;
            } else if (v.x < -150.0f) {
                v.x = -150.0f;
            }
            sLib::addCalcAngle(field_0x118A.ref(), 400, 1, 20);
            sLib::addCalcAngle(mMdlCallback.field_0x18.ref(), mAng(v.x * -40.0f), 2, field_0x118A);
            v.y -= 270.0f;
            if (v.y > 100.0f) {
                v.y = 100.0f;
            } else if (v.y < -150.0f) {
                v.y = -150.0f;
            }
            sLib::addCalcAngle(mMdlCallback.field_0x1A.ref(), mAng(v.y * -40.0f), 2, field_0x118A);
        } else {
            field_0x118A = 0;
            sLib::addCalcAngle(mMdlCallback.field_0x18.ref(), 0, 4, 400);
            sLib::addCalcAngle(mMdlCallback.field_0x1A.ref(), 0, 4, 400);
        }
        field_0x1133 = 1;
    }

    if (field_0x117C == 0) {
        if (!checkForRangeAttack()) {
            checkForCloseRangeAttack();
            checkForCounter();
        }
        mCc1.SetTgFlag_0xA(0x1FF);
    } else {
        // TODO
        mCc1.SetTgFlag_0xA(0);
    }

    if (mTimers[TIMER_4] == 1) {
        field_0x1178 = 40;
    } else if (mTimers[TIMER_4] <= 10) {
        mNumConsecutiveSameDirectionAttacks = 0;
        mNumConsecutiveOppositeDirectionAttacks = 0;
        mChanceAttackCounter = 0;
        mLastAttackDir = mSecondLastAttackDirection = -1;
    }
}
void dAcBlastboss_c::finalizeState_Fight() {}

void dAcBlastboss_c::initializeState_Attack() {
    f32 rnd = cM::rnd();
    mSubState = SUB_STATE_0;
    if (rnd < 0.3333f) {
        setAnm("AttackU", 5.0f);
    } else if (rnd < 0.6666f) {
        setAnm("AttackR", 5.0f);
    } else {
        setAnm("AttackL", 5.0f);
    }

    if (mIsSwordEmpowered) {
        mSwordMdl.setDamageMaybe(8);
    } else {
        mSwordMdl.setDamageMaybe(4);
    }
    mSwordMdl.fn_8006B7A0(0x20000);
}
void dAcBlastboss_c::executeState_Attack() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    field_0x1131 = 1;
    f32 speed = 0.0f;
    // TODO major typing crime here but I'm not sure how this actually works
    cCcD_Obj *atHit = mSwordMdl.mCcList.find((dColliderLinkedList::ccPtmf)&dCcD_Check::ChkAtHit);

    switch (mSubState) {
        case SUB_STATE_0: {
            if (field_0x1152 >= 15 && mMdl.getAnm().checkFrame(7.0f) && cM::rnd() < 0.1f) {
                mStateMgr.changeState(StateID_Fight);
                field_0x113F = 20;
            } else {
                if (mMdl.getAnm().getFrame() < 20.0f) {
                    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x800);
                }

                if (mMdl.getAnm().getFrame() >= 25.0f && mMdl.getAnm().getFrame() <= 28.0f) {
                    mSwordMdl.enable();
                    speed = 20.0f;
                    field_0x11A4 = 20.0f;
                    field_0x113D = 10;
                }
                if (atHit != nullptr) {
                    mSubState = SUB_STATE_1;
                } else {
                    if (mMdl.getAnm().getFrame() > 28.0f) {
                        mSubState = SUB_STATE_1;
                        if (field_0x113E != 0) {
                            setAnmRate(0.5f);
                        }
                    }
                }
            }
            break;
        }
        case SUB_STATE_1: {
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
            }
            break;
        }
    }

    mSpeed = speed;
    if (!field_0x1149 && link->checkCurrentAction(/* BACKFLIP */ 0x62)) {
        field_0x113E = 20;
    }
    if (field_0x113E != 0) {
        if (field_0x1149) {
            mSubState = SUB_STATE_0;
            mStateMgr.changeState(StateID_CounterAttack);
            return;
        }
        mCc1.SetTgFlag_0xA(0);
    } else {
        if (mpCurrentAnm == "AttackR") {
            mCc1.SetTgFlag_0xA(0xE);
        } else if (mpCurrentAnm == "AttackL") {
            mCc1.SetTgFlag_0xA(0xE0);
        } else {
            mCc1.SetTgFlag_0xA(0x1);
        }
    }
    checkForCounter();
}
void dAcBlastboss_c::finalizeState_Attack() {}

void dAcBlastboss_c::initializeState_CounterAttack() {
    mSwordMdl.fn_8006B7A0(0x20000);
    if (mSubState == 0) {
        setAnm("Counter", 5.0f);
    } else {
        f32 rnd = cM::rnd();
        if (rnd < 0.3333f) {
            setAnm("AttackU", 5.0f);
        } else if (rnd < 0.6666f) {
            setAnm("AttackR", 5.0f);
        } else {
            setAnm("AttackL", 5.0f);
        }
        mMdl.setFrame(5.0f);
    }

    mNumConsecutiveSameDirectionAttacks = 0;
    mNumConsecutiveOppositeDirectionAttacks = 0;
    mLastAttackDir = mSecondLastAttackDirection = -1;
}
void dAcBlastboss_c::executeState_CounterAttack() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    f32 speed = 0.0f;
    bool b = false;
    field_0x1131 = 1;
    if (mSubState == SUB_STATE_0) {
        if (mMdl.getAnm().getFrame() < 15.0f) {
            speed = -10.0f;
        }
        if (mMdl.getAnm().getFrame() < 10.0f ||
            (mMdl.getAnm().getFrame() > 30.0f && mMdl.getAnm().getFrame() < 40.0f)) {
            sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x800);
        }

        if ((mMdl.getAnm().getFrame() >= 20.0f && mMdl.getAnm().getFrame() <= 23.0f) ||
            (mMdl.getAnm().getFrame() >= 43.0f && mMdl.getAnm().getFrame() <= 47.0f)) {
            mSwordMdl.enable();
            speed = 30.0f;
            field_0x11A4 = 20.0f;
        }

        int targetFrame = 42;
        if (mMdl.getAnm().checkFrame(targetFrame) && mXZDistanceToLink > 400.0f) {
            mStateMgr.changeState(StateID_Fight);
            return;
        }
        if (mMdl.getAnm().getFrame() >= 52.0f) {
            b = true;
        }
    } else {
        if (mMdl.getAnm().getFrame() < 20.0f) {
            sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x800);
        }
        if (mMdl.getAnm().getFrame() >= 25.0f && mMdl.getAnm().getFrame() <= 28.0f) {
            mSwordMdl.enable();
            speed = 10.0f;
            field_0x11A4 = 20.0f;
            field_0x113D = 5;
        }
        if (mMdl.getAnm().getFrame() >= 33.0f) {
            b = true;
        }
    }

    if (link->checkCurrentAction(/* BACKFLIP */ 0x62)) {
        field_0x113E = 20;
    }
    mCc1.SetTgFlag_0xA(0);
    if (field_0x113E != 0) {
        field_0x1142 = 0;
        field_0x1134 = 0;
    } else if (b) {
        mCc1.SetTgFlag_0xA(0x1FF);
        field_0x1142 = 0;
        checkForCounter();
    }

    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
    }
    mSpeed = speed;
}
void dAcBlastboss_c::finalizeState_CounterAttack() {}

void dAcBlastboss_c::initializeState_PunchAttack() {
    setAnm("AttackBreak", 5.0f);
    mSwordMdl.fn_8006B7A0(0x20000);
    mSubState = 0;
}
void dAcBlastboss_c::executeState_PunchAttack() {
    f32 speed = 0.0f;
    field_0x1131 = 1;
    mCc1.SetTgFlag_0xA(0);
    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x800);
    if (mMdl.getAnm().getFrame() < 15.0f) {
        speed = -10.0f;
    }
    if (mMdl.getAnm().getFrame() >= 15.0f && mMdl.getAnm().getFrame() <= 23.0f) {
        speed = 40.0f;
    }
    if (mMdl.getAnm().checkFrame(15.0f)) {
        field_0x1139 = 1;
    }
    if (mMdl.getAnm().checkFrame(23.0f)) {
        field_0x1139 = 0;
    }
    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
    }
    mSpeed = speed;
}
void dAcBlastboss_c::finalizeState_PunchAttack() {}

void dAcBlastboss_c::initializeState_DashAttack() {
    mSubState = 0;
    mSwordMdl.setDamageMaybe(8);
    mSwordMdl.fn_8006B7A0(0x20000);
}
void dAcBlastboss_c::executeState_DashAttack() {
    field_0x1131 = 1;
    f32 targetSpeed = 0.0f;
    f32 stepSize = 40.0f; // never set differently
    mCc1.SetTgFlag_0xA(0x1FF);
    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x1000);
    switch (mSubState) {
        case SUB_STATE_0: {
            if (mXZDistanceToLink < 400.0f) {
                mStateMgr.changeState(StateID_CounterAttack);
                return;
            }
            setAnm("AttackJumpStart", 5.0f);
            mSwordMdl.fn_8006B7A0(0x20000);
            mSubState = SUB_STATE_1;
            // fall-through
        }
        case SUB_STATE_1: {
            if (mMdl.getAnm().getFrame() >= 23.0f) {
                field_0x11A4 = 30.0f;
                targetSpeed = 40.0f;
            }
            if (mMdl.getAnm().isStop()) {
                setAnm("AttackJumpLoop", 0.0f);
                mSubState = SUB_STATE_2;
                mTimers[TIMER_0] = 200;
            }
            break;
        }
        case SUB_STATE_2: {
            field_0x11A4 = 30.0f;
            targetSpeed = 80.0f;
            field_0x11B8 = 5.0f;
            if (mXZDistanceToLink < 750.0f) {
                setAnm("AttackJumpEnd", 0.0f);
                mSubState = SUB_STATE_3;
            } else if (mTimers[TIMER_0] == 0) {
                mStateMgr.changeState(StateID_Fight);
            }
            break;
        }
        case SUB_STATE_3: {
            if (mXZDistanceToLink > 300.0f && mMdl.getAnm().getFrame() <= 6.0f) {
                targetSpeed = 80.0f;
            } else {
                stepSize = 40.0f;
            }
            if (mMdl.getAnm().getFrame() >= 6.0f && mMdl.getAnm().getFrame() <= 9.0f) {
                mSwordMdl.enable();
                field_0x113D = 10;
            }

            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
            }
            break;
        }
    }

    sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 1.0f, stepSize);
    if (mSpeed > 50.0f) {
        // @bug: effect rotation never updated due to static...
        static mAng3_c sEffRot(0, mRotation.y, 0);
        static mVec3_c sEffScale(1.0f, 1.0f, 1.0f);
        mVec3_c effPos(mPosition.x, 10.0f, mPosition.z);
        mEmitter2.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_858_, effPos, &sEffRot, &sEffScale, nullptr, nullptr);
    }
}
void dAcBlastboss_c::finalizeState_DashAttack() {}

void dAcBlastboss_c::initializeState_SmallAttack() {
    setAnm("AttackSwordBreak", 5.0f);
    mSubState = SUB_STATE_0;
}
void dAcBlastboss_c::executeState_SmallAttack() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    field_0x1131 = 1;
    f32 targetSpeed = 0.0f;

    mCc1.SetTgFlag_0xA(0);
    switch (mSubState) {
        case SUB_STATE_0: {
            if (mMdl.getAnm().getFrame() >= 31.0f && mMdl.getAnm().getFrame() <= 33.0f) {
                field_0x11A4 = 15.0f;
                if (checkForLinkSwordBySwordHit()) {
                    mVec3_c effPos;

                    effPos = link->vt_0x278() + (link->getSwordPos() - link->vt_0x278()) * 0.4f;
                    dJEffManager_c::spawnEffect(
                        PARTICLE_RESOURCE_ID_MAPPING_388_, effPos, &mRotation, nullptr, nullptr, nullptr, 0, 0
                    );
                    dRumble_c::start(dRumble_c::sRumblePreset4, dRumble_c::FLAG_SLOT0);
                    startSound(SE_BLasBos_SwordBySword);
                    link->onForceOrPreventActionFlags(0x80000);
                    mSubState = SUB_STATE_1;
                }
            }
            break;
        }
    }

    if (mMdl.getAnm().checkFrame(44.0f) && mSubState != SUB_STATE_1) {
        mStateMgr.changeState(StateID_Fight);
        mpCurrentAnm = nullptr;
        setAnm("WaitBt", 40.0f);
        field_0x113E = 10;
        field_0x1134 = 0;
    }

    if (mMdl.getAnm().getFrame() >= 55.0f && mMdl.getAnm().getFrame() <= 59.0f) {
        mSwordMdl.enable();
        targetSpeed = 40.0f;
        field_0x11A4 = 20.0f;
    }

    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
    }

    sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 1.0f, 20.0f);
    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x200);
}
void dAcBlastboss_c::finalizeState_SmallAttack() {}

void dAcBlastboss_c::initializeState_ThunderAttack() {
    setAnm("AttackThunder", 5.0f);
    mSubState = SUB_STATE_0;
}
void dAcBlastboss_c::executeState_ThunderAttack() {
    field_0x1131 = 1;
    f32 targetSpeed = 0.0f;
    mCc1.SetTgFlag_0xA(0);
    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x400);
    if (mMdl.getAnm().getFrame() >= 21.0f && mMdl.getAnm().getFrame() <= 25.0f) {
        mSwordMdl.enable();
        field_0x11A4 = 20.0f;
        if (mMdl.getAnm().checkFrame(23.0f)) {
            mIsSwordEmpowered = false;
            mCc1.ClrTg_0x8000000();
            mSwordMdl.fn_8006B800(0);
            field_0x1147 = 1;
            field_0x1148 = 60;
            field_0x1150 = 0;
            field_0x11DC = mRotation;
            mVec3_c v(0.0f, 100.0f, 300.0f);
            mMtx_c mtx;
            mtx.YrotS(mRotation.y);
            MTXMultVec(mtx, v, field_0x1220);
            field_0x1220 += mPosition;

            v.set(0.0f, 0.0f, 50.0f);
            MTXMultVec(mtx, v, field_0x122C);
            mCc5.setCenter(field_0x1220);
            setLightningTimerMaybe(30);
            startSound(SE_BLasBos_EmitThunderBeam);
        }
    }
    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
    }

    sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 1.0f, 5.0f);
}
void dAcBlastboss_c::finalizeState_ThunderAttack() {}

void dAcBlastboss_c::initializeState_Guard() {
    mSubState = SUB_STATE_0;
    mMdlCallback.field_0x30 = 0.0f;
    mMdlCallback.field_0x18 = 0;
}
void dAcBlastboss_c::executeState_Guard() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    field_0x1131 = 1;
    f32 targetSpeed = 0.0f;

    static const char *sGuardNames[] = {
        "GuardR", "GuardRU", "GuardRD", "GuardL", "GuardLU", "GuardLD", "GuardU", "GuardD", "GuardC", "WaitBt",
    };

    static const u32 sGuardFlags[] = {
        0x8 | 0x4 | 0x2,
        0x8 | 0x4 | 0x2,
        0x10 | 0x8 | 0x4 | 0x2,
        0x80 | 0x40 | 0x20,
        0x80 | 0x40 | 0x20,
        0x80 | 0x40 | 0x20 | 0x10,
        0x80 | 0x2 | 0x1,
        0x20 | 0x10 | 0x8,
        0x100,
        0x100 | 0x80 | 0x40 | 0x20 | 0x10 | 0x8 | 0x4 | 0x2 | 0x1,
    };

    switch (mSubState) {
        case SUB_STATE_0: {
            if (link->isAttackingJumpSlash()) {
                mSubState = SUB_STATE_2;
                setAnm("WalkBt", 2.0f);
                setAnmRate(-2.0f);
                mTimers[TIMER_0] = 5;
                break;
            } else {
                if (mGuardDirection == GUARD_BT) {
                    setAnm(sGuardNames[mGuardDirection], 3.0f);
                    mTimers[TIMER_0] = 14;
                } else {
                    forceSetAnm(sGuardNames[mGuardDirection], 3.0f);
                }
                mSubState = SUB_STATE_1;
                // fall-through
            }
        }
        case SUB_STATE_1: {
            if (link->isAttackingSpin()) {
                mTimers[TIMER_0] = 5;
            }

            if (mGuardDirection == GUARD_BT) {
                field_0x1133 = 1;
                if (mTimers[TIMER_0] == 0) {
                    mStateMgr.changeState(StateID_Fight);
                    mTimers[TIMER_4] = 15;
                }
            } else if (mMdl.getAnm().getFrame() >= 13.0f) {
                mStateMgr.changeState(StateID_Fight);
                mTimers[TIMER_4] = 15;
            }

            if (!link->isAttackingJumpSlash()) {
                checkForCounter();
            }
            break;
        }
        case SUB_STATE_2: {
            field_0x11A4 = 20.0f;
            targetSpeed = -30.0f;
            if (mTimers[TIMER_0] == 0) {
                mGuardDirection = GUARD_UP;
                forceSetAnm(sGuardNames[mGuardDirection], 3.0);
                mSubState = SUB_STATE_1;
            }
            break;
        }
        case SUB_STATE_10: {
            if (mGuardDirection == GUARD_RIGHT) {
                setAnm("AttackRGuard", 3.0f);
            } else if (mGuardDirection == GUARD_LEFT) {
                setAnm("AttackLGuard", 3.0f);
            } else {
                setAnm("GuardU", 3.0f);
            }
            mSubState = SUB_STATE_11;
            // fall-through
        }
        case SUB_STATE_11: {
            if (mMdl.getAnm().getFrame() >= 13.0f) {
                mStateMgr.changeState(StateID_Fight);
                mTimers[TIMER_4] = 15;
            }
            break;
        }
    }

    sLib::addCalcScaledDiff(&mSpeed, targetSpeed, 1.0f, 5.0f);
    sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x1000);

    if (field_0x113E != 0) {
        setAnmRate(0.2f);
    } else {
        setAnmRate(1.0f);
        if (link->isAttacking()) {
            switch (dAcPy_c::GetLinkM()->getSpecificAttackDirection()) {
                case daPlayerActBase_c::ATTACK_DIRECTION_DOWN:      mGuardDirection = GUARD_UP; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_LEFT:      mGuardDirection = GUARD_LEFT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT:  mGuardDirection = GUARD_UPLEFT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT:    mGuardDirection = GUARD_DOWNLEFT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_RIGHT:     mGuardDirection = GUARD_RIGHT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT: mGuardDirection = GUARD_UPRIGHT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT:   mGuardDirection = GUARD_DOWNRIGHT; break;
                case daPlayerActBase_c::ATTACK_DIRECTION_UP:        mGuardDirection = GUARD_DOWN; break;
                default:                                            mGuardDirection = GUARD_CENTER; break;
            }
            setAnm(sGuardNames[mGuardDirection], 3.0);
        }
    }

    mCc1.SetTgFlag_0xA(sGuardFlags[mGuardDirection]);
    checkForRangeAttack();
}
void dAcBlastboss_c::finalizeState_Guard() {}

void dAcBlastboss_c::initializeState_GuardBreak() {
    s32 side;
    s32 attackDir = dAcPy_c::GetLinkM()->getSpecificAttackDirection();
    if (attackDir == daPlayerActBase_c::ATTACK_DIRECTION_LEFT ||
        attackDir == daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT ||
        attackDir == daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT) {
        side = 0;
    } else if (attackDir == daPlayerActBase_c::ATTACK_DIRECTION_RIGHT ||
               attackDir == daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT ||
               attackDir == daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT) {
        side = 1;
    } else {
        side = (s32)cM::rndF(1.99f);
    }

    if (side == 0) {
        setAnm("GuardBreak", 5.0f);
    } else {
        setAnm("GuardBreakB", 5.0f);
    }
    mSubState = 0;
    mTimers[TIMER_0] = 3;
}
void dAcBlastboss_c::executeState_GuardBreak() {
    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
        mTimers[TIMER_4] = 15;
    }

    if (mTimers[TIMER_0] == 1) {
        mSpeed = -30.0f;
    }
    sLib::addCalcScaled(&mSpeed, 1.0f, 4.0f);
    mCc1.SetTgFlag_0xA(0x1FF);
    checkForCounter();
}
void dAcBlastboss_c::finalizeState_GuardBreak() {}

void dAcBlastboss_c::initializeState_Damage() {
    forceSetAnm("Damage", 5.0f);
    mSubState = SUB_STATE_0;
    mSpeed = -20.0f;
    mChanceAttackCounter++;
    field_0x1142 = 5;
    startSound(SE_BLasBos_VO_DamageNormal);
}
void dAcBlastboss_c::executeState_Damage() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    sLib::addCalcScaled(&mSpeed, 1.0f, 2.0f);
    if (mMdl.getAnm().isStop()) {
        mStateMgr.changeState(StateID_Fight);
        mTimers[TIMER_4] = 15;
        mTimers[TIMER_2] = 0;
    }
    field_0x113E = 0;
    if (field_0x1149 == 0 && !link->isAttackingSpin()) {
        mCc1.SetTgFlag_0xA(0x1FF);
    }
    checkForCounter();
}
void dAcBlastboss_c::finalizeState_Damage() {}

void dAcBlastboss_c::initializeState_SitDamage() {
    setAnm("Chance", 5.0f);
    mSubState = 0;
    mSpeed = -20.0f;
    mChanceAttackCounter = 0;
    mTimers[TIMER_0] = 80;
    field_0x1142 = 5;
    mChanceCounter++;
}
void dAcBlastboss_c::executeState_SitDamage() {
    sLib::addCalcScaled(&mSpeed, 1.0f, 2.0f);
    switch (mSubState) {
        case SUB_STATE_0: {
            if (mMdl.getAnm().isStop()) {
                setAnm("ChanceLoop", 0.0f);
                mSubState = SUB_STATE_1;
            }
            break;
        }
        case SUB_STATE_1: {
            if (mTimers[TIMER_0] == 0 || mChanceAttackCounter >= 5) {
                setAnm("ChanceReverse", 5.0f);
                mSubState = SUB_STATE_2;
                // fall-through
            } else {
                break;
            }
        }
        case SUB_STATE_2: {
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
                mTimers[TIMER_4] = 15;
            }
            checkForCounter();
            mCc1.SetTgFlag_0xA(0x1FF);
            return;
        }
        case SUB_STATE_5: {
            if (field_0x1137 == 0) {
                forceSetAnm("ChanceDamageU", 2.0f);
            } else if (field_0x1137 == 1) {
                forceSetAnm("ChanceDamageR", 2.0f);
            } else if (field_0x1137 == 2) {
                forceSetAnm("ChanceDamageL", 2.0f);
            } else {
                forceSetAnm("ChanceDamageD", 2.0f);
            }
            mSubState = SUB_STATE_6;
            break;
        }
        case SUB_STATE_6: {
            if (mTimers[TIMER_0] == 0 || mChanceAttackCounter >= 5) {
                setAnm("ChanceReverse", 5.0f);
                mSubState = SUB_STATE_2;
                // fall-through
            } else if (mMdl.getAnm().isStop()) {
                setAnm("ChanceLoop", 0.0f);
                mSubState = SUB_STATE_1;
            }
        }
    }
    mCc1.SetTgFlag_0xA(0);
}
void dAcBlastboss_c::finalizeState_SitDamage() {}

void dAcBlastboss_c::initializeState_Down() {
    setAnm("DownBStart", 5.0f);
    mSubState = SUB_STATE_0;
    mSpeed = -20.0f;
    mAcceleration = -5.0f;
    mVelocity.y = 60.0f;
    mIsSwordEmpowered = false;
    mCc1.ClrTg_0x8000000();
    mSwordMdl.fn_8006B800(0);
}
void dAcBlastboss_c::executeState_Down() {
    mVec3_c v;
    mMtx_c mtx;

    field_0x1141 = 1;
    field_0x1142 = 5;
    bool b = false;
    setLightningTimerMaybe(30);
    mGuardDirection = -1;

    // TODO: Are all these fall-throughs intentional??????
    switch (mSubState) {
        case SUB_STATE_0: {
            if (mMdl.getAnm().isStop()) {
                mSubState = SUB_STATE_1;
                setAnm("DownBLoop", 0.0f);
                field_0x11C0 = 20.0f;
            }
            // fall-through
        }
        case SUB_STATE_1: {
            sLib::addCalcAngle(mAngle.x.ref(), -0x4000, 2, 0x300);
            if (field_0x11BC <= -10.0f && field_0x1140) {
                mSubState = SUB_STATE_2;
                b = true;
                setAnm("DownBEnd", 2.0f);
                mVelocity.y = 30.0f;
            } else {
                field_0x11A4 = -30.0f;
                break;
            }
            // fall-through
        }
        case SUB_STATE_2: {
            if (mVelocity.y < 0.0f) {
                field_0x11C0 = -35.0f;
            }
            sLib::addCalcAngle(mAngle.x.ref(), -0x4000, 2, 0x300);
            if (field_0x11BC <= -10.0f && field_0x1140) {
                b = true;
            }
            if (mMdl.getAnm().isStop()) {
                setAnm("DownLoop", 0.0f);
                mSubState = SUB_STATE_3;
                if (field_0x113B >= 3 || mDemoState != DEMO_STATE_NONE) {
                    mTimers[TIMER_0] = 70;
                } else if (field_0x113B == 2) {
                    mTimers[TIMER_0] = 40;
                } else {
                    mTimers[TIMER_0] = 25;
                }
                field_0x113B++;
            } else {
                sLib::addCalcScaled(&mSpeed, 1.0f, 1.0f);
                if (mSpeed < 1.0f) {
                    mFinalBlowPosition.set(mChestTranslation.x, mChestTranslation.y, mChestTranslation.z);
                    setInteractionFlags(INTERACT_0x1);
                }
                break;
            }
            // fall-through
        }
        case SUB_STATE_3: {
            mFinalBlowPosition.set(mChestTranslation.x, mChestTranslation.y, mChestTranslation.z);
            sLib::addCalcScaled(&mSpeed, 1.0f, 1.0f);
            if (checkInteractionFlags(INTERACT_0x1000)) {
                setAnm("End", 3.0f);
                mSubState = SUB_STATE_50;
                mDemoState = DEMO_STATE_100;
                startSound(SE_BLasBos_LastHit);
                field_0x114F = 1;
                resetInteractionFlags(INTERACT_0x1000);
                resetInteractionFlags(INTERACT_0x1);
            } else if (mTimers[TIMER_0] == 0) {
                if (cM::rnd() < 0.5f || mDemoState != DEMO_STATE_NONE) {
                    setAnm("DownEscapeR", 10.0f);
                } else {
                    setAnm("DownEscapeL", 10.0f);
                }
                setAnmRate(0.65f);
                mSubState = SUB_STATE_5;
                field_0x11C8 = 0.0f;
                resetInteractionFlags(INTERACT_0x1);
                if (mDemoState != DEMO_STATE_NONE) {
                    mDemoState = DEMO_STATE_3;
                    field_0x2D00 = 0.0f;
                    dLightEnv_c::GetPInstance()->setField_0x38DC(1);
                }
            }
            break;
        }
        case SUB_STATE_4: {
            if (mMdl.getAnm().getFrame() >= 52.0f) {
                field_0x1141 = 0;
                sLib::addCalcAngle(mAngle.x.ref(), 0, 1, 910);
            }
            if (mMdl.getAnm().getFrame() >= 170.0f) {
                field_0x1131 = 2;
            }
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
                mTimers[TIMER_4] = 15;
            }
            break;
        }
        case SUB_STATE_5: {
            sLib::addCalcAngle(mAngle.x.ref(), 0, 1, 0x400);
            if ((int)mMdl.getAnm().getFrame() <= 14) {
                sLib::addCalcScaledDiff(&field_0x11C8, 15.0f, 1.0f, 5.0f);
                field_0x11A4 = 20.0f;
            } else {
                field_0x1141 = 0;
                sLib::addCalcAngle(mAngle.y.ref(), mYAngleToLink, 2, 0x800);
                sLib::addCalcScaled(&field_0x11C8, 1.0f, 5.0f);
            }
            mtx.YrotS(mRotation.y);
            if (mpCurrentAnm == "DownEscapeR") {
                v.set(-field_0x11C8, 0.0f, 0.0f);
            } else {
                v.set(field_0x11C8, 0.0f, 0.0f);
            }
            MTXMultVec(mtx, v, v);
            mPosition.x += v.x;
            mPosition.z += v.z;

            if (mMdl.getAnm().isStop()) {
                if (mDemoState != DEMO_STATE_NONE) {
                    setAnm("ThunderDemo", 10.0f);
                    mSubState = SUB_STATE_20;
                } else {
                    if (field_0x113B >= 3 && mXZDistanceToLink < 500.0f) {
                        mStateMgr.changeState(StateID_Attack);
                        mMdl.setFrame(10.0f);
                    } else {
                        mStateMgr.changeState(StateID_Fight);
                        mTimers[TIMER_4] = 15;
                    }
                }
            }
            break;
        }
        case SUB_STATE_10: {
            // TODO: Is this state ever set?
            if (mMdl.getAnm().isStop()) {
                setAnm("DownGetUp", 10.0f);
                mSubState = SUB_STATE_4;
            }
            break;
        }
        case SUB_STATE_20: {
            field_0x1141 = 0;
            if (mMdl.getAnm().isStop()) {
                mSubState = SUB_STATE_21;
                setAnm("CatchThunderLoop", 0.0f);
            }
            break;
        }
        case SUB_STATE_21: {
            field_0x1141 = 0;
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
                mTimers[TIMER_4] = 15;
                mDemoState = DEMO_STATE_1000;
            }
            break;
        }
    }

    if (b) {
        v.set(0.0f, 0.0f, -100.0f);
        mtx.YrotS(mRotation.y);
        MTXMultVec(mtx, v, v);
        v.x += mBackbone1Translation.x;
        v.z += mBackbone1Translation.z;
        v.y = 10.0f;

        static mVec3_c sScale(3.0f, 3.0f, 3.0f);
        dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_91_, v, nullptr, &sScale, nullptr, nullptr, 0, 0);
    }
}
void dAcBlastboss_c::finalizeState_Down() {}

void dAcBlastboss_c::initializeState_Stun() {
    setAnm("Stan", 3.0f);
    mSubState = SUB_STATE_0;
    mChanceAttackCounter = 0;
    mInvulnerabilityTimerMaybe = 10;
    mStunCounter++;
    mIsSwordEmpowered = false;
    mCc1.ClrTg_0x8000000();
    mSwordMdl.fn_8006B800(0);
    startSound(SE_BLasBos_HitThunderBeam);
}
void dAcBlastboss_c::executeState_Stun() {
    setLightningTimerMaybe(40);
    field_0x114B = 1;
    int frame = mMdl.getAnm().getFrame();
    switch (mSubState) {
        case SUB_STATE_0: {
            if (mMdl.getAnm().isStop()) {
                mSubState = SUB_STATE_1;
                setAnm("StanLoop", 0.0f);
                mTimers[TIMER_0] = 100;
            }
            break;
        }
        case SUB_STATE_1: {
            if (mTimers[TIMER_0] == 0) {
                mSubState = SUB_STATE_2;
                setAnm("StanReverse", 10.0f);
            }
            break;
        }
        case SUB_STATE_2: {
            if (frame <= 58) {
                if (frame == 58) {
                    field_0x114B = 2;
                }
            } else {
                field_0x114B = 0;
                if (frame <= 61) {
                    field_0x11A4 = 25.0f;
                }
            }
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
            }
            break;
        }
    }

    sLib::addCalcScaled(&mSpeed, 1.0f, 10.0f);
    if (mChanceAttackCounter >= 7) {
        mCc1.SetTgFlag_0xA(0x1FF);
        checkForCounter();
    } else {
        if (mInvulnerabilityTimerMaybe != 0) {
            mCc1.SetTgFlag_0xA(0x1FF);
        } else {
            mCc1.SetTgFlag_0xA(0);
        }
    }
}
void dAcBlastboss_c::finalizeState_Stun() {}

void dAcBlastboss_c::initializeState_ThunderWait() {
    setAnm("CatchThunderStart", 10.0f);
    mSubState = SUB_STATE_0;
    mGuardDirection = -1;
    mTimers[TIMER_0] = 30;
}
void dAcBlastboss_c::executeState_ThunderWait() {
    dAcPy_c *link = dAcPy_c::GetLinkM();
    mCc1.SetTgFlag_0xA(0);

    switch (mSubState) {
        case SUB_STATE_0: {
            if (mMdl.getAnm().isStop()) {
                mSubState = SUB_STATE_1;
                setAnm("CatchThunderLoop", 0.0f);
            }
            // fall-through
        }
        case SUB_STATE_1: {
            if (link->checkSwordAndMoreStates(0x400000)) {
                mStateMgr.changeState(StateID_Fight);
            } else if (mTimers[TIMER_0] == 0) {
                dLightEnv_c::GetPInstance()->setField_0x38DC(7);
                mSubState = SUB_STATE_5;
                setAnm("CatchThunderEnd", 5.0f);
                mVec3_c vEff;
                vEff.x = field_0x1208.x;
                vEff.y = field_0x1208.y + 110.0f;
                vEff.z = field_0x1208.z;
                dJEffManager_c::spawnEffect(
                    PARTICLE_RESOURCE_ID_MAPPING_624_, vEff, nullptr, nullptr, nullptr, nullptr, 0, 0
                );
                mTimers[TIMER_3] = cM::rndF(100.0f) + 900.0f;
                mIsSwordEmpowered = true;
                mSwordMdl.fn_8006B800(4);
                mCc1.OnTg_0x8000000();
                field_0x114A = 10;
                startSound(SE_BLasBos_ThunderFallSword);
            }
            break;
        }
        case SUB_STATE_5: {
            if (mMdl.getAnm().getFrame() >= 20.0f) {
                mCc1.SetTgFlag_0xA(0x1FF);
                checkForCounter();
            }
            if (mMdl.getAnm().isStop()) {
                mStateMgr.changeState(StateID_Fight);
            }
            break;
        }
    }
    sLib::addCalcScaled(&mSpeed, 1.0f, 4.0f);
}
void dAcBlastboss_c::finalizeState_ThunderWait() {}

bool dAcBlastboss_c::checkDamage() {
    mVec3_c v;
    mVec3_c v2;
    mMtx_c mtx;

    mHealth = 400;
    // TODO: typing crime
    cCcD_Obj *atHit = mSwordMdl.mCcList.find((dColliderLinkedList::ccPtmf)&dCcD_Check::ChkAtHit);
    if (atHit != nullptr && atHit->GetAtFlag0x8()) {
        field_0x113E = 20;
    }

    if (sLib::calcTimer(&field_0x1134) == 0) {
        // TODO: regswap between unk and link.
        // Making this a u16 or s16 fixes regalloc but breaks instructions...
        s32 unk = someEnemyDamageCollisionStuffMaybe(mCcList, nullptr);

        // TODO: typing crime
        cCcD_Obj *tgHit = mCcList.find((dColliderLinkedList::ccPtmf)&dCcD_Check::ChkTgHit);
        if (tgHit == nullptr) {
            return false;
        }

        dAcPy_c *link = dAcPy_c::GetLinkM();
        s32 mLastAttackDirection = link->getSpecificAttackDirection();

        // TODO: enum?
        switch (unk) {
            case 0: {
                break;
            }
            case 6: {
                startSound(SE_BLasBos_SwordBySword);
                // TODO inline?
                if (tgHit->GetTgDamageFlags() & 0x4) {
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 20.0f;
                    mtx.YrotS(link->mRotation.y);
                    MTXMultVec(mtx, v, v2);
                    field_0x119C = v2.x;
                    field_0x11A0 = v2.z;
                    mStateMgr.changeState(StateID_Stun);
                    break;
                }

                if (tgHit->ChkTgAtHitType(AT_TYPE_0x800000)) {
                    mGuardDirection = GUARD_UP;
                    mStateMgr.changeState(StateID_Guard);
                    break;
                }
                mMdlCallback.field_0x04.set(7);
                if (field_0x113F != 0) {
                    mSubState = SUB_STATE_1;
                    mStateMgr.changeState(StateID_CounterAttack);
                    break;
                } else if (mGuardDirection == GUARD_BT) {
                    if (classifyAttackDirection(mLastAttackDirection) == classifyAttackDirection(mLastAttackDir)) {
                        mNumConsecutiveOppositeDirectionAttacks++;
                    } else {
                        mNumConsecutiveSameDirectionAttacks++;
                    }
                    if (mNumConsecutiveSameDirectionAttacks >= 2) {
                        mSubState = SUB_STATE_1;
                        mStateMgr.changeState(StateID_CounterAttack);
                        break;
                    }

                    if (classifyAttackDirection(mLastAttackDirection) == 0) {
                        // Left
                        mMdlCallback.field_0x30 += 3.0f;
                        mMdlCallback.field_0x18 += 0x7D0;
                    } else if (classifyAttackDirection(mLastAttackDirection) == 1) {
                        // Right
                        mMdlCallback.field_0x30 -= 3.0f;
                        mMdlCallback.field_0x18 -= 0x7D0;
                    }
                    if (mNumConsecutiveOppositeDirectionAttacks >= 1) {
                        mStateMgr.changeState(StateID_GuardBreak);
                    }
                } else if (mLastAttackDir >= 0) {
                    if (classifyAttackDirection(mLastAttackDirection) == classifyAttackDirection(mLastAttackDir)) {
                        mNumConsecutiveSameDirectionAttacks++;
                    } else {
                        mNumConsecutiveOppositeDirectionAttacks++;
                    }
                    s8 x = 3;
                    if (classifyAttackDirection(mLastAttackDirection) == classifyAttackDirection(mLastAttackDir)) {
                        x = 1;
                    }
                    if (mNumConsecutiveSameDirectionAttacks >= x ||
                        (mSecondLastAttackDirection >= 0 && (classifyAttackDirection(mSecondLastAttackDirection) & 2) ==
                                                                (classifyAttackDirection(mLastAttackDirection) & 2))) {
                        mSubState = SUB_STATE_0;
                        mStateMgr.changeState(StateID_CounterAttack);
                    } else {
                        if (mNumConsecutiveOppositeDirectionAttacks >= 1) {
                            field_0x113E = 25;
                            mNumConsecutiveOppositeDirectionAttacks = 1;
                            field_0x1182 = 20;
                        }
                    }
                }
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = -5.0f;
                mtx.YrotS(mYAngleToLink);
                MTXMultVec(mtx, v, v2);
                field_0x119C = v2.x;
                field_0x11A0 = v2.z;
                mLastAttackDir = mLastAttackDirection;
                break;
            }
            case 7: {
                break;
            }
            default: {
                if (tgHit != nullptr &&
                    (tgHit->ChkTgAtHitType(AT_TYPE_SLINGSHOT) || tgHit->ChkTgAtHitType(AT_TYPE_CLAWSHOT) ||
                     tgHit->ChkTgAtHitType(AT_TYPE_BOMB) || tgHit->ChkTgAtHitType(AT_TYPE_ARROW) ||
                     tgHit->ChkTgAtHitType(AT_TYPE_WHIP) || tgHit->ChkTgAtHitType(AT_TYPE_0x40))) {
                    mGuardDirection = GUARD_UP;
                    mStateMgr.changeState(StateID_Guard);

                    if (tgHit->ChkTgAtHitType(AT_TYPE_BOMB)) {
                        field_0x11A4 = 30.0f;
                        field_0x11B8 = 15.0f;
                        v.x = 0.0f;
                        v.y = 0.0f;
                        v.z = -10.0f;
                        mtx.YrotS(mRotation.y);
                        MTXMultVec(mtx, v, v2);
                        field_0x119C = v2.x;
                        field_0x11A0 = v2.z;
                    }

                    break;
                }

                // TODO inline?
                if (tgHit->GetTgDamageFlags() & 4) {
                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = 20.0f;
                    mtx.YrotS(link->mRotation.y);
                    MTXMultVec(mtx, v, v2);
                    field_0x119C = v2.x;
                    field_0x11A0 = v2.z;
                    mStateMgr.changeState(StateID_Stun);
                    break;
                }

                field_0x1152++;
                if (field_0x1152 == 15) {
                    dSndBgmMgr_c::GetInstance()->fn_80372D70(3);
                }

                if (field_0x1149 >= 0) {
                    if (field_0x1152 == 30) {
                        dStageMgr_c::GetInstance()->fn_80199B60(4);
                        mStateMgr.changeState(StateID_Down);
                        mDemoState = DEMO_STATE_1;
                        startSound(SE_BLasBos_DownHit);
                        break;
                    }
                } else {
                    if (field_0x1152 == 50) {
                        mStateMgr.changeState(StateID_Down);
                        field_0x1152 = 45;
                        break;
                    }
                }

                if ((!field_0x113E && (mStateMgr.isState(StateID_Attack) || mStateMgr.isState(StateID_SmallAttack) ||
                                       mStateMgr.isState(StateID_CounterAttack))) ||
                    mStateMgr.isState(StateID_ThunderWait)) {
                    field_0x1188 = 10;
                    break;
                }

                if (field_0x113E != 0 && field_0x1149 != 0) {
                    field_0x1188 = 10;
                    break;
                }

                if (mStateMgr.isState(StateID_Stun)) {
                    mNumConsecutiveSameDirectionAttacks = 0;
                    mSecondLastAttackDirection = mLastAttackDirection;
                    mChanceAttackCounter++;

                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = -20.0f;
                    mtx.YrotS(mYAngleToLink);
                    MTXMultVec(mtx, v, v2);
                    field_0x119C = v2.x;
                    field_0x11A0 = v2.z;
                    field_0x1138 = 10;
                    field_0x11B8 = 5.0f;
                    field_0x11A4 = 7.0f;
                    if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_LEFT ||
                        mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT ||
                        mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT) {
                        field_0x1137 = 1;
                    } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_RIGHT ||
                               mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT ||
                               mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT) {
                        field_0x1137 = 2;
                    } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_STAB) {
                        field_0x1137 = 3;
                    } else {
                        field_0x1137 = 0;
                    }
                    startSound(SE_BLasBos_VO_DamageChance);
                    break;
                }

                if (mStateMgr.isState(StateID_SitDamage)) {
                    mNumConsecutiveSameDirectionAttacks = 0;
                    mSecondLastAttackDirection = mLastAttackDirection;
                    mChanceAttackCounter++;

                    if (mChanceAttackCounter >= 5 && mChanceCounter >= 3) {
                        if (field_0x1149 >= 0) {
                            mChanceAttackCounter = 0;
                            field_0x1152 = 30;
                            dStageMgr_c::GetInstance()->fn_80199B60(4);
                            mStateMgr.changeState(StateID_Down);
                            mDemoState = DEMO_STATE_1;
                            startSound(SE_BLasBos_DownHit);
                            break;
                        }
                        mStateMgr.changeState(StateID_Down);
                        field_0x1152 = 45;
                        break;
                    }

                    v.x = 0.0f;
                    v.y = 0.0f;
                    v.z = -20.0f;
                    mtx.YrotS(mYAngleToLink);
                    MTXMultVec(mtx, v, v2);
                    field_0x119C = v2.x;
                    field_0x11A0 = v2.z;
                    field_0x11B8 = 5.0f;
                    field_0x11A4 = 7.0f;
                    if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_LEFT ||
                        mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT ||
                        mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT) {
                        field_0x1137 = 1;
                    } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_RIGHT ||
                               mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT ||
                               mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT) {
                        field_0x1137 = 2;
                    } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWN) {
                        field_0x1137 = 3;
                    } else {
                        field_0x1137 = 0;
                    }
                    mSubState = SUB_STATE_5;
                    startSound(SE_BLasBos_VO_DamageChance);
                    break;
                }

                if (mChanceAttackCounter >= 3) {
                    mStateMgr.changeState(StateID_SitDamage);
                    mChanceAttackCounter = 0;
                } else {
                    mStateMgr.changeState(StateID_Damage);
                }

                mNumConsecutiveSameDirectionAttacks = -1;
                mSecondLastAttackDirection = mLastAttackDirection;
                v.x = 0.0f;
                v.y = 0.0f;
                v.z = -20.0f;
                mtx.YrotS(mYAngleToLink);
                MTXMultVec(mtx, v, v2);
                field_0x119C = v2.x;
                field_0x11A0 = v2.z;
                field_0x1138 = 10;
                field_0x11B8 = 5.0f;
                field_0x11A4 = 7.0f;
                if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_LEFT ||
                    mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT ||
                    mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT) {
                    field_0x1137 = 1;
                } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_RIGHT ||
                           mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT ||
                           mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT) {
                    field_0x1137 = 2;
                } else if (mLastAttackDirection == daPlayerActBase_c::ATTACK_DIRECTION_STAB) {
                    field_0x1137 = 3;
                } else {
                    field_0x1137 = 0;
                }
                break;
            }
        }
    }

    return true;
}

void dAcBlastboss_c::setAnm(const char *name, f32 blend) {
    if (mpCurrentAnm != name) {
        mMdl.setAnm(name, m3d::PLAY_MODE_4, blend);
        mpCurrentAnm = name;
    }
}

void dAcBlastboss_c::forceSetAnm(const char *name, f32 blend) {
    mMdl.setAnm(name, m3d::PLAY_MODE_4, blend);
    mpCurrentAnm = name;
}

void dAcBlastboss_c::setAnmRate(f32 rate) {
    mMdl.setRate(rate);
}

int dAcBlastboss_c::draw() {
    drawModelType1(&mMdl.getModel());
    mSwordMdl.entry(this, nullptr, nullptr);
    if (field_0x114C) {
        drawModelType1(&mThunderMdl);
    }
    return SUCCEEDED;
}

bool dAcBlastboss_c::checkForCloseRangeAttack() {
    dAcPy_c *link = dAcPy_c::GetLinkM();

    bool ret = false;
    if (mXZDistanceToLink < 500.0f) {
        if (field_0x1149 != 0) {
            if (mTimers[TIMER_2] == 0) {
                mStateMgr.changeState(StateID_Attack);
                ret = true;
            }
        } else {
            if (link->isUsingShield()) {
                mLinkCloseRangeShieldTime++;
            } else {
                mLinkCloseRangeShieldTime = 0;
            }
            if (mLinkCloseRangeShieldTime > 40) {
                mStateMgr.changeState(StateID_PunchAttack);
                ret = true;
            } else if (mTimers[TIMER_2] == 0) {
                if (checkForLinkSwordBySwordHit() && cM::rnd() < 0.35f) {
                    mStateMgr.changeState(StateID_SmallAttack);
                    ret = true;
                } else {
                    mStateMgr.changeState(StateID_Attack);
                    ret = true;
                }
                mLinkCloseRangeShieldTime = 0;
            }
            // ret = true;
        }

    } else {
        mLinkCloseRangeShieldTime = 0;
    }

    return ret;
}

bool dAcBlastboss_c::checkForRangeAttack() {
    dAcPy_c *link = dAcPy_c::GetLinkM();

    bool ret = false;

    if (field_0x1149 != 0 && !link->isRecovering() && mStunCounter != 0 && link->getSwordPos().y > 270.0f) {
        field_0x1162++;
    } else {
        field_0x1162 = 0;
    }

    if (field_0x1149 != 0) {
        if (field_0x1162 >= 20) {
            mStateMgr.changeState(StateID_DashAttack);
            return true;
        } else if (mIsSwordEmpowered) {
            if ((link->checkSwordAndMoreStates(0x400000) && mStunCounter != 0) ||
                (mXZDistanceToLink < 1500.0f && mTimers[TIMER_3] == 0)) {
                mStateMgr.changeState(StateID_ThunderAttack);
                ret = true;
            }
        } else {
            if (link->checkSwordAndMoreStates(0x400000)) {
                if ((mCounter & 0x7) == 0 && mStunCounter != 0) {
                    mStateMgr.changeState(StateID_DashAttack);
                    ret = true;
                }
            } else {
                if (mXZDistanceToLink < 1500.0f && dLightEnv_c::GetPInstance()->getField_0x38DC() == 6) {
                    if (field_0x1162 > 10) {
                        mStateMgr.changeState(StateID_DashAttack);
                        ret = true;
                    } else {
                        mStateMgr.changeState(StateID_ThunderWait);
                        ret = true;
                    }
                }
            }
        }
    }

    return ret;
}

bool dAcBlastboss_c::checkForCounter() {
    bool ret = false;

    if (field_0x113E != 0) {
        return false;
    }

    f32 attackRange = 450.0f;
    if (dAcPy_c::GetLinkM()->isAttackingJumpSlash()) {
        attackRange = 750.0f;
    }

    if (mXZDistanceToLink < attackRange) {
        dAcPy_c *link = dAcPy_c::GetLinkM();
        if (link->checkFlags0x354(0x04)) {
            if (link->isAttackingSpin()) {
                field_0x11A4 = 15.0f;
            } else if (link->isAttacking()) {
                field_0x11A4 = 7.0f;
            }

            s32 attackDir = link->getSpecificAttackDirection();

            if (mStateMgr.isState(StateID_Attack)) {
                if (mpCurrentAnm == "AttackR" &&
                    (attackDir == dAcPy_c::ATTACK_DIRECTION_RIGHT || attackDir == dAcPy_c::ATTACK_DIRECTION_UPRIGHT ||
                     attackDir == dAcPy_c::ATTACK_DIRECTION_DOWNRIGHT)) {
                    mGuardDirection = GUARD_RIGHT;
                } else if (mpCurrentAnm == "AttackL" && (attackDir == dAcPy_c::ATTACK_DIRECTION_LEFT ||
                                                         attackDir == dAcPy_c::ATTACK_DIRECTION_UPLEFT ||
                                                         attackDir == dAcPy_c::ATTACK_DIRECTION_DOWNLEFT)) {
                    mGuardDirection = GUARD_LEFT;
                } else if (mpCurrentAnm == "AttackU" && attackDir == daPlayerActBase_c::ATTACK_DIRECTION_DOWN) {
                    mGuardDirection = GUARD_UP;
                } else {
                    return false;
                }

                mStateMgr.changeState(StateID_Guard);
                mSubState = SUB_STATE_10;
            } else {
                if ((field_0x1149 > 0 || mXZDistanceToLink < 200.0f) && mNumConsecutiveOppositeDirectionAttacks < 1) {
                    mNumConsecutiveOppositeDirectionAttacks = 1;
                }

                switch (attackDir) {
                    case daPlayerActBase_c::ATTACK_DIRECTION_DOWN:      mGuardDirection = GUARD_UP; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_LEFT:      mGuardDirection = GUARD_LEFT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT:  mGuardDirection = GUARD_UPLEFT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT:    mGuardDirection = GUARD_DOWNLEFT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_RIGHT:     mGuardDirection = GUARD_RIGHT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT: mGuardDirection = GUARD_UPRIGHT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT:   mGuardDirection = GUARD_DOWNRIGHT; break;
                    case daPlayerActBase_c::ATTACK_DIRECTION_UP:        mGuardDirection = GUARD_DOWN; break;
                    default:                                            mGuardDirection = GUARD_CENTER; break;
                }
                if (!mStateMgr.isState(StateID_Guard)) {
                    mStateMgr.changeState(StateID_Guard);
                }

                mSubState = SUB_STATE_0;
            }
            ret = true;
        }
    }
    return ret;
}

void dAcBlastboss_c::updateMainNodeTransforms() {
    if (field_0x1131 != 0) {
        s16 ang = 0x800;
        if (field_0x1131 == 2) {
            ang = 0x300;
        }
        dAcPy_c *link = dAcPy_c::GetLinkM();
        mVec3_c attentionTarget = link->getPosition();
        attentionTarget.y += 250.0f;
        if (link->isUsingBugnet() && mXZDistanceToLink < 400.0f) {
            field_0x117C = 5;
            mBugNetPos = link->getBugNetPos();
        }

        s16 diff;
        if (field_0x117C != 0) {
            attentionTarget = mBugNetPos;
            mAng targetAngle = cLib::targetAngleY(mPosition, attentionTarget);
            diff = targetAngle - mRotation.y;
        } else {
            diff = mYAngleToLink - mRotation.y;
        }
        if (diff > 0x1800) {
            diff = 0x1800;
        } else if (diff < -0x1800) {
            diff = -0x1800;
        }
        sLib::addCalcAngle(mMdlCallback.field_0x06.ref(), -diff, 2, ang);

        s16 diff2 = cLib::targetAngleX(attentionTarget, mPositionCopy2);
        if (diff2 > 0x1000) {
            diff2 = 0x1000;
        } else if (diff2 < -0x1000) {
            diff2 = -0x1000;
        }
        sLib::addCalcAngle(mMdlCallback.field_0x0A.ref(), diff2, 2, ang);

        if ((mCounter & 0xF) == 0 && cM::rnd() < 0.5f) {
            field_0x1170 = cM::rndFX(2000.0f);
        }
        sLib::addCalcAngle(mMdlCallback.field_0x0C.ref(), field_0x1170, 8, 0x200);
    } else {
        mMdlCallback.field_0x06 = mMdlCallback.field_0x0A = mMdlCallback.field_0x0C = 0;
        if (mDemoState == DEMO_STATE_101) {
            mMdlCallback.field_0x0A = 2000;
        }
    }

    if (field_0x1178 != 0) {
        field_0x1178--;
        s16 target = mAng(field_0x1178 * 0xE00).sin() * 1500.0f;
        sLib::addCalcAngle(mMdlCallback.field_0x08.ref(), target, 2, 200);
    } else {
        sLib::addCalcAngle(mMdlCallback.field_0x08.ref(), 0, 2, 100);
    }

    s16 t3 = 0;
    if (field_0x1182 != 0) {
        field_0x1182--;
        t3 = 4000;
    }
    sLib::addCalcAngle(mMdlCallback.field_0x12.ref(), t3, 4, 200);
    sLib::addCalcAngle(mMdlCallback.field_0x14.ref(), field_0x1172, 2, 0x200);
    sLib::addCalcAngle(mMdlCallback.field_0x16.ref(), field_0x1174, 2, 0x200);

    field_0x1172 = 0;
    field_0x1174 = 0;

    if (field_0x1138 != 0) {
        field_0x1138--;

        field_0x117A = 2000;
        s16 t4 = 0, t5 = 0;
        if (field_0x1137 == 0) {
            field_0x117A = 3000;
            t5 = 5000;
        } else if (field_0x1137 == 1) {
            t5 = 3000;
            t4 = 3000;
        } else if (field_0x1137 == 2) {
            t5 = 3000;
            t4 = -3000;
        } else if (field_0x1137 == 3) {
            field_0x117A = 3000;
            t5 = -5000;
        }

        sLib::addCalcAngle(mMdlCallback.field_0x0E.ref(), t4, 2, field_0x117A);
        sLib::addCalcAngle(mMdlCallback.field_0x10.ref(), t5, 2, field_0x117A);
    } else if (field_0x1188 != 0) {
        field_0x1188--;
        s16 target = mAng(mCounter * 0x6510).sin() * 300.0f;
        mMdlCallback.field_0x2E = target;
    } else {
        sLib::addCalcAngle(mMdlCallback.field_0x0E.ref(), 0, 4, field_0x117A / 4);
        sLib::addCalcAngle(mMdlCallback.field_0x10.ref(), 0, 4, field_0x117A / 4);
    }

    if (field_0x1133 == 0) {
        sLib::addCalcScaled(&mMdlCallback.field_0x30, 0.5f, 0.5f);
        sLib::addCalcAngle(mMdlCallback.field_0x18.ref(), 0, 4, 400);
        sLib::addCalcAngle(mMdlCallback.field_0x1A.ref(), 0, 4, 400);
    }
}

void dAcBlastboss_c::updateSkirtHairTransforms() {
    static const u16 sNodeIds[] = {
        B_LAST_BOSS_NODE_skirtAU1, B_LAST_BOSS_NODE_skirtBU1, B_LAST_BOSS_NODE_skirtCU1, B_LAST_BOSS_NODE_skirtDU1,
        B_LAST_BOSS_NODE_skirtEU1, B_LAST_BOSS_NODE_skirtFU1, B_LAST_BOSS_NODE_skirtGU1, B_LAST_BOSS_NODE_skirtHU1,
    };

    static const s16 sAngs[] = {
        0x2000, 0x4000, 0x6000, 0x8000, 0xA000, 0xC000, 0xE000,
    };

    f32 f = 80.0f;

    if (mpCurrentAnm == "AttackU") {
        if (mMdl.getAnm().getFrame() >= 18.0f && mMdl.getAnm().getFrame() <= 40.0f) {
            f = 150.0f;
        }
    } else if (mpCurrentAnm == "Counter") {
        if (mMdl.getAnm().getFrame() >= 17.0f && mMdl.getAnm().getFrame() <= 68.0f) {
            f = 120.0f;
        }
    } else if (mpCurrentAnm == "TurnR") {
        if (mMdl.getAnm().getFrame() >= 19.0f && mMdl.getAnm().getFrame() <= 30.0f) {
            f = 100.0f;
        }
    } else if (mpCurrentAnm == "AttackJumpEnd") {
        if (mMdl.getAnm().getFrame() <= 18.0f) {
            f = 120.0f;
        }
    } else if (mpCurrentAnm == "AttackJumpStart" || mpCurrentAnm == "AttackJumpLoop") {
        f = 120.0f;
    } else if (mpCurrentAnm == "DownGetUp") {
        // same value
        f = 80.0f;
    }
    sLib::addCalcScaledDiff(&field_0x11B4, f, 0.5f, 20.0f);

    mMtx_c mtx;
    mVec3_c v;
    mVec3_c t(0.0f, 0.0f, 12.0f);
    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x1268); i++) {
        field_0x1268[i].field_0x002++;
        field_0x1268[i].field_0x004 = sAngs[i] + mRotation.y;
        mMdl.getModel().getNodeWorldMtx(sNodeIds[i], mtx);
        mtx.getTranslation(v);
        mtx.YrotS(field_0x1268[i].field_0x004);
        MTXMultVec(mtx, t, field_0x1268[i].field_0x014[0]);
        field_0x1268[i].field_0x014[0] += v;
        if (nw4r::math::FAbs(field_0x1268[i].field_0x010) < 1.0f) {
            // TODO - regswaps between tmp and abs result
            f32 tmp = field_0x1268[(i + 1) & 7].field_0x010;
            if (nw4r::math::FAbs(tmp) > 1.0f) {
                field_0x1268[i].field_0x010 = tmp * 0.5f;
            }
            tmp = field_0x1268[(i - 1) & 7].field_0x010;
            if (nw4r::math::FAbs(tmp) > 1.0f) {
                field_0x1268[i].field_0x010 = tmp * 0.5f;
            }
        }

        f32 f4 = field_0x1268[i].field_0x010 + field_0x11A4;
        if (nw4r::math::FAbs(f4) > 0.1f) {
            if (field_0x11A4 > 0.1f) {
                field_0x1268[i].field_0x006 = mRotation.y + 0x8000 + moreAngs[i];
            } else if (field_0x11A4 < -0.1f) {
                field_0x1268[i].field_0x006 = mRotation.y + 0x8000 - moreAngs[i];
            }

            if (nw4r::math::FAbs(field_0x11A4) > 25.0f) {
                field_0x1268[i].field_0x00A = 8000;
            } else {
                field_0x1268[i].field_0x00A = 5000;
            }

            if (i >= 2 && i <= 6) {
                field_0x1268[i].field_0x00C = f4;
            } else {
                field_0x1268[i].field_0x00C = 0.75f * f4;
            }
            field_0x11B0 = 0.0f;
        } else {
            sLib::addCalcScaled(&field_0x1268[i].field_0x00C, 0.05f, 2.0f);
            sLib::addCalcAngle(&field_0x1268[i].field_0x00A, 1000, 4, 200);
        }
        field_0x1268[i].field_0x008 += field_0x1268[i].field_0x00A;
        updateSkirtTransform(i);
    }

    mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_head, mtx);
    mtx.fn_802F1C40(0, 1);
    mAng3_c ang;
    mtx.toRot(ang);
    ang.y += 0x4000;
    ang.x += 0;
    ang.z += 6500;
    t.set(-20.0f, -20.0f, 0.0f);
    for (int i = 0; i < 1; i++) {
        field_0x2948[i].field_0x002 = ang.y;
        field_0x2948[i].field_0x004 = -ang.z;
        field_0x2948[i].field_0x006 = -ang.x;
        mMdl.getModel().getNodeWorldMtx(B_LAST_BOSS_NODE_head, mtx);
        MTXMultVec(mtx, t, field_0x2948[i].field_0x008[0]);
        updateHairTransform(i);
    }

    if (field_0x1141 == 0) {
        for (int i = 0; i < 9; i++) {
            f32 f = (i % 3) * 3 + 1;
            if (i < 3) {
                mMdlCallback.field_0x1C[i] = (s16)(f * 500.0f * mAng(mCounter * 0x2600 - (i * 24000)).sin());
            } else if (i < 6) {
                mMdlCallback.field_0x1C[i] = -(s16)(f * 500.0f * mAng(mCounter * 0x2800 - (i * 25000)).sin());
            } else {
                mMdlCallback.field_0x1C[i] = -(s16)(f * 500.0f * mAng(mCounter * 0x2A00 - (i * 26000)).sin());
            }
        }
    }

    if (field_0x114E != 0) {
        holdSound(SE_BLasBos_SkirtAir);
    }
}

void dAcBlastboss_c::updateSkirtTransform(s32 level) {
    // NONMATCHING but probably equivalent

    UnkLastBossCcSph2 &cc = field_0x1268[level];

    mVec3_c t1(0.0f, 0.0f, 33.25f);

    s32 offset1 = 0;

    mVec3_c d1;

    d1 = mToeTranslation[0] - mBackbone1Translation;

    mMtx_c mtx;
    mtx.YrotS(-mRotation.y);
    MTXMultVec(mtx, d1, d1);
    if (d1.z > 0.0f) {
        offset1 = -1;
    }

    d1 = mToeTranslation[1] - mBackbone1Translation;
    MTXMultVec(mtx, d1, d1);
    if (d1.z > 20.0f) {
        offset1 = 1;
    }

    // hmm yes
    mVec3_c p3;
    mVec3_c v4;
    mVec3_c p2;
    mVec3_c v5;
    mVec3_c p1;

    p1.set(0.0f, 0.0f, 0.0f);
    p2.set(0.0f, 0.0f, 0.0f);
    bool isAttackJumpStart = false;
    if (mpCurrentAnm == "AttackJumpStart") {
        isAttackJumpStart = true;
    }
    f32 f;
    if (level == 7 && offset1 > 0) {
        f = 10.0f;
    } else if (level >= 0 && level <= 1 && offset1 < 0) {
        f = 50.0f;
    } else {
        f = -3.0f;
    }
    if (level >= 0 && level < 2 && isAttackJumpStart) {
        p1.y = 500.0f;
        f = 50.0f;
    }

    sLib::addCalcScaledDiff(&field_0x11D0, f, 1.0f, 2.0f);
    p1.z = field_0x11D0;

    p3.set(0.0f, 0.0f, 2.0f);
    mtx.YrotS(cc.field_0x004);
    MTXMultVec(mtx, p3, v4);
    MTXMultVec(mtx, p1, v5);
    mtx.YrotS(mRotation.y);
    MTXMultVec(mtx, p2, p2);

    f32 fAdd = -15.0f;
    f32 f1 = cc.field_0x00C;
    sLib::addCalcScaledDiff(&field_0x11AC, field_0x11B0, 1.0f, 0.02f);

    s32 offset2 = 0;
    if (offset1 < 0 && (level == 0) || level == 1 || level == 2 || level == 7) {
        offset2 = 1;
    }

    f32 f2 = field_0x11B4;
    u32 u12 = (cc.field_0x002 & 1) + 1;
    if (level >= 2 && level <= 6) {
        u12 += 2;
        cc.mCc.SetR(30.0f);
    } else {
        cc.mCc.SetR(50.0f);
    }

    if (field_0x1141 != 0) {
        offset2 = 0;
        v4.set(0.0f, 0.0f, 0.0f);
        p2.set(0.0f, 0.0f, 0.0f);
    }

    f1 *= 0.75f;

    static const f32 sFloats[] = {
        15.0f, 15.0f, 15.0f, 15.0f, 13.0f, 11.0f, 9.0f, 7.0f, 5.0f,
    };

    s32 actual = offset2 - 1;

    for (u32 i = 1; i < 10; i++) {
        // TODO: These strength reduced and moved to before the loop.
        // However, we need them at the start of the function!
        mVec3_c *v1 = &cc.field_0x014[i];
        mVec3_c *v2 = &cc.field_0x08C[i] - 1;
        mVec3_c *v3 = &cc.field_0x104[i];

        mtx.YrotS(cc.field_0x006);
        d1.x = 0.0f;
        d1.y = mAng(cc.field_0x008 - i * 8000).sin() * f1;
        d1.z = f1;
        mVec3_c v6;
        MTXMultVec(mtx, d1, v6);
        s32 zero = 0;
        f1 = f1 * (zero * 0.01f + 1.1f);

        f32 fY = fAdd + v1[0].y + v4.y + v3[0].y + v6.y + v5.y;
        if (i >= 1) {
            v5.y = 0.0f;
        } else {
            // @bug unreachable
            v5.y *= 0.5f;
        }

        f32 f4;
        if (field_0x1141 != 0) {
            f4 = 5.0f;
        } else {
            f4 = sFloats[i - 1];
        }

        if (fY <= f4) {
            fY = f4;
        }

        f32 fX = v1[0].x + v4.x + p2.x + v3[0].x + v6.x + v5.x;
        f32 fZ = v1[0].z + v4.z + p2.z + v3[0].z + v6.z + v5.z;
        if (offset2 != 0) {
            f32 dX = fX - field_0x1250[actual].x;
            f32 dZ = fZ - field_0x1250[actual].z;
            f32 f11 = nw4r::math::FSqrt(dX * dX + dZ * dZ);
            if (f11 < f2) {
                mtx.YrotS(cM::atan2s(dX, dZ));
                d1.set(0.0f, 0.0f, f2);
                MTXMultVec(mtx, d1, d1);
                fX = d1.x + field_0x1250[actual].x;
                fZ = d1.z + field_0x1250[actual].z;
            }
        }

        f32 fX2 = fX - v1[-1].x;
        f32 fY2 = fY - v1[-1].y;
        f32 fZ2 = fZ - v1[-1].z;
        v5.x *= 0.4f;
        v5.z *= 0.4f;
        v2[0].x = -cM::atan2s(fY2, fZ2);
        f32 f8 = nw4r::math::FSqrt(fY2 * fY2 + fZ2 * fZ2);

        v2[0].y = cM::atan2s(fX2, f8);
        v2[0].z = mRotation.y;

        mtx.XrotS(v2[0].x);
        mtx.YrotM(v2[0].y);
        MTXMultVec(mtx, t1, d1);
        v3[0] = v1[0];
        v1[0] = v1[-1] + d1;
        v3[0] = (v1[0] - v3[0]) * field_0x11AC;
        if (i == u12) {
            cc.field_0x010 = 0.0f;
            cc.mCc.SetC(v1[0]);
            if (cc.mCc.ChkTgHit()) {
                cc.field_0x006 = moreAngs[level] + dAcPy_c::GetLinkM()->mRotation.y;
                cc.field_0x010 = 25.0f;
                field_0x114E = 1;
            }
        }
    }
}

void dAcBlastboss_c::updateHairTransform(s32 level) {
    // Might hold off with this one until updateSkirtTransform is fixed

    // floats
    47.5f;
    0.9f;
    1.6f;
}

bool dAcBlastboss_c::checkForLinkSwordBySwordHit() {
    dAcPy_c *link = dAcPy_c::GetLinkM();

    bool ret = !link->isUsingShield();
    if (ret) {
        ret = link->isUsingSword();
    }
    if (ret) {
        ret = link->getSwordPos().y > 100.0f;
    }
    if (ret) {
        ret = link->getSwordPos().y < 230.0f;
    }
    if (ret) {
        ret = (mPosition - link->getSwordPos()).absXZ() < 260.0f;
    }

    return ret;
}

u8 dAcBlastboss_c::classifyAttackDirection(s32 attackDir) {
    switch (attackDir) {
        case daPlayerActBase_c::ATTACK_DIRECTION_LEFT:
        case daPlayerActBase_c::ATTACK_DIRECTION_DOWNLEFT:
        case daPlayerActBase_c::ATTACK_DIRECTION_UPLEFT:    return 0;

        case daPlayerActBase_c::ATTACK_DIRECTION_RIGHT:
        case daPlayerActBase_c::ATTACK_DIRECTION_DOWNRIGHT:
        case daPlayerActBase_c::ATTACK_DIRECTION_UPRIGHT:   return 1;

        case daPlayerActBase_c::ATTACK_DIRECTION_DOWN:      return 2;
        case daPlayerActBase_c::ATTACK_DIRECTION_UP:        return 3;

        case daPlayerActBase_c::ATTACK_DIRECTION_STAB:      return 4;

        default:                                            return -1;
    }
}

void dAcBlastboss_c::executeDemo() {
    dCamera_c *cam = dScGame_c::getCamera();
    dAcPy_c *link = dAcPy_c::GetLinkM();

    mMtx_c mtx;
    mMtx_c mtx1;
    mVec3_c v;
    mVec3_c effPos;

    bool b1 = false;
    bool b2 = false;
    bool spawnEffect = false;
    dLightEnv_c &lightEnv = dLightEnv_c::GetInstance();

    switch (mDemoState) {
        case DEMO_STATE_1: {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event("BLS_demo2", 100, f2 & ~f1, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &e, nullptr);
            }
            if (EventManager::isInEvent(this, "BLS_demo2")) {
                field_0x2CEE = 0;
                field_0x2D00 = 0.0f;
                field_0x2CFC = 60.0f;
                mDemoState = DEMO_STATE_2;
                // huh
                field_0x2D10.x = cam->getField_0x78().x;
                field_0x2D10.y = cam->getField_0x78().y;
                field_0x2D10.z = cam->getField_0x78().z;
                field_0x2D04.x = cam->getPositionMaybe().x;
                field_0x2D04.y = cam->getPositionMaybe().y;
                field_0x2D04.z = cam->getPositionMaybe().z;
                field_0x1149 = -1;
                // fall-through
            } else {
                break;
            }
        }
        case DEMO_STATE_2: {
            if (field_0x2CEE < 30) {
                b2 = true;
            }
            mtx.YrotS((s16)field_0x2CF4 + mRotation.y);
            v.set(0.0f, 100.0f, 400.0f);
            MTXMultVec(mtx, v, v);
            v += mPosition;
            cLib::addCalcPos2(&field_0x2D04, v, 0.1f, 30.0f);
            if (field_0x2CEE > 5) {
                sLib::addCalcScaledDiff(&field_0x2D00, 1.0f, 1.0f, 0.1f);
                sLib::addCalcScaledDiff(&field_0x2CF4, 6000.0f, 0.05f, field_0x2D00 * 100.0f);
            }
            // fall-through
        }
        case DEMO_STATE_3: {
            b1 = true;
            if (field_0x2CEE > 205) {
                sLib::addCalcScaledDiff(&field_0x2D00, 1.0f, 1.0f, 0.1f);
                if (field_0x2CEE >= 290) {
                    if (field_0x2CEE == 290) {
                        field_0x2D00 = 0.0f;
                    }
                    sLib::addCalcScaledDiff(&field_0x2CFC, 50.0f, 0.05f, field_0x2D00);
                    sLib::addCalcScaledDiff(&field_0x2D10.y, 290.0f, 0.1f, field_0x2D00 * 5.0f);
                } else {
                    sLib::addCalcScaledDiff(&field_0x2CFC, 70.0f, 0.05f, field_0x2D00);
                    sLib::addCalcScaledDiff(&field_0x2D10.y, 420.0f, 0.1f, field_0x2D00 * 100.0f);
                    if (field_0x2CEE == 0xFA) {
                        dLightEnv_c::GetPInstance()->setField_0x38DC(7);
                        setAnm("CatchThunderEnd", 5.0f);
                        spawnEffect = true;
                        effPos.set(field_0x1208.x, field_0x1208.y + 110.0f, field_0x1208.z);
                        mIsSwordEmpowered = true;
                        mSwordMdl.fn_8006B800(4);
                        mCc1.OnTg_0x8000000();
                        startSound(SE_BLasBos_ThunderFallSword);
                        mSubState = SUB_STATE_21;
                        mTimers[TIMER_3] = cM::rndF(30.0f) + 200.0f;
                        dRumble_c::start(dRumble_c::sRumblePreset4, dRumble_c::FLAG_ACTIVE | dRumble_c::FLAG_SLOT0);
                        field_0x2CF0 = 15;
                    }
                    if (field_0x2CEE >= 200 && field_0x2CEE <= 250) {
                        b2 = true;
                    }
                }
            } else {
                cLib::addCalcPos2(&field_0x2D10, mChestTranslation, 0.1f, 50.0f);
            }
            break;
        }
        case DEMO_STATE_100: {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event("BLS_end", 100, f2 & ~f1, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &e, nullptr);
            }
            if (EventManager::isInEvent(this, "BLS_end")) {
                field_0x2CEE = 0;
                field_0x2D00 = 0.0f;
                field_0x2CFC = 50.0f;
                mDemoState = DEMO_STATE_101;
                link->onFlags_0x360(0x200);
                if (mpSwordBattleGame != nullptr) {
                    mpSwordBattleGame->notifyBossDeath(false);
                }
                StoryflagManager::sInstance->setFlagOrCounterToValue(STORYFLAG_692, 0);
                dStage_c::GetInstance()->getRoom(0)->someLastBossThing(false);
                // fall-through
            } else {
                break;
            }
        }
        case DEMO_STATE_101: {
            b1 = true;
            if (field_0x2CEE < 30) {
                b2 = true;
            }
            mtx.YrotS(mRotation.y - 0x4000);
            v.set(0.0f, 100.0f, 1500.0f);
            MTXMultVec(mtx, v, v);
            field_0x2D04 = mBackbone1Translation + v;
            field_0x2D10.x = mBackbone1Translation.x;
            field_0x2D10.y = 400.0f;
            field_0x2D10.z = mBackbone1Translation.z;
            sLib::addCalcScaledDiff(&field_0x2CFC, 60.0f, 0.035f, 0.1f);
            if (field_0x2CEE == 160) {
                field_0x114C = 1;
            }

            if (field_0x2CEE >= 159) {
                if (field_0x2CEE < 170) {
                    sLib::addCalcScaledDiff(&field_0x11C4, cM::rndF(0.3f) + 0.7f, 1.0f, 0.4f);
                } else {
                    sLib::addCalcScaled(&field_0x11C4, 1.0f, 0.02f);
                }
            } else {
                if (field_0x2CEE < 10) {
                    sLib::addCalcScaledDiff(&field_0x11C4, 0.7f, 1.0f, 0.35f);
                } else {
                    sLib::addCalcScaled(&field_0x11C4, 1.0f, 0.035f);
                }
            }

            if (field_0x2CEE == 230) {
                link->offFlags_0x360(0x200);
            }

            if (field_0x2CEE == 270 && mpSwordBattleGame != nullptr) {
                mDemoState = DEMO_STATE_1000;
            } else {
                lightEnv.setField_0x5D48(field_0x11C4);
                if (field_0x2CEE == 360) {
                    dScGame_c::GetInstance()->triggerExit(mRoomID, 1);
                }
            }
            break;
        }
        case DEMO_STATE_200: {
            {
                // this works but I don't like it
                static volatile u32 FLAGS_1 = 0x00000001;
                static u32 FLAGS_2 = 0x00100001;
                u32 f1 = FLAGS_1;
                u32 f2 = FLAGS_2;
                Event e = Event("BLS_start", 100, f2 & ~f1, nullptr, nullptr);
                EventManager::alsoSetAsCurrentEvent(this, &e, nullptr);
            }
            if (EventManager::isInEvent(this, "BLS_start")) {
                field_0x2CEE = 0;
                field_0x2D00 = 0.0f;
                field_0x2CFC = 60.0f;
                mDemoState = DEMO_STATE_201;
                field_0x2D10.set(0.0f, 200.0f, -587.0f);
                field_0x2D04.set(0.0f, 197.0f, -207.0f);
                dAcPy_c::GetLinkM()->vt_0x2AC();
                dLytBossCaption_c::GetInstance()->show("BOSS_11_caption", 0);
                // fall-through
            } else {
                break;
            }
        }
        case DEMO_STATE_201: {
            b1 = true;
            sLib::addCalcScaledDiff(&field_0x2D00, 1.0f, 1.0f, 0.05f);
            cLib::addCalcPos2(&field_0x2D10, mVec3_c(-2.6f, 118.0f, 298.0f), 0.1f, field_0x2D00 * 80.0f);
            cLib::addCalcPos2(&field_0x2D04, mVec3_c(-132.0f, 89.0f, 654.0f), 0.1f, field_0x2D00 * 80.0f);

            if (field_0x2CEE >= 20) {
                sLib::addCalcScaledDiff(&field_0x2CFC, 40.0f, 0.05f, 0.1f);
            }

            if (field_0x2CEE == 40) {
                dAcPy_c::GetLinkM()->triggerMoveEventMaybe(4, 0, 0, nullptr, 0, 1, 0);
            }

            if (field_0x2CEE == 110) {
                dLytBossCaption_c::GetInstance()->unk_inline();
            }

            if (field_0x2CEE >= 140) {
                EventManager::finishEvent(this, nullptr);
                dScGame_c::getCamera()->fn_8019EA70(true);
                mDemoState = DEMO_STATE_NONE;
                unsetActorProperty(AC_PROP_0x4);
            }
            break;
        }
        case DEMO_STATE_1000: {
            EventManager::finishEvent(this, nullptr);
            dScGame_c::getCamera()->fn_8019EA70(true);
            mDemoState = DEMO_STATE_NONE;
            break;
        }
    }

    if (spawnEffect) {
        dJEffManager_c::spawnEffect(
            PARTICLE_RESOURCE_ID_MAPPING_624_, effPos, nullptr, nullptr, nullptr, nullptr, 0, 0
        );
        field_0x114A = 10;
    }

    if (b1) {
        field_0x2CEE++;
        mVec3_c tmp1 = field_0x2D10;
        if (field_0x2CF0 != 0) {
            field_0x2CF0--;
            s16 val = field_0x2CF0;
            f32 fac = val * 3.0f;
            tmp1.y += mAng(val * 0x1C20).cos() * fac;
        }
        cam->fn_8019EA00(tmp1, field_0x2D04, field_0x2CFC, 0.0f);
        if (b2) {
            mtx1.transS(0.5f, 0.5f, 0.0f);
            // REGSWAP
            s32 scale = 95;
            mtx1.scaleM(scale / 100.0f, scale / 100.0f, 0.0f);
            mtx1.transM(-0.5f, -0.5f, 0.0f);
            dStageMgr_c::GetInstance()->procfn_800192F0(0x96, mtx1, 0x32);
        }
    }

    if (field_0x114C != 0) {
        mtx.YrotS(cam->fn_8019E3B0());
        v.set(0.0f, 10.0f, 10000.0f);
        MTXMultVec(mtx, v, v);
        v.x += cam->getPositionMaybe().x;
        v.z += cam->getPositionMaybe().z;
        mtx.transS(v);
        v.set(0.7f, 0.7f, 0.7f);
        mThunderMdl.setScale(v);
        mThunderMdl.setLocalMtx(mtx);
        mThunderAnmMatClr.play();
    }
}

void dAcBlastboss_c::setLightningTimerMaybe(s32 arg) {
    f32 f = arg + cM::rndF(arg * 0.5f);

    if (mStunCounter >= 2) {
        f *= 0.5f;
    } else if (mStunCounter == 1) {
        f *= 0.75f;
    }

    mLightningTimerMaybe = f;
}
