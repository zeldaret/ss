#include "d/a/obj/d_a_obj_roulette_island_r.h"

#include "common.h"
#include "d/a/obj/d_a_obj_fortune_ring.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_rumble.h"
#include "d/d_stage.h"
#include "d/flag/storyflag_manager.h"
#include "d/lyt/d_lyt_mini_game.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "s/s_Math.h"

#include <climits>

SPECIAL_ACTOR_PROFILE(OBJ_ROULETTE_ISLAND_R, dAcOrouletteIslandR_c, fProfile::OBJ_ROULETTE_ISLAND_R, 0x1CB, 0, 2);

dAcOrouletteIslandR_c *dAcOrouletteIslandR_c::sInstance;

STATE_DEFINE(dAcOrouletteIslandR_c, Wait);
STATE_DEFINE(dAcOrouletteIslandR_c, Move);
STATE_DEFINE(dAcOrouletteIslandR_c, Reduce);
STATE_DEFINE(dAcOrouletteIslandR_c, End);

void dAcOrouletteIslandR_c::rideCallback(dBgW *pBgW, dAcObjBase_c *pThis, dAcObjBase_c *pActor) {
    if (pActor->isActorPlayer() && pThis) {
        static_cast<dAcOrouletteIslandR_c *>(pThis)->mbPlayerOnIsland = true;

        if (!StoryflagManager::sInstance->getFlag(STORYFLAG_FUN_FUN_ISLAND_DISCOVERED)) {
            StoryflagManager::sInstance->setFlag(STORYFLAG_FUN_FUN_ISLAND_DISCOVERED);
        }
    }
}
dAcOrouletteIslandR_c::dAcOrouletteIslandR_c() : mStateMgr(*this) {
    sInstance = this;
}
dAcOrouletteIslandR_c::~dAcOrouletteIslandR_c() {
    sInstance = nullptr;
}

bool dAcOrouletteIslandR_c::createHeap() {
    mRes = nw4r::g3d::ResFile(getOarcResFile("IslRouRot"));
    dStage_c::bindStageResToFile(&mRes);
    dStage_c::bindSkyCmnToResFile(&mRes);

    nw4r::g3d::ResMdl resMdl = mRes.GetResMdl("IslRouRot");
    TRY_CREATE(mMdl.create(resMdl, &mAllocator, 0x324));
    TRY_CREATE(mAnmTexSrt.create(resMdl, mRes.GetResAnmTexSrt("IslRouRot"), &mAllocator, nullptr, 1));

    TRY_CREATE(mMdl_s.create(mRes.GetResMdl("IslRouRot_s"), &mAllocator, 0x324));

    cBgD_t *pDzb = static_cast<cBgD_t *>(getOarcFile("IslRouRot", "dzb/IslRouRot.dzb"));
    PLC *pPLC = static_cast<PLC *>(getOarcFile("IslRouRot", "dat/IslRouRot.plc"));
    updateMatrix();
    mMdl.setScale(mScale);
    mMdl_s.setScale(mScale);
    TRY_CREATE(!mBgW.Set(pDzb, pPLC, cBgW::MOVE_BG_e, &mWorldMtx, &mScale));

    return true;
}

