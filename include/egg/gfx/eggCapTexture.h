#ifndef EGG_CAP_TEXTURE_H
#define EGG_CAP_TEXTURE_H

#include "egg/gfx/eggCpuTexture.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

struct CopyFilter {
    u8 values[7];
};

class CapTexture : public CpuTexture {
public:
    CapTexture() {}
    CapTexture(u16 width, u16 height, GXTexFmt texFmt) : CpuTexture(width, height, texFmt) {}
    CapTexture(const GXTexObj *pObj) : CpuTexture(pObj) {}

    virtual void configure() override; // at 0xC

    void capture(u16 x, u16 y, bool upscale, int format);
    void fn_8049D9D0(u8 arg);

    void setPixModeSync(bool enable) {
        if (enable) {
            mCapFlags |= 0x10;
        } else {
            mCapFlags &= ~0x10;
        }
    }

    void onCapFlag(u8 flag) {
        mCapFlags |= flag;
    }
    void offCapFlag(u8 flag) {
        mCapFlags &= ~flag;
    }

    void setClearColor(const GXColor &clr) {
        mClearColor = clr;
    }

    void setCopyFilter(const CopyFilter &filt) {
        mCopyFilterArg = filt;
    }

private:
    /* 0x18 */ u8 mCapFlags;
    /* 0x19 */ GXColor mClearColor;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ CopyFilter mCopyFilterArg;
};

} // namespace EGG

#endif
