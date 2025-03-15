#ifndef RVL_SDK_GX_TRANSFORM_H
#define RVL_SDK_GX_TRANSFORM_H
#include "common.h"
#include "rvl/GX/GXTypes.h"

#include "rvl/MTX.h" // IWYU pragma: export


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GX_FIELD_EVEN,
    GX_FIELD_ODD,
} GXInterlaceField;

void GXSetProjection(const Mtx44 proj, GXProjectionType type);
void GXSetProjectionv(const f32 proj[7]);
void GXGetProjectionv(f32 proj[7]);
void GXLoadPosMtxImm(const Mtx mtx, u32 id);
void GXLoadPosMtxIndx(u16 index, u32 id);
void GXLoadNrmMtxImm(const Mtx mtx, u32 id);
void GXLoadNrmMtxIndx3x3(u16 index, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXMtxType type);
void GXSetViewportJitter(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far, u32 nextField);
void GXSetViewport(f32 ox, f32 oy, f32 sx, f32 sy, f32 near, f32 far);
void GXGetViewportv(f32 view[6]);
void GXSetZScaleOffset(f32 scale, f32 offset);
void GXSetScissor(u32 x, u32 y, u32 w, u32 h);
void GXGetScissor(u32 *x, u32 *y, u32 *w, u32 *h);
void GXSetScissorBoxOffset(u32 ox, u32 oy);
void GXSetClipMode(GXClipMode mode);

void __GXSetProjection(void);
void __GXSetViewport(void);
void __GXSetMatrixIndex(GXAttr index);

static void GXSetViewportv(const f32 *vp) {
    GXSetViewport(vp[0], vp[1], vp[2], vp[3], vp[4], vp[5]);
}

#ifdef __cplusplus
}
#endif
#endif
