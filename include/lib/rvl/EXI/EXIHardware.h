#ifndef RVL_SDK_EXI_HARDWARE_H
#define RVL_SDK_EXI_HARDWARE_H
#include <lib/rvl/EXI/EXICommon.h>
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Documentation from:
 * https://www.gc-forever.com/yagcd/chap5.html#sec5.9
 */
typedef struct ExiChannelControl {
    u32 csr;       // at 0x0
    void* dmaAddr; // at 0x4
    u32 dmaLen;    // at 0x8
    u32 cr;        // at 0xC
    u32 imm;       // at 0x10
} ExiChannelControl;

volatile ExiChannelControl EXI_CHAN_CTRL[EXI_MAX_CHAN] : 0xCD006800;

// CSR - Control Status Register
#define EXI_CSR_EXIINTMASK (1 << 0)
#define EXI_CSR_EXIINT (1 << 1)
#define EXI_CSR_TCINTMASK (1 << 2)
#define EXI_CSR_TCINT (1 << 3)
#define EXI_CSR_EXTINTMASK (1 << 10)
#define EXI_CSR_EXTINT (1 << 11)
#define EXI_CSR_EXT (1 << 12)
#define EXI_CSR_ROMDIS (1 << 13)

#ifdef __cplusplus
}
#endif
#endif
