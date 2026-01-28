#include "d/a/e/d_a_e_sm.h"

#include "c/c_lib.h"
#include "c/c_math.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_pc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_s_gnd_chk.h"
#include "d/col/bg/d_bg_s_lin_chk.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_camera.h"
#include "d/d_jnt_col.h"
#include "d/d_light_env.h"
#include "d/d_linkage.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_quat.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/time_area_mgr.h"

#include <cmath.h>

SPECIAL_ACTOR_PROFILE(E_SM, dAcEsm_c, fProfile::E_SM, 0xEB, 0, 4098);

static dCcD_SrcSph sSphSrc = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_DAMAGE, 0xD, {0, 0, 0}, 0, 0, 0, 0, 0, 0},
     /* mObjTg */ {~(AT_TYPE_BUGNET | AT_TYPE_BEETLE | AT_TYPE_0x8000 | AT_TYPE_WIND), 0x3303, {0, 3, 0x40F}, 8, 0},
     /* mObjCo */ {0xC5}},
    /* mSphInf */
    {50.f},
};

void dAcEsm_c::fn_187_5D0() {
    if (mStateMgr.isState(StateID_Absorption)) {
        fn_187_6C20(0);
    }
}

bool dAcEsm_c::createHeap() {
    void *rresData = getOarcResFile("Sm");
    TRY_CREATE(mMdl.create3(*this, rresData, "sm", "awa", 0x123));

    nw4r::g3d::ResFile rres(rresData);
    nw4r::g3d::ResMdl mdl(rres.GetResMdl("sm"));
    nw4r::g3d::ResAnmTexPat texPat(rres.GetResAnmTexPat("sm"));

    TRY_CREATE(mAnmTexPat.create(mdl, texPat, &mAllocator, nullptr, 1));
    mMdl.getModel().setAnm(mAnmTexPat);

    nw4r::g3d::ResAnmClr anmClr = rres.GetResAnmClr("attack");
    if (mType == SM_YELLOW) {
        anmClr = rres.GetResAnmClr("Spark");
    }
    TRY_CREATE(mAnmMatClr.create(mdl, anmClr, &mAllocator, nullptr, 1));

    mMdl.getModel().setAnm(mAnmMatClr);
    mScnCallback.attach(mMdl.getModel());

    return true;
}

STATE_DEFINE(dAcEsm_c, BaseMother);
STATE_DEFINE(dAcEsm_c, Wait);
STATE_DEFINE(dAcEsm_c, Walk);
STATE_DEFINE(dAcEsm_c, BirthJump);
STATE_DEFINE(dAcEsm_c, Shake);
STATE_DEFINE(dAcEsm_c, Attack);
STATE_DEFINE(dAcEsm_c, Electrical);
STATE_DEFINE(dAcEsm_c, Absorption);
STATE_DEFINE(dAcEsm_c, Fusion);
STATE_DEFINE(dAcEsm_c, Dead);

bool dAcEsm_c::sSomeArrayInit = false;
bool dAcEsm_c::sSomeArray[9];

int dAcEsm_c::actorCreate() {
    // Check for Batreaux being human and on Skyloft/Waterfall cave
    if (StoryflagManager::sInstance->getCounterOrFlag(STORYFLAG_GRATITUDE_QUEST_FINISHED) &&
        (dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("D000"))) {
        return FAILED;
    }

    mType = GetType();
    field_0xBBF = GetParam_s4_0xF();
    shift8_0xFF = GetParam_s8_0xFF();

    field_0xB74 = GetParam_s16_0xFF();
    field_0xB78 = mRotation.z;
    if (field_0xB74 == 0.f) {
        field_0xB74 = 400.f;
    }
    field_0xBC6 = GetParam_s24_0x7();

    mOrigRotZ = mRotation.z;

    if (field_0xB78 <= 0.f) {
        field_0xB78 = 0.f;
    } else {
        field_0xB78 = 200.f * field_0xB78;
    }

    mStartingPos.set(mPosition.x, mPosition.y, mPosition.z);
    mHomePos1.set(mPosition.x, mPosition.y, mPosition.z);
    mEffPos.set(mPosition.x, mPosition.y, mPosition.z);

    CREATE_ALLOCATOR(dAcEsm_c);

    mRotation.z = mAngle.x = 0;

    mBombRef.unlink();

    setActorProperty(AC_PROP_0x1);
    // Ignore these bounding box sets. `fixBoundBox` completely overwrites them with the scale
    switch ((int)mScale.x) {
        default:
        case SM_SMALL: {
            mBoundingBox.Set(mVec3_c(-250.f, -250.f, -250.f), mVec3_c(250.f, 375.f, 250.f));
            mScale.set(0.8f, 0.8f, 0.8f);
        } break;
        case SM_LARGE: {
            mBoundingBox.Set(mVec3_c(-500.f, -500.f, -500.f), mVec3_c(500.f, 750.f, 500.f));
            mScale.set(0.4f, 0.4f, 0.4f);
        } break;
        case SM_MASSIVE: {
            mBoundingBox.Set(mVec3_c(-800.f, -800.f, -800.f), mVec3_c(800.f, 1200.f, 800.f));
            mScale.set(0.25f, 0.25f, 0.25f);
        } break;
        case SM_TINY: {
            mBoundingBox.Set(mVec3_c(-150.f, -150.f, -150.f), mVec3_c(150.f, 225.f, 150.f));
            mScale.set(1.2f, 1.2f, 1.2f);
        } break;
    }

    updateBoundingBox();

    mScale.CopyTo(mScaleTarget);
    mScale.CopyTo(mScaleCopy2);

    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(mScaleTarget.y * 100.f, mScaleTarget.x * 100.f);
    mStts.SetRank(10);
    mObjAcch.SetGndThinCellingOff();
    mObjAcch.field_0x390 = 1;

    mMaxSpeed = -40.f;

    mSph.Set(sSphSrc);

    mSph.SetStts(mStts);

    mPosition.CopyTo(mPositionCopy2);
    mPosition.CopyTo(mPosCopy1);
    mPosition.CopyTo(mPositionCopy3);
    mPositionCopy3.y += 50.f;

    mRotCopy.set(mRotation);

    field_0xB98 = 0;
    field_0xB58 = 1.f;
    field_0xB90 = -1;

    // This is really weird...
    if (!sSomeArrayInit) {
        sSomeArrayInit = true;
        bool *parr = getArray();
        for (int i = 0; i < 8; i++) {
            *parr++ = false;
        }
        *parr++ = false;
    }

    static Vec data0 = {0.f, 100.f, 0.f};
    static dJntColData_c data1 = {0, 1, 0, 130.f, &data0};
    mJntCol.init(this, &data1, &mMdl.getModel(), 1);

    mLightInfo.SetColor(mColor(0xCC, 0xFF, 0xFF, 0xFF));
    mLightInfo.SetScale(0.f);

    dLightEnv_c::GetPInstance()->plight_set(&mLightInfo);
    if (field_0xBBF == 1) {
        mRotation.x = 0x8000;
        field_0xBCC = 1;
    }

    mRotUnk.clear();

    mEffArr[0].init(this);
    mEffArr[1].init(this);

    if (field_0xBBF == 1) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mStateMgr.changeState(StateID_Walk);
    }

    mHealth = 100;

    if (field_0xBBF != 1) {
        mAcceleration = -3.f;
    }
    calcVelocity();
    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    mObjAcch.CrrPos(*dBgS::GetInstance());
    fn_187_4CC0();

    f32 anim_frame;
    switch (mType) {
        // SM_RED
        default: {
            mTargetFiTextID = 59;
            anim_frame = 0.f;
            mSph.SetAtModifier(AT_MOD_FIRE);
            mType = SM_RED;
        } break;
        case SM_GREEN_ALT:
        case SM_GREEN:     {
            mTargetFiTextID = 62;
            anim_frame = 3.f;
            mType = SM_GREEN;
        } break;
        case SM_YELLOW_ALT:
        case SM_YELLOW:     {
            mTargetFiTextID = 61;
            anim_frame = 2.f;
            mSph.SetAtModifier(AT_MOD_ELECTRIC);
            mType = SM_YELLOW;
        } break;

        case SM_BLUE: {
            mTargetFiTextID = 60;
            anim_frame = 1.f;
        } break;
    }

    if (field_0xBBF != 6) {
        mSplashFx.init(this, 100.f, mScaleTarget.x * 2.f, 0.f);
        mSplashFx.setIsSmall(1);
    }

    mAnmTexPat.setFrame(anim_frame, 0);

    setInteractionFlags(INTERACT_0x40);

    fn_187_44C0();

    mSph.SetAtDamage(4);

    return SUCCEEDED;
}