int dAcOrouletteIslandR_c::create() {
    CREATE_ALLOCATOR(dAcOrouletteIslandR_c);

    mBgW.SetCrrFunc(dBgS_MoveBGProc_Typical);
    dBgS::GetInstance()->Regist(&mBgW, this);

    mMdl.setAnm(mAnmTexSrt);

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

int dAcOrouletteIslandR_c::doDelete() {
    return SUCCEEDED;
}

int dAcOrouletteIslandR_c::actorExecute() {
    executeState();
    calcVelocity();

    mPosition += mVelocity;
    mPosition += mStts.GetCcMove();

    updateMatrix();
    mMdl.setLocalMtx(mWorldMtx);
    mMdl_s.setLocalMtx(mWorldMtx);

    mAnmTexSrt.play();

    mBgW.Move();

    return SUCCEEDED;
}

int dAcOrouletteIslandR_c::draw() {
    drawModel(&mMdl);
    drawModel(&mMdl_s);
    return SUCCEEDED;
}
void dAcOrouletteIslandR_c::initializeState_Wait() {
    mbMove = false;
}
void dAcOrouletteIslandR_c::executeState_Wait() {
    if (mbMove) {
        changeState(StateID_Move);
    }
}
void dAcOrouletteIslandR_c::finalizeState_Wait() {
    field_0x5E7 = false;
    mbPlayerOnIsland = false;
    mRotationSpeed = 750;
}

void dAcOrouletteIslandR_c::initializeState_Move() {}
void dAcOrouletteIslandR_c::executeState_Move() {
    if (mbPlayerOnIsland) {
        changeState(StateID_End);
        return;
    }

    if (field_0x5E7) {
        changeState(StateID_Wait);
        return;
    }

    mRotation.y += mRotationSpeed;
    if (mbReduceSpeed) {
        changeState(StateID_Reduce);
    }
}
void dAcOrouletteIslandR_c::finalizeState_Move() {}

void dAcOrouletteIslandR_c::initializeState_Reduce() {
    mRotationSpeedTarget = mRotationSpeed - 60;
}
void dAcOrouletteIslandR_c::executeState_Reduce() {
    sLib::addCalcAngle(&mRotationSpeed, mRotationSpeedTarget, 4, 0x100, 0x10);

    // TODO: mVal access??
    mRotation.y.mVal += mRotationSpeed;

    if (mRotationSpeed == mRotationSpeedTarget) {
        changeState(StateID_Move);
    }
}
void dAcOrouletteIslandR_c::finalizeState_Reduce() {
    mbReduceSpeed = false;
}

void dAcOrouletteIslandR_c::initializeState_End() {
    dRumble_c::start(dRumble_c::sRumblePreset3, dRumble_c::FLAG_SLOT0 | dRumble_c::FLAG_ACTIVE);
}
void dAcOrouletteIslandR_c::executeState_End() {
    s32 tile;

    s16 diff = getXZAngleToPlayer();
    diff = diff - mRotation.y;
    switch (diff) {
        case 0x0000:               break;
        case 0x0001 ... 0x0FFF:    tile = 0; break;

        case 0x1000:               break;
        case 0x1001 ... 0x1FFF:    tile = 3; break;

        case 0x2000:               break;
        case 0x2001 ... 0x2FFF:    tile = 1; break;

        case 0x3000:               break;
        case 0x3001 ... 0x3FFF:    tile = 2; break;

        case 0x4000:               break;
        case 0x4001 ... 0x4FFF:    tile = 0; break;

        case 0x5000:               break;
        case 0x5001 ... 0x5FFF:    tile = 1; break;

        case 0x6000:               break;
        case 0x6001 ... 0x6FFF:    tile = 6; break;

        case 0x7000:               break;
        case 0x7001 ... INT_MAX:   tile = 4; break;

        case INT_MIN ... - 0x7001: tile = 6; break;
        case -0x7000:              break;

        case -0x6FFF ... - 0x6001: tile = 1; break;
        case -0x6000:              break;

        case -0x5FFF ... - 0x4001: tile = 2; break;
        case -0x4000:              break;

        case -0x3FFF ... - 0x3001: tile = 0; break;
        case -0x3000:              break;

        case -0x2FFF ... - 0x1001: tile = 1; break;
        case -0x1000:              break;

        case -0x0FFF ... - 0x0001: tile = 3; break;
    }
    dLytMiniGame_c::GetInstance()->fn_80295900(tile);
    dAcOfortuneRing_c::getInstance()->endGame(tile);
    changeState(StateID_Wait);
}
void dAcOrouletteIslandR_c::finalizeState_End() {}

dAcOrouletteIslandR_c *dAcOrouletteIslandR_c::getInstance() {
    return sInstance;
}
