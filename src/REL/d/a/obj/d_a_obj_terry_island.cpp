#include "d/a/obj/d_a_obj_terry_island.h"

#include "c/c_lib.h"
#include "cmath"
#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_light_env.h"
#include "d/d_sc_game.h"
#include "d/d_stage.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_angle.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "rvl/GX/GXTypes.h"
#include "s/s_Math.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_TERRY_ISLAND, dAcOterryIsland_c, fProfile::OBJ_TERRY_ISLAND, 0x1E7, 0, 3);

struct dAcOterryIsland_HIO_c {
    f32 field_0x00;
    GXColor mFireColor;
    f32 mFireScale;
    s16 mFireRespawnTime;
    s16 mFireSmokeTime;

    static const dAcOterryIsland_HIO_c sHIO;
};
const dAcOterryIsland_HIO_c dAcOterryIsland_HIO_c::sHIO = {
    100000.0f, (GXColor){0xFF, 0xA0, 0x50, 0x00},
     350.0f, 30, 300
};

static mVec3_c sPosOffset(750.0f, 0.0f, 750.0f);

STATE_DEFINE(dAcOterryIsland_c, Wait);
STATE_DEFINE(dAcOterryIsland_c, Vanish);
STATE_DEFINE(dAcOterryIsland_c, Vacuum);
STATE_DEFINE(dAcOterryIsland_c, Smoke);

static dCcD_SrcSph sSrcSph = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_0x100, 0x111, {0, 0, 1}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {AT_TYPE_BELLOWS | AT_TYPE_WIND | AT_TYPE_BOMB, 0x311, {0, 0x0, 0x48}, 8, CUT_DIR_NONE},
     /* mObjCo */ {0xE9}},
    /* mSphInf */
    {30.0f}
};

bool dAcOterryIsland_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslTerry"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);
    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("IslTerry");
    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x120));
    TRY_CREATE(mAnmTexSrt.create(resMdl, mRes.GetResAnmTexSrt("IslTerry"), &mAllocator, nullptr, 1));

    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile("IslTerry", "dzb/IslTerry.dzb"));
    PLC *pPlc = static_cast<PLC *>(getOarcFile("IslTerry", "dat/IslTerry.plc"));
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    TRY_CREATE(!mBgW.Set(pDzb, pPlc, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
    mBgW.Lock();

    return true;
}

int dAcOterryIsland_c::create() {
    CREATE_ALLOCATOR(dAcOterryIsland_c);
    dBgS::GetInstance()->Regist(&mBgW, this);
    mVec3_c offset(sPosOffset);
    offset.rotY(mRotation.y);
    mCampfirePos = mPosition + offset;
    mLight.mPos = mCampfirePos;
    mLight.field_0x18 = true;
    mLight.mClr.r = 0xFF;
    mLight.mClr.g = 0xA0;
    mLight.mClr.b = 0x50;
    mLight.mScale = 350.0f;
    dLightEnv_c::GetPInstance()->plight_set(&mLight);
    mpPosition = &mCampfirePos;
    mStts.SetRank(13);
    mCcFire.Set(sSrcSph);
    mCcFire.SetStts(mStts);
    mCcFire.SetC(mCampfirePos + mVec3_c::Ey * 30.0f);
    mMdl.setAnm(mAnmTexSrt);
    changeState(StateID_Wait);
    mMdl.setPriorityDraw(0x1C, 9);

    mBoundingBox.Set(mVec3_c(-1600.0f, -1500.0f, -1600.0f), mVec3_c(1700.0f, 1500.0f, 1600.0f));
    mCullingDistance = 500000.0f;

    return SUCCEEDED;
}

int dAcOterryIsland_c::doDelete() {
    dLightEnv_c::GetPInstance()->plight_cut(&mLight);
    return SUCCEEDED;
}

int dAcOterryIsland_c::actorExecute() {
    executeState();

    if (dScGame_c::currentSpawnInfo.isNight()) {
        if (getStateID() == StateID_Wait || getStateID() == StateID_Vacuum) {
            dCcS::GetInstance()->Set(&mCcFire);
        }
    }
    mAnmTexSrt.play();

    return SUCCEEDED;
}

int dAcOterryIsland_c::draw() {
    drawModelType1(&mMdl);
    return SUCCEEDED;
}

void dAcOterryIsland_c::initializeState_Wait() {}
void dAcOterryIsland_c::executeState_Wait() {
    if (dScGame_c::currentSpawnInfo.isNight()) {
        mEmiter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_19_, mCampfirePos, &mRotation, &mScale, nullptr, nullptr);
        holdSound(SE_O_TAKIBI_FIRE);
        sLib::chase(&mLight.mScale, 350.0f, 50.0f);

        if (isHitBomb()) {
            changeState(StateID_Vanish);
        } else if (isHitBellows()) {
            changeState(StateID_Vacuum);
        } else if (isHitWind()) {
            changeState(StateID_Smoke);
        }
    } else {
        mLight.mScale = 0.0f;
    }
}
void dAcOterryIsland_c::finalizeState_Wait() {}

void dAcOterryIsland_c::initializeState_Vanish() {
    mStateTimer = 30;
}
void dAcOterryIsland_c::executeState_Vanish() {
    sLib::chase(&mLight.mScale, 0.0f, 50.0f);
    if (sLib::calcTimer(&mStateTimer) == 0) {
        changeState(StateID_Wait);
    }
}
void dAcOterryIsland_c::finalizeState_Vanish() {}

void dAcOterryIsland_c::initializeState_Vacuum() {}
void dAcOterryIsland_c::executeState_Vacuum() {
    holdSound(SE_O_TAKIBI_WIND);
    mAng3_c rot = mRotation;
    rot.y = cLib::targetAngleY(dAcPy_c::GetLink()->mPosition, mCampfirePos);
    mEmiter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_18_, mCampfirePos, &rot, &mScale, nullptr, nullptr);
    if (dScGame_c::currentSpawnInfo.isNight()) {
        if (!isHitBellows()) {
            changeState(StateID_Wait);
        }
    } else {
        changeState(StateID_Wait);
    }
}
void dAcOterryIsland_c::finalizeState_Vacuum() {}

void dAcOterryIsland_c::initializeState_Smoke() {
    startSound(SE_O_TAKIBI_FIRE_OFF);
    mStateTimer = 300;
}
void dAcOterryIsland_c::executeState_Smoke() {
    if (mStateTimer > 0) {
        mEmiter.holdEffect(PARTICLE_RESOURCE_ID_MAPPING_17_, mCampfirePos, &mRotation, &mScale, nullptr, nullptr);
        mStateTimer--;
    }
    sLib::chase(&mLight.mScale, 0.0f, 50.0f);
}
void dAcOterryIsland_c::finalizeState_Smoke() {}

bool dAcOterryIsland_c::isHitBomb() {
    return mCcFire.ChkTgHit() && mCcFire.ChkTgAtHitType(AT_TYPE_BOMB);
}
bool dAcOterryIsland_c::isHitBellows() {
    return mCcFire.ChkTgHit() && mCcFire.ChkTgAtHitType(AT_TYPE_BELLOWS);
}
bool dAcOterryIsland_c::isHitWind() {
    return mCcFire.ChkTgHit() && mCcFire.GetTgDamageFlags() & 0x40;
}
