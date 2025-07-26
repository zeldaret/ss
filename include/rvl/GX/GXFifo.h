#ifndef RVL_SDK_GX_FIFO_H
#define RVL_SDK_GX_FIFO_H
#include "common.h"
#include "rvl/GX/GXInternal.h"
#include "rvl/OS/OSThread.h"

#ifdef __cplusplus
extern "C" {
#endif

GX_DECL_PUBLIC_STRUCT(GXFifoObj, 128);

void GXSetCPUFifo(GXFifoObj *);
BOOL GXGetCPUFifo(GXFifoObj *);

u32 GXGetFifoCount(GXFifoObj *);
u8 GXGetFifoWrap(GXFifoObj *);

// IDK where this goes
OSThread *GXSetCurrentGXThread();
OSThread *GXGetCurrentGXThread();
void GXSetDrawDoneCallback(void *);
#ifdef __cplusplus
}
#endif
#endif
