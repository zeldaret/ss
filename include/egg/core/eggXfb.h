#ifndef EGG_XFB_H
#define EGG_XFB_H

#include "egg/core/eggHeap.h"
#include <common.h>

namespace EGG {

class Xfb {
public:
    // Non-Official assumed based on actions
    enum XFBState {
        XFB_UNPROCESSED,
        XFB_COPIED,
        XFB_SHOWN,
    };

public:
    /* 0x00 */ u16 mWidth;
    /* 0x02 */ u16 mHeight;
    /* 0x04 */ u8 *mBuffer;
    /* 0x08 */ XFBState mState;
    /* 0x0C */ Xfb *mNext; // idk the actual order, but they always link to each other
    /* 0x10 */ Xfb *mPrev; // idk the actual order, but they always link to each other
public:
    /* 804989e0 */ void init(u16 width, u16 height, Heap *heap);
    /* 80498a60 */ Xfb(Heap *heap);
    /* 80498ad0 */ static u32 calcBufferSize(u16 width, u16 height);
};

} // namespace EGG

#endif
