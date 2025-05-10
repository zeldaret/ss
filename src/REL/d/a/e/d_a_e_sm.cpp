#include "d/a/e/d_a_e_sm.h"

#include "c/c_lib.h"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/c/c_cc_d.h"
#include "d/d_sc_game.h"
#include "d/flag/sceneflag_manager.h"
#include "d/flag/storyflag_manager.h"
#include "f/f_base.h"
#include "m/m3d/m_fanm.h"
#include "m/m_angle.h"
#include "m/m_color.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resanmclr.h"
#include "nw4r/g3d/res/g3d_resanmtexpat.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"
#include "toBeSorted/blur_and_palette_manager.h"
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
    mHomePos2.set(position.x, position.y, position.z);

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

    mScale.CopyTo(mScaleCopy1);
    mScale.CopyTo(mScaleCopy2);

    mObjAcch.Set(this, 1, &mAcchCir);
    mAcchCir.SetWall(mScaleCopy1.y * 100.f, mScaleCopy1.x * 100.f);
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
        mEffExt.init(this, 100.f, mScaleCopy1.x * 2.f, 0.f);
        mEffExt.setIsSmall(1);
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
            field_0xA7C = 0;
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), GetPosition())) {
                field_0xA74 = 1.f;
                field_0xB8C = 1.f;
                fn_80030700();
            } else {
                field_0xA74 = 0.f;
                field_0xB8C = 0.f;
                fn_800306d0();
            }
        } break;
        case 2: {
            field_0xA7C = 1;
            if (dTimeAreaMgr_c::GetInstance()->fn_800B9B60(getRoomId(), GetPosition())) {
                field_0xB8C = 0.f;
                field_0xA74 = 0.f;
                fn_800306d0();
            } else {
                field_0xB8C = 1.f;
                field_0xA74 = 1.f;
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
    BlurAndPaletteManager::GetPInstance()->fn_800223A0(&mLightInfo);
    return SUCCEEDED;
}

int dAcEsm_c::actorExecute() {
    mLightInfo.SetScale(0.f);
    if (SceneflagManager::sInstance->checkFlag(getRoomId(), shift8_0xFF)) {
        return SUCCEEDED;
    }

    if (dAcPy_c::GetLink()->checkActionFlagsCont(0x400000)) {
        return SUCCEEDED;
    }

    dAcBomb_c *pObj = mBombRef.get();
    if (mBombRef.isLinked() && pObj != nullptr) {
        mVec3_c target = GetPosition();
        if (!checkSize(SM_MASSIVE) && !checkSize(SM_LARGE)) {
            target.y += 60.f + mScaleCopy1.y;
        }
        cLib::addCalcPos2(&position, target, 0.8f, 20.f + 1.5f * forwardSpeed);

        field_0xB6C = 0.5f;

        if (field_0xB65 != 2) {
            mMdl.setAnm("attack", m3d::PLAY_MODE_4, 4.f);
            field_0xB65 = 2;
        }

        if (0 == sLib::calcTimer(&timer_0xBAE)) {}
    }
}

void dAcEsm_c::initializeState_BaseMother() {}
void dAcEsm_c::executeState_BaseMother() {}
void dAcEsm_c::finalizeState_BaseMother() {}

void dAcEsm_c::initializeState_Wait() {
    fn_187_4CB0(1);
}
void dAcEsm_c::executeState_Wait() {
    field_0xBA6 += (s16)(1000.f + field_0xB70 / mScaleCopy1.x);
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
            if (mScaleCopy1.x > 0.4f && mScaleCopy1.x <= 0.8f) {
                ret = true;
            }
        } break;
        case SM_LARGE: {
            if (mScaleCopy1.x > 0.25f && mScaleCopy1.x <= 0.4f) {
                ret = true;
            }
        } break;
        case SM_MASSIVE: {
            if (mScaleCopy1.x <= 0.25f) {
                ret = true;
            }
        } break;
        case SM_TINY: {
            if (mScaleCopy1.x > 0.8f && mScaleCopy1.x <= 1.2f) {
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
