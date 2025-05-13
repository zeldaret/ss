#ifndef NAND_RESULT_TRACKER_H
#define NAND_RESULT_TRACKER_H

#include "egg/core/eggHeap.h"
#include "rvl/NAND.h"

class NandResultTracker {
public:
    static void create(EGG::Heap *heap);
    static NandResultTracker *GetInstance();
    bool isFailure(NANDResult status);

private:
    NandResultTracker *sInstance;
};

#endif
