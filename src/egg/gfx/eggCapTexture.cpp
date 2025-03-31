#include "egg/gfx/eggCapTexture.h"

#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/GX/GXFrameBuf.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

static const GXColor sBlack = {0, 0, 0, 0};
static const CopyFilter defaultFilter = {
    {0, 0, 1, 0x3F, 0, 0, 0}
};

static const u8 copyArg2[12][2] = {};

void CapTexture::configure() {
    CpuTexture::configure();
    mCapFlags = 0;
    mClearColor = sBlack;
    field_0x20 = 0xFFFFFF;
    mCopyFilterArg = defaultFilter;
}

void CapTexture::capture(u16 x, u16 y, bool upscale, int format) {
    GXTexFmt fmt = format == -1 ? getFormat() : static_cast<GXTexFmt>(format);
    u16 width = upscale ? getWidth() * 2 : getWidth();
    u16 height = upscale ? getHeight() * 2 : getHeight();

    GXSetTexCopySrc(x, y, width, height);
    GXSetTexCopyDst(mWidth, mHeight, fmt, upscale);

    GXSetCopyFilter(
        0, copyArg2, (mCapFlags & 8) ? true : false, (mCapFlags & 8) ? mCopyFilterArg.values : defaultFilter.values
    );

    StateGX::ScopedColor colorUpdate((mCapFlags & 1) != 0);
    StateGX::ScopedAlpha alphaUpdate((mCapFlags & 2) != 0);
    GXSetZMode(true, GX_ALWAYS, (mCapFlags & 4) != 0);
    GXSetCopyClear(mClearColor, field_0x20);
    GXSetCopyClamp(3);
    StateGX::GXCopyTex_(dataPtr, true);
    GXSetCopyFilter(0, copyArg2, 0, defaultFilter.values);

    if ((mCapFlags & 0x10) != 0) {
        GXPixModeSync();
    }

    if ((mCapFlags & 0x20) != 0) {
        StateGX::invalidateTexAllGX();
    }
}

void CapTexture::fn_8049D9D0(u8 arg) {
    onCapFlag(0x8);

    CopyFilter v;
    v.values[0] = arg;
    v.values[1] = 0;
    v.values[2] = 1;
    v.values[3] = 64 - (2 * arg) - 1;
    v.values[4] = 0;
    v.values[5] = arg;
    v.values[6] = 0;
    setCopyFilter(v);
}

} // namespace EGG
