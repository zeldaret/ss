#include "d/a/npc/d_a_ordinary_npc.h"

#include "common.h"
#include "d/a/obj/d_a_obj_base.h"
#include "f/f_base.h"

STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Wait);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Pain);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Surprised);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Withstand);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Walk);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, LookAwaySt);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, LookAway);
STATE_VIRTUAL_DEFINE(dAcOrdinaryNpc_c, Turn);
STATE_DEFINE(dAcOrdinaryNpc_c, PreWalkTurn);

int dAcOrdinaryNpc_c::preCreate() {
    if (dAcObjBase_c::preCreate() == NOT_READY) {
        return NOT_READY;
    }

    if (!checkActorProperty(AC_PROP_0x8000000)) {
        mSpawnStoryflag = getFromParams(10, 0x7FF);
        mDespawnStoryflag = getFromParams(21, 0x7FF);

        mPathId0 = mRotation.x >> 0 & 0xFF;
        mPathId1 = mRotation.x >> 8 & 0xFF;

        mRotation.x = 0;

        if (acNpc_vt_0x1F8()) {
            deleteRequest();
            return NOT_READY;
        }

        if (acNpc_vt_0x1F4()) {
            field_0x684 = 0;
        } else {
            field_0x684 = 1;
        }

        bool couldSet = setField0xA7CFromParams();
        bool couldResolve = resolvePathId();
        if (!(couldSet && couldResolve)) {
            field_0xA7C = 0;
        }
    }

    return SUCCEEDED;
}
void dAcOrdinaryNpc_c::postCreate(MAIN_STATE_e state) {
    mStts.SetRank(13);
    if (acNpc_vt_0x1FC() && mpCc != nullptr) {
        mpCc->ClrSet();
    }

    if (field_0x7D4 != nullptr) {
        // Two functions for the unknown class of 7D4
    }

    // Really calls dAcBase_c::postCreate - But I think this makes more sense
    dAcNpc_c::postCreate(state);
    setField0xA4C(mRotationCopy);
}
int dAcOrdinaryNpc_c::preExecute() {}

bool dAcOrdinaryNpc_c::createHeap() {}

int dAcOrdinaryNpc_c::create() {}
int dAcOrdinaryNpc_c::doDelete() {}

int dAcOrdinaryNpc_c::actorExecute() {}

void dAcOrdinaryNpc_c::initializeState_Pain() {
    acOrdinaryNpc_vt_0x29C();
}
void dAcOrdinaryNpc_c::executeState_Pain() {}
void dAcOrdinaryNpc_c::finalizeState_Pain() {}

void dAcOrdinaryNpc_c::initializeState_Surprised() {
    acOrdinaryNpc_vt_0x2A0();
}
void dAcOrdinaryNpc_c::executeState_Surprised() {}
void dAcOrdinaryNpc_c::finalizeState_Surprised() {}

void dAcOrdinaryNpc_c::initializeState_Withstand() {
    acOrdinaryNpc_vt_0x2A4();
}
void dAcOrdinaryNpc_c::executeState_Withstand() {
    acOrdinaryNpc_vt_0x310();
}
void dAcOrdinaryNpc_c::finalizeState_Withstand() {}

void dAcOrdinaryNpc_c::initializeState_Wait() {
    acOrdinaryNpc_vt_0x280();
    acOrdinaryNpc_vt_0x284();
    acOrdinaryNpc_vt_0x2FC();
}
void dAcOrdinaryNpc_c::executeState_Wait() {
    acOrdinaryNpc_vt_0x314();
}
void dAcOrdinaryNpc_c::finalizeState_Wait() {}

void dAcOrdinaryNpc_c::initializeState_Walk() {
    acOrdinaryNpc_vt_0x288();
    acOrdinaryNpc_vt_0x28C();
    acOrdinaryNpc_vt_0x290();
}
void dAcOrdinaryNpc_c::executeState_Walk() {}
void dAcOrdinaryNpc_c::finalizeState_Walk() {}

void dAcOrdinaryNpc_c::initializeState_LookAwaySt() {
    acOrdinaryNpc_vt_0x2BC(-1.0f);
}
void dAcOrdinaryNpc_c::executeState_LookAwaySt() {
    acOrdinaryNpc_vt_0x30C();
}
void dAcOrdinaryNpc_c::finalizeState_LookAwaySt() {}

void dAcOrdinaryNpc_c::initializeState_LookAway() {
    acOrdinaryNpc_vt_0x2B0(-1.0f);
}
void dAcOrdinaryNpc_c::executeState_LookAway() {
    acOrdinaryNpc_vt_0x2C0();
}
void dAcOrdinaryNpc_c::finalizeState_LookAway() {
    acOrdinaryNpc_vt_0x338();
}

void dAcOrdinaryNpc_c::initializeState_Turn() {}
void dAcOrdinaryNpc_c::executeState_Turn() {}
void dAcOrdinaryNpc_c::finalizeState_Turn() {}

void dAcOrdinaryNpc_c::initializeState_PreWalkTurn() {}
void dAcOrdinaryNpc_c::executeState_PreWalkTurn() {}
void dAcOrdinaryNpc_c::finalizeState_PreWalkTurn() {}
