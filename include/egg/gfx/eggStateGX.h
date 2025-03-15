#ifndef EGG_STATE_GX_H
#define EGG_STATE_GX_H

#include "common.h"
#include "rvl/GX.h" // IWYU pragma: export

namespace EGG {

class StateGX {
public:
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

    static GXBool GXSetColorUpdate_(GXBool);
    static GXBool GXSetAlphaUpdate_(GXBool);
    static GXBool GXSetDither_(GXBool);
    static GXBool GXSetDstAlpha_(GXBool, u8);

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
};

} // namespace EGG

#endif