int dAcEsm_c::actorPostCreate() {
    switch ((u32)field_0xBC6) {
        case 1: {
            mTimeArea.setField0x08(0);
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), getPosition())) {
                mTimeArea.setField0x00(1.f);
                field_0xB8C = 1.f;
                fn_80030700();
            } else {
                mTimeArea.setField0x00(0.f);
                field_0xB8C = 0.f;
                fn_800306d0();
            }
        } break;
        case 2: {
            mTimeArea.setField0x08(1);
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), getPosition())) {
                field_0xB8C = 0.f;
                mTimeArea.setField0x00(0.f);
                fn_800306d0();
            } else {
                field_0xB8C = 1.f;
                mTimeArea.setField0x00(1.f);
                fn_80030700();
            }
        } break;
    }

    if (field_0xBBF == 2 || field_0xBBF == 3) {
        mScale.set(0.1f, 0.1f, 0.1f);
        field_0xBBF = 3;
    }

    if (field_0xBBF == 1 || field_0xBBF == 3) {
        unsetActorProperty(AC_PROP_0x1);
        fn_800306d0();
        setBattleBgmRelated(0);
    }

    return SUCCEEDED;
}

int dAcEsm_c::doDelete() {
    dLightEnv_c::GetPInstance()->plight_cut(&mLightInfo);
    return SUCCEEDED;
}

