#ifndef EGG_GFXENGINE_H
#define EGG_GFXENGINE_H

#include <common.h>
#include <rvl/GX.h>

namespace EGG {

namespace GfxEngine {

struct Configuration {
    /* 0x00 */ u16 efbWidth;
    /* 0x02 */ u16 efbHeight;
    /* 0x04 */ GXColor clearColor;
    /* 0x08 */ u32 field_0x08;
    /* 0x0C */ u16 field_0x0C;
    /* 0x0E */ u16 field_0x0E;
    /* 0x10 */ u32 bufferSize;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u8 UNK_0x18[4];
    Configuration();
    virtual ~Configuration() {}
};

} // namespace GfxEngine

} // namespace EGG

#endif
