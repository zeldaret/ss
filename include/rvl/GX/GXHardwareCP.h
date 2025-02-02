#ifndef RVL_SDK_GX_HARDWARE_CP_H
#define RVL_SDK_GX_HARDWARE_CP_H
#include <rvl/GX/GXTypes.h>

#include <common.h>


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *
 *
 * GX Command Processor (CP)
 *
 *
 *****************************************************************************/

/**
 * CP registers
 */
typedef enum {
    GX_CP_REG_MATRIXINDEXA = 0x30,
    GX_CP_REG_MATRIXINDEXB = 0x40,
    GX_CP_REG_VCD_LO = 0x50,
    GX_CP_REG_VCD_HI = 0x60,
    GX_CP_REG_VAT_GROUP0 = 0x70,
    GX_CP_REG_VAT_GROUP1 = 0x80,
    GX_CP_REG_VAT_GROUP2 = 0x90,
    GX_CP_REG_ARRAYBASE = 0xA0,
    GX_CP_REG_ARRAYSTRIDE = 0xB0,
} GX_CP_REG;

/******************************************************************************
 * CP register 0x30 - MatrixIndexA
 *****************************************************************************/
// TEX3 [2:7] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXA_TEX3_B 2
/* end bit    */ #define GX_CP_MATRIXINDEXA_TEX3_E 7
/* bit size   */ #define GX_CP_MATRIXINDEXA_TEX3_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX3_MASK (((1 << 6) - 1) << 31 - 7)
/* local mask */ #define GX_CP_MATRIXINDEXA_TEX3_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX3_SHIFT 24

/* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX3(reg) GX_BITGET((reg), 2, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX3(reg, x) ((reg) = GX_BITSET((reg), 2, 6, x))

// TEX2 [8:13] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXA_TEX2_B 8
/* end bit    */ #define GX_CP_MATRIXINDEXA_TEX2_E 13
/* bit size   */ #define GX_CP_MATRIXINDEXA_TEX2_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX2_MASK (((1 << 6) - 1) << 31 - 13)
/* local mask */ #define GX_CP_MATRIXINDEXA_TEX2_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX2_SHIFT 18

/* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX2(reg) GX_BITGET((reg), 8, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX2(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX1 [14:19] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXA_TEX1_B 14
/* end bit    */ #define GX_CP_MATRIXINDEXA_TEX1_E 19
/* bit size   */ #define GX_CP_MATRIXINDEXA_TEX1_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX1_MASK (((1 << 6) - 1) << 31 - 19)
/* local mask */ #define GX_CP_MATRIXINDEXA_TEX1_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX1_SHIFT 12

/* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX1(reg) GX_BITGET((reg), 14, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX1(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX0 [20:25] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXA_TEX0_B 20
/* end bit    */ #define GX_CP_MATRIXINDEXA_TEX0_E 25
/* bit size   */ #define GX_CP_MATRIXINDEXA_TEX0_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXA_TEX0_MASK (((1 << 6) - 1) << 31 - 25)
/* local mask */ #define GX_CP_MATRIXINDEXA_TEX0_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXA_TEX0_SHIFT 6

/* get value  */ #define GX_CP_GET_MATRIXINDEXA_TEX0(reg) GX_BITGET((reg), 20, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXA_TEX0(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// POSNRM [26:31] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXA_POSNRM_B 26
/* end bit    */ #define GX_CP_MATRIXINDEXA_POSNRM_E 31
/* bit size   */ #define GX_CP_MATRIXINDEXA_POSNRM_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXA_POSNRM_MASK (((1 << 6) - 1) << 31 - 31)
/* local mask */ #define GX_CP_MATRIXINDEXA_POSNRM_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXA_POSNRM_SHIFT 0

/* get value  */ #define GX_CP_GET_MATRIXINDEXA_POSNRM(reg) GX_BITGET((reg), 26, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXA_POSNRM(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

/******************************************************************************
 * CP register 0x40 - MatrixIndexB
 *****************************************************************************/
// TEX7 [8:13] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXB_TEX7_B 8
/* end bit    */ #define GX_CP_MATRIXINDEXB_TEX7_E 13
/* bit size   */ #define GX_CP_MATRIXINDEXB_TEX7_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX7_MASK (((1 << 6) - 1) << 31 - 13)
/* local mask */ #define GX_CP_MATRIXINDEXB_TEX7_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX7_SHIFT 18

/* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX7(reg) GX_BITGET((reg), 8, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX7(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX6 [14:19] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXB_TEX6_B 14
/* end bit    */ #define GX_CP_MATRIXINDEXB_TEX6_E 19
/* bit size   */ #define GX_CP_MATRIXINDEXB_TEX6_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX6_MASK (((1 << 6) - 1) << 31 - 19)
/* local mask */ #define GX_CP_MATRIXINDEXB_TEX6_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX6_SHIFT 12

/* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX6(reg) GX_BITGET((reg), 14, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX6(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX5 [20:25] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXB_TEX5_B 20
/* end bit    */ #define GX_CP_MATRIXINDEXB_TEX5_E 25
/* bit size   */ #define GX_CP_MATRIXINDEXB_TEX5_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX5_MASK (((1 << 6) - 1) << 31 - 25)
/* local mask */ #define GX_CP_MATRIXINDEXB_TEX5_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX5_SHIFT 6

/* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX5(reg) GX_BITGET((reg), 20, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX5(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX4 [26:31] (6)
/* start bit  */ #define GX_CP_MATRIXINDEXB_TEX4_B 26
/* end bit    */ #define GX_CP_MATRIXINDEXB_TEX4_E 31
/* bit size   */ #define GX_CP_MATRIXINDEXB_TEX4_SZ 6

/* raw mask   */ #define GX_CP_MATRIXINDEXB_TEX4_MASK (((1 << 6) - 1) << 31 - 31)
/* local mask */ #define GX_CP_MATRIXINDEXB_TEX4_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_CP_MATRIXINDEXB_TEX4_SHIFT 0

/* get value  */ #define GX_CP_GET_MATRIXINDEXB_TEX4(reg) GX_BITGET((reg), 26, 6)
/* set value  */ #define GX_CP_SET_MATRIXINDEXB_TEX4(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

/******************************************************************************
 * CP register 0x50 - VCD_Lo
 *****************************************************************************/
// POSMATIDX [31:31] (1)
/* start bit  */ #define GX_CP_VCD_LO_POSMATIDX_B 31
/* end bit    */ #define GX_CP_VCD_LO_POSMATIDX_E 31
/* bit size   */ #define GX_CP_VCD_LO_POSMATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_POSMATIDX_MASK (((1 << 1) - 1) << 31 - 31)
/* local mask */ #define GX_CP_VCD_LO_POSMATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_POSMATIDX_SHIFT 0

/* get value  */ #define GX_CP_GET_VCD_LO_POSMATIDX(reg) GX_BITGET((reg), 31, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_POSMATIDX(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// TEX0MATIDX [30:30] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX0MATIDX_B 30
/* end bit    */ #define GX_CP_VCD_LO_TEX0MATIDX_E 30
/* bit size   */ #define GX_CP_VCD_LO_TEX0MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX0MATIDX_MASK (((1 << 1) - 1) << 31 - 30)
/* local mask */ #define GX_CP_VCD_LO_TEX0MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX0MATIDX_SHIFT 1

/* get value  */ #define GX_CP_GET_VCD_LO_TEX0MATIDX(reg) GX_BITGET((reg), 30, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX0MATIDX(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// TEX1MATIDX [29:29] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX1MATIDX_B 29
/* end bit    */ #define GX_CP_VCD_LO_TEX1MATIDX_E 29
/* bit size   */ #define GX_CP_VCD_LO_TEX1MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX1MATIDX_MASK (((1 << 1) - 1) << 31 - 29)
/* local mask */ #define GX_CP_VCD_LO_TEX1MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX1MATIDX_SHIFT 2

/* get value  */ #define GX_CP_GET_VCD_LO_TEX1MATIDX(reg) GX_BITGET((reg), 29, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX1MATIDX(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

// TEX2MATIDX [28:28] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX2MATIDX_B 28
/* end bit    */ #define GX_CP_VCD_LO_TEX2MATIDX_E 28
/* bit size   */ #define GX_CP_VCD_LO_TEX2MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX2MATIDX_MASK (((1 << 1) - 1) << 31 - 28)
/* local mask */ #define GX_CP_VCD_LO_TEX2MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX2MATIDX_SHIFT 3

/* get value  */ #define GX_CP_GET_VCD_LO_TEX2MATIDX(reg) GX_BITGET((reg), 28, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX2MATIDX(reg, x) ((reg) = GX_BITSET((reg), 28, 1, x))

// TEX3MATIDX [27:27] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX3MATIDX_B 27
/* end bit    */ #define GX_CP_VCD_LO_TEX3MATIDX_E 27
/* bit size   */ #define GX_CP_VCD_LO_TEX3MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX3MATIDX_MASK (((1 << 1) - 1) << 31 - 27)
/* local mask */ #define GX_CP_VCD_LO_TEX3MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX3MATIDX_SHIFT 4

/* get value  */ #define GX_CP_GET_VCD_LO_TEX3MATIDX(reg) GX_BITGET((reg), 27, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX3MATIDX(reg, x) ((reg) = GX_BITSET((reg), 27, 1, x))

