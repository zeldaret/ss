#include "d/a/obj/d_a_obj_treasure_island_b.h"

#include "common.h"
#include "d/a/d_a_bird.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_plc.h"
#include "d/col/bg/d_bg_s.h"
#include "d/col/bg/d_bg_w.h"
#include "d/col/c/c_cc_d.h"
#include "d/col/cc/d_cc_s.h"
#include "d/d_stage.h"
#include "d/flag/sceneflag_manager.h"
#include "d/snd/d_snd_wzsound.h"
#include "f/f_base.h"
#include "m/m_vec.h"
#include "nw4r/g3d/res/g3d_resfile.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "toBeSorted/d_emitter.h"

SPECIAL_ACTOR_PROFILE(OBJ_TREASURE_ISLAND_B, dAcOtreasureIslandB_c, fProfile::OBJ_TREASURE_ISLAND_B, 0x206, 0, 2);

STATE_DEFINE(dAcOtreasureIslandB_c, Wait);
STATE_DEFINE(dAcOtreasureIslandB_c, CoverBreak);

static dCcD_SrcSph sSrcSph = {
    /* mObjInf */
    {/* mObjAt */ {AT_TYPE_NONE, 0, {0, 0, 0}, 0, 0, 0, 0, CUT_DIR_NONE, 0},
     /* mObjTg */ {AT_TYPE_SPIRAL_CHARGE, 0x311, {0, 0x0, 0x407}, 0, CUT_DIR_NONE},
     /* mObjCo */ {0}},
    /* mSphInf */
    {550.0f}
};

static mVec3_c sRockOffset(0.0f, 240.0f, 0.0f);

void float_ordering() {
    100000.0f;
}

bool dAcOtreasureIslandB_c::createHeap() {
    cBgD_t *pDzb;
    PLC *pPLC;
    {
        mRes = nw4r::g3d::ResFile(getOarcResFile("IslTreI"));
        dStage_c::bindStageResToFile(&mRes);
        dStage_c::bindSkyCmnToResFile(&mRes);

        TRY_CREATE(mMdl[ISLAND].create(mRes.GetResMdl("IslTreI"), &mAllocator, 0x120));

        pDzb = static_cast<cBgD_t *>(getOarcFile("IslTreI", "dzb/IslTreI.dzb"));
        pPLC = static_cast<PLC *>(getOarcFile("IslTreI", "dat/IslTreI.plc"));
        updateMatrix();
        mMdl[ISLAND].setLocalMtx(mWorldMtx);
        TRY_CREATE(!mBgW[ISLAND].Set(pDzb, pPLC, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
        mBgW[ISLAND].Lock();
    }
    {
        mRes = nw4r::g3d::ResFile(getOarcResFile("IslTreIRock"));

        TRY_CREATE(mMdl[ROCK].create(mRes.GetResMdl("IslTreIRock"), &mAllocator, 0x120));

        pDzb = static_cast<cBgD_t *>(getOarcFile("IslTreIRock", "dzb/IslTreIRock.dzb"));
        pPLC = static_cast<PLC *>(getOarcFile("IslTreIRock", "dat/IslTreIRock.plc"));
        updateMatrix();
        mMdl[ROCK].setLocalMtx(mWorldMtx);
        TRY_CREATE(!mBgW[ROCK].Set(pDzb, pPLC, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));
        mBgW[ROCK].Lock();
    }

    for (int i = 0; i < 2; ++i) {
        TRY_CREATE(mBgW[i].InitMapStuff(&mAllocator));
    }

    return true;
}

int dAcOtreasureIslandB_c::create() {
    CREATE_ALLOCATOR(dAcOtreasureIslandB_c);
    for (int i = 0; i < 2; ++i) {
        dBgS::GetInstance()->Regist(&mBgW[i], this);
    }
    for (int i = 0; i < 2; ++i) {
        dBgS::GetInstance()->RegistBg(&mBgW[i], this);
    }
    mStts.SetRank(13);
    mCcRock.Set(sSrcSph);
    mCcRock.SetStts(mStts);
    mRockBrokenFlag = getFromParams(0, 0xFF);
    if (SceneflagManager::sInstance->checkBoolFlag(mRoomID, mRockBrokenFlag)) {
        changeState(StateID_CoverBreak);
    } else {
        changeState(StateID_Wait);
    }
    for (int i = 0; i < 2; ++i) {
        mMdl[i].setPriorityDraw(0x1C, 9);
    }

    mBoundingBox.Set(mVec3_c(-1200.0f, -1200.0f, -1100.0f), mVec3_c(1200.0f, 1600.0f, 1200.0f));
    mCullingDistance = 500000.0f;

    return SUCCEEDED;
}

int dAcOtreasureIslandB_c::doDelete() {
    for (int i = 0; i < 2; ++i) {
        dBgS::GetInstance()->UnRegist(&mBgW[i]);
    }
    return SUCCEEDED;
}

int dAcOtreasureIslandB_c::actorExecute() {
    executeState();

    if (getStateID() != StateID_CoverBreak) {
        mVec3_c pos = mPosition + sRockOffset;
        mCcRock.SetC(pos);
        dCcS::GetInstance()->Set(&mCcRock);
    }
    return SUCCEEDED;
}

int dAcOtreasureIslandB_c::draw() {
    drawModel(&mMdl[ISLAND]);
    if (getStateID() != StateID_CoverBreak) {
        drawModel(&mMdl[ROCK]);
    }
    return SUCCEEDED;
}

void dAcOtreasureIslandB_c::initializeState_Wait() {}
void dAcOtreasureIslandB_c::executeState_Wait() {
    if (dAcPy_c::GetLink()->getRidingActorType() == dAcPy_c::RIDING_LOFTWING) {
        if (dBird_c::getInstance()->getCameraState() == 5) {
            dBgS::GetInstance()->Release(&mBgW[ROCK]);
        } else {
            dBgS::GetInstance()->Regist(&mBgW[ROCK], this);
        }
    }

    if (shouldBreak()) {
        changeState(StateID_CoverBreak);
    }
}
void dAcOtreasureIslandB_c::finalizeState_Wait() {
    startSound(SE_RockSkB_BREAK);
    dJEffManager_c::spawnEffect(PARTICLE_RESOURCE_ID_MAPPING_817_, mPosition, nullptr, &mScale, nullptr, nullptr, 0, 0);
    dEmitterBase_c *pEmitter = dJEffManager_c::spawnEffect(
        PARTICLE_RESOURCE_ID_MAPPING_819_, mPosition, nullptr, &mScale, nullptr, nullptr, 0, 0
    );
    if (pEmitter != nullptr) {
        pEmitter->bindShpEmitter(dJEffManager_c::IslTreIRock, true);
    }

    SceneflagManager::sInstance->setFlag(mRoomID, mRockBrokenFlag);
}

void dAcOtreasureIslandB_c::initializeState_CoverBreak() {
    dBgS::GetInstance()->Release(&mBgW[ROCK]);
    dBgS::GetInstance()->UnRegist(&mBgW[ROCK]);
}
void dAcOtreasureIslandB_c::executeState_CoverBreak() {}
void dAcOtreasureIslandB_c::finalizeState_CoverBreak() {}

bool dAcOtreasureIslandB_c::shouldBreak() {
    return mCcRock.ChkTgHit() && mCcRock.ChkTgAtHitType(AT_TYPE_SPIRAL_CHARGE);
}
