#include "d/a/d_a_fish_mgr_base.h"

#include "d/col/bg/d_bg_s_lin_chk.h"

STATE_VIRTUAL_DEFINE(dAcFishMgrBase_c, Wait);

int dAcFishMgrBase_c::actorCreate() {
    return dTg_c::actorCreate();
}

int dAcFishMgrBase_c::actorPostCreate() {
    f32 f = 1000.0f; // Temp needed for reg alloc?
    (void)200.0f;
    field_0x148 = f;
    mVec3_c v = mPosition;
    v.y += 20000.0f;
    if (dBgS_WtrLinChk::SetIsWater(&v, &mPosition, nullptr)) {
        field_0x14C = dBgS_WtrLinChk::GetInstance().GetLinEnd().y;
        if (mPosition.y + field_0x148 > field_0x14C) {
            field_0x150 = true;
            field_0x148 = nw4r::math::FAbs(field_0x14C - mPosition.y);
        } else {
            field_0x150 = false;
        }
    } else {
        field_0x150 = false;
    }

    f32 offset = 200.0f;
    f32 f0 = field_0x14C - field_0x148 - mPosition.y;
    if (f0 < offset) {
        f32 t = field_0x14C - mPosition.y - offset;
        if (t > offset) {
            field_0x148 = t;
            field_0x151 = false;
        } else {
            field_0x148 = t + offset;
            field_0x151 = true;
        }
    } else {
        field_0x151 = false;
    }

    return SUCCEEDED;
}

int dAcFishMgrBase_c::doDelete() {
    return dTg_c::doDelete();
}

int dAcFishMgrBase_c::draw() {
    return SUCCEEDED;
}

bool dAcFishMgrBase_c::vt_0x80() {
    return true;
}

void dAcFishMgrBase_c::initializeState_Wait() {}

void dAcFishMgrBase_c::executeState_Wait() {}

void dAcFishMgrBase_c::finalizeState_Wait() {}
