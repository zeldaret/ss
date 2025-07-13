#ifndef EGG_VIDEO_H
#define EGG_VIDEO_H

#include "common.h"
#include "egg/prim/eggBitFlag.h"
#include "rvl/GX.h" // IWYU pragma: export
#include "rvl/VI.h" // IWYU pragma: export

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

    const GXRenderModeObj *getCurrentOrFallbackRenderModeObj() const {
        if (pRenderMode == nullptr) {
            return &GXNtsc480Int;
        }
        return pRenderMode;
    }

public:
    void setBlack(bool b) {
        VISetBlack(!mFlag.onBit(0));
        mFlag.toggleBit(0);
    }
};

} // namespace EGG

#endif
