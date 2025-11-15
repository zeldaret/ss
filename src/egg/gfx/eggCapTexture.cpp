#include "egg/gfx/eggCapTexture.h"

#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/GX/GXFrameBuf.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTypes.h"

namespace {
static const GXColor DEFUALT_CLEAR_COLOR = {0, 0, 0, 0};
static const u8 SAMPLE_PATTERN_OFF[12][2] = {};

} // namespace
namespace EGG {

const CopyFilter CapTexture::VFILTER_BLUR = {
    {0x15, 0, 0, 0x16, 0, 0x15, 0}
};

const CopyFilter CapTexture::VFILTER_OFF = {
    {0, 0, 1, 0x3F, 0, 0, 0}
};

void CapTexture::configure() {
    CpuTexture::configure();
    mCapFlags = 0;
    mClearColor = DEFUALT_CLEAR_COLOR;
    field_0x20 = 0xFFFFFF;
    mCopyFilterArg = VFILTER_OFF;
}

void CapTexture::capture(u16 x, u16 y, bool upscale, int format) {
    GXTexFmt fmt = format == -1 ? getFormat() : static_cast<GXTexFmt>(format);
    u16 width = upscale ? getWidth() * 2 : getWidth();
    u16 height = upscale ? getHeight() * 2 : getHeight();

    GXSetTexCopySrc(x, y, width, height);
    GXSetTexCopyDst(mWidth, mHeight, fmt, upscale);

    GXSetCopyFilter(
        0, SAMPLE_PATTERN_OFF, (mCapFlags & 8) ? true : false,
        (mCapFlags & 8) ? mCopyFilterArg.values : VFILTER_OFF.values
    );

    StateGX::ScopedColor colorUpdate((mCapFlags & 1) != 0);
    StateGX::ScopedAlpha alphaUpdate((mCapFlags & 2) != 0);
    GXSetZMode(true, GX_ALWAYS, (mCapFlags & 4) != 0);
    GXSetCopyClear(mClearColor, field_0x20);
    GXSetCopyClamp(3);
    StateGX::GXCopyTex_(dataPtr, true);
    GXSetCopyFilter(0, SAMPLE_PATTERN_OFF, 0, VFILTER_OFF.values);

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
