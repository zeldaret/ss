#ifndef EGG_CAP_TEXTURE_H
#define EGG_CAP_TEXTURE_H

#include "egg/gfx/eggCpuTexture.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

struct CopyFilter {
    u8 values[7];
};

class CapTexture : public CpuTexture {
    virtual void configure() override; // at 0xC

    void capture(u16 x, u16 y, bool upscale, int format);

private:
    /* 0x18 */ u8 mCapFlags;
    /* 0x19 */ GXColor mClearColor;
    /* 0x20 */ u32 field_0x20;
    /* 0x24 */ CopyFilter mCopyFilterArg;
};

} // namespace EGG

#endif
