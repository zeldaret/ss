#include "d/a/e/d_a_e_sm.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_base.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_bomb.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "f/f_profile_name.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_mtx.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "rvl/MTX/mtxvec.h"
#include "s/s_Math.h"
#include "toBeSorted/blur_and_palette_manager.h"
#include "toBeSorted/d_emitter.h"
#include "toBeSorted/small_sound_mgr.h"
#include "toBeSorted/time_area_mgr.h"

#pragma explicit_zero_data on
int dAcEsm_c::sDefaultRotX(0);
int dAcEsm_c::sDefaultRotY(0);
int dAcEsm_c::sDefaultRotZ(0);
#pragma explicit_zero_data off

SPECIAL_ACTOR_PROFILE(E_SM, dAcEsm_c, fProfile::E_SM, 0xEB, 0, 4098);

dCcD_SrcSph dAcEsm_c::sSphSrc = {
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

    TRY_CREATE(mAnmTexPat.create(mdl, texPat, &heap_allocator, nullptr, 1));
    mMdl.getModel().setAnm(mAnmTexPat);

    nw4r::g3d::ResAnmClr anmClr = rres.GetResAnmClr("attack");
    if (mType == SM_YELLOW) {
        anmClr = rres.GetResAnmClr("Spark");
    }
    TRY_CREATE(mAnmMatClr.create(mdl, anmClr, &heap_allocator, nullptr, 1));

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

#pragma explicit_zero_data on
static int anotherAngleZ = 0;
#pragma explicit_zero_data off

bool dAcEsm_c::sSomeArrayInit = false;
bool dAcEsm_c::sSomeArray[9];

// Complete Guess
todoStruct00::InternalData data0 = {0, 100.f, nullptr};
todoStruct00::InternalData data1 = {1, 130.f, &data0};

int dAcEsm_c::actorCreate() {
    // Check for Batreaux being human and on Skyloft/Waterfall cave
    if (StoryflagManager::sInstance->getCounterOrFlag(360) &&
        (dScGame_c::isCurrentStage("F000") || dScGame_c::isCurrentStage("D000"))) {
        return FAILED;
    }

    mType = GetType();
    field_0xBBF = GetParam_s4_0xF();
    shift8_0xFF = GetParam_s8_0xFF();

    field_0xB74 = GetParam_s16_0xFF();
    field_0xB78 = rotation.z;
    if (field_0xB74 == 0.f) {
        field_0xB74 = 400.f;
    }
    field_0xBC6 = GetParam_s24_0x7();

    mOrigRotZ = rotation.z;

    if (field_0xB78 <= 0.f) {
        field_0xB78 = 0.f;
    } else {
        field_0xB78 = 200.f * field_0xB78;
    }

    mStartingPos.set(position.x, position.y, position.z);
    mHomePos1.set(position.x, position.y, position.z);
    mEffPos.set(position.x, position.y, position.z);

    CREATE_ALLOCATOR(dAcEsm_c);

    rotation.z = angle.z = anotherAngleZ;

    mBombRef.unlink();

    setActorProperty(0x1);
    // Ignore these bounding box sets. `fixBoundBox` completely overwrites them with the scale
    switch ((int)mScale.x) {
        default:
        case SM_SMALL: {
            boundingBox.Set(mVec3_c(-250.f, -250.f, -250.f), mVec3_c(250.f, 375.f, 250.f));
            mScale.set(0.8f, 0.8f, 0.8f);
        } break;
        case SM_LARGE: {
            boundingBox.Set(mVec3_c(-500.f, -500.f, -500.f), mVec3_c(500.f, 750.f, 500.f));
            mScale.set(0.4f, 0.4f, 0.4f);
        } break;
        case SM_MASSIVE: {
            boundingBox.Set(mVec3_c(-800.f, -800.f, -800.f), mVec3_c(800.f, 1200.f, 800.f));
            mScale.set(0.25f, 0.25f, 0.25f);
        } break;
        case SM_TINY: {
            boundingBox.Set(mVec3_c(-150.f, -150.f, -150.f), mVec3_c(150.f, 225.f, 150.f));
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
    mObjAcch.mField_0x390 = 1;

    forwardMaxSpeed = -40.f;

    mSph.Set(sSphSrc);

    mSph.SetStts(mStts);

    position.CopyTo(poscopy2);
    position.CopyTo(mPosCopy1);
    position.CopyTo(poscopy3);
    poscopy3.y += 50.f;

    // Fake ??
    const s16 rotz = rotation.z;
    const s16 roty = rotation.y;
    const s16 rotx = rotation.x;
    mRotCopy.set(rotx, roty, rotz);

    field_0xB98 = 0;
    field_0xB58 = 1.f;
    field_0xB90 = -1;

    // This is really weird...
    if (!sSomeArrayInit) {
        sSomeArrayInit = true;
        bool *parr = getArray();
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
        *parr++ = false;
    }

    field_0x9f8.Set(this, &data1, &mMdl.getModel(), 1);

    mLightInfo.SetColor(mColor(0xCC, 0xFF, 0xFF, 0xFF));
    mLightInfo.SetScale(0.f);

    BlurAndPaletteManager::GetPInstance()->fn_800225F0(&mLightInfo);
    if (field_0xBBF == 1) {
        static u32 anglex = {0x8000};
        rotation.x = anglex;
        field_0xBCC = 1;
    }

    mRotUnk.x = sDefaultRotX;
    mRotUnk.y = sDefaultRotY;
    mRotUnk.z = sDefaultRotZ;

    mEffArr[0].init(this);
    mEffArr[1].init(this);

    if (field_0xBBF == 1) {
        mStateMgr.changeState(StateID_Wait);
    } else {
        mStateMgr.changeState(StateID_Walk);
    }

    mHealth = 100;

    if (field_0xBBF != 1) {
        forwardAccel = -3.f;
    }
    calcVelocity();
    position += velocity;
    position += mStts.GetCcMove();

    mObjAcch.CrrPos(*dBgS::GetInstance());
    updateMatrix();

    f32 anim_frame;
    switch (mType) {
        // SM_RED
        default: {
            targetFiTextId = 59;
            anim_frame = 0.f;
            mSph.SetAtModifier(1);
            mType = 0;
        } break;
        case SM_GREEN_ALT:
        case SM_GREEN:     {
            targetFiTextId = 62;
            anim_frame = 3.f;
            mType = 1;
        } break;
        case SM_YELLOW_ALT:
        case SM_YELLOW:     {
            targetFiTextId = 61;
            anim_frame = 2.f;
            mSph.SetAtModifier(4);
            mType = 2;
        } break;

        case SM_BLUE: {
            targetFiTextId = 60;
            anim_frame = 1.f;
        } break;
    }

    if (field_0xBBF != 6) {
        mSplashFx.init(this, 100.f, mScaleTarget.x * 2.f, 0.f);
        mSplashFx.setIsSmall(1);
    }

    mAnmTexPat.setFrame(anim_frame, 0);

    mInteractionFlags |= 0x40;

    fn_187_44C0();

    mSph.SetAtDamage(4);

    return SUCCEEDED;
}

int dAcEsm_c::actorPostCreate() {
    switch ((u32)field_0xBC6) {
        case 1: {
            mTimeArea.setField0x08(0);
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), GetPosition())) {
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
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), GetPosition())) {
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
        clearActorProperty(1);
        fn_800306d0();
        FUN_8002d860(0);
    }

    return SUCCEEDED;
}

