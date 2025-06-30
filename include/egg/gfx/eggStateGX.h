#ifndef EGG_STATE_GX_H
#define EGG_STATE_GX_H

#include "common.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

#include "rvl/GX.h" // IWYU pragma: export

namespace EGG {

class StateGX {
public:
    struct ScopedColor {
        ScopedColor(bool x) {
            old = GXSetColorUpdate_(x);
        }

        ~ScopedColor() {
            GXSetColorUpdate_(old);
        }

        bool old;
    };

    struct ScopedAlpha {
        ScopedAlpha(bool x) {
            old = GXSetAlphaUpdate_(x);
        }

        ~ScopedAlpha() {
            GXSetAlphaUpdate_(old);
        }

        bool old;
    };

    struct ScopedDither {
        ScopedDither(bool x) {
            old = GXSetDither_(x);
        }

        ~ScopedDither() {
            GXSetDither_(old);
        }

        bool old;
    };

    struct CacheGX {
        /* 0x00 */ u8 _0x00[0xC];
        /* 0x0C */ bool mDstAlphaEnable;
        /* 0x0D */ u8 mDstAlpha;
        /* 0x0E */ u8 _0x0E[0x18 - 0x0E];
    };

    struct MemLayout {
        /* 0x00 */ int mLayout;
        /* 0x04 */ u32 numRegions;
        /* 0x08 */ GXTexRegion mRegions[10];
    };

    static GXColor &getClearEfb() {
        return s_clearEfb;
    }

    static void initialize(u16, u16, GXColor, GXPixelFmt);
    static void frameInit();
    static void setTMemLayout(int layout);

    static void resetGX();
    static void resetVtx();
    static void resetColorChannel();
    static void resetIndirect();
    static void resetTexture();
    static void resetTexGen();
    static void resetTev();
    static void resetPE();

    static void resetGXCache();

    static void invalidateTexAllGX();

    static void setPixelFormatGX(GXPixelFmt pixelFmt, GXZFmt16 zFmt);
    static bool GXSetColorUpdate_(bool);
    static bool GXSetAlphaUpdate_(bool);
    static bool GXSetDither_(bool);
    static void GXCopyTex_(void *data, bool);
    static CacheGX &GXSetDstAlpha_(bool, u8);
    static void GXSetProjection_(Mtx44, GXProjectionType);
    static void GXSetProjectionv_(const f32 *);
    static void GXSetViewport_(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far);
    static void GXSetScissor_(u32, u32, u32, u32);
    static void GXSetScissorBoxOffset_(s32, s32);

    // not sure
    static void CalculateScreenScissor(const f32 src[4], u32 dst[4]);

    static u16 s_widthEfb;
    static u16 s_heightEfb;
    static GXPixelFmt s_pixFormat;
    static GXZFmt16 s_zFmt16;
    static GXColor s_clearEfb;
    static u16 s_flag;
    static u16 s_commandFlag;

    static CacheGX s_cacheGX;
    // Inofficial names
    static GXTexObj sDefaultTexObj;
    static MemLayout s_tmem_layout; // same name, different type
};

} // namespace EGG

#endif
