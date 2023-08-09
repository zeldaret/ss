#ifndef RVL_SDK_GX_VERT_H
#define RVL_SDK_GX_VERT_H
#include <lib/rvl/GX/GXHardware.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

static inline void GXCmd1u8(u8 uc) { WGPIPE.c = uc; }

static inline void GXCmd1u16(u16 us) { WGPIPE.us = us; }

static inline void GXCmd1u32(u32 ul) { WGPIPE.i = ul; }

static inline void GXPosition3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXPosition3u8(u8 x, u8 y, u8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXPosition3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXPosition3u16(u16 x, u16 y, u16 z) {
    WGPIPE.us = x;
    WGPIPE.us = y;
    WGPIPE.us = z;
}

static inline void GXPosition3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXPosition2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXPosition2u8(u8 x, u8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXPosition2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXPosition2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXPosition2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXPosition1x16(u16 us) { WGPIPE.us = us; }

static inline void GXPosition1x8(u8 uc) { WGPIPE.c = uc; }

static inline void GXNormal3f32(f32 x, f32 y, f32 z) {
    WGPIPE.f = x;
    WGPIPE.f = y;
    WGPIPE.f = z;
}

static inline void GXNormal3s16(s16 x, s16 y, s16 z) {
    WGPIPE.s = x;
    WGPIPE.s = y;
    WGPIPE.s = z;
}

static inline void GXNormal3s8(s8 x, s8 y, s8 z) {
    WGPIPE.c = x;
    WGPIPE.c = y;
    WGPIPE.c = z;
}

static inline void GXNormal1x16(u16 us) { WGPIPE.us = us; }

static inline void GXNormal1x8(u8 uc) { WGPIPE.c = uc; }

static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
    WGPIPE.c = r;
    WGPIPE.c = g;
    WGPIPE.c = b;
    WGPIPE.c = a;
}

static inline void GXColor1u32(u32 color) { WGPIPE.i = color; }

static inline void GXColor3u8(u8 r, u8 g, u8 b) {
    WGPIPE.c = r;
    WGPIPE.c = g;
    WGPIPE.c = b;
}

static inline void GXColor1u16(u16 us) { WGPIPE.us = us; }

static inline void GXColor1x16(u16 us) { WGPIPE.us = us; }

static inline void GXColor1x8(u8 uc) { WGPIPE.c = uc; }

static inline void GXTexCoord2f32(f32 x, f32 y) {
    WGPIPE.f = x;
    WGPIPE.f = y;
}

static inline void GXTexCoord2s16(s16 x, s16 y) {
    WGPIPE.s = x;
    WGPIPE.s = y;
}

static inline void GXTexCoord2u16(u16 x, u16 y) {
    WGPIPE.us = x;
    WGPIPE.us = y;
}

static inline void GXTexCoord2s8(s8 x, s8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXTexCoord2u8(u8 x, u8 y) {
    WGPIPE.c = x;
    WGPIPE.c = y;
}

static inline void GXTexCoord1f32(f32 f) { WGPIPE.f = f; }

static inline void GXTexCoord1s16(s16 s) { WGPIPE.s = s; }

static inline void GXTexCoord1u16(u16 us) { WGPIPE.us = us; }

static inline void GXTexCoord1s8(s8 c) { WGPIPE.c = c; }

static inline void GXTexCoord1u8(u8 uc) { WGPIPE.c = uc; }

static inline void GXTexCoord1x16(u16 us) { WGPIPE.us = us; }

static inline void GXTexCoord1x8(u8 uc) { WGPIPE.c = uc; }

#ifdef __cplusplus
}
#endif
#endif
