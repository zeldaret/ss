#ifndef RVL_SDK_GX_PIXEL_H
#define RVL_SDK_GX_PIXEL_H
#include "common.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX.h" // IWYU pragma: export

#ifdef __cplusplus
extern "C" {
#endif

#define GX_FOG_ADJ_TABLE_SZ 10
typedef struct _GXFogAdjTable {
    u16 r[GX_FOG_ADJ_TABLE_SZ]; // at 0x0
} GXFogAdjTable;

void GXSetFog(GXFogType type, GXColor color, f32 start, f32 end, f32 near, f32 far);
void GXInitFogAdjTable(GXFogAdjTable *table, u16 width, const Mtx44 proj);
void GXSetFogRangeAdj(GXBool enable, u16 center, const GXFogAdjTable *table);
void GXSetBlendMode(GXBlendMode mode, GXBlendFactor src, GXBlendFactor dst, GXLogicOp op);
void GXGetColorUpdate(GXBool *enable);
void GXSetColorUpdate(GXBool enable);
void GXGetAlphaUpdate(GXBool *enable);
void GXSetAlphaUpdate(GXBool enable);
void GXSetZMode(GXBool enableTest, GXCompare func, GXBool enableUpdate);
void GXSetZCompLoc(GXBool beforeTex);
void GXGetPixelFmt(GXPixelFmt *pixelFmt, GXZFmt16 *zFmt);
void GXSetPixelFmt(GXPixelFmt pixelFmt, GXZFmt16 zFmt);
void GXGetDither(GXBool *enable);
void GXSetDither(GXBool enable);
void GXSetDstAlpha(GXBool enable, u8 alpha);
void GXSetFieldMask(GXBool enableEven, GXBool enableOdd);
void GXSetFieldMode(GXBool texLOD, GXBool adjustAR);

#ifdef __cplusplus
}
#endif
#endif
