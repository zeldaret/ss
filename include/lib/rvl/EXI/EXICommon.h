#ifndef RVL_SDK_EXI_COMMON_H
#define RVL_SDK_EXI_COMMON_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct OSContext;

typedef enum {
    EXI_STATE_DMA_ACCESS = (1 << 0),
    EXI_STATE_IMM_ACCESS = (1 << 1),
    EXI_STATE_SELECTED = (1 << 2),
    EXI_STATE_ATTACHED = (1 << 3),
    EXI_STATE_LOCKED = (1 << 4),

    EXI_STATE_BUSY = EXI_STATE_DMA_ACCESS | EXI_STATE_IMM_ACCESS
} EXIState;

typedef enum { EXI_CHAN_0, EXI_CHAN_1, EXI_CHAN_2, EXI_MAX_CHAN } EXIChannel;

typedef enum { EXI_READ, EXI_WRITE, EXI_TYPE_2, EXI_MAX_TYPE } EXIType;

typedef void (*EXICallback)(EXIChannel chan, struct OSContext* ctx);

extern const u32 __EXIFreq;

static u32 __EXISwap32(u32 val) {
    return val >> 24 & 0x000000FF | val >> 8 & 0x0000FF00 |
           val << 8 & 0x00FF0000 | val << 24 & 0xFF000000;
}

BOOL EXIWriteReg(EXIChannel chan, u32 dev, UNKWORD r5, const void* buf,
                 s32 len);

#ifdef __cplusplus
}
#endif
#endif
