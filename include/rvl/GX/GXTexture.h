#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
#include "common.h"
#include "rvl/GX/GXInternal.h"
#include "rvl/GX/GXTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXTexObj, 32);
GX_DECL_PUBLIC_STRUCT(GXTlutObj, 12);

GX_DECL_PUBLIC_STRUCT(GXTexRegion, 16);
GX_DECL_PUBLIC_STRUCT(GXTlutRegion, 16);

typedef GXTexRegion *(*GXTexRegionCallback)(const GXTexObj *pObj, GXTexMapID map);

typedef GXTlutRegion *(*GXTlutRegionCallback)(u32 id);

void __GXSetSUTexRegs(void);

void GXInitTexObj(
    GXTexObj *pTexObj, void *pImage, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrapS, GXTexWrapMode wrapT,
    u8 mipmap
);
void GXInitTexObjLOD(
    GXTexObj *pTexObj, GXTexFilter minFilter, GXTexFilter magFilter, f32 minLOD, f32 maxLOD, f32 LODBias,
    u8 biasClampEnable, u8 edgeLODEnable, GXAnisotropy anisotropy
);
void GXInitTexObjTlut(GXTexObj *, u32);
void GXInitTexObjCI(GXTexObj *, void *, u16, u16, GXTexFmt, GXTexWrapMode, GXTexWrapMode, u8, u32);

void GXInitTlutObj(GXTlutObj *, void *, GXTlutFmt, u16);
void GXLoadTlut(GXTlutObj *, u32);
void GXLoadTexObj(const GXTexObj *, GXTexMapID);

void GXGetTexObjAll(
    const GXTexObj *pTexObj, void **pImage, u16 *width, u16 *height, GXTexFmt *format, GXTexWrapMode *wrapS,
    GXTexWrapMode *wrapT, u8 *mipmap
);
void GXGetTexObjLODAll(
    const GXTexObj *pTexObj, GXTexFilter *minFilter, GXTexFilter *magFilter, f32 *minLOD, f32 *maxLOD, f32 *LODBias,
    u8 *biasClampEnable, u8 *edgeLODEnable, GXAnisotropy *anisotropy
);

u16 GXGetTexObjWidth(const GXTexObj *obj);
u16 GXGetTexObjHeight(const GXTexObj *obj);
GXTexFmt GXGetTexObjFormat(const GXTexObj *obj);
GXBool GXGetTexObjMipMap(const GXTexObj *obj);
GXTexWrapMode GXGetTexObjWrapS(const GXTexObj *obj);
u32 GXGetTexObjTlut(const GXTexObj *);
GXTexWrapMode GXGetTexObjWrapT(const GXTexObj *);
GXTexFilter GXGetTexObjMinFilt(const GXTexObj *);
GXTexFilter GXGetTexObjMagFilt(const GXTexObj *);
void *GXGetTexObjData(const GXTexObj *);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool arg3, u8 arg4);

void GXInvalidateTexAll();

void GXInitTexCacheRegion(
    GXTexRegion *pRegion, GXBool r4, u32 addrTMemEven, u32 sizeTMemEven, u32 addrTMemOdd, u32 sizeTMemOdd
);

void GXInitTlutRegion(GXTlutRegion *pRegion, u32 addrTMem, u32 sizeTMem);
GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback callback);
GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback callback);

#ifdef __cplusplus
}
#endif
#endif
