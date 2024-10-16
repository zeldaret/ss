#ifndef RVL_SDK_GX_FIFO_H
#define RVL_SDK_GX_FIFO_H
#include "common.h"
#include "rvl/GX/GXInternal.h"

#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXFifoObj, 128);

void GXSetCPUFifo(GXFifoObj *);
BOOL GXGetCPUFifo(GXFifoObj *);

u32 GXGetFifoCount(GXFifoObj *);
u8 GXGetFifoWrap(GXFifoObj *);

#ifdef __cplusplus
}
#endif
#endif
