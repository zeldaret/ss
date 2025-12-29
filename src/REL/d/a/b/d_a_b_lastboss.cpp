#include "d/a/b/d_a_b_lastboss.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_base.h"
#include "d/d_light_env.h"
#include "d/d_rumble.h"
#include "d/d_sc_game.h"
#include "d/d_stage_mgr.h"
#include "d/d_vec.h"
#include "d/flag/sceneflag_manager.h"
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
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
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

    for (int i = 0; i < (int)ARRAY_LENGTH(field_0x1260); i++) {
        field_0x1260[i].field_0x000 = i;
        field_0x1260[i].field_0x002 = cM::rndF(65536.0f);
        mCcList.addCc(field_0x1260[i].mCc, sSrcSph);
        for (int j = 0; j < (int)ARRAY_LENGTH(field_0x1260[j].field_0x014); j++) {
            field_0x1260[i].field_0x014[j] = mPosition;
        }
        field_0x1260[i].field_0x008 = i * 15000;
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
    field_0x1164 = cM::rndF(65536.0f);

    mHealth = 400;
    mPositionCopy3 = mPosition;

    field_0x11D4 = field_0x11D8 = -1;

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
    fn_143_A110(100);

    if (!field_0x114D) {
        field_0x2CEC = 200;
        setActorProperty(AC_PROP_0x4);
    }

    return SUCCEEDED;
}

