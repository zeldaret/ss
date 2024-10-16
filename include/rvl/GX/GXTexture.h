#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
#include "rvl/GX/GXInternal.h"
#include "rvl/GX/GXTypes.h"

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXTexObj, 32);
GX_DECL_PUBLIC_STRUCT(GXTlutObj, 12);

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
void GXLoadTexObj(GXTexObj *, GXTexMapID);

void GXGetTexObjAll(
    const GXTexObj *pTexObj, void **pImage, u16 *width, u16 *height, GXTexFmt *format, GXTexWrapMode *wrapS,
    GXTexWrapMode *wrapT, u8 *mipmap
);
void GXGetTexObjLODAll(
    const GXTexObj *pTexObj, GXTexFilter *minFilter, GXTexFilter *magFilter, f32 *minLOD, f32 *maxLOD, f32 *LODBias,
    u8 *biasClampEnable, u8 *edgeLODEnable, GXAnisotropy *anisotropy
);
u32 GXGetTexObjTlut(GXTexObj *);
u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool arg3, u8 arg4);
void GXInvalidateTexAll();

#ifdef __cplusplus
}
#endif
#endif
