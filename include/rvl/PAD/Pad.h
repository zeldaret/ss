#ifndef RVL_SDK_PAD_H
#define RVL_SDK_PAD_H
#include <common.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} PADStatus;

extern u32 __PADSpec;

BOOL __PADDisableRecalibration(BOOL);

#ifdef __cplusplus
}
#endif
#endif
