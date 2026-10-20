#include "d/a/obj/d_a_obj_D3_dummy.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/bg/d_bg_s.h"
#include "d/d_sc_game.h"
#include "d/flag/dungeonflag_manager.h"
#include "f/f_base.h"
#include "m/m_vec.h"

const char *d3Stages[8] = {
    "D003_0", "D003_1", "D003_2", "D003_3", "D003_4", "D003_5", "D003_6", "D003_7",
};

SPECIAL_ACTOR_PROFILE(OBJ_D3_DUMMY, dAcOD3Dummy_c, fProfile::OBJ_D3_DUMMY, 0x15E, 0, 6);

STATE_DEFINE(dAcOD3Dummy_c, Wait);

bool dAcOD3Dummy_c::createHeap() {
    const char *oarcName = "D3MapDummy";
    const char *iconName = "dat/d3icon.bil";
    const char *names[8] = {
        "00", "01", "02", "03", "04", "05", "06", "07",
    };
    mCurrentRoomTileId = getCurrentSkyKeepRoomTileId();
    updateTileMtx();

    for (int i = 0; i < 8; i++) {
        if (i == mCurrentRoomTileId) {
            continue;
        }
        void *dzb = getOarcDZB(oarcName, names[i]);
        void *plc = getOarcPLC(oarcName, names[i]);
        if (mTileBgWs[i].Set((cBgD_t *)dzb, (PLC *)plc, cBgW::MOVE_BG_e, &mTileMtx[i], &mScale)) {
            return false;
        }
        bool result = mTileBgWs[i].InitMapStuff(&mAllocator);
        if (!result) {
            return false;
        }
        mTileBgWs[i].Set0x2F(i);
    }

    mIconData = getOarcFile(oarcName, iconName);
    if (mIconData == nullptr) {
        return false;
    }

    return true;
}

int dAcOD3Dummy_c::create() {
    CREATE_ALLOCATOR_SIZE(dAcOD3Dummy_c, 0x40000);
    // check has map
    if (DungeonflagManager::sInstance->getCounterOrFlag(2, 8)) {
        registerBgs();
    }
    mGravity = 0.f;
    mMaxFallSpeed = 0.f;
    mStateMgr.changeState(StateID_Wait);
    mBoundingBox.Set(mVec3_c(-0.f, -0.f, -0.f), mVec3_c(0.f, 0.f, 0.f));
    return SUCCEEDED;
}

int dAcOD3Dummy_c::doDelete() {
    return SUCCEEDED;
}

int dAcOD3Dummy_c::actorExecute() {
    // check has map
    if (DungeonflagManager::sInstance->getCounterOrFlag(2, 8)) {
        registerBgs();
    }
    mStateMgr.executeState();
    if (mUpdateTilePositions) {
        updateTileMtx();
        for (int i = 0; i < 8; i++) {
            if (i != mCurrentRoomTileId) {
                mTileBgWs[i].Move();
            }
        }
    }
    mUpdateTilePositions = false;
    return SUCCEEDED;
}

int dAcOD3Dummy_c::draw() {
    return SUCCEEDED;
}

void dAcOD3Dummy_c::initializeState_Wait() {}
void dAcOD3Dummy_c::executeState_Wait() {}
void dAcOD3Dummy_c::finalizeState_Wait() {}

s32 dAcOD3Dummy_c::getCurrentSkyKeepRoomTileId() {
    for (int i = 0; i < 8; i++) {
        if (dScGame_c::isCurrentStage(d3Stages[i])) {
            return i;
        }
    }
    return -1;
}

void dAcOD3Dummy_c::updateTileMtx() {
    s32 currentStageSpot = getSpotForTile(mCurrentRoomTileId);

    for (int i = 0; i < 8; i++) {
        if (i == mCurrentRoomTileId) {
            continue;
        }
        s32 spot = getSpotForTile(i);
        if (spot < 0) {
            continue;
        }
        mVec3_c vec(
            (spot % 3 - currentStageSpot % 3) * 7000.f, mPosition.y, (spot / 3 - currentStageSpot / 3) * 7000.f
        );
        mTileMtx[i].transS(vec);
    }
}

void dAcOD3Dummy_c::registerBgs() {
    if (!mHasBgsRegistered) {
        for (int i = 0; i < 8; i++) {
            if (i == mCurrentRoomTileId) {
                continue;
            }
            dBgS::GetInstance()->RegistBg(&mTileBgWs[i], this);
        }
        mHasBgsRegistered = true;
    }
}
