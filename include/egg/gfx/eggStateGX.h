#ifndef EGG_STATE_GX_H
#define EGG_STATE_GX_H

#include "common.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace EGG {

class StateGX {
public:

        struct ScopedColor
        {
            ScopedColor(bool x)
            {
                old = GXSetColorUpdate_(x);
            }

            ~ScopedColor()
            {
                GXSetColorUpdate_(old);
            }

            bool old;
        };

        struct ScopedAlpha
        {
            ScopedAlpha(bool x)
            {
                old = GXSetAlphaUpdate_(x);
            }

            ~ScopedAlpha()
            {
                GXSetAlphaUpdate_(old);
            }

            bool old;
        };

        struct ScopedDither
        {
            ScopedDither(bool x)
            {
                old = GXSetDither_(x);
            }

            ~ScopedDither()
            {
                GXSetDither_(old);
            }

            bool old;
        };

    static GXColor& getDefaultTexColor() { return sDefaultTexColor; }

    static void initialize(u16, u16, GXColor, GXPixelFmt);
    static void frameInit();
    static void textureInit(); // Guess for 804b4810

    static void resetGX();
    static void resetVtx();
    static void resetColorChannel();
    static void resetIndirect();
    static void resetTexture();
    static void resetTexGen();
    static void resetTev();
    static void resetPE();

    static void resetGXCache();

    static void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt16 zFmt);

    static void invalidateTexAllGX();

    static bool GXSetColorUpdate_(bool);
    static bool GXSetAlphaUpdate_(bool);
    static bool GXSetDither_(bool);
    static bool GXSetDstAlpha_(bool, u8);

    static bool GXSetColorUpdate(bool);
    static bool GXSetAlphaUpdate(bool);
    static bool GXSetDither();
    static void GXCopyTex(void *data, bool);
    static bool GXSetDstAlpha();
    // Unk func here
    static void GXSetProjection(Mtx44, int);
    static void GXSetProjectionv(const f32 *);
    static void GXSetViewport(f32, f32, f32, f32, f32, f32);
    static void GZSetScissor(u32, u32, u32, u32);
    static void GZSetScissorBoxOffset(s32, s32);


    static u16 s_commandFlag;
    static u16 s_flag;
    static GXColor sDefaultTexColor;
};

} // namespace EGG

#endif
