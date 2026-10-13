#include "d/a/obj/d_a_obj_roulette_island_c.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/a/obj/d_a_obj_fortune_ring.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_rumble.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_mini_game.h"
#include "f/f_base.h"
#include "m/m_vec.h"

SPECIAL_ACTOR_PROFILE(OBJ_ROULETTE_ISLAND_C, dAcOrouletteIslandC_c, fProfile::OBJ_ROULETTE_ISLAND_C, 0x1CA, 0, 2);

dAcOrouletteIslandC_c *dAcOrouletteIslandC_c::sInstance;

STATE_DEFINE(dAcOrouletteIslandC_c, Wait);
STATE_DEFINE(dAcOrouletteIslandC_c, Play);

void dAcOrouletteIslandC_c::rideCallback(dBgW *pBgW, dAcObjBase_c *pThis, dAcObjBase_c *pActor) {
    if (pActor->isActorPlayer() && pThis) {
        static_cast<dAcOrouletteIslandC_c *>(pThis)->mbPlayerOnIsland = true;

        if (!StoryflagManager::sInstance->getFlag(STORYFLAG_FUN_FUN_ISLAND_DISCOVERED)) {
            StoryflagManager::sInstance->setFlag(STORYFLAG_FUN_FUN_ISLAND_DISCOVERED);
        }
    }
}

dAcOrouletteIslandC_c::dAcOrouletteIslandC_c() : mStateMgr(*this) {
    sInstance = this;
}
dAcOrouletteIslandC_c::~dAcOrouletteIslandC_c() {
    sInstance = nullptr;
}

bool dAcOrouletteIslandC_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslRouMain"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);

    TRY_CREATE(mMdl.create(mRes.GetResMdl("IslRouMain"), &mAllocator, 0x120));
    TRY_CREATE(mMdl_s.create(mRes.GetResMdl("IslRouMain_s"), &mAllocator, 0x120));

    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile("IslRouMain", "dzb/IslRouMain.dzb"));
    PLC *pPLC = static_cast<PLC *>(getOarcFile("IslRouMain", "dat/IslRouMain.plc"));
    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl_s.setLocalMtx(mWorldMtx);
    mMdl.setScale(mScale);
    mMdl_s.setScale(mScale);
    TRY_CREATE(!mBgW.Set(pDzb, pPLC, 1, &mWorldMtx, &mScale));
    mBgW.Lock();
    return true;
}

int dAcOrouletteIslandC_c::create() {
    CREATE_ALLOCATOR(dAcOrouletteIslandC_c);

    dBgS::GetInstance()->Regist(&mBgW, this);

    changeState(StateID_Wait);

    mMdl.setPriorityDraw(0x1C, 9);
    mMdl_s.setPriorityDraw(0x22, 9);

    mVec3_c min, max;
    mMdl.getBounds(&min, &max);
    mBoundingBox.Set(min, max);
    mCullingDistance = 500000.0f;

    mBgW.SetRideCallback(rideCallback);

    return SUCCEEDED;
}

int dAcOrouletteIslandC_c::doDelete() {
    return SUCCEEDED;
}

int dAcOrouletteIslandC_c::actorExecute() {
    executeState();
    return SUCCEEDED;
}

int dAcOrouletteIslandC_c::draw() {
    drawModelType1(&mMdl);
    drawModelType1(&mMdl_s);
    return SUCCEEDED;
}

void dAcOrouletteIslandC_c::initializeState_Wait() {
    mbStartPlay = false;
}
void dAcOrouletteIslandC_c::executeState_Wait() {
    if (mbStartPlay) {
        changeState(StateID_Play);
    }
}
void dAcOrouletteIslandC_c::finalizeState_Wait() {}

void dAcOrouletteIslandC_c::initializeState_Play() {
    field_0x5BA = false;
    mbPlayerOnIsland = false;
}
void dAcOrouletteIslandC_c::executeState_Play() {
    if (mbPlayerOnIsland) {
        dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
        dLytMiniGame_c::GetInstance()->fn_80295900(0);
        dAcOfortuneRing_c::getInstance()->endGame(0);
        changeState(StateID_Wait);
    } else if (field_0x5BA) {
        changeState(StateID_Wait);
    }
}
void dAcOrouletteIslandC_c::finalizeState_Play() {}

dAcOrouletteIslandC_c *dAcOrouletteIslandC_c::getInstance() {
    return sInstance;
}
