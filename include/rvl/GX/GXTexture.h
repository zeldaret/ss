#ifndef RVL_SDK_GX_TEXTURE_H
#define RVL_SDK_GX_TEXTURE_H
#include "rvl/GX/GXTypes.h"
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXTexObj, 32);
GX_DECL_PUBLIC_STRUCT(GXTlutObj, 12);

void __GXSetSUTexRegs(void);

void GXInitTexObj(GXTexObj *, void *, u16, u16, GXTexFmt, GXTexWrapMode, GXTexWrapMode, u8);
void GXInitTexObjLOD(GXTexObj *, GXTexFilter, GXTexFilter, f32, f32, f32, u8, u8, GXAnisotropy);

void GXLoadTexObj(GXTexObj *, GXTexMapID);

#ifdef __cplusplus
}
#endif
#endif
