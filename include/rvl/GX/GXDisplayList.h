#ifndef RVL_SDK_GX_DISPLAY_LIST_H
#define RVL_SDK_GX_DISPLAY_LIST_H
#include "common.h"
#include "rvl/GX/GXHardware.h"


#ifdef __cplusplus
extern "C" {
#endif

void GXBeginDisplayList(void *list, u32 size);
u32 GXEndDisplayList(void);
void GXCallDisplayList(void *list, u32 size);

static inline void GXFastCallDisplayList(void *list, u32 size) {
    WGPIPE.c = GX_FIFO_CMD_CALL_DL;
    WGPIPE.p = list;
    WGPIPE.i = size;
}

#ifdef __cplusplus
}
#endif
#endif
