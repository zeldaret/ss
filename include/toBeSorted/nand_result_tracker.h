#ifndef NAND_RESULT_TRACKER_H
#define NAND_RESULT_TRACKER_H

#include "rvl/NAND.h"

class NandResultTracker {
    public:
        static NandResultTracker *GetInstance();
        bool isFailure(NANDResult status);
    
    private:
        NandResultTracker *sInstance;
    };

#endif
