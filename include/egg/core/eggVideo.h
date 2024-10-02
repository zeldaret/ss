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
    Video() : pRenderMode(0), mFlag(), mConfiguredTime(){};
    void initialize(GXRenderModeObj *, const RenderModeObjSet *);
    GXRenderModeObj *configure(GXRenderModeObj *, const RenderModeObjSet *);
    static u32 getTickPerVRetrace(u32 tvFormat);
    static u32 getTickPerVRetrace();
    static const GXRenderModeObj *getStandardRenderModeObj(const RenderModeObjSet *);

public:
    void setBlack(bool b) {
        VISetBlack(!mFlag.onBit(0));
        mFlag.toggleBit(0);
    }
};

} // namespace EGG

#endif
