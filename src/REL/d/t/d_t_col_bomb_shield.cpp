#include "d/t/d_t_col_bomb_shield.h"

#include "common.h"
#include "d/a/d_a_player.h"
#include "d/a/obj/d_a_obj_base.h"
#include "d/col/cc/d_cc_s.h"
#include "m/m_mtx.h"
#include "rvl/MTX/mtx.h"

SPECIAL_ACTOR_PROFILE(COL_BOMSLD, dTgColBombShield, fProfile::COL_BOMSLD, 0x12, 0, 0);

STATE_DEFINE(dTgColBombShield, Wait);

bool dTgColBombShield::createHeap() {
    return true;
}

int dTgColBombShield::create() {
    0.0f; // cool
    CREATE_ALLOCATOR(dTgColBombShield);
    static const f32 offset = 0.1f;
    mScale.x += offset;
    mScale.y += offset;
    mScale.z += offset;
    mColMtx.transS(mPosition.x, mPosition.y + mScale.y * 0.5f - 0.05f, mPosition.z);
    mColMtx.ZXYrotM(mRotation.x, mRotation.y, mRotation.z);
    mMtx_c tmp;
    PSMTXScale(tmp, mScale.x, mScale.y, mScale.z);
    mColMtx += tmp;
    mStateMgr.changeState(StateID_Wait);
    return SUCCEEDED;
}

int dTgColBombShield::doDelete() {
    return SUCCEEDED;
}

int dTgColBombShield::actorExecute() {
    if (dAcPy_c::LINK != nullptr && roomid == dAcPy_c::LINK->roomid) {
        dCcS::GetInstance()->SetArea(&mColMtx);
    }
    return SUCCEEDED;
}

int dTgColBombShield::draw() {
    return SUCCEEDED;
}

void dTgColBombShield::initializeState_Wait() {}
void dTgColBombShield::executeState_Wait() {}
void dTgColBombShield::finalizeState_Wait() {}