int dAcEsm_c::actorExecute() {
    mLightInfo.SetScale(0.f);
    if (shift8_0xFF != 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, shift8_0xFF)) {
        return SUCCEEDED;
    }

    if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }

    dAcBomb_c *bomb = mBombRef.get();
    if (mBombRef.isLinked() && bomb) {
        mVec3_c target;
        getPosition().CopyTo(target);
        if (!checkSize(SM_MASSIVE) && !checkSize(SM_LARGE)) {
            target.y += mScaleTarget.y * 60.f;
        }

        cLib::addCalcPos2(&bomb->getPosition(), target, 0.8f, 20.f + getSpeed() * 1.5f);

        field_0xB6C = 0.5f;

        if (field_0xBC5 != 2) {
            mMdl.setAnm("attack", m3d::PLAY_MODE_4, 4.f);
            field_0xBC5 = 2;
        }

        if (0 == sLib::calcTimer(&mTimer_0xBAE)) {
            fn_187_4B50();
        }
    }

    if (!mStateMgr.isState(dAcEsm_c::StateID_Dead)) {
        if (mHealth != 0 && mScaleTarget.x > 0.25f) {
            mHealth = 100;
        }

        if (field_0xBC6 == 1 || field_0xBC6 == 2) {
            int timeCheck = 0;
            if (mHealth != 0) {
                if (field_0xBBF == 3 || field_0xBBF == 1 || mStateMgr.isState(StateID_Absorption) ||
                    mObjAcch.ChkGndHit()) {
                    timeCheck = mTimeArea.check(getRoomId(), getPosition(), 0, 10.f, 0.2f);
                }
            }

            if (timeCheck != 0) {
                if (field_0xBBF == 3) {
                    if (checkInteractionFlags(INTERACT_0x4)) {
                        fn_800306d0();
                    }
                    return SUCCEEDED;
                }

                u16 effectID;
                mVec3_c effScale(1.f, 1.f, 1.f);
                if (checkSize(SM_TINY)) {
                    effScale.set(1.5f, 1.5f, 1.5f);
                    effectID = PARTICLE_RESOURCE_ID_MAPPING_463_;
                } else if (checkSize(SM_SMALL)) {
                    effectID = PARTICLE_RESOURCE_ID_MAPPING_463_;
                } else if (checkSize(SM_LARGE)) {
                    effectID = PARTICLE_RESOURCE_ID_MAPPING_465_;
                } else {
                    effectID = PARTICLE_RESOURCE_ID_MAPPING_464_;
                }

                dJEffManager_c::spawnEffect(effectID, mEffPos, nullptr, &effScale, nullptr, nullptr, 0, 0);

                if (timeCheck > 0) {
                    startSound(SE_TIMESLIP_TIMESLIP);
                    fn_800306d0();
                } else {
                    startSound(SE_TIMESLIP_TIMESLIP_REV);
                    fn_80030700();
                }
            }

            if (field_0xBBF == 3 && mTimeArea.isNearZero()) {
                if (checkInteractionFlags(INTERACT_0x4)) {
                    fn_800306d0();
                }
                return SUCCEEDED;
            }

            if (field_0xB8C < 1.f) {
                if (mStateMgr.isState(StateID_Absorption) || mHealth == 0) {
                    fn_187_5730();
                    fillUpperParams2Byte();
                    deleteRequest();
                }
                mSpeed = 0.f;

                // TODO: Maybe Inline - Common Pattern. Check GetCcMove
                calcVelocity();
                mPosition += mVelocity;
                mPosition += mStts.GetCcMove();

                mObjAcch.CrrPos(*dBgS::GetInstance());
                fn_187_44C0();
                return SUCCEEDED;
            }
        }

        if (field_0xBBF == 3 || field_0xBBF == 1) {
            if (isWithinPlayerRadius(field_0xB74) || field_0xB94 != 0) {
                if (field_0xBBF == 3) {
                    if (!(field_0xBC6 == 1 || field_0xBC6 == 2) || !mTimeArea.isNearZero()) {
                        mTimer_0xBC4 = 6;
                        mStateMgr.changeState(StateID_BirthJump);
                        field_0xBB8 = 0;
                        fn_80030700();
                        field_0xBC2 = 2;
                        fn_187_5390();
                    }
                } else {
                    mStateMgr.changeState(StateID_BirthJump);
                    field_0xBB8 = 0;
                }
                field_0xBBF = 0;
                setBattleBgmRelated(2);
                fn_80030700();
                mRotation.y = mAngle.y = getXZAngleToPlayer();
            }
            if (field_0xBBF == 3) {
                return SUCCEEDED;
            }
        }
    }

    mStateMgr.executeState();

    if (mStateMgr.isState(StateID_Absorption)) {
        mSph.SetTgType(~(AT_TYPE_COMMON0 | AT_TYPE_0x800000 | AT_TYPE_SWORD));
    } else {
        mSph.SetTgType(~AT_TYPE_COMMON0);

        if (mType == SM_RED) {
            mAnmMatClr.setFrame(0.f, 0);
        }
        if (mType == SM_YELLOW && !mSph.ChkTgElectric()) {
            mAnmMatClr.setFrame(0.f, 0);
        }
    }

    if (0 == sLib::calcTimer(&mTimer_0xBC4)) {
        calcVelocity();
        mPosition += mVelocity;
        mPosition += mStts.GetCcMove();
    }

    if (!mStateMgr.isState(StateID_Absorption)) {
        mObjAcch.CrrPos(*dBgS::GetInstance());
    }

    if (field_0xBBF != 6) {
        mSplashFx.execute(mObjAcch.GetWtrGroundH(), mObjAcch.GetGroundH());
    }

    if (!mStateMgr.isState(StateID_Dead)) {
        if (field_0xBBF != 6 && dBgS_WtrChk::CheckPos(&getPosition(), true, 500.0f, -500.0f) &&
            std::abs(mObjAcch.GetGroundH() - dBgS_WtrChk::GetWaterHeight()) > 200.0f &&
            (getPosition().y < dBgS_WtrChk::GetWaterHeight() - 100.0f)) {
            mHealth = 0;
            fn_187_4540(2);
        }

        if (mObjAcch.ChkGndHit() && fn_187_4C50() && mHealth != 0) {
            mHealth = 0;
            fn_187_4540(2);
        }
    }

    if (mHealth != 0) {
        fn_187_3F60();
        mAcchCir.SetWall(mScaleTarget.y * 100.f, mScaleTarget.x * 120.f);
    }

    fn_187_4CC0();

    if (mHealth == 0 || mStateMgr.isState(StateID_Dead)) {
        return SUCCEEDED;
    }

    if (mType == SM_YELLOW && mSph.ChkTgElectric()) {
        fn_187_5940();
    }

    mHitPos.set(0.f, 0.f, 0.f);

    if (0 == sLib::calcTimer(&mDamageTimer)) {
        field_0xB94 = fn_8002fde0(mSph, nullptr);
        if (field_0xB94 != 0) {
            if (mSph.ChkTgHit()) {
                if (field_0xB94 == 7) {
                    field_0xB6C = 0.5f;
                    mSpeed = 0.f;
                } else {
                    int val = 0; // IDK what this would have been
                    field_0xB6C = 1.5f + 0.1f * val;
                    field_0xB68 = -0.5f + 0.01f * val;
                }
            }
        }

        if (!(field_0xB94 >= 6 && field_0xB94 <= 10 || field_0xB94 == 0)) {
            field_0xB98 = 0;

            if (!fn_187_4B50() && mSph.ChkTgHit()) {
                int hitType = mSph.GetTgAtHitType();
                // const cast required... Maybe the const/non-const funcs are flipped?
                const mVec3_c &hitPos = const_cast<const dCcD_Sph *>(&mSph)->GetTgHitPos();
                hitPos.CopyTo(mHitPos);

                switch (hitType) {
                    case AT_TYPE_SWORD:
                        if (mSph.ChkTgElectric()) {
                            break;
                        }
                    case AT_TYPE_0x800000:
                        field_0xB98 = fn_187_52A0();
                        dJEffManager_c::spawnHitMarkEffect(6, mSph, nullptr, true);
                        if (field_0xB98 != 7) {
                            fn_187_61B0(0);
                            fn_187_4540(field_0xB94);
                        }
                        break;
                    default: {
                        dAcObjBase_c *tgActor = mSph.GetTgActor();
                        if (tgActor->mProfileName == fProfile::OBJ_FIRE_PILLAR) {
                            mHealth = 0;
                            fn_187_4540(2);
                        } else {
                            mHealth = 100;
                            startSound(SE_ESm_NO_DMG);
                            fn_187_61B0(7);
                        }

                    } break;
                    case AT_TYPE_BOMB:
                    case AT_TYPE_BUBBLE:
                    case 0:              {
                        if (mStateMgr.isState(StateID_Absorption)) {
                            fn_187_5730();
                        }
                        mHealth = 0;
                        fn_187_4540(2);

                    } break;
                }
            }
        }

        if (field_0xB94 == 7 && field_0xB80 < 1.f) {
            field_0xB80 = 4.f;
        }
    }

    if (mHealth != 0 && field_0xBC8 == 0 && field_0xBA0 == 0 && field_0xB90 < 0) {
        // Check to make sure the chu is slow enough/or on the ground
        if ((!mStateMgr.isState(StateID_BirthJump) || mObjAcch.ChkGndHit() || mVelocity.y <= -6.f) &&
            field_0xBB6 == 0) {
            // Check to make sure chu isnt in a "death"-like state.
            if (!(mStateMgr.isState(StateID_Absorption) || mStateMgr.isState(StateID_Dead) ||
                  mStateMgr.isState(StateID_Fusion))) {
                // CHECK FOR FUSION - Another ESm
                if (mSph.ChkCoHit() && mSph.GetCoActor()->mProfileName == fProfile::E_SM) {
                    // The other chu to interact with
                    dAcEsm_c *pOther = static_cast<dAcEsm_c *>(mSph.GetCoActor());

                    // Some checks to make sure the other chu can be interacted with
                    if (!fn_187_4B50() && pOther->field_0xBC8 == 0 && pOther->mHealth != 0 &&
                        pOther->field_0xBA0 == 0 && pOther->field_0xB90 < 0 && pOther->field_0xBB6 == 0) {
                        if (!pOther->mStateMgr.isState(StateID_BirthJump) || pOther->mObjAcch.ChkGndHit() ||
                            pOther->mVelocity.y <= -6.f) {
                            // Check for non-fusion/absorption status and if the other Chu is smaller than [this] chu
                            if (!(pOther->mStateMgr.isState(StateID_Absorption) ||
                                  pOther->mStateMgr.isState(StateID_Fusion)) &&
                                mScaleTarget.x >= pOther->mScaleTarget.x) {
                                // Merging of properties
                                mScaleTarget.CopyTo(mScaleCopy2);
                                mScaleCopy2 += pOther->mScaleTarget;

                                getPosition().CopyTo(mPosCopy1);
                                getPosition().CopyTo(pOther->mPosCopy1);

                                if (mScaleCopy2.x > 1.2f) {
                                    mScaleCopy2.set(1.2f, 1.2f, 1.2f);
                                }

                                pOther->mScaleCopy2.set(0.01f, 0.01f, 0.01f);

                                pOther->field_0xBC8 = 1;
                                pOther->mDamageTimer = 8;
                                pOther->mStateMgr.changeState(StateID_Fusion);
                                startSound(SE_ESm_UNITE);
                                mStateMgr.changeState(StateID_Fusion);
                            }
                        }
                    }
                }
                // CHECK FOR BOMB REFERENCE
                if (!mBombRef.get()) {
                    f32 lookRadius = 200.f;
                    lookRadius *= mScaleTarget.x;
                    if (lookRadius < 80.f) {
                        lookRadius = 80.f;
                    }

                    dAcBomb_c *bomb = getBombWithinRadius(lookRadius);

                    if (bomb != nullptr && std::abs(bomb->getPosition().y - mHomePos1.y) < 0.7f * lookRadius) {
                        if (bomb->getLinkage().tryAttach(bomb, this, &mBombRef, dLinkage_c::CONNECTION_1, false)) {
                            mTimer_0xBAE = 160;
                            startSound(SE_ESm_BRING_IN);
                            unsetActorProperty(AC_PROP_0x1);
                        }
                    }
                }
            }
        }
    }

    if (mSph.ChkTgElectric()) {
        fn_187_4B50();
    }
    s8 var = 0;
    f32 radius = 150.f + var;
    nw4r::g3d::ResNode node = mMdl.getModel().getResMdl().GetResNode("sm");

    mMtx_c nodeMtx;
    mMdl.getModel().getNodeWorldMtx(node.GetID(), nodeMtx);
    mVec3_c center(0.f, 0.f, 0.f);
    center.y = 90.f;

    MTXMultVec(nodeMtx, center, mHomePos1);

    mPosition.CopyTo(mPositionCopy3);
    mPosition.CopyTo(mPositionCopy2);
    mPositionCopy2.y += 130.f * mScaleTarget.y;
    mPositionCopy3.y += 260.f * mScaleTarget.y;

    mSph.SetC(mHomePos1);
    if (mSph.ChkTgElectric()) {
        holdSound(SE_ESm_ELEC_LV);
    }

    radius *= mScaleTarget.x;
    if (radius < 43.f) {
        radius = 43.f;
    }
    mSph.SetR(radius);
    dCcS::GetInstance()->Set(&mSph);

    f32 speedTarget = mSpeed;
    speedTarget -= field_0xB80;
    if (speedTarget < 0.f) {
        speedTarget = 0.f;
    }
    sLib::addCalcScaledDiff(&mSpeed, speedTarget, 0.3f, 1.f);
    sLib::addCalcScaled(&field_0xB80, 0.3f, 1.f);

    if (field_0xBCB == 0) {
        holdSound(SE_ESm_MOVE);
    } else {
        holdSound(SE_ESm_MOVE_HI);
    }

    if (field_0xBC8 == 0 && !mStateMgr.isState(StateID_Absorption)) {
        fn_80030c20(3, 700.f, 50.f, -200.f, 200.f);
    }

    if (field_0xBBF != 3 && mScaleTarget.z != mScale.z) {
        cLib::addCalcPos2(&mScale, mScaleTarget, 0.3f, 0.3f);
    }

    return SUCCEEDED;
}

