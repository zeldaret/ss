#ifndef RVL_SDK_DSP_HARDWARE_H
#define RVL_SDK_DSP_HARDWARE_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * DSP Hardware Registers
 * https://www.gc-forever.com/yagcd/chap5.html#sec5.6
 */
volatile u16 DSP_HW_REGS[] : 0xCC005000;

/**
 * Hardware register indexes
 */
typedef enum {
    DSP_DSPMBOX_H,        //!< 0xCC005000
    DSP_DSPMBOX_L,        //!< 0xCC005002
    DSP_CPUMBOX_H,        //!< 0xCC005004
    DSP_CPUMBOX_L,        //!< 0xCC005006
    DSP_REG_0x8,          //!< 0xCC005008
    DSP_CSR,              //!< 0xCC00500A
    DSP_REG_0xC,          //!< 0xCC00500C
    DSP_REG_0xE,          //!< 0xCC00500E
    DSP_REG_0x10,         //!< 0xCC005010
    DSP_AR_SIZE,          //!< 0xCC005012
    DSP_REG_0x14,         //!< 0xCC005014
    DSP_AR_MODE,          //!< 0xCC005016
    DSP_REG_0x18,         //!< 0xCC005018
    DSP_AR_REFRESH,       //!< 0xCC00501A
    DSP_REG_0x1C,         //!< 0xCC00501C
    DSP_REG_0x1E,         //!< 0xCC00501E
    DSP_AR_DMA_MMADDR_H,  //!< 0xCC005020
    DSP_AR_DMA_MMADDR_L,  //!< 0xCC005022
    DSP_AR_DMA_ARADDR_H,  //!< 0xCC005024
    DSP_AR_DMA_ARADDR_L,  //!< 0xCC005026
    DSP_AR_DMA_CNT_H,     //!< 0xCC005028
    DSP_AR_DMA_CNT_L,     //!< 0xCC00502A
    DSP_REG_0x2C,         //!< 0xCC00502C
    DSP_REG_0x2E,         //!< 0xCC00502E
    DSP_AI_DMA_START_H,   //!< 0xCC005030
    DSP_AI_DMA_START_L,   //!< 0xCC005032
    DSP_REG_0x34,         //!< 0xCC005034
    DSP_AI_DMA_CSR,       //!< 0xCC005036
    DSP_REG_0x38,         //!< 0xCC005038
    DSP_AI_DMA_BYTES_LEFT //!< 0xCC00503A
} DSPHwReg;

// DSPMBOX_H - DSP Mailbox High (to DSP)
#define DSP_DSPMBOX_H_STATUS (1 << 15)

// CPUMBOX_H - CPU Mailbox High (from DSP)
#define DSP_CPUMBOX_H_STATUS (1 << 15)

// CSR - Control Status Register
#define DSP_CSR_RES (1 << 11)
#define DSP_CSR_DMAINT (1 << 9)
#define DSP_CSR_DSPINTMSK (1 << 8)
#define DSP_CSR_DSPINT (1 << 7)
#define DSP_CSR_ARINTMSK (1 << 6)
#define DSP_CSR_ARINT (1 << 5)
#define DSP_CSR_AIDINTMSK (1 << 4)
#define DSP_CSR_AIDINT (1 << 3)
#define DSP_CSR_HALT (1 << 2)
#define DSP_CSR_PIINT (1 << 1)

// AI_DMA_CSR - AI DMA Control Status Register
#define DSP_AI_DMA_CSR_PLAY (1 << 15)

#ifdef __cplusplus
}
#endif
#endif