// TEX4MATIDX [26:26] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX4MATIDX_B 26
/* end bit    */ #define GX_CP_VCD_LO_TEX4MATIDX_E 26
/* bit size   */ #define GX_CP_VCD_LO_TEX4MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX4MATIDX_MASK (((1 << 1) - 1) << 31 - 26)
/* local mask */ #define GX_CP_VCD_LO_TEX4MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX4MATIDX_SHIFT 5

/* get value  */ #define GX_CP_GET_VCD_LO_TEX4MATIDX(reg) GX_BITGET((reg), 26, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX4MATIDX(reg, x) ((reg) = GX_BITSET((reg), 26, 1, x))

// TEX5MATIDX [25:25] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX5MATIDX_B 25
/* end bit    */ #define GX_CP_VCD_LO_TEX5MATIDX_E 25
/* bit size   */ #define GX_CP_VCD_LO_TEX5MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX5MATIDX_MASK (((1 << 1) - 1) << 31 - 25)
/* local mask */ #define GX_CP_VCD_LO_TEX5MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX5MATIDX_SHIFT 6

/* get value  */ #define GX_CP_GET_VCD_LO_TEX5MATIDX(reg) GX_BITGET((reg), 25, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX5MATIDX(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))

// TEX6MATIDX [24:24] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX6MATIDX_B 24
/* end bit    */ #define GX_CP_VCD_LO_TEX6MATIDX_E 24
/* bit size   */ #define GX_CP_VCD_LO_TEX6MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX6MATIDX_MASK (((1 << 1) - 1) << 31 - 24)
/* local mask */ #define GX_CP_VCD_LO_TEX6MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX6MATIDX_SHIFT 7

/* get value  */ #define GX_CP_GET_VCD_LO_TEX6MATIDX(reg) GX_BITGET((reg), 24, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX6MATIDX(reg, x) ((reg) = GX_BITSET((reg), 24, 1, x))

// TEX7MATIDX [23:23] (1)
/* start bit  */ #define GX_CP_VCD_LO_TEX7MATIDX_B 23
/* end bit    */ #define GX_CP_VCD_LO_TEX7MATIDX_E 23
/* bit size   */ #define GX_CP_VCD_LO_TEX7MATIDX_SZ 1

/* raw mask   */ #define GX_CP_VCD_LO_TEX7MATIDX_MASK (((1 << 1) - 1) << 31 - 23)
/* local mask */ #define GX_CP_VCD_LO_TEX7MATIDX_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_TEX7MATIDX_SHIFT 8

/* get value  */ #define GX_CP_GET_VCD_LO_TEX7MATIDX(reg) GX_BITGET((reg), 23, 1)
/* set value  */ #define GX_CP_SET_VCD_LO_TEX7MATIDX(reg, x) ((reg) = GX_BITSET((reg), 23, 1, x))

// POSITION [21:22] (2)
/* start bit  */ #define GX_CP_VCD_LO_POSITION_B 21
/* end bit    */ #define GX_CP_VCD_LO_POSITION_E 22
/* bit size   */ #define GX_CP_VCD_LO_POSITION_SZ 2

/* raw mask   */ #define GX_CP_VCD_LO_POSITION_MASK (((1 << 2) - 1) << 31 - 22)
/* local mask */ #define GX_CP_VCD_LO_POSITION_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_POSITION_SHIFT 9

/* get value  */ #define GX_CP_GET_VCD_LO_POSITION(reg) GX_BITGET((reg), 21, 2)
/* set value  */ #define GX_CP_SET_VCD_LO_POSITION(reg, x) ((reg) = GX_BITSET((reg), 21, 2, x))

// NORMAL [19:20] (2)
/* start bit  */ #define GX_CP_VCD_LO_NORMAL_B 19
/* end bit    */ #define GX_CP_VCD_LO_NORMAL_E 20
/* bit size   */ #define GX_CP_VCD_LO_NORMAL_SZ 2

/* raw mask   */ #define GX_CP_VCD_LO_NORMAL_MASK (((1 << 2) - 1) << 31 - 20)
/* local mask */ #define GX_CP_VCD_LO_NORMAL_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_NORMAL_SHIFT 11

/* get value  */ #define GX_CP_GET_VCD_LO_NORMAL(reg) GX_BITGET((reg), 19, 2)
/* set value  */ #define GX_CP_SET_VCD_LO_NORMAL(reg, x) ((reg) = GX_BITSET((reg), 19, 2, x))

// COLORDIFFUSED [17:18] (2)
/* start bit  */ #define GX_CP_VCD_LO_COLORDIFFUSED_B 17
/* end bit    */ #define GX_CP_VCD_LO_COLORDIFFUSED_E 18
/* bit size   */ #define GX_CP_VCD_LO_COLORDIFFUSED_SZ 2

