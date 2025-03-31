#include "egg/gfx/eggIScnProc.h"

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggG3DUtility.h"
#include "egg/gfx/eggGlobalDrawState.h"
#include "egg/gfx/eggStateGX.h"
#include "nw4r/g3d/g3d_scnobj.h"
#include "nw4r/math/math_types.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

IScnProc::~IScnProc() {
    if (mpDataSet != nullptr) {
        for (int i = 0; i < getNumScnProc(); i++) {
            mpDataSet[i].mpScnProc->Destroy();
            mpDataSet[i].mpScnProc = nullptr;
        }

        delete[] mpDataSet;
        mpDataSet = nullptr;
    }
}

void IScnProc::createScnProc(u16 procNum, MEMAllocator *pAllocator) {
    mNumScnProc = procNum;
    mpDataSet = new ProcData[mNumScnProc];
    nw4r::math::MTX34 mtx;
    PSMTXIdentity(mtx);

    for (u16 i = 0; i < getNumScnProc(); i++) {
        u32 sp8;
        MEMAllocator *allocator = (pAllocator == nullptr) ? G3DUtility::getAllocator() : pAllocator;

        mpDataSet[i].mpScnProc = nw4r::g3d::ScnProc::Construct(allocator, &sp8, IScnProc::drawProcFunc, true, false, 0);

        mpDataSet[i].mpScnProc->SetUserData(&mpDataSet[i]);
        mpDataSet[i].mpScnProc->SetMtx(nw4r::g3d::ScnObj::MTX_LOCAL, mtx);

        mpDataSet[i].mIndex = i;
        mpDataSet[i].mpThis = this;
        mpDataSet[i].mFlags = 1 | 2;

        setPriorityScnProc(i, i, true);
    }
}

void IScnProc::setPriorityScnProc(u16 procIndex, u8 priority, bool bUseOpa) {
    nw4r::g3d::ScnProc *pScnProc = getData(procIndex)->mpScnProc;
    if (bUseOpa) {
        pScnProc->SetDrawProc(IScnProc::drawProcFunc, true, false);
        pScnProc->SetPriorityDrawOpa(priority);
        mpDataSet[procIndex].mFlags |= 1;
    } else {
        pScnProc->SetDrawProc(IScnProc::drawProcFunc, false, true);
        pScnProc->SetPriorityDrawXlu(priority);
        mpDataSet[procIndex].mFlags &= ~1;
    }
}

void IScnProc::pushBackToScnGroup(nw4r::g3d::ScnGroup *pScnGroup) {
    for (u16 i = 0; i < getNumScnProc(); i++) {
        bool is_push_back = pScnGroup->Insert(pScnGroup->Size(), getData(i)->mpScnProc);
    }
}

void IScnProc::removeFromScnGroup(nw4r::g3d::ScnGroup *pScnGroup) {
    for (u16 i = 0; i < getNumScnProc(); i++) {
        bool is_remove = pScnGroup->Remove(getData(i)->mpScnProc);
    }
}

void IScnProc::drawProcFunc(nw4r::g3d::ScnProc *pScnProc, bool b) {
    GlobalDrawState::setDrawSettingGX(b, false);
    StateGX::s_commandFlag &= ~0xF;
    drawProcFuncNoGlobalState(pScnProc, b);
    StateGX::resetGXCache();

    GlobalDrawState::setDrawSettingGX(b, (StateGX::s_commandFlag & 0xF) != 0);
}

void IScnProc::drawProcFuncNoGlobalState(nw4r::g3d::ScnProc *pScnProc, bool b) {
    ProcData *dat = static_cast<ProcData *>(pScnProc->GetUserData());
    dat->mpThis->scnProcDraw(dat->mIndex);
}

} // namespace EGG