int dAcEsm_c::draw() {
    if (fn_187_5AC0()) {
        return SUCCEEDED;
    }
    if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }
    if (mScale.squareMagXZ() < 0.0004f) {
        return SUCCEEDED;
    }
    if (shift8_0xFF != 0xFF && !SceneflagManager::sInstance->checkBoolFlag(mRoomID, shift8_0xFF)) {
        return SUCCEEDED;
    }

    if (field_0xBBF == 3) {
        return SUCCEEDED;
    }

    drawModelType1(&mMdl.getModel());

    if (mHealth == 0) {
        return SUCCEEDED;
    }

    s8 var = 0;
    mQuat_c q(mVec3_c(0.f, 50.f, 0.f), mScale.x * (var + 240.f));
    drawShadow(mShadowCircle, nullptr, mWorldMtx, &q, -1, -1, -1, -1, -1, mPosition.y - mObjAcch.GetGroundH());
    return SUCCEEDED;
}

void dAcEsm_c::initializeState_BaseMother() {}
void dAcEsm_c::executeState_BaseMother() {}
void dAcEsm_c::finalizeState_BaseMother() {}

void dAcEsm_c::initializeState_Wait() {
    Set_0xBBC(1);
}
void dAcEsm_c::executeState_Wait() {
    field_0xBA6 += (s16)(1000.f + field_0xB70 / mScaleTarget.x);
}
void dAcEsm_c::finalizeState_Wait() {}

void dAcEsm_c::initializeState_Walk() {
    if (field_0xBC5 != 2) {
        mMdl.setAnm("awa", m3d::PLAY_MODE_4, 4.f);
        field_0xBC5 = 0;
        mMdl.setFrame(cM::rndF(mMdl.getAnm().getEndFrame() - 1.f));
    }

    mVec3_c endPos;
    mStartingPos.CopyTo(mPosCopy1);
    mStartingPos.CopyTo(endPos);

    endPos.y += 10.f;
    if (dBgS_ObjLinChk::LineCross(&mHomePos1, &endPos, nullptr)) {
        mStartingPos.set(mPosition.x, mPosition.y, mPosition.z);
    }

    if (mType == SM_YELLOW) {
        field_0xBC1 = 60.f + cM::rndF(60.f);
    } else {
        field_0xBC1 = 30.f + cM::rndF(30.f);
    }

    field_0xBB8 = 0;
    field_0xB98 = 0;
    mRotation.x = 0;

    Set_0xBBC(2);
    field_0xBAA = mAngle.y;
    field_0xBA8 = 0;
    field_0xBCB = 0;
    mAcceleration = -3.f;
}
void dAcEsm_c::executeState_Walk() {
    field_0xBA6 += (s16)(1000.f + field_0xB70 / mScaleTarget.x);
    if (sLib::calcTimer(&field_0xBB6)) {
        return;
    }
    if (fn_187_42C0()) {
        return;
    }
    fn_187_5810();
    mMdl.setRate(1.f);
    field_0xBCB = 0;

    if (field_0xB78 == 0.f) {
    } else {
    }
}
void dAcEsm_c::finalizeState_Walk() {}

