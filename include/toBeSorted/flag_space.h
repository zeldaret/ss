#pragma once

#include "types.h"

struct BaseFlagSpace {
    u16 *mpFlags;
    u16 mCount;
    BaseFlagSpace(u16 *pFlags, u16 count): mpFlags(pFlags), mCount(count) {}
};

class FlagSpace: public BaseFlagSpace {
    public:
    void init(u16 *pFlags, u16 count) {
        mpFlags = pFlags;
        mCount = count;
    }
    FlagSpace(u16 *pFlags, u16 count): BaseFlagSpace(pFlags, count) {}
    u16* getFlagPtrChecked();
    u16* getFlagPtrUnchecked();
    void unsetAll();
    void setAllToZero(u16 offset, u16 flagCount);
    void copyFromSaveFile2(u16 *pSaved, u16 offset, u16 flagCount);
    void copyFromSaveFile(u16 *pSaved, u16 offset, u16 flagCount);
    virtual void filemanagerCheck();
};