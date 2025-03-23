#include "egg/gfx/eggDrawPathBase.h"

#include "common.h"
#include "egg/gfx/eggIScnProc.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/g3d/g3d_scnproc.h"

namespace EGG {

DrawPathBase::DrawPathBase() {}

void DrawPathBase::createScnProc(MEMAllocator *allocator) {
    IScnProc::createScnProc(getNumStep(), allocator);
    for (u16 i = 0; i < getNumStep(); i++) {
        getProc(i)->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_UPDATEFRAME, true);
        getProc(i)->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_WORLD, true);
        getProc(i)->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_MAT, true);
        getProc(i)->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_VTX, true);
        getProc(i)->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_VIEW, true);
    }
    nw4r::g3d::ScnProc *p = getProc(0);
    p->SetScnObjOption(nw4r::g3d::ScnObj::OPTION_DISABLE_CALC_VIEW, false);
    p->EnableScnObjCallbackExecOp(nw4r::g3d::ScnObj::EXECOP_CALC_VIEW);
    p->EnableScnObjCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_A);
    p->DisableScnObjCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_B);
    p->DisableScnObjCallbackTiming(nw4r::g3d::ScnObj::CALLBACK_TIMING_C);
    p->SetCallback(this);
}

void DrawPathBase::ExecCallback_CALC_VIEW(
    nw4r::g3d::ScnObj::Timing timing, nw4r::g3d::ScnObj *pObj, u32 param, void *pInfo
) {
    if (mFlag & 1) {
        internalResetForDraw();
    }
}

void DrawPathBase::scnProcDraw(u16 idx) {
    if (mFlag & 1) {
        internalDraw(idx);
    }
}

} // namespace EGG
