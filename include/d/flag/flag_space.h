#ifndef FLAG_SPACE_H
#define FLAG_SPACE_H

#include "common.h"

struct BaseFlagSpace {
    u16 *mpFlags;
    u16 mCount;
    BaseFlagSpace(u16 *pFlags, u16 count) : mpFlags(pFlags), mCount(count) {}
};

class FlagSpace : public BaseFlagSpace {
public:
    void init(u16 *pFlags, u16 count) {
        mpFlags = pFlags;
        mCount = count;
    }
    FlagSpace(u16 *pFlags, u16 count) : BaseFlagSpace(pFlags, count) {}
    u16 *getFlagPtrChecked() const;
    u16 *getFlagPtrUnchecked() const;
    void unsetAll();
    void setAllToZero(u16 offset, u16 flagCount);
    void copyFromSaveFile2(const u16 *pSaved, u16 offset, u16 flagCount);
    void copyFromSaveFile(const u16 *pSaved, u16 offset, u16 flagCount);
    virtual void filemanagerCheck() const;
};

#endif