/* raw mask   */ #define GX_CP_VCD_LO_COLORDIFFUSED_MASK (((1 << 2) - 1) << 31 - 18)
/* local mask */ #define GX_CP_VCD_LO_COLORDIFFUSED_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_COLORDIFFUSED_SHIFT 13

/* get value  */ #define GX_CP_GET_VCD_LO_COLORDIFFUSED(reg) GX_BITGET((reg), 17, 2)
/* set value  */ #define GX_CP_SET_VCD_LO_COLORDIFFUSED(reg, x) ((reg) = GX_BITSET((reg), 17, 2, x))

// COLORSPECULAR [15:16] (2)
/* start bit  */ #define GX_CP_VCD_LO_COLORSPECULAR_B 15
/* end bit    */ #define GX_CP_VCD_LO_COLORSPECULAR_E 16
/* bit size   */ #define GX_CP_VCD_LO_COLORSPECULAR_SZ 2

/* raw mask   */ #define GX_CP_VCD_LO_COLORSPECULAR_MASK (((1 << 2) - 1) << 31 - 16)
/* local mask */ #define GX_CP_VCD_LO_COLORSPECULAR_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_LO_COLORSPECULAR_SHIFT 15

/* get value  */ #define GX_CP_GET_VCD_LO_COLORSPECULAR(reg) GX_BITGET((reg), 15, 2)
/* set value  */ #define GX_CP_SET_VCD_LO_COLORSPECULAR(reg, x) ((reg) = GX_BITSET((reg), 15, 2, x))

/******************************************************************************
 * CP register 0x60 - VCD_Hi
 *****************************************************************************/
// TEX0COORD [30:31] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX0COORD_B 30
/* end bit    */ #define GX_CP_VCD_HI_TEX0COORD_E 31
/* bit size   */ #define GX_CP_VCD_HI_TEX0COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX0COORD_MASK (((1 << 2) - 1) << 31 - 31)
/* local mask */ #define GX_CP_VCD_HI_TEX0COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX0COORD_SHIFT 0

/* get value  */ #define GX_CP_GET_VCD_HI_TEX0COORD(reg) GX_BITGET((reg), 30, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX0COORD(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// TEX1COORD [28:29] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX1COORD_B 28
/* end bit    */ #define GX_CP_VCD_HI_TEX1COORD_E 29
/* bit size   */ #define GX_CP_VCD_HI_TEX1COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX1COORD_MASK (((1 << 2) - 1) << 31 - 29)
/* local mask */ #define GX_CP_VCD_HI_TEX1COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX1COORD_SHIFT 2

/* get value  */ #define GX_CP_GET_VCD_HI_TEX1COORD(reg) GX_BITGET((reg), 28, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX1COORD(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// TEX2COORD [26:27] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX2COORD_B 26
/* end bit    */ #define GX_CP_VCD_HI_TEX2COORD_E 27
/* bit size   */ #define GX_CP_VCD_HI_TEX2COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX2COORD_MASK (((1 << 2) - 1) << 31 - 27)
/* local mask */ #define GX_CP_VCD_HI_TEX2COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX2COORD_SHIFT 4

/* get value  */ #define GX_CP_GET_VCD_HI_TEX2COORD(reg) GX_BITGET((reg), 26, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX2COORD(reg, x) ((reg) = GX_BITSET((reg), 26, 2, x))

// TEX3COORD [24:25] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX3COORD_B 24
/* end bit    */ #define GX_CP_VCD_HI_TEX3COORD_E 25
/* bit size   */ #define GX_CP_VCD_HI_TEX3COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX3COORD_MASK (((1 << 2) - 1) << 31 - 25)
/* local mask */ #define GX_CP_VCD_HI_TEX3COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX3COORD_SHIFT 6

/* get value  */ #define GX_CP_GET_VCD_HI_TEX3COORD(reg) GX_BITGET((reg), 24, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX3COORD(reg, x) ((reg) = GX_BITSET((reg), 24, 2, x))

// TEX4COORD [22:23] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX4COORD_B 22
/* end bit    */ #define GX_CP_VCD_HI_TEX4COORD_E 23
/* bit size   */ #define GX_CP_VCD_HI_TEX4COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX4COORD_MASK (((1 << 2) - 1) << 31 - 23)
/* local mask */ #define GX_CP_VCD_HI_TEX4COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX4COORD_SHIFT 8

/* get value  */ #define GX_CP_GET_VCD_HI_TEX4COORD(reg) GX_BITGET((reg), 22, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX4COORD(reg, x) ((reg) = GX_BITSET((reg), 22, 2, x))

// TEX5COORD [20:21] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX5COORD_B 20
/* end bit    */ #define GX_CP_VCD_HI_TEX5COORD_E 21
/* bit size   */ #define GX_CP_VCD_HI_TEX5COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX5COORD_MASK (((1 << 2) - 1) << 31 - 21)
/* local mask */ #define GX_CP_VCD_HI_TEX5COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX5COORD_SHIFT 10

