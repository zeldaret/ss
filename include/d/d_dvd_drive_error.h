#ifndef D_DVD_DRIVE_ERROR_H
#define D_DVD_DRIVE_ERROR_H

#include "common.h"
#include "egg/core/eggHeap.h"

class dDvdDriveError_c {
public:
    static void create(EGG::Heap *heap);
    void draw();
    void execute();
    void init();
    bool isError() const;

    static dDvdDriveError_c *GetInstance() {
        return sInstance;
    }

private:
    static dDvdDriveError_c *sInstance;

    /* 0x00 */ s32 mDvdDriveStatus;
    /* 0x04 */ bool mIsError;
};

#endif
