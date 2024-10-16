#ifndef EGG_XFB_MANAGER_H
#define EGG_XFB_MANAGER_H

#include "common.h"
#include "egg/core/eggXfb.h"


namespace EGG {

class XfbManager {
public:
    /* 0x0 */ Xfb *mNextXfb;    // next xfb to process?
    /* 0x4 */ Xfb *mToCopyXfb;  // current xfb to use in copyEFB
    /* 0x8 */ Xfb *mToShowXfb;  // current xfb to use in setNextFrameBuffer
    /* 0xC */ u8 mNumXfbs;      // Total number of Xfbs ever attached
    /* 0xD */ u8 mNumXfbs_Copy; // Unsure of purpose yet, but showing wont proceed until its under 3
public:
    bool isRegisterd(Xfb &xfb) const; // yes. this is correct spelling
    bool attach(Xfb *xfb);
    void copyEFB(bool);
    void setNextFrameBuffer();
};

} // namespace EGG

#endif
