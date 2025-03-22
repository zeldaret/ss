#ifndef EGG_ISCNPROC_H
#define EGG_ISCNPROC_H

#include "common.h"
#include "nw4r/g3d/g3d_scnproc.h"
#include "rvl/MEM/mem_allocator.h"

namespace EGG {

class IScnProc {
protected:
    // Unofficial name
    struct ProcData {
        IScnProc *mpThis;              // at 0x0
        nw4r::g3d::ScnProc *mpScnProc; // at 0x4
        u16 mIndex;                    // at 0x8
        u8  mFlags;                  // at 0xA
    };

    ProcData *mpDataSet; // at 0x0
    u16 mNumScnProc;     // at 0x4

public:
    IScnProc() : mpDataSet(nullptr), mNumScnProc(0) {}
    virtual void scnProcDraw(u16) = 0; // at 0x8
    virtual ~IScnProc();          // at 0xC

    void createScnProc(u16 procNum, MEMAllocator *allocator);
    void setPriorityScnProc(u16, u8, bool);
    void pushBackToScnGroup(nw4r::g3d::ScnGroup *);
    void removeFromScnGroup(nw4r::g3d::ScnGroup *);
    static void drawProcFunc(nw4r::g3d::ScnProc *, bool);
    static void drawProcFuncNoGlobalState(nw4r::g3d::ScnProc *, bool);

    u16 getNumScnProc() {
        return mNumScnProc;
    }
    ProcData *getData(u16 procIndex) {
        return &mpDataSet[procIndex];
    }
    nw4r::g3d::ScnProc *getProc(u16 procIndex) {
        return mpDataSet[procIndex].mpScnProc;
    }
};

} // namespace EGG

#endif
