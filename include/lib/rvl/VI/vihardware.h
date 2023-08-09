#ifndef RVL_SDK_VI_HARDWARE_H
#define RVL_SDK_VI_HARDWARE_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * VI Hardware Registers
 * https://www.gc-forever.com/yagcd/chap5.html#sec5.3
 */
volatile u16 VI_HW_REGS[] : 0xCC002000;

/**
 * Hardware register indexes
 */
typedef enum {
    VI_REG_VTR,    //!< 0xCC002000
    VI_REG_DCR,    //!< 0xCC002002
    VI_REG_HTR0_H, //!< 0xCC002004
    VI_REG_HTR0_L, //!< 0xCC002006
    VI_REG_HTR1_H, //!< 0xCC002008
    VI_REG_HTR1_L, //!< 0xCC00200A
    VI_REG_VTO_H,  //!< 0xCC00200C
    VI_REG_VTO_L,  //!< 0xCC00200E
    VI_REG_VTE_H,  //!< 0xCC002010
    VI_REG_VTE_L,  //!< 0xCC002012
    VI_REG_BBEI_H, //!< 0xCC002014
    VI_REG_BBEI_L, //!< 0xCC002016
    VI_REG_BBOI_H, //!< 0xCC002018
    VI_REG_BBOI_L, //!< 0xCC00201A
    VI_REG_TFBL_H, //!< 0xCC00201C
    VI_REG_TFBL_L, //!< 0xCC00201E
    VI_REG_TFBR_H, //!< 0xCC002020
    VI_REG_TFBR_L, //!< 0xCC002022
    VI_REG_BFBL_H, //!< 0xCC002024
    VI_REG_BFBL_L, //!< 0xCC002026
    VI_REG_BFBR_H, //!< 0xCC002028
    VI_REG_BFBR_L, //!< 0xCC00202A
    VI_REG_DPV,    //!< 0xCC00202C
    VI_REG_DPH,    //!< 0xCC00202E
    VI_REG_DI0_H,  //!< 0xCC002030
    VI_REG_DI0_L,  //!< 0xCC002032
    VI_REG_DI1_H,  //!< 0xCC002034
    VI_REG_DI1_L,  //!< 0xCC002036
    VI_REG_DI2_H,  //!< 0xCC002038
    VI_REG_DI2_L,  //!< 0xCC00203A
    VI_REG_DI3_H,  //!< 0xCC00203C
    VI_REG_DI3_L,  //!< 0xCC00203E
    VI_REG_DL0_H,  //!< 0xCC002040
    VI_REG_DL0_L,  //!< 0xCC002042
    VI_REG_DL1_H,  //!< 0xCC002044
    VI_REG_DL1_L,  //!< 0xCC002046
    VI_REG_HSW,    //!< 0xCC002048
    VI_REG_HSR,    //!< 0xCC00204A
    VI_REG_FCT0_H, //!< 0xCC00204C
    VI_REG_FCT0_L, //!< 0xCC00204E
    VI_REG_FCT1_H, //!< 0xCC002050
    VI_REG_FCT1_L, //!< 0xCC002052
    VI_REG_FCT2_H, //!< 0xCC002054
    VI_REG_FCT2_L, //!< 0xCC002056
    VI_REG_FCT3_H, //!< 0xCC002058
    VI_REG_FCT3_L, //!< 0xCC00205A
    VI_REG_FCT4_H, //!< 0xCC00205C
    VI_REG_FCT4_L, //!< 0xCC00205E
    VI_REG_FCT5_H, //!< 0xCC002060
    VI_REG_FCT5_L, //!< 0xCC002062
    VI_REG_FCT6_H, //!< 0xCC002064
    VI_REG_FCT6_L, //!< 0xCC002066
    VI_REG_0x68,   //!< 0xCC002068
    VI_REG_0x6A,   //!< 0xCC00206A
    VI_REG_VICLK,  //!< 0xCC00206C
    VI_REG_VISEL,  //!< 0xCC00206E
    VI_REG_0x70,   //!< 0xCC002070
    VI_REG_HBE,    //!< 0xCC002072
    VI_REG_HBS,    //!< 0xCC002074
    VI_REG_0x76,   //!< 0xCC002076
    VI_REG_0x78,   //!< 0xCC002078
    VI_REG_0x7A,   //!< 0xCC00207A
    VI_REG_0x7C,   //!< 0xCC00207C
} VIHwReg;

// DCR - Display Configuration Register
#define VI_DCR_ENB (1 << 0)
#define VI_DCR_RST (1 << 1)
#define VI_DCR_NIN (1 << 2)
#define VI_DCR_FMT_MASK (0b0000001100000000)

// DI{n} - Display Interrupt Register N
#define VI_DI_INT (1 << 15)
#define VI_DI_ENB (1 << 12)

// VICLK - VI Clock Select Register
#define VI_VICLK_27MHZ 0
#define VI_VICLK_54MHZ 1

// HBE - Border HBE
#define VI_HBE_BRDR_EN (1 << 15)

#ifdef __cplusplus
}
#endif
#endif