void dAcEsm_c::initializeState_BirthJump() {
    unsetActorProperty(AC_PROP_0x1);
    field_0xBA6 = 0;
    field_0xB54 = 0.1f;
    field_0xB68 = 0.f;

    mSpeed = 0.f;
    field_0xBB8 = 3;
    field_0xB90 = -1;

    if (field_0xBCE == 0 && mTimer_0xBC4 == 0 && mType == SM_YELLOW) {
        fn_187_4450();
    }
}
void dAcEsm_c::executeState_BirthJump() {}
void dAcEsm_c::finalizeState_BirthJump() {}

void dAcEsm_c::initializeState_Shake() {}
void dAcEsm_c::executeState_Shake() {}
void dAcEsm_c::finalizeState_Shake() {}

void dAcEsm_c::initializeState_Attack() {}
void dAcEsm_c::executeState_Attack() {}
void dAcEsm_c::finalizeState_Attack() {}

void dAcEsm_c::initializeState_Electrical() {}
void dAcEsm_c::executeState_Electrical() {}
void dAcEsm_c::finalizeState_Electrical() {}

void dAcEsm_c::initializeState_Absorption() {}
void dAcEsm_c::executeState_Absorption() {
    mMdl.getModel().getResMdl().GetResNode("mouth_ue");
}
void dAcEsm_c::finalizeState_Absorption() {}

void dAcEsm_c::initializeState_Fusion() {}
void dAcEsm_c::executeState_Fusion() {}
void dAcEsm_c::finalizeState_Fusion() {}

void dAcEsm_c::initializeState_Dead() {}
void dAcEsm_c::executeState_Dead() {}
void dAcEsm_c::finalizeState_Dead() {}

// . . .
void dAcEsm_c::fn_187_3F60() {
    f32 target = nw4r::math::SinIdx(field_0xBA4) * field_0xB6C;
    field_0xBA4 += (s16)(3000.f / mScaleTarget.x);

    sLib::addCalcScaledDiff(&field_0xB5C, field_0xB68, 0.3f, 1.f);

    if (mHealth != 0) {
        sLib::addCalcScaledDiff(&field_0xB58, 1.f + target, 0.1f, 3.f);
    } else {
        sLib::addCalcScaled(&field_0xB58, 0.1f, 3.f);
    }

    sLib::addCalcScaledDiff(&field_0xB50, 0.015f + field_0xB54, 0.5f, 0.5f);
    sLib::addCalcScaled(&field_0xB70, 0.01f, 1000.0f);
    sLib::addCalcScaled(&field_0xB6C, 0.1f, 0.1f);
    sLib::addCalcScaled(&field_0xB54, 0.03f, 0.03f);
}

bool dAcEsm_c::fn_187_4090() {
    mVec3_c pos(mPosition.x, mPosition.y + 20.f, mPosition.z);
    if (mVelocity.y <= 0.f && dBgS_ObjGndChk::CheckPos(pos) && dBgS_ObjGndChk::GetGroundHeight() + 5.f > mPosition.y) {
        mSpeed = 0.f;
        field_0xB6C = 1.5f;
        if (field_0xB84) {
            field_0xB6C = field_0xB84 * 0.1f + 0.7f;
        }
        field_0xB84 = 0.f;
        field_0xBA4 = 0xC180;
        field_0xB54 = 0.f;
        field_0xB50 = 0.f;
        field_0xB70 = 0.f;

        startSound(SE_ESm_LAND);

        if (field_0xBA0 == 0) {
            if (fn_800301b0(mPosition, mRotation.y, true, 10.f) == 3 /* TODO: Enum?*/) {
                int code = dBgS::GetInstance()->GetSpecialCode(dBgS_ObjGndChk::GetInstance());
                if (code != POLY_ATTR_SAND_SHALLOW && code != POLY_ATTR_SAND_MED) {
                    mHealth = 0;
                    fn_187_4540(2);
                    return false;
                }
            }
        }

        return true;
    }
    return false;
}

void dAcEsm_c::fn_187_4200() {
    if (mType == SM_BLUE || field_0xBCC != 0 | field_0xBA0 != 0) {
        return;
    }
    mVec3_c pos(mPosition.x, mPosition.y + 20.f, mPosition.z);

    if (dBgS_ObjGndChk::CheckPos(pos) && mPosition.y - dBgS_ObjGndChk::GetGroundHeight() >= 700.f) {
        if (field_0xBA0 == 0) {
            field_0xBA0 = 1;
        }
    }
}

bool dAcEsm_c::fn_187_42C0() {
    if (sLib::calcTimer(&field_0xBAC)) {
        return false;
    }

    if (mSph.ChkAtHit() && mSph.GetAtActor()->mProfileName == fProfile::PLAYER) {
        if (fn_187_4B50()) {
            return true;
        }

        if (sValueFromPlayer > 30) {
            sValueFromPlayer = 0;
        }

        if (mHealth != 0 && !fn_187_6B10() && !ChkCrossPlayer(0.f) && !mStateMgr.isState(StateID_Absorption) &&
            !mStateMgr.isState(StateID_BirthJump)) {
            mStateMgr.changeState(StateID_Absorption);
            return true;
        }
    }

    return false;
}

void dAcEsm_c::fn_187_4450() {
    if (mType == SM_YELLOW) {
        mSph.OnTgElectric();
        mAnmMatClr.setRate(1.f, 0);
    }
    mSph.OffAt_0x40();
    mSph.OnAtSet();
}

void dAcEsm_c::fn_187_44C0() {
    if (mType == SM_YELLOW) {
        mSph.ClrTgElectric();
        mAnmMatClr.setRate(0.f, 0);
        mAnmMatClr.setFrame(0.f, 0);
    }
    mSph.OnAt_0x40();
}