/* get value  */ #define GX_CP_GET_VCD_HI_TEX5COORD(reg) GX_BITGET((reg), 20, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX5COORD(reg, x) ((reg) = GX_BITSET((reg), 20, 2, x))

// TEX6COORD [18:19] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX6COORD_B 18
/* end bit    */ #define GX_CP_VCD_HI_TEX6COORD_E 19
/* bit size   */ #define GX_CP_VCD_HI_TEX6COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX6COORD_MASK (((1 << 2) - 1) << 31 - 19)
/* local mask */ #define GX_CP_VCD_HI_TEX6COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX6COORD_SHIFT 12

/* get value  */ #define GX_CP_GET_VCD_HI_TEX6COORD(reg) GX_BITGET((reg), 18, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX6COORD(reg, x) ((reg) = GX_BITSET((reg), 18, 2, x))

// TEX7COORD [16:17] (2)
/* start bit  */ #define GX_CP_VCD_HI_TEX7COORD_B 16
/* end bit    */ #define GX_CP_VCD_HI_TEX7COORD_E 17
/* bit size   */ #define GX_CP_VCD_HI_TEX7COORD_SZ 2

/* raw mask   */ #define GX_CP_VCD_HI_TEX7COORD_MASK (((1 << 2) - 1) << 31 - 17)
/* local mask */ #define GX_CP_VCD_HI_TEX7COORD_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_CP_VCD_HI_TEX7COORD_SHIFT 14

/* get value  */ #define GX_CP_GET_VCD_HI_TEX7COORD(reg) GX_BITGET((reg), 16, 2)
/* set value  */ #define GX_CP_SET_VCD_HI_TEX7COORD(reg, x) ((reg) = GX_BITSET((reg), 16, 2, x))

/******************************************************************************
 * CP register 0x70 - VAT_group0
 *****************************************************************************/
// POS_CNT [31:31] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_POS_CNT_B 31
/* end bit    */ #define GX_CP_VAT_GROUP0_POS_CNT_E 31
/* bit size   */ #define GX_CP_VAT_GROUP0_POS_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_POS_CNT_MASK (((1 << 1) - 1) << 31 - 31)
/* local mask */ #define GX_CP_VAT_GROUP0_POS_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_POS_CNT_SHIFT 0

/* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_CNT(reg) GX_BITGET((reg), 31, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_CNT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// POS_TYPE [28:30] (3)
/* start bit  */ #define GX_CP_VAT_GROUP0_POS_TYPE_B 28
/* end bit    */ #define GX_CP_VAT_GROUP0_POS_TYPE_E 30
/* bit size   */ #define GX_CP_VAT_GROUP0_POS_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP0_POS_TYPE_MASK (((1 << 3) - 1) << 31 - 30)
/* local mask */ #define GX_CP_VAT_GROUP0_POS_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_POS_TYPE_SHIFT 1

/* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_TYPE(reg) GX_BITGET((reg), 28, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_TYPE(reg, x) ((reg) = GX_BITSET((reg), 28, 3, x))

// POS_SHIFT [23:27] (5)
/* start bit  */ #define GX_CP_VAT_GROUP0_POS_SHIFT_B 23
/* end bit    */ #define GX_CP_VAT_GROUP0_POS_SHIFT_E 27
/* bit size   */ #define GX_CP_VAT_GROUP0_POS_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP0_POS_SHIFT_MASK (((1 << 5) - 1) << 31 - 27)
/* local mask */ #define GX_CP_VAT_GROUP0_POS_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_POS_SHIFT_SHIFT 4

/* get value  */ #define GX_CP_GET_VAT_GROUP0_POS_SHIFT(reg) GX_BITGET((reg), 23, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_POS_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 23, 5, x))

// NRM_CNT [22:22] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_NRM_CNT_B 22
/* end bit    */ #define GX_CP_VAT_GROUP0_NRM_CNT_E 22
/* bit size   */ #define GX_CP_VAT_GROUP0_NRM_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_NRM_CNT_MASK (((1 << 1) - 1) << 31 - 22)
/* local mask */ #define GX_CP_VAT_GROUP0_NRM_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_NRM_CNT_SHIFT 9

/* get value  */ #define GX_CP_GET_VAT_GROUP0_NRM_CNT(reg) GX_BITGET((reg), 22, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_NRM_CNT(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// NRM_TYPE [19:21] (3)
/* start bit  */ #define GX_CP_VAT_GROUP0_NRM_TYPE_B 19
/* end bit    */ #define GX_CP_VAT_GROUP0_NRM_TYPE_E 21
/* bit size   */ #define GX_CP_VAT_GROUP0_NRM_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP0_NRM_TYPE_MASK (((1 << 3) - 1) << 31 - 21)
/* local mask */ #define GX_CP_VAT_GROUP0_NRM_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_NRM_TYPE_SHIFT 10

