#ifndef RVL_SDK_GX_FRAMEBUF_H
#define RVL_SDK_GX_FRAMEBUF_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXRenderModeObj {
    u32 tvInfo;               // at 0x0
    u16 fbWidth;              // at 0x4
    u16 efbHeight;            // at 0x6
    u16 xfbHeight;            // at 0x8
    u16 viXOrigin;            // at 0xA
    u16 viYOrigin;            // at 0xC
    u16 viWidth;              // at 0xE
    u16 viHeight;             // at 0x10
    u32 xfbMode;              // at 0x14
    u8 field_rendering;       // at 0x18
    u8 aa;                    // at 0x19
    u8 sample_pattern[12][2]; // at 0x1A
    u8 vfilter[7];            // at 0x32
} GXRenderModeObj;

void GXSetCopyFilter(u8, const u8[12][2], u8, const u8[7]);
void GXSetCopyClear(GXColor, u32 zClear);
void GXSetDispCopySrc(u16 left, u16 top, u16 width, u16 height);
f32 GXGetYScaleFactor(u16 height, u16 width);
u16 GXGetNumXfbLines(u16 height, f32 scale);
void GXSetDispCopyDst(u16 width, u16 height);
u32 GXSetDispCopyYScale(f32 scale);
void GXCopyDisp(void *data, GXBool bUpdate);

void GXSetTexCopySrc(u16 left, u16 top, u16 width, u16 height);
void GXSetTexCopyDst(u16 left, u16 top, GXTexFmt fmt, u8 mipmap);
void GXCopyTex(void *data, GXBool bUpdate);

// Not sure where this belongs
void GXPixModeSync();

extern GXRenderModeObj GXNtsc480IntDf;
extern GXRenderModeObj GXMpal480IntDf;
extern GXRenderModeObj GXPal528IntDf;
extern GXRenderModeObj GXEurgb60Hz480IntDf;

#ifdef __cplusplus
}
#endif
#endif
