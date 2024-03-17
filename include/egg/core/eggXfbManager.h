#ifndef EGG_XFB_MANAGER_H
#define EGG_XFB_MANAGER_H

#include "egg/core/eggXfb.h"
#include <common.h>

namespace EGG {

class XfbManager {
public:
    /* 0x0 */ Xfb *mNextXfb;    // next xfb to process?
    /* 0x4 */ Xfb *mToCopyXfb;  // current xfb to use in copyEFB
    /* 0x8 */ Xfb *mToShowXfb;  // current xfb to use in setNextFrameBuffer
    /* 0xC */ u8 mNumXfbs;      // Total number of Xfbs ever attached
    /* 0xD */ u8 mNumXfbs_Copy; // Unsure of purpose yet, but showing wont proceed until its under 3
public:
    /* 80498af0 */ bool isRegisterd(Xfb *xfb) const; // yes. this is correct spelling
    /* 80498b30 */ bool attach(Xfb *xfb);
    /* 80498c10 */ void copyEFB(bool);
    /* 80498d00 */ void setNextFrameBuffer();
};

} // namespace EGG

#endif