void dAcEsm_c::fn_187_4540(int param0) {
    const dAcPy_c *player = dAcPy_c::GetLink();
    mAng3_c rot = getAngle();

    fn_187_44C0();
    if (mStateMgr.isState(StateID_Dead)) {
        return;
    }

    if ((param0 != 0 && mScaleTarget.x <= 0.25f) || mHealth == 0) {
        if (param0 != 0 && param0 != 6 && param0 != 2 && param0 != 7) {
            mHealth = 0;
        }

        // NOTE: Unused Return Value
        mStateMgr.isState(StateID_Absorption);

        fn_187_61B0(0);
        mStateMgr.changeState(StateID_Dead);
        return;
    }

    if (param0 != 0) {
        if (mScaleTarget.x == 1.2f) {
            mScaleTarget.set(1.6f, 1.6f, 1.6f);
        }
        mScaleTarget *= 0.5f;
        mScale *= 0.5f;

        rot.y = (s16)cLib::targetAngleY(mPosition, player->mPosition) + cM::rndFX(1024.f);

        if (field_0xB98 != 2) {
            rot.y = fn_187_5150(false);
        }
        rot.y = mOrigRotZ;

        mVec3_c spawnPos;
        mHitPos.CopyTo(spawnPos);
        if (field_0xB98 == 1 || field_0xB98 == 2) {
            mPosition.CopyTo(spawnPos);
        }

        f32 scale = 0.9999f;
        f32 upper = 30.f;
        f32 lower = 20.f;
        f32 v = 0.f;
        f32 lowest = 10.f;
        u8 timer = 8;
        u8 bce = 1;
        u8 bb8 = 0;
        f32 f = 16384.f;

        dAcEsm_c *pChild = static_cast<dAcEsm_c *>(
            create(fProfile::E_SM, mRoomID, (mParams & ~0xFF) | mType, &spawnPos, &rot, nullptr, 0)
        );
        if (pChild == nullptr) {
            return;
        }

        if (field_0xB98 == 2) {
            mVec3_c temp0 = mScaleTarget;
            mVec3_c temp1 = mScale;
            temp0 *= scale;
            temp1 *= scale;
            temp0.CopyTo(pChild->mScaleTarget);
            temp1.CopyTo(pChild->mScale);
        } else {
            mScaleTarget.CopyTo(pChild->mScaleTarget);
            pChild->mScale.set(mScale.x, mScale.y, mScale.z);
        }
        pChild->mStartingPos.set(mStartingPos.x, mStartingPos.y, mStartingPos.z);
        pChild->mDamageTimer = timer;
        pChild->field_0xBB2 = timer;
        pChild->field_0xB98 = field_0xB98;
        pChild->field_0xBCE = bce;
        pChild->mStateMgr.changeState(StateID_BirthJump);

        if (field_0xB98 == 0) {
            rot.y -= mAng(f + cM::rndFX(4096.f));
        } else {
            rot.y = fn_187_51F0(true);
            const f32 y = sSmDataArr[field_0xB98].field_0x04.y;
            if (y != v) {
                pChild->mVelocity.y = y;

                pChild->mVelocity.y *= mScaleTarget.y;
                if (field_0xB98 != 2) {
                    if (pChild->mVelocity.y < lower) {
                        pChild->mVelocity.y = lower;
                    }
                } else {
                    if (pChild->mVelocity.y < upper) {
                        pChild->mVelocity.y = upper;
                    }
                }
            }

            pChild->field_0xB84 = pChild->mVelocity.y;
            pChild->mSpeed = mScaleTarget.x * sSmDataArr[field_0xB98].field_0x04.x;
            if (pChild->mSpeed != v && pChild->mSpeed < lowest) {
                pChild->mSpeed = lowest;
            }
            pChild->field_0xBB8 = bb8;
        }
    }

    mSph.OnCoSet();
    if (field_0xB98 != 2) {
        mAngle.y = rot.y;
    }
    if (field_0xB98 != 0) {
        field_0xBCE = 1;
        mDamageTimer = 8;
        field_0xBB2 = 8;
    }
    fn_187_6C20(1);
    if (field_0xB98 != 0) {
        if (sSmDataArr[field_0xB98].field_0x10.y) {
            mVelocity.y = sSmDataArr[field_0xB98].field_0x10.y;
            mVelocity.y *= mScaleTarget.y;
            if (field_0xB98 != 2) {
                if (mVelocity.y < 20.f) {
                    mVelocity.y = 20.f;
                }
            } else {
                if (mVelocity.y < 30.f) {
                    mVelocity.y = 30.f;
                }
            }
        }

        field_0xB84 = mVelocity.y;
        mSpeed = sSmDataArr[field_0xB98].field_0x10.y * mScaleTarget.x;
        if (mSpeed && mSpeed < 10.f) {
            mSpeed = 10.f;
        }
        field_0xBB8 = 0;
        if (field_0xB98 == 2) {
            mTimer_0xBC4 = 20;
        }
        startSound(SE_ESm_JUMP);
    }
}

bool dAcEsm_c::fn_187_4B50() {
    if (mStateMgr.isState(StateID_Dead)) {
        return false;
    }

    if (mBombRef.isLinked()) {
        dAcBomb_c *pBomb = mBombRef.get();
        if (pBomb) {
            pBomb->On_0xA3C(0x4);
            pBomb->Off_0xA3C(0x800000);
            mBombRef.unlink();
        }

        if (mStateMgr.isState(StateID_Absorption)) {
            fn_187_5730();
        }
        mStateMgr.changeState(StateID_Dead);
        return true;
    }

    return false;
}

bool dAcEsm_c::fn_187_4C50() {
    int attr = dBgS::GetInstance()->GetSpecialCode(mObjAcch.GetGnd());

    return (attr == POLY_ATTR_LAVA || attr == POLY_ATTR_SAND_DEEP_INSTANT || attr == POLY_ATTR_SAND_DEEP_SLOW);
}

void dAcEsm_c::Set_0xBBC(u8 val) {
    field_0xBBC = val;
}