/* get value  */ #define GX_CP_GET_VAT_GROUP0_NRM_TYPE(reg) GX_BITGET((reg), 19, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_NRM_TYPE(reg, x) ((reg) = GX_BITSET((reg), 19, 3, x))

// COLORDIFF_CNT [18:18] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_B 18
/* end bit    */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_E 18
/* bit size   */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_MASK (((1 << 1) - 1) << 31 - 18)
/* local mask */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_COLORDIFF_CNT_SHIFT 13

/* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORDIFF_CNT(reg) GX_BITGET((reg), 18, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORDIFF_CNT(reg, x) ((reg) = GX_BITSET((reg), 18, 1, x))

// COLORDIFF_TYPE [15:17] (3)
/* start bit  */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_B 15
/* end bit    */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_E 17
/* bit size   */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_MASK (((1 << 3) - 1) << 31 - 17)
/* local mask */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_COLORDIFF_TYPE_SHIFT 14

/* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORDIFF_TYPE(reg) GX_BITGET((reg), 15, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORDIFF_TYPE(reg, x) ((reg) = GX_BITSET((reg), 15, 3, x))

// COLORSPEC_CNT [14:14] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_B 14
/* end bit    */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_E 14
/* bit size   */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_MASK (((1 << 1) - 1) << 31 - 14)
/* local mask */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_COLORSPEC_CNT_SHIFT 17

/* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORSPEC_CNT(reg) GX_BITGET((reg), 14, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORSPEC_CNT(reg, x) ((reg) = GX_BITSET((reg), 14, 1, x))

// COLORSPEC_TYPE [11:13] (3)
/* start bit  */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_B 11
/* end bit    */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_E 13
/* bit size   */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_MASK (((1 << 3) - 1) << 31 - 13)
/* local mask */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_COLORSPEC_TYPE_SHIFT 18

/* get value  */ #define GX_CP_GET_VAT_GROUP0_COLORSPEC_TYPE(reg) GX_BITGET((reg), 11, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_COLORSPEC_TYPE(reg, x) ((reg) = GX_BITSET((reg), 11, 3, x))

// TXC0_CNT [10:10] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_CNT_B 10
/* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_CNT_E 10
/* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_CNT_MASK (((1 << 1) - 1) << 31 - 10)
/* local mask */ #define GX_CP_VAT_GROUP0_TXC0_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_CNT_SHIFT 21

/* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_CNT(reg) GX_BITGET((reg), 10, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_CNT(reg, x) ((reg) = GX_BITSET((reg), 10, 1, x))

// TXC0_TYPE [7:9] (3)
/* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_B 7
/* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_E 9
/* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_MASK (((1 << 3) - 1) << 31 - 9)
/* local mask */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_TYPE_SHIFT 22

/* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_TYPE(reg) GX_BITGET((reg), 7, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_TYPE(reg, x) ((reg) = GX_BITSET((reg), 7, 3, x))

// TXC0_SHIFT [2:6] (5)
/* start bit  */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_B 2
/* end bit    */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_E 6
/* bit size   */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_MASK (((1 << 5) - 1) << 31 - 6)
/* local mask */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_TXC0_SHIFT_SHIFT 25

/* get value  */ #define GX_CP_GET_VAT_GROUP0_TXC0_SHIFT(reg) GX_BITGET((reg), 2, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_TXC0_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 2, 5, x))

// BYTEDEQUANT [1:1] (1)
/* start bit  */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_B 1
/* end bit    */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_E 1
/* bit size   */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_MASK (((1 << 1) - 1) << 31 - 1)
/* local mask */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_BYTEDEQUANT_SHIFT 30

/* get value  */ #define GX_CP_GET_VAT_GROUP0_BYTEDEQUANT(reg) GX_BITGET((reg), 1, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_BYTEDEQUANT(reg, x) ((reg) = GX_BITSET((reg), 1, 1, x))

// NORMALINDEX3 [0:0] (1) - Input will be treated as three staggered indices (one per triple biased by component size)
// into normal table
/* start bit  */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_B 0
/* end bit    */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_E 0
/* bit size   */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_MASK (((1 << 1) - 1) << 31 - 0)
/* local mask */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP0_NORMALINDEX3_SHIFT 31

/* get value  */ #define GX_CP_GET_VAT_GROUP0_NORMALINDEX3(reg) GX_BITGET((reg), 0, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP0_NORMALINDEX3(reg, x) ((reg) = GX_BITSET((reg), 0, 1, x))

/******************************************************************************
 * CP register 0x80 - VAT_group1
 *****************************************************************************/
