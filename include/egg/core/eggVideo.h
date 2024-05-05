#ifndef EGG_VIDEO_H
#define EGG_VIDEO_H

#include "egg/prim/eggBitFlag.h"
#include "rvl/GX.h"
#include "rvl/VI.h"
#include <common.h>

namespace EGG {

class Video {
public:
    struct RenderModeObjSet {
        const GXRenderModeObj *table[10];
    };

public:
    /* 0x00 */ GXRenderModeObj *pRenderMode;
    /* 0x04 */ TBitFlag<u8> mFlag;
    /* 0x08 */ u32 mConfiguredTime;

public:
    /*  inline  */ Video() : pRenderMode(0), mFlag(), mConfiguredTime(){};
    /* 80498690 */ void initialize(GXRenderModeObj *, const RenderModeObjSet *);
    /* 804986f0 */ GXRenderModeObj *configure(GXRenderModeObj *, const RenderModeObjSet *);
    /* 80498800 */ static u32 getTickPerVRetrace(u32 tvFormat);
    /* 80498860 */ static u32 getTickPerVRetrace();
    /* 80498890 */ static const GXRenderModeObj *getStandardRenderModeObj(const RenderModeObjSet *);

public:
    /*  inline  */ void setBlack(bool b) {
        VISetBlack(!mFlag.onBit(0));
        mFlag.toggleBit(0);
    }
};

} // namespace EGG

#endif