void dAcEsm_c::fn_187_4CC0() {
    mVec3_c scale = mScale;
    if (field_0xBC6 == 1 || field_0xBC6 == 2) {
        field_0xB8C = mTimeArea.getDistMaybe();
        scale.multScalar(field_0xB8C);
    }

    if (mObjAcch.ChkWallHit(nullptr)) {
        sLib::addCalcAngle(mAngle.y.ref(), mAcchCir.GetWallAngleY(), 4, 0x200);
    }
    sLib::addCalcAngle(mRotation.y.ref(), mAngle.y, 2, 0x800);
    sLib::addCalcAngle(mRotUnk.x.ref(), mTargetRotX, 4, 0x800);
    sLib::addCalcAngle(mRotUnk.z.ref(), mTargetRotZ, 4, 0x800);

    mWorldMtx.transS(mPosition.x, mPosition.y, mPosition.z);
    mWorldMtx.XrotM(mRotUnk.x);
    mWorldMtx.ZrotM(mRotUnk.z);
    mWorldMtx.ZXYrotM(mRotation.x, mRotation.y, mRotation.z);

    f32 s = 1.f - 0.2f * field_0xB5C;
    f32 z = 150.f * mScaleTarget.x;
#define _WORLD_MTX_TRANS(x, y, z)                                                                                      \
    {                                                                                                                  \
        const f32 _x = x;                                                                                              \
        const f32 _z = z;                                                                                              \
        const f32 _y = y;                                                                                              \
        mMtx_c m;                                                                                                      \
        MTXTrans(m, _x, _y, _z);                                                                                       \
        mWorldMtx += m;                                                                                                \
    }
#define _WORLD_MTX_SCALE(x, y, z)                                                                                      \
    {                                                                                                                  \
        const f32 _x = x;                                                                                              \
        const f32 _z = z;                                                                                              \
        const f32 _y = y;                                                                                              \
        mMtx_c m;                                                                                                      \
        MTXScale(m, _x, _y, _z);                                                                                       \
        mWorldMtx += m;                                                                                                \
    }

    _WORLD_MTX_TRANS(0.f, 0.f, z);
    _WORLD_MTX_SCALE(s, s - field_0xB5C, 1.f + field_0xB5C);
    _WORLD_MTX_TRANS(0.f, 0.f, -z);
    _WORLD_MTX_TRANS(0.f, 0.f, field_0xB48);
    _WORLD_MTX_SCALE(scale.y / field_0xB58, scale.x * field_0xB58, scale.y / field_0xB58);
    _WORLD_MTX_TRANS(0.f, 0.f, -field_0xB48);
    mRotCopy.z = field_0xBA6;
    _WORLD_MTX_TRANS(20.f, 0.f, 20.f);
    mWorldMtx.ZrotM(mRotCopy.z);
    mWorldMtx.XrotM(mRotCopy.z);
    _WORLD_MTX_SCALE(1.f - field_0xB50, 1.f + field_0xB50, 1.f - field_0xB50);
    mWorldMtx.XrotM(-mRotCopy.z);
    mWorldMtx.ZrotM(-mRotCopy.z);
    _WORLD_MTX_TRANS(-20.f, -0.f, -20.f);
    _WORLD_MTX_TRANS(20.f, 0.f, -20.f);
    mWorldMtx.ZrotM(mRotCopy.z);
    mWorldMtx.YrotM(mRotCopy.z);
    _WORLD_MTX_SCALE(1.f - field_0xB50, 1.f + field_0xB50, 1.f - field_0xB50);
    mWorldMtx.YrotM(-mRotCopy.z);
    mWorldMtx.ZrotM(-mRotCopy.z);
    _WORLD_MTX_TRANS(-20.f, -0.f, 20.f);
    _WORLD_MTX_TRANS(-20.f, 0.f, 20.f);
    mWorldMtx.ZrotM(mRotCopy.z);
    _WORLD_MTX_SCALE(1.f - field_0xB50, 1.f + field_0xB50, 1.f - field_0xB50);
    mWorldMtx.ZrotM(-mRotCopy.z);
    _WORLD_MTX_TRANS(20.f, -0.f, -20.f);
    _WORLD_MTX_TRANS(-20.f, 0.f, -20.f);
    mWorldMtx.XrotM(mRotCopy.z);
    _WORLD_MTX_SCALE(1.f - field_0xB50, 1.f + field_0xB50, 1.f - field_0xB50);
    mWorldMtx.XrotM(-mRotCopy.z);
#undef _WORLD_MTX_TRANS
#undef _WORLD_MTX_SCALE

    mMdl.play();
    mMdl.getModel().setLocalMtx(mWorldMtx);
    mMdl.getModel().calc(false);
    mWorldMtx.getTranslation(mEffPos);
    updateBoundingBox();
}
// . . .

bool dAcEsm_c::checkSize(dAcEsm_c::SmSize_e size) const {
    bool ret = false;

    switch (size) {
        default:
        case SM_SMALL: {
            if (mScaleTarget.x > 0.4f && mScaleTarget.x <= 0.8f) {
                ret = true;
            }
        } break;
        case SM_LARGE: {
            if (mScaleTarget.x > 0.25f && mScaleTarget.x <= 0.4f) {
                ret = true;
            }
        } break;
        case SM_MASSIVE: {
            if (mScaleTarget.x <= 0.25f) {
                ret = true;
            }
        } break;
        case SM_TINY: {
            if (mScaleTarget.x > 0.8f && mScaleTarget.x <= 1.2f) {
                ret = true;
            }
        } break;
    }
    return ret;
}

// . . .
void dAcEsm_c::fn_187_5940() {
    mMdl.getModel().getResMdl().GetResNode("center");
}
void dAcEsm_c::updateBoundingBox() {
    f32 min = -200.f / mScale.x;
    f32 max = 200.f / mScale.x;
    mBoundingBox.Set(mVec3_c(min, min, min), mVec3_c(max, 250.f / mScale.x, max));
}