// TXC1_CNT [31:31] (1)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_CNT_B 31
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_CNT_E 31
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_CNT_MASK (((1 << 1) - 1) << 31 - 31)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC1_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_CNT_SHIFT 0

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_CNT(reg) GX_BITGET((reg), 31, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_CNT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// TXC1_TYPE [28:30] (3)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_B 28
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_E 30
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_MASK (((1 << 3) - 1) << 31 - 30)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_TYPE_SHIFT 1

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_TYPE(reg) GX_BITGET((reg), 28, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_TYPE(reg, x) ((reg) = GX_BITSET((reg), 28, 3, x))

// TXC1_SHIFT [23:27] (5)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_B 23
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_E 27
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_MASK (((1 << 5) - 1) << 31 - 27)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC1_SHIFT_SHIFT 4

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC1_SHIFT(reg) GX_BITGET((reg), 23, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC1_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 23, 5, x))

// TXC2_CNT [22:22] (1)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_CNT_B 22
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_CNT_E 22
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_CNT_MASK (((1 << 1) - 1) << 31 - 22)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC2_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_CNT_SHIFT 9

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_CNT(reg) GX_BITGET((reg), 22, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_CNT(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// TXC2_TYPE [19:21] (3)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_B 19
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_E 21
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_MASK (((1 << 3) - 1) << 31 - 21)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_TYPE_SHIFT 10

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_TYPE(reg) GX_BITGET((reg), 19, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_TYPE(reg, x) ((reg) = GX_BITSET((reg), 19, 3, x))

// TXC2_SHIFT [14:18] (5)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_B 14
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_E 18
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_MASK (((1 << 5) - 1) << 31 - 18)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC2_SHIFT_SHIFT 13

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC2_SHIFT(reg) GX_BITGET((reg), 14, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC2_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 14, 5, x))

// TXC3_CNT [13:13] (1)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_CNT_B 13
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_CNT_E 13
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_CNT_MASK (((1 << 1) - 1) << 31 - 13)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC3_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_CNT_SHIFT 18

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_CNT(reg) GX_BITGET((reg), 13, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_CNT(reg, x) ((reg) = GX_BITSET((reg), 13, 1, x))

// TXC3_TYPE [10:12] (3)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_B 10
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_E 12
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_MASK (((1 << 3) - 1) << 31 - 12)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_TYPE_SHIFT 19

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_TYPE(reg) GX_BITGET((reg), 10, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_TYPE(reg, x) ((reg) = GX_BITSET((reg), 10, 3, x))

// TXC3_SHIFT [5:9] (5)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_B 5
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_E 9
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_MASK (((1 << 5) - 1) << 31 - 9)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC3_SHIFT_SHIFT 22

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC3_SHIFT(reg) GX_BITGET((reg), 5, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC3_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 5, 5, x))

// TXC4_CNT [4:4] (1)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC4_CNT_B 4
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC4_CNT_E 4
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC4_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC4_CNT_MASK (((1 << 1) - 1) << 31 - 4)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC4_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC4_CNT_SHIFT 27

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC4_CNT(reg) GX_BITGET((reg), 4, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC4_CNT(reg, x) ((reg) = GX_BITSET((reg), 4, 1, x))

// TXC4_TYPE [1:3] (3)
/* start bit  */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_B 1
/* end bit    */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_E 3
/* bit size   */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_MASK (((1 << 3) - 1) << 31 - 3)
/* local mask */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP1_TXC4_TYPE_SHIFT 28

/* get value  */ #define GX_CP_GET_VAT_GROUP1_TXC4_TYPE(reg) GX_BITGET((reg), 1, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP1_TXC4_TYPE(reg, x) ((reg) = GX_BITSET((reg), 1, 3, x))

/******************************************************************************
 * CP register 0x90 - VAT_group2
 *****************************************************************************/
// TXC4_SHIFT [27:31] (5)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_B 27
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_E 31
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_MASK (((1 << 5) - 1) << 31 - 31)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC4_SHIFT_SHIFT 0

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC4_SHIFT(reg) GX_BITGET((reg), 27, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC4_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 27, 5, x))

// TXC5_CNT [26:26] (1)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_CNT_B 26
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_CNT_E 26
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_CNT_MASK (((1 << 1) - 1) << 31 - 26)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC5_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_CNT_SHIFT 5

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_CNT(reg) GX_BITGET((reg), 26, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_CNT(reg, x) ((reg) = GX_BITSET((reg), 26, 1, x))

// TXC5_TYPE [23:25] (3)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_B 23
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_E 25
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_MASK (((1 << 3) - 1) << 31 - 25)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_TYPE_SHIFT 6

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_TYPE(reg) GX_BITGET((reg), 23, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_TYPE(reg, x) ((reg) = GX_BITSET((reg), 23, 3, x))

// TXC5_SHIFT [18:22] (5)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_B 18
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_E 22
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_MASK (((1 << 5) - 1) << 31 - 22)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC5_SHIFT_SHIFT 9

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC5_SHIFT(reg) GX_BITGET((reg), 18, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC5_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 18, 5, x))

// TXC6_CNT [17:17] (1)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_CNT_B 17
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_CNT_E 17
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_CNT_MASK (((1 << 1) - 1) << 31 - 17)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC6_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_CNT_SHIFT 14

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_CNT(reg) GX_BITGET((reg), 17, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_CNT(reg, x) ((reg) = GX_BITSET((reg), 17, 1, x))

// TXC6_TYPE [14:16] (3)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_B 14
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_E 16
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_MASK (((1 << 3) - 1) << 31 - 16)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_TYPE_SHIFT 15

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_TYPE(reg) GX_BITGET((reg), 14, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_TYPE(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))

// TXC6_SHIFT [9:13] (5)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_B 9
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_E 13
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_MASK (((1 << 5) - 1) << 31 - 13)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC6_SHIFT_SHIFT 18

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC6_SHIFT(reg) GX_BITGET((reg), 9, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC6_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 9, 5, x))

// TXC7_CNT [8:8] (1)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_CNT_B 8
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_CNT_E 8
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_CNT_SZ 1

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_CNT_MASK (((1 << 1) - 1) << 31 - 8)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC7_CNT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_CNT_SHIFT 23

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_CNT(reg) GX_BITGET((reg), 8, 1)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_CNT(reg, x) ((reg) = GX_BITSET((reg), 8, 1, x))

// TXC7_TYPE [5:7] (3)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_B 5
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_E 7
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_SZ 3

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_MASK (((1 << 3) - 1) << 31 - 7)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_TYPE_SHIFT 24

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_TYPE(reg) GX_BITGET((reg), 5, 3)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_TYPE(reg, x) ((reg) = GX_BITSET((reg), 5, 3, x))

// TXC7_SHIFT [0:4] (5)
/* start bit  */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_B 0
/* end bit    */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_E 4
/* bit size   */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_SZ 5

/* raw mask   */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_MASK (((1 << 5) - 1) << 31 - 4)
/* local mask */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_CP_VAT_GROUP2_TXC7_SHIFT_SHIFT 27

/* get value  */ #define GX_CP_GET_VAT_GROUP2_TXC7_SHIFT(reg) GX_BITGET((reg), 0, 5)
/* set value  */ #define GX_CP_SET_VAT_GROUP2_TXC7_SHIFT(reg, x) ((reg) = GX_BITSET((reg), 0, 5, x))

/******************************************************************************
 * CP register 0xA0 - ArrayBase
 *****************************************************************************/
// BASE [6:31] (26)
/* start bit  */ #define GX_CP_ARRAYBASE_BASE_B 6
/* end bit    */ #define GX_CP_ARRAYBASE_BASE_E 31
/* bit size   */ #define GX_CP_ARRAYBASE_BASE_SZ 26

/* raw mask   */ #define GX_CP_ARRAYBASE_BASE_MASK (((1 << 26) - 1) << 31 - 31)
/* local mask */ #define GX_CP_ARRAYBASE_BASE_LMASK ((1 << 26) - 1)
/* bit shift  */ #define GX_CP_ARRAYBASE_BASE_SHIFT 0

/* get value  */ #define GX_CP_GET_ARRAYBASE_BASE(reg) GX_BITGET((reg), 6, 26)
/* set value  */ #define GX_CP_SET_ARRAYBASE_BASE(reg, x) ((reg) = GX_BITSET((reg), 6, 26, x))

/******************************************************************************
 * CP register 0xB0 - ArrayStride
 *****************************************************************************/
// STRIDE [24:31] (8)
/* start bit  */ #define GX_CP_ARRAYSTRIDE_STRIDE_B 24
/* end bit    */ #define GX_CP_ARRAYSTRIDE_STRIDE_E 31
/* bit size   */ #define GX_CP_ARRAYSTRIDE_STRIDE_SZ 8

/* raw mask   */ #define GX_CP_ARRAYSTRIDE_STRIDE_MASK (((1 << 8) - 1) << 31 - 31)
/* local mask */ #define GX_CP_ARRAYSTRIDE_STRIDE_LMASK ((1 << 8) - 1)
/* bit shift  */ #define GX_CP_ARRAYSTRIDE_STRIDE_SHIFT 0

/* get value  */ #define GX_CP_GET_ARRAYSTRIDE_STRIDE(reg) GX_BITGET((reg), 24, 8)
/* set value  */ #define GX_CP_SET_ARRAYSTRIDE_STRIDE(reg, x) ((reg) = GX_BITSET((reg), 24, 8, x))

#ifdef __cplusplus
}
#endif
#endif
