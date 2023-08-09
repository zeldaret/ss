#ifndef RVL_SDK_GX_MISC_H
#define RVL_SDK_GX_MISC_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void GXSetMisc(UNKWORD token, UNKWORD val);
void GXFlush(void);
void GXResetWriteGatherPipe(void);

void GXAbortFrame(void);

#ifdef __cplusplus
}
#endif
#endif