void dAcEsm_c::fn_187_61B0(u8 param0) {
    if (param0 == 0 && field_0xB98 == 0) {
        return;
    }

    const dAcPy_c *player = dAcPy_c::GetLink();

    // Yellowish
    mColor clr1(0xFF, 0xC8, 0x32, 0xFF);
    // Redish
    mColor clr2(0xC8, 0x32, 0x00, 0xFF);

    mAng3_c ang(0, 0, 0);
    mMtx_c mtx_trans = mWorldMtx;
    mVec3_c scale;

    if (mType == SM_BLUE) {
        // Blue/purple
        clr1 = mColor(0x64, 0x64, 0xFF, 0xFF);
        // Purple
        clr2 = mColor(0x50, 0x50, 0x96, 0xFF);
    } else if (mType == SM_YELLOW) {
        // Yellow
        clr1 = mColor(0xFF, 0xFF, 0x00, 0xFF);
        // Darker Yellow
        clr2 = mColor(0x96, 0x96, 0x00, 0xFF);
    } else if (mType == SM_GREEN) {
        // Light Green
        clr1 = mColor(0x00, 0xBE, 0x73, 0xFF);
        // Darker Blue-Green
        clr2 = mColor(0x00, 0x73, 0x69, 0xFF);
    }

    switch (param0) {
        case 0: {
            mMtx_c mtx_scale;
            ang.set(0, 0, 0);
            ang.z += player->vt_0x258();
            MTXTrans(mtx_trans, mHitPos.x, mHitPos.y, mHitPos.z);
            mtx_trans.YrotM(mRotation.y);
            mtx_trans.ZYXrotM(ang);
            MTXScale(mtx_scale, mScaleTarget.x, mScaleTarget.x, mScaleTarget.x);

            mtx_trans += mtx_scale;
            for (int i = 0; i <= 2; ++i) {
                dEmitterBase_c *emitter =
                    dJEffManager_c::spawnEffect(sEmitterResArr[param0 + i], mtx_trans, &clr1, &clr2, 0, 0);
                if (emitter == nullptr) {
                    continue;
                }
                switch (i) {
                    case 0: {
                        if (checkSize(SM_LARGE)) {
                            emitter->setRate(20.0f);
                            scale.set(2.f, 2.f, 2.f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        } else if (checkSize(SM_MASSIVE)) {
                            emitter->setRate(15.0f);
                            scale.set(2.5f, 2.5f, 2.5f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        }
                    } break;
                    case 1: {
                        if (checkSize(SM_LARGE)) {
                            scale.set(1.14f, 1.3f, 1.14f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        } else if (checkSize(SM_MASSIVE)) {
                            scale.set(1.43f, 2.67f, 1.43f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        }
                    } break;
                    case 2: {
                        if (checkSize(SM_LARGE)) {
                            scale.set(1.12f, 1.12f, 1.12f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        } else if (checkSize(SM_MASSIVE)) {
                            scale.set(1.22f, 1.22f, 1.22f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        }
                    } break;
                }
            }
        } break;
        case 3: {
            mMtx_c mtx_scale;
            MTXTrans(mtx_trans, mPosition.x, mPosition.y, mPosition.z);
            MTXScale(mtx_scale, mScaleTarget.x, mScaleTarget.y, mScaleTarget.z);
            mtx_trans += mtx_scale;

            if (mEffArr[0].startEffect(sEmitterResArr[3], mtx_trans, &clr1, &clr2)) {
                if (checkSize(SM_LARGE)) {
                    mEffArr[0].setRate(12.0f);
                    scale.set(1.7f, 1.7f, 1.7f);
                    scale *= mScaleTarget.x;
                    mEffArr[0].setParticleScale(scale);
                } else if (checkSize(SM_MASSIVE)) {
                    mEffArr[0].setRate(7.0f);
                    scale.set(2.5f, 2.5f, 2.5f);
                    scale *= mScaleTarget.x;
                    mEffArr[0].setParticleScale(scale);
                }
            }

        } break;
        case 4: {
            mMtx_c mtx_scale;
            MTXTrans(mtx_trans, mPosition.x, mPosition.y, mPosition.z);
            mtx_trans.YrotM(mRotation.y);
            MTXScale(mtx_scale, mScaleTarget.x, mScaleTarget.y, mScaleTarget.z);
            mtx_trans += mtx_scale;
            dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_280_, mtx_trans, &clr1, &clr2, 0, 0);

            for (int i = 4; i <= 5; ++i) {
                dEmitterBase_c *emitter = dJEffManager_c::spawnEffect(sEmitterResArr[i], mtx_trans, &clr1, &clr2, 0, 0);
                if (emitter == nullptr) {
                    continue;
                }
                switch (i) {
                    case 4: {
                        if (checkSize(SM_LARGE)) {
                            emitter->setRate(5.0f);
                            scale.set(1.17f, 1.17f, 1.17f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        } else if (checkSize(SM_MASSIVE)) {
                            emitter->setRate(3.0f);
                            scale.set(1.25f, 1.25f, 1.25f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        }
                    } break;
                    case 5: {
                        if (checkSize(SM_LARGE)) {
                            emitter->setRate(20.0f);
                            scale.set(1.5f, 1.5f, 1.5f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        } else if (checkSize(SM_MASSIVE)) {
                            emitter->setRate(15.0f);
                            scale.set(2.f, 2.f, 2.f);
                            scale *= mScaleTarget.x;
                            emitter->setParticleScale(scale);
                        }
                    } break;
                }
            }
        } break;
        case 7: {
            mScaleTarget.CopyTo(scale);
            if (checkSize(SM_LARGE) || checkSize(SM_MASSIVE)) {
                scale *= 2.f;
            }
            mMtx_c mtx_scale;
            MTXTrans(mtx_trans, mHitPos.x, mHitPos.y, mHitPos.z);
            mtx_trans.YrotM(mRotation.y);
            MTXScale(mtx_scale, scale.x, scale.y, scale.z);
            mtx_trans += mtx_scale;
            dJEffManager_c::spawnEffect(sEmitterResArr[param0], mtx_trans, &clr1, &clr2, 0, 0);
        } break;
    }

    return;
}

bool dAcEsm_c::fn_187_6B10() {
    const dAcPy_c *player = dAcPy_c::GetLink();
    if (mStateMgr.isState(StateID_Dead)) {
        return true;
    }
    if (fn_187_5AC0() || player->getRidingMinecartActor() ||
        player->getRidingActorType() == dAcPy_c::RIDING_TRUCK_MINECART || player->isRecovering() ||
        player->isRidingBall() || player->checkActionFlags(0x1000000) || player->checkActionFlags(0x40)) {
        return true;
    }

    return false;
}

void dAcEsm_c::fn_187_6C20(bool param0) {
    fn_187_5730();
    if (!param0) {
        fn_187_4540(0);
    }

    mSph.SetAtFlagsUpper(0);
    sTimer = 0;
    setInteractionFlags(INTERACT_0x4);
    fn_187_44C0();
    field_0xBAC = 60;

    if (!param0) {
        dCamera_c *cam = dScGame_c::getCamera(0);
        mAngle.y = cLib::targetAngleY(cam->getPositionMaybe(), cam->getField_0x78());
        mAngle.y += cM::rndFX(16384.f);
    }

    field_0xBCE = 1;
    mStateMgr.changeState(StateID_BirthJump);
}

const u16 dAcEsm_c::sEmitterResArr[8] = {PARTICLE_RESOURCE_ID_MAPPING_283_, PARTICLE_RESOURCE_ID_MAPPING_284_,
                                         PARTICLE_RESOURCE_ID_MAPPING_285_, PARTICLE_RESOURCE_ID_MAPPING_278_,
                                         PARTICLE_RESOURCE_ID_MAPPING_281_, PARTICLE_RESOURCE_ID_MAPPING_282_,
                                         PARTICLE_RESOURCE_ID_MAPPING_280_, PARTICLE_RESOURCE_ID_MAPPING_279_};

const dAcEsm_c::SmData_c dAcEsm_c::sSmDataArr[8] = {
    {     0,      0,  mVec3_c(0.f,  0.f, 0.f),  mVec3_c(0.f,  0.f, 0.f)},
    {0x4000, 0xC000, mVec3_c(25.f, 30.f, 0.f), mVec3_c(25.f, 30.f, 0.f)},
    {     0,      0,  mVec3_c(0.f, 40.f, 0.f),  mVec3_c(0.f,  0.f, 0.f)},
    {0x4000, 0xC000, mVec3_c(20.f, 35.f, 0.f), mVec3_c(10.f, 25.f, 0.f)},
    {0xC000, 0x4000, mVec3_c(20.f, 35.f, 0.f), mVec3_c(10.f, 25.f, 0.f)},
    {0x4000, 0xC000, mVec3_c(20.f, 35.f, 0.f), mVec3_c(10.f, 25.f, 0.f)},
    {0xC000, 0x4000, mVec3_c(20.f, 35.f, 0.f), mVec3_c(10.f, 25.f, 0.f)},
    {     0,      0,  mVec3_c(0.f, 40.f, 0.f),  mVec3_c(0.f,  0.f, 0.f)},
};