// TODO
extern "C" const s16 notsure[] = {0xA000, 0xC000, 0xE000, 0xF000, 0x0000, 0x1000, 0x2000, 0x4000};

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

    field_0x1164++;

    field_0x1133 = 0;
    field_0x1131 = 0;
    field_0x1141 = 0;
    field_0x114E = 0;

    field_0x11A4 = 0.0f;
    mLightInfo.mScale = 0.0f;
    field_0x11B0 = 0.7f;

    for (int i = 0; i < 6; i++) {
        if (field_0x1156[i] != 0) {
            field_0x1156[i]--;
        }
    }

    if (mMdlCallback.field_0x04 != 0) {
        mMdlCallback.field_0x04--;
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
    field_0x116A = cLib::targetAngleY(mPosition, linkPos);
    field_0x1190 = link->getPosition().absXZTo(mPosition);

    field_0x1186 = field_0x116A - mRotation.y;
    mSwordMdl.setAttackActive(false);

    mCc4.ClrAtSet();
    mCc5.ClrAtSet();

    fn_143_6720();
    if (mpSwordBattleGame == nullptr || mpSwordBattleGame->checkFightStarted() == true) {
        mStateMgr.executeState();
    }

    mRotation.x = mAngle.x;
    mRotation.z = mAngle.z;
    sLib::addCalcAngle(&mRotation.y.mVal, mAngle.y, 2, 0x800);

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

    fn_143_7B00();

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
    mMtx_c nodeMtx;
    mMdl.getModel().getNodeWorldMtx(resMdl.GetResNode("loc_sword01").GetID(), nodeMtx);
    mMtx_c rotMtx;
    mMtx_c scaleMtx;
    scaleMtx.scaleS(1.05f, 1.05f, 1.05f);
    MTXConcat(nodeMtx, scaleMtx, nodeMtx);

    mSwordMdl.calc(nodeMtx, mRotation.y, false);

    vEff.set(0.0f, -120.0f, 0.0f);
    // TODO number
    if ((s32)field_0x1144 == 9) {
        vEff.y = -80.0f;
        if (field_0x1190 > 300.0f) {
            vEff.y -= (field_0x1190 - 300.0f);
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
        // TODO number
        if ((s32)field_0x1144 == 9) {
            tmpCc.x = 0.0f;
            tmpCc.y = 0.0f;
            tmpCc.z = 100.0f;
            if (field_0x1190 > 300.0f) {
                tmpCc.z = (field_0x1190 - 300.0f) + 100.0f;
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

    if (link->isAttacking() && field_0x1190 < 400.0f) {
        tmpCc.y += 100.0f;
        mCc1.SetH(100.0f);
    } else {
        mCc1.SetH(250.0f);
    }
    mCc1.SetC(tmpCc);

    // TODO - ...
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

    // TODO
    field_0x1250[0] = mToeTranslation[0] + (mToeTranslation[1] - mToeTranslation[0]) * 0.5f;
    // UB: this should use string comparison functions for robustness, but
    // probably works fine in MWCC
    if ((field_0x2D1C == "CatchThunderEnd" || field_0x2D1C == "Stan") && mMdl.getAnm().getFrame() <= 3.0f) {
        field_0x11A4 = 25.0f;
    }

    fn_143_7F80();

    if (!link->isAttacking()) {
        field_0x1143 = 1;
    }

    if (field_0x1143 == 0) {
        mCc1.SetTg_0x4B(1);
        // TODO
        mCc1.mTg.field_0x6C = field_0x1208;
    } else {
        field_0x1143--;
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
    if (field_0x2D1C == "Walk" || field_0x2D1C == "WalkBt") {
        if (mMdl.getAnm().checkFrame(22.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(48.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "TurnR") {
        if (mMdl.getAnm().checkFrame(31.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(19.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "TurnL") {
        if (mMdl.getAnm().checkFrame(31.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(19.0f)) {
            footSplash[0] = true;
        }
    } else if (field_0x2D1C == "AttackBreak") {
        if (mMdl.getAnm().checkFrame(16.0f)) {
            footSplash[0] = true;
        }
    } else if (field_0x2D1C == "AttackL") {
        if (mMdl.getAnm().checkFrame(26.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "AttackR") {
        if (mMdl.getAnm().checkFrame(24.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "AttackU") {
        if (mMdl.getAnm().checkFrame(10.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(24.0f)) {
            footSplash[0] = true;
        }
    } else if (field_0x2D1C == "Counter") {
        if (mMdl.getAnm().checkFrame(21.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(45.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "AttackJumpEnd") {
        if (mMdl.getAnm().checkFrame(6.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "DownGetUp") {
        if (mMdl.getAnm().checkFrame(137.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "DownEscapeL") {
        if (mMdl.getAnm().checkFrame(15.0f)) {
            footSplash[0] = true;
        } else if (mMdl.getAnm().checkFrame(17.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "DownEscapeR") {
        if (mMdl.getAnm().checkFrame(15.0f)) {
            footSplash[1] = true;
        } else if (mMdl.getAnm().checkFrame(17.0f)) {
            footSplash[0] = true;
        }
    } else if (field_0x2D1C == "ThunderDemo") {
        if (mMdl.getAnm().checkFrame(18.0f)) {
            footSplash[1] = true;
        }
    } else if (field_0x2D1C == "AttackThunder") {
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
            if (field_0x1184 == 0) {
                if (dLightEnv_c::GetPInstance()->getField_0x38DC() == 0) {
                    dLightEnv_c::GetPInstance()->setField_0x38DC(1);
                    fn_143_A110(100);
                }
            } else if (!mIsSwordEmpowered || (dScGame_c::getUpdateFrameCount() & 0b11) == 0) {
                field_0x1184--;
            }
        } else {
            fn_143_A110(100);
        }
        sLib::addCalcScaledDiff(&field_0x11CC, 1.0f, 1.0f, 0.1f);
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
    fn_143_9610();

    SceneflagManager *mgr = SceneflagManager::sInstance;
    if (mgr != nullptr) {
        if (mStateMgr.isState(StateID_ThunderWait)) {
            mgr->setFlag(getRoomId(), field_0x1130);
        } else {
            mgr->unsetFlag(getRoomId(), field_0x1130);
        }
    }

    if (field_0x1149) {
        // "Target lock: Demise" (Phase 2)
        mTargetFiTextID = 0x38;
    } else {
        // "Target lock: Demise" (Phase 1)
        mTargetFiTextID = 0x37;
    }

    return SUCCEEDED;
}

void dAcBlastboss_c::callback_c::timingB(u32 nodeId, nw4r::g3d::WorldMtxManip *manip, nw4r::g3d::ResMdl mdl) {}

void dAcBlastboss_c::initializeState_Fight() {}
void dAcBlastboss_c::executeState_Fight() {}
void dAcBlastboss_c::finalizeState_Fight() {}
void dAcBlastboss_c::initializeState_Guard() {}
void dAcBlastboss_c::executeState_Guard() {}
void dAcBlastboss_c::finalizeState_Guard() {}
void dAcBlastboss_c::initializeState_GuardBreak() {}
void dAcBlastboss_c::executeState_GuardBreak() {}
void dAcBlastboss_c::finalizeState_GuardBreak() {}
void dAcBlastboss_c::initializeState_Attack() {}
void dAcBlastboss_c::executeState_Attack() {}
void dAcBlastboss_c::finalizeState_Attack() {}
void dAcBlastboss_c::initializeState_CounterAttack() {}
void dAcBlastboss_c::executeState_CounterAttack() {}
void dAcBlastboss_c::finalizeState_CounterAttack() {}
void dAcBlastboss_c::initializeState_PunchAttack() {}
void dAcBlastboss_c::executeState_PunchAttack() {}
void dAcBlastboss_c::finalizeState_PunchAttack() {}
void dAcBlastboss_c::initializeState_DashAttack() {}
void dAcBlastboss_c::executeState_DashAttack() {}
void dAcBlastboss_c::finalizeState_DashAttack() {}
void dAcBlastboss_c::initializeState_SmallAttack() {}
void dAcBlastboss_c::executeState_SmallAttack() {}
void dAcBlastboss_c::finalizeState_SmallAttack() {}
void dAcBlastboss_c::initializeState_ThunderAttack() {}
void dAcBlastboss_c::executeState_ThunderAttack() {}
void dAcBlastboss_c::finalizeState_ThunderAttack() {}
void dAcBlastboss_c::initializeState_Damage() {}
void dAcBlastboss_c::executeState_Damage() {}
void dAcBlastboss_c::finalizeState_Damage() {}
void dAcBlastboss_c::initializeState_SitDamage() {}
void dAcBlastboss_c::executeState_SitDamage() {}
void dAcBlastboss_c::finalizeState_SitDamage() {}
void dAcBlastboss_c::initializeState_Down() {}
void dAcBlastboss_c::executeState_Down() {}
void dAcBlastboss_c::finalizeState_Down() {}
void dAcBlastboss_c::initializeState_Stun() {}
void dAcBlastboss_c::executeState_Stun() {}
void dAcBlastboss_c::finalizeState_Stun() {}
void dAcBlastboss_c::initializeState_ThunderWait() {}
void dAcBlastboss_c::executeState_ThunderWait() {}
void dAcBlastboss_c::finalizeState_ThunderWait() {}

int dAcBlastboss_c::draw() {
    drawModelType1(&mMdl.getModel());
    mSwordMdl.entry(this, nullptr, nullptr);
    if (field_0x114C) {
        drawModelType1(&mThunderMdl);
    }
    return SUCCEEDED;
}
