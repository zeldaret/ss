#ifndef RVL_SDK_GX_ATTR_H
#define RVL_SDK_GX_ATTR_H
#include <lib/rvl/GX/GXTypes.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXVtxDescList {
    GXAttr attr;     // at 0x0
    GXAttrType type; // at  0x4
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
    GXAttr attr;         // at 0x0
    GXCompCnt compCnt;   // at 0x4
    GXCompType compType; // at 0x8
    u8 shift;            // at 0xC
} GXVtxAttrFmtList;

void GXSetVtxDesc(GXAttr name, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* list);
void GXGetVtxDesc(GXAttr name, GXAttrType* type);
void GXGetVtxDescv(GXVtxDescList* list);
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt compCnt,
                     GXCompType compType, u8 shift);

// TODO: Please find a way to get rid of this
#ifdef GXATTR_MATCH_HACK
void GXSetVtxAttrFmtv(s16 fmt, const GXVtxAttrFmtList* list);
#else
void GXSetVtxAttrFmtv(GXVtxFmt fmt, const GXVtxAttrFmtList* list);
#endif

void GXGetVtxAttrFmt(GXVtxFmt fmt, GXAttr attr, GXCompCnt* compCnt,
                     GXCompType* compType, u8* shift);
void GXGetVtxAttrFmtv(GXVtxFmt fmt, GXVtxAttrFmtList* list);
void GXSetArray(GXAttr attr, u32 base, u8 stride);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID id, GXTexGenType type, GXTexGenSrc src,
                       u32 texMtxIdx, GXBool normalize, u32 dualTexMtxIdx);
void GXSetNumTexGens(u8 num);

void __GXSetVCD(void);
void __GXCalculateVLim(void);
void __GXSetVAT(void);

static void GXSetTexCoordGen(GXTexCoordID id, GXTexGenType type,
                             GXTexGenSrc src, u32 texMtxIdx) {
    // TODO: What matrix index is this???
    GXSetTexCoordGen2(id, type, src, texMtxIdx, FALSE, 125);
}

#ifdef __cplusplus
}
#endif
#endif