int dAcEsm_c::doDelete() {
    BlurAndPaletteManager::GetPInstance()->fn_800226E0(&mLightInfo);
    return SUCCEEDED;
}

int dAcEsm_c::actorExecute() {
    mLightInfo.SetScale(0.f);
    if (shift8_0xFF != 0xFF && !SceneflagManager::sInstance->checkBoolFlag(roomid, shift8_0xFF)) {
        return SUCCEEDED;
    }

    if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }

    dAcBomb_c *bomb = mBombRef.get();
    if (mBombRef.isLinked() && bomb) {
        mVec3_c target;
        GetPosition().CopyTo(target);
        if (!checkSize(SM_MASSIVE) && !checkSize(SM_LARGE)) {
            target.y += mScaleTarget.y * 60.f;
        }

        cLib::addCalcPos2(&bomb->GetPosition(), target, 0.8f, 20.f + GetSpeed() * 1.5f);

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
                    timeCheck = mTimeArea.check(getRoomId(), GetPosition(), 0, 10.f, 0.2f);
                }
            }

            if (timeCheck != 0) {
                if (field_0xBBF == 3) {
                    if (checkInteractionFlags(4)) {
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
                    playSound(SE_TIMESLIP_TIMESLIP);
                    fn_800306d0();
                } else {
                    playSound(SE_TIMESLIP_TIMESLIP_REV);
                    fn_80030700();
                }
            }

            if (field_0xBBF == 3 && mTimeArea.isNearZero()) {
                if (checkInteractionFlags(4)) {
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
                forwardSpeed = 0.f;

                // TODO: Maybe Inline - Common Pattern. Check GetCcMove
                calcVelocity();
                position += velocity;
                position += mStts.GetCcMove();

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
                FUN_8002d860(2);
                fn_80030700();
                rotation.y = angle.y = getXZAngleToPlayer();
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
        position += velocity;
        position += mStts.GetCcMove();
    }

    if (!mStateMgr.isState(StateID_Absorption)) {
        mObjAcch.CrrPos(*dBgS::GetInstance());
    }

    if (field_0xBBF != 6) {
        mSplashFx.execute(mObjAcch.GetWtrGroundH(), mObjAcch.GetGroundH());
    }

    if (!mStateMgr.isState(StateID_Dead)) {
        if (field_0xBBF != 6 && dBgS_WtrChk::CheckPos(&GetPosition(), true, 500.0f, -500.0f) &&
            std::abs(mObjAcch.GetGroundH() - dBgS_WtrChk::GetWaterHeight()) > 200.0f &&
            (GetPosition().y < dBgS_WtrChk::GetWaterHeight() - 100.0f)) {
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
                    forwardSpeed = 0.f;
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
                        if (tgActor->profile_name == fProfile::OBJ_FIRE_PILLAR) {
                            mHealth = 0;
                            fn_187_4540(2);
                        } else {
                            mHealth = 100;
                            playSound(SE_ESm_NO_DMG);
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
        if ((!mStateMgr.isState(StateID_BirthJump) || mObjAcch.ChkGndHit() || velocity.y <= -6.f) && field_0xBB6 == 0) {
            // Check to make sure chu isnt in a "death"-like state.
            if (!(mStateMgr.isState(StateID_Absorption) || mStateMgr.isState(StateID_Dead) ||
                  mStateMgr.isState(StateID_Fusion))) {
                // CHECK FOR FUSION - Another ESm
                if (mSph.ChkCoHit() && mSph.GetCoActor()->profile_name == fProfile::E_SM) {
                    // The other chu to interact with
                    dAcEsm_c *pOther = static_cast<dAcEsm_c *>(mSph.GetCoActor());

                    // Some checks to make sure the other chu can be interacted with
                    if (!fn_187_4B50() && pOther->field_0xBC8 == 0 && pOther->mHealth != 0 &&
                        pOther->field_0xBA0 == 0 && pOther->field_0xB90 < 0 && pOther->field_0xBB6 == 0) {
                        if (!pOther->mStateMgr.isState(StateID_BirthJump) || pOther->mObjAcch.ChkGndHit() ||
                            pOther->velocity.y <= -6.f) {
                            // Check for non-fusion/absorption status and if the other Chu is smaller than [this] chu
                            if (!(pOther->mStateMgr.isState(StateID_Absorption) ||
                                  pOther->mStateMgr.isState(StateID_Fusion)) &&
                                mScaleTarget.x >= pOther->mScaleTarget.x) {
                                // Merging of properties
                                mScaleTarget.CopyTo(mScaleCopy2);
                                mScaleCopy2 += pOther->mScaleTarget;

                                GetPosition().CopyTo(mPosCopy1);
                                GetPosition().CopyTo(pOther->mPosCopy1);

                                if (mScaleCopy2.x > 1.2f) {
                                    mScaleCopy2.set(1.2f, 1.2f, 1.2f);
                                }

                                pOther->mScaleCopy2.set(0.01f, 0.01f, 0.01f);

                                pOther->field_0xBC8 = 1;
                                pOther->mDamageTimer = 8;
                                pOther->mStateMgr.changeState(StateID_Fusion);
                                playSound(SE_ESm_UNITE);
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

                    if (bomb != nullptr && std::abs(bomb->GetPosition().y - mHomePos1.y) < 0.7f * lookRadius) {
                        if (bomb->mActorCarryInfo.tryAttachWithRef(bomb, this, &mBombRef, 1, false)) {
                            mTimer_0xBAE = 160;
                            playSound(SE_ESm_BRING_IN);
                            clearActorProperty(1);
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

    position.CopyTo(poscopy3);
    position.CopyTo(poscopy2);
    poscopy2.y += 130.f * mScaleTarget.y;
    poscopy3.y += 260.f * mScaleTarget.y;

    mSph.SetC(mHomePos1);
    if (mSph.ChkTgElectric()) {
        playSoundEffect1(SE_ESm_ELEC_LV);
    }

    radius *= mScaleTarget.x;
    if (radius < 43.f) {
        radius = 43.f;
    }
    mSph.SetR(radius);
    dCcS::GetInstance()->Set(&mSph);

    f32 speedTarget = forwardSpeed;
    speedTarget -= field_0xB80;
    if (speedTarget < 0.f) {
        speedTarget = 0.f;
    }
    sLib::addCalcScaledDiff(&forwardSpeed, speedTarget, 0.3f, 1.f);
    sLib::addCalcScaled(&field_0xB80, 0.3f, 1.f);

    if (field_0xBCB == 0) {
        playSoundEffect1(SE_ESm_MOVE);
    } else {
        playSoundEffect1(SE_ESm_MOVE_HI);
    }

    if (field_0xBC8 == 0 && !mStateMgr.isState(StateID_Absorption)) {
        fn_80030c20(3, 700., 50.f, -200.f, 200.f);
    }

    if (field_0xBBF != 3 && mScaleTarget.z != mScale.z) {
        cLib::addCalcPos2(&mScale, mScaleTarget, 0.3f, 0.3f);
    }

    return SUCCEEDED;
}

void dAcEsm_c::initializeState_BaseMother() {}
void dAcEsm_c::executeState_BaseMother() {}
void dAcEsm_c::finalizeState_BaseMother() {}

void dAcEsm_c::initializeState_Wait() {
    fn_187_4CB0(1);
}
void dAcEsm_c::executeState_Wait() {
    field_0xBA6 += (s16)(1000.f + field_0xB70 / mScaleTarget.x);
}
void dAcEsm_c::finalizeState_Wait() {}

void dAcEsm_c::initializeState_Walk() {}
void dAcEsm_c::executeState_Walk() {}
void dAcEsm_c::finalizeState_Walk() {}

void dAcEsm_c::initializeState_BirthJump() {}
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
void dAcEsm_c::executeState_Absorption() {}
void dAcEsm_c::finalizeState_Absorption() {}

void dAcEsm_c::initializeState_Fusion() {}
void dAcEsm_c::executeState_Fusion() {}
void dAcEsm_c::finalizeState_Fusion() {}

void dAcEsm_c::initializeState_Dead() {}
void dAcEsm_c::executeState_Dead() {}
void dAcEsm_c::finalizeState_Dead() {}

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

void dAcEsm_c::updateBoundingBox() {
    f32 min = -200.f / mScale.x;
    f32 max = 200.f / mScale.x;
    boundingBox.Set(mVec3_c(min, min, min), mVec3_c(max, 250.f / mScale.x, max));
}
