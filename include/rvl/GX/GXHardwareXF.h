#ifndef RVL_SDK_GX_HARDWARE_XF_H
#define RVL_SDK_GX_HARDWARE_XF_H
#include "rvl/GX/GXTypes.h"

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 *
 *
 * GX Transform Unit (XF)
 *
 *
 *****************************************************************************/

/**
 * XF registers
 */
typedef enum {
    GX_XF_REG_ERROR = 0x1000,
    GX_XF_REG_DIAGNOSTICS = 0x1001,
    GX_XF_REG_STATE0 = 0x1002,
    GX_XF_REG_STATE1 = 0x1003,
    GX_XF_REG_CLOCK = 0x1004,
    GX_XF_REG_CLIPDISABLE = 0x1005,
    GX_XF_REG_PERF0 = 0x1006,
    GX_XF_REG_PERF1 = 0x1007,
    GX_XF_REG_INVERTEXSPEC = 0x1008,
    GX_XF_REG_NUMCOLORS = 0x1009,
    GX_XF_REG_AMBIENT0 = 0x100A,
    GX_XF_REG_AMBIENT1 = 0x100B,
    GX_XF_REG_MATERIAL0 = 0x100C,
    GX_XF_REG_MATERIAL1 = 0x100D,
    GX_XF_REG_COLOR0CNTRL = 0x100E,
    GX_XF_REG_COLOR1CNTRL = 0x100F,
    GX_XF_REG_ALPHA0CNTRL = 0x1010,
    GX_XF_REG_ALPHA1CNTRL = 0x1011,
    GX_XF_REG_DUALTEXTRAN = 0x1012,
    GX_XF_REG_MATRIXINDEX0 = 0x1018,
    GX_XF_REG_MATRIXINDEX1 = 0x1019,
    GX_XF_REG_SCALEX = 0x101A,
    GX_XF_REG_SCALEY = 0x101B,
    GX_XF_REG_SCALEZ = 0x101C,
    GX_XF_REG_OFFSETX = 0x101D,
    GX_XF_REG_OFFSETY = 0x101E,
    GX_XF_REG_OFFSETZ = 0x101F,
    GX_XF_REG_PROJECTIONA = 0x1020,
    GX_XF_REG_PROJECTIONB = 0x1021,
    GX_XF_REG_PROJECTIONC = 0x1022,
    GX_XF_REG_PROJECTIOND = 0x1023,
    GX_XF_REG_PROJECTIONE = 0x1024,
    GX_XF_REG_PROJECTIONF = 0x1025,
    GX_XF_REG_PROJECTORTHO = 0x1026,
    GX_XF_REG_NUMTEX = 0x103F,
    GX_XF_REG_TEX0 = 0x1040,
    GX_XF_REG_TEX1 = 0x1041,
    GX_XF_REG_TEX2 = 0x1042,
    GX_XF_REG_TEX3 = 0x1043,
    GX_XF_REG_TEX4 = 0x1044,
    GX_XF_REG_TEX5 = 0x1045,
    GX_XF_REG_TEX6 = 0x1046,
    GX_XF_REG_TEX7 = 0x1047,
    GX_XF_REG_DUALTEX0 = 0x1050,
    GX_XF_REG_DUALTEX1 = 0x1051,
    GX_XF_REG_DUALTEX2 = 0x1052,
    GX_XF_REG_DUALTEX3 = 0x1053,
    GX_XF_REG_DUALTEX4 = 0x1054,
    GX_XF_REG_DUALTEX5 = 0x1055,
    GX_XF_REG_DUALTEX6 = 0x1056,
    GX_XF_REG_DUALTEX7 = 0x1057,
} GX_XF_REG;

/******************************************************************************
 * XF register 0x1005 - ClipDisable
 *****************************************************************************/
// DETECT [31:31] (1) - Disable clipping detection
/* start bit  */ #define GX_XF_CLIPDISABLE_DETECT_B 31
/* end bit    */ #define GX_XF_CLIPDISABLE_DETECT_E 31
/* bit size   */ #define GX_XF_CLIPDISABLE_DETECT_SZ 1

/* raw mask   */ #define GX_XF_CLIPDISABLE_DETECT_MASK (((1 << 1) - 1) << 31 - 31)
/* local mask */ #define GX_XF_CLIPDISABLE_DETECT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_CLIPDISABLE_DETECT_SHIFT 0

/* get value  */ #define GX_XF_GET_CLIPDISABLE_DETECT(reg) GX_BITGET((reg), 31, 1)
/* set value  */ #define GX_XF_SET_CLIPDISABLE_DETECT(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// REJECT [30:30] (1) - Disable trivial rejection
/* start bit  */ #define GX_XF_CLIPDISABLE_REJECT_B 30
/* end bit    */ #define GX_XF_CLIPDISABLE_REJECT_E 30
/* bit size   */ #define GX_XF_CLIPDISABLE_REJECT_SZ 1

/* raw mask   */ #define GX_XF_CLIPDISABLE_REJECT_MASK (((1 << 1) - 1) << 31 - 30)
/* local mask */ #define GX_XF_CLIPDISABLE_REJECT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_CLIPDISABLE_REJECT_SHIFT 1

/* get value  */ #define GX_XF_GET_CLIPDISABLE_REJECT(reg) GX_BITGET((reg), 30, 1)
/* set value  */ #define GX_XF_SET_CLIPDISABLE_REJECT(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// ACCEL [29:29] (1) - Disable cpoly clipping acceleration
/* start bit  */ #define GX_XF_CLIPDISABLE_ACCEL_B 29
/* end bit    */ #define GX_XF_CLIPDISABLE_ACCEL_E 29
/* bit size   */ #define GX_XF_CLIPDISABLE_ACCEL_SZ 1

/* raw mask   */ #define GX_XF_CLIPDISABLE_ACCEL_MASK (((1 << 1) - 1) << 31 - 29)
/* local mask */ #define GX_XF_CLIPDISABLE_ACCEL_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_CLIPDISABLE_ACCEL_SHIFT 2

/* get value  */ #define GX_XF_GET_CLIPDISABLE_ACCEL(reg) GX_BITGET((reg), 29, 1)
/* set value  */ #define GX_XF_SET_CLIPDISABLE_ACCEL(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

/******************************************************************************
 * XF register 0x1007 - Perf1
 *****************************************************************************/
// TARGET [25:31] (7) - Target performance (Cycles/vertex)
/* start bit  */ #define GX_XF_PERF1_TARGET_B 25
/* end bit    */ #define GX_XF_PERF1_TARGET_E 31
/* bit size   */ #define GX_XF_PERF1_TARGET_SZ 7

/* raw mask   */ #define GX_XF_PERF1_TARGET_MASK (((1 << 7) - 1) << 31 - 31)
/* local mask */ #define GX_XF_PERF1_TARGET_LMASK ((1 << 7) - 1)
/* bit shift  */ #define GX_XF_PERF1_TARGET_SHIFT 0

/* get value  */ #define GX_XF_GET_PERF1_TARGET(reg) GX_BITGET((reg), 25, 7)
/* set value  */ #define GX_XF_SET_PERF1_TARGET(reg, x) ((reg) = GX_BITSET((reg), 25, 7, x))

/******************************************************************************
 * XF register 0x1008 - InVertexSpec
 *****************************************************************************/
// CLR [30:31] (2)
/* start bit  */ #define GX_XF_INVERTEXSPEC_CLR_B 30
/* end bit    */ #define GX_XF_INVERTEXSPEC_CLR_E 31
/* bit size   */ #define GX_XF_INVERTEXSPEC_CLR_SZ 2

/* raw mask   */ #define GX_XF_INVERTEXSPEC_CLR_MASK (((1 << 2) - 1) << 31 - 31)
/* local mask */ #define GX_XF_INVERTEXSPEC_CLR_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_XF_INVERTEXSPEC_CLR_SHIFT 0

/* get value  */ #define GX_XF_GET_INVERTEXSPEC_CLR(reg) GX_BITGET((reg), 30, 2)
/* set value  */ #define GX_XF_SET_INVERTEXSPEC_CLR(reg, x) ((reg) = GX_BITSET((reg), 30, 2, x))

// NRM [28:29] (2)
/* start bit  */ #define GX_XF_INVERTEXSPEC_NRM_B 28
/* end bit    */ #define GX_XF_INVERTEXSPEC_NRM_E 29
/* bit size   */ #define GX_XF_INVERTEXSPEC_NRM_SZ 2

/* raw mask   */ #define GX_XF_INVERTEXSPEC_NRM_MASK (((1 << 2) - 1) << 31 - 29)
/* local mask */ #define GX_XF_INVERTEXSPEC_NRM_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_XF_INVERTEXSPEC_NRM_SHIFT 2

/* get value  */ #define GX_XF_GET_INVERTEXSPEC_NRM(reg) GX_BITGET((reg), 28, 2)
/* set value  */ #define GX_XF_SET_INVERTEXSPEC_NRM(reg, x) ((reg) = GX_BITSET((reg), 28, 2, x))

// TXC [24:27] (4)
/* start bit  */ #define GX_XF_INVERTEXSPEC_TXC_B 24
/* end bit    */ #define GX_XF_INVERTEXSPEC_TXC_E 27
/* bit size   */ #define GX_XF_INVERTEXSPEC_TXC_SZ 4

/* raw mask   */ #define GX_XF_INVERTEXSPEC_TXC_MASK (((1 << 4) - 1) << 31 - 27)
/* local mask */ #define GX_XF_INVERTEXSPEC_TXC_LMASK ((1 << 4) - 1)
/* bit shift  */ #define GX_XF_INVERTEXSPEC_TXC_SHIFT 4

/* get value  */ #define GX_XF_GET_INVERTEXSPEC_TXC(reg) GX_BITGET((reg), 24, 4)
/* set value  */ #define GX_XF_SET_INVERTEXSPEC_TXC(reg, x) ((reg) = GX_BITSET((reg), 24, 4, x))

/******************************************************************************
 * XF register 0x100E - Color0Cntrl
 *****************************************************************************/
// MATSRC [31:31] (1)
/* start bit  */ #define GX_XF_COLOR0CNTRL_MATSRC_B 31
/* end bit    */ #define GX_XF_COLOR0CNTRL_MATSRC_E 31
/* bit size   */ #define GX_XF_COLOR0CNTRL_MATSRC_SZ 1

/* raw mask   */ #define GX_XF_COLOR0CNTRL_MATSRC_MASK (((1 << 1) - 1) << 31 - 31)
/* local mask */ #define GX_XF_COLOR0CNTRL_MATSRC_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_MATSRC_SHIFT 0

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_MATSRC(reg) GX_BITGET((reg), 31, 1)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_MATSRC(reg, x) ((reg) = GX_BITSET((reg), 31, 1, x))

// LIGHT [30:30] (1)
/* start bit  */ #define GX_XF_COLOR0CNTRL_LIGHT_B 30
/* end bit    */ #define GX_XF_COLOR0CNTRL_LIGHT_E 30
/* bit size   */ #define GX_XF_COLOR0CNTRL_LIGHT_SZ 1

/* raw mask   */ #define GX_XF_COLOR0CNTRL_LIGHT_MASK (((1 << 1) - 1) << 31 - 30)
/* local mask */ #define GX_XF_COLOR0CNTRL_LIGHT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_LIGHT_SHIFT 1

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_LIGHT(reg) GX_BITGET((reg), 30, 1)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_LIGHT(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// LMASKLO [26:29] (4)
/* start bit  */ #define GX_XF_COLOR0CNTRL_LMASKLO_B 26
/* end bit    */ #define GX_XF_COLOR0CNTRL_LMASKLO_E 29
/* bit size   */ #define GX_XF_COLOR0CNTRL_LMASKLO_SZ 4

/* raw mask   */ #define GX_XF_COLOR0CNTRL_LMASKLO_MASK (((1 << 4) - 1) << 31 - 29)
/* local mask */ #define GX_XF_COLOR0CNTRL_LMASKLO_LMASK ((1 << 4) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_LMASKLO_SHIFT 2

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_LMASKLO(reg) GX_BITGET((reg), 26, 4)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_LMASKLO(reg, x) ((reg) = GX_BITSET((reg), 26, 4, x))

// AMBSRC [25:25] (1)
/* start bit  */ #define GX_XF_COLOR0CNTRL_AMBSRC_B 25
/* end bit    */ #define GX_XF_COLOR0CNTRL_AMBSRC_E 25
/* bit size   */ #define GX_XF_COLOR0CNTRL_AMBSRC_SZ 1

/* raw mask   */ #define GX_XF_COLOR0CNTRL_AMBSRC_MASK (((1 << 1) - 1) << 31 - 25)
/* local mask */ #define GX_XF_COLOR0CNTRL_AMBSRC_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_AMBSRC_SHIFT 6

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_AMBSRC(reg) GX_BITGET((reg), 25, 1)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_AMBSRC(reg, x) ((reg) = GX_BITSET((reg), 25, 1, x))

// DIFFUSEATTN [23:24] (2)
/* start bit  */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_B 23
/* end bit    */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_E 24
/* bit size   */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_SZ 2

/* raw mask   */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_MASK (((1 << 2) - 1) << 31 - 24)
/* local mask */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_LMASK ((1 << 2) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_DIFFUSEATTN_SHIFT 7

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_DIFFUSEATTN(reg) GX_BITGET((reg), 23, 2)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_DIFFUSEATTN(reg, x) ((reg) = GX_BITSET((reg), 23, 2, x))

// ATTNENABLE [22:22] (1)
/* start bit  */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_B 22
/* end bit    */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_E 22
/* bit size   */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_SZ 1

/* raw mask   */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_MASK (((1 << 1) - 1) << 31 - 22)
/* local mask */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_ATTNENABLE_SHIFT 9

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_ATTNENABLE(reg) GX_BITGET((reg), 22, 1)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_ATTNENABLE(reg, x) ((reg) = GX_BITSET((reg), 22, 1, x))

// ATTNSELECT [21:21] (1)
/* start bit  */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_B 21
/* end bit    */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_E 21
/* bit size   */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_SZ 1

/* raw mask   */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_MASK (((1 << 1) - 1) << 31 - 21)
/* local mask */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_ATTNSELECT_SHIFT 10

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_ATTNSELECT(reg) GX_BITGET((reg), 21, 1)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_ATTNSELECT(reg, x) ((reg) = GX_BITSET((reg), 21, 1, x))

// LMASKHI [17:20] (4)
/* start bit  */ #define GX_XF_COLOR0CNTRL_LMASKHI_B 17
/* end bit    */ #define GX_XF_COLOR0CNTRL_LMASKHI_E 20
/* bit size   */ #define GX_XF_COLOR0CNTRL_LMASKHI_SZ 4

/* raw mask   */ #define GX_XF_COLOR0CNTRL_LMASKHI_MASK (((1 << 4) - 1) << 31 - 20)
/* local mask */ #define GX_XF_COLOR0CNTRL_LMASKHI_LMASK ((1 << 4) - 1)
/* bit shift  */ #define GX_XF_COLOR0CNTRL_LMASKHI_SHIFT 11

/* get value  */ #define GX_XF_GET_COLOR0CNTRL_LMASKHI(reg) GX_BITGET((reg), 17, 4)
/* set value  */ #define GX_XF_SET_COLOR0CNTRL_LMASKHI(reg, x) ((reg) = GX_BITSET((reg), 17, 4, x))

/******************************************************************************
 * XF register 0x1018 - MatrixIndex0
 *****************************************************************************/
// GEOM [26:31] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX0_GEOM_B 26
/* end bit    */ #define GX_XF_MATRIXINDEX0_GEOM_E 31
/* bit size   */ #define GX_XF_MATRIXINDEX0_GEOM_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX0_GEOM_MASK (((1 << 6) - 1) << 31 - 31)
/* local mask */ #define GX_XF_MATRIXINDEX0_GEOM_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX0_GEOM_SHIFT 0

/* get value  */ #define GX_XF_GET_MATRIXINDEX0_GEOM(reg) GX_BITGET((reg), 26, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX0_GEOM(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// TEX0 [20:25] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX0_TEX0_B 20
/* end bit    */ #define GX_XF_MATRIXINDEX0_TEX0_E 25
/* bit size   */ #define GX_XF_MATRIXINDEX0_TEX0_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX0_MASK (((1 << 6) - 1) << 31 - 25)
/* local mask */ #define GX_XF_MATRIXINDEX0_TEX0_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX0_SHIFT 6

/* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX0(reg) GX_BITGET((reg), 20, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX0(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX1 [14:19] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX0_TEX1_B 14
/* end bit    */ #define GX_XF_MATRIXINDEX0_TEX1_E 19
/* bit size   */ #define GX_XF_MATRIXINDEX0_TEX1_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX1_MASK (((1 << 6) - 1) << 31 - 19)
/* local mask */ #define GX_XF_MATRIXINDEX0_TEX1_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX1_SHIFT 12

/* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX1(reg) GX_BITGET((reg), 14, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX1(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX2 [8:13] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX0_TEX2_B 8
/* end bit    */ #define GX_XF_MATRIXINDEX0_TEX2_E 13
/* bit size   */ #define GX_XF_MATRIXINDEX0_TEX2_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX2_MASK (((1 << 6) - 1) << 31 - 13)
/* local mask */ #define GX_XF_MATRIXINDEX0_TEX2_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX2_SHIFT 18

/* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX2(reg) GX_BITGET((reg), 8, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX2(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

// TEX3 [2:7] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX0_TEX3_B 2
/* end bit    */ #define GX_XF_MATRIXINDEX0_TEX3_E 7
/* bit size   */ #define GX_XF_MATRIXINDEX0_TEX3_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX0_TEX3_MASK (((1 << 6) - 1) << 31 - 7)
/* local mask */ #define GX_XF_MATRIXINDEX0_TEX3_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX0_TEX3_SHIFT 24

/* get value  */ #define GX_XF_GET_MATRIXINDEX0_TEX3(reg) GX_BITGET((reg), 2, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX0_TEX3(reg, x) ((reg) = GX_BITSET((reg), 2, 6, x))

/******************************************************************************
 * XF register 0x1019 - MatrixIndex1
 *****************************************************************************/
// TEX4 [26:31] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX1_TEX4_B 26
/* end bit    */ #define GX_XF_MATRIXINDEX1_TEX4_E 31
/* bit size   */ #define GX_XF_MATRIXINDEX1_TEX4_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX4_MASK (((1 << 6) - 1) << 31 - 31)
/* local mask */ #define GX_XF_MATRIXINDEX1_TEX4_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX4_SHIFT 0

/* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX4(reg) GX_BITGET((reg), 26, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX4(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// TEX5 [20:25] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX1_TEX5_B 20
/* end bit    */ #define GX_XF_MATRIXINDEX1_TEX5_E 25
/* bit size   */ #define GX_XF_MATRIXINDEX1_TEX5_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX5_MASK (((1 << 6) - 1) << 31 - 25)
/* local mask */ #define GX_XF_MATRIXINDEX1_TEX5_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX5_SHIFT 6

/* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX5(reg) GX_BITGET((reg), 20, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX5(reg, x) ((reg) = GX_BITSET((reg), 20, 6, x))

// TEX6 [14:19] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX1_TEX6_B 14
/* end bit    */ #define GX_XF_MATRIXINDEX1_TEX6_E 19
/* bit size   */ #define GX_XF_MATRIXINDEX1_TEX6_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX6_MASK (((1 << 6) - 1) << 31 - 19)
/* local mask */ #define GX_XF_MATRIXINDEX1_TEX6_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX6_SHIFT 12

/* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX6(reg) GX_BITGET((reg), 14, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX6(reg, x) ((reg) = GX_BITSET((reg), 14, 6, x))

// TEX7 [8:13] (6)
/* start bit  */ #define GX_XF_MATRIXINDEX1_TEX7_B 8
/* end bit    */ #define GX_XF_MATRIXINDEX1_TEX7_E 13
/* bit size   */ #define GX_XF_MATRIXINDEX1_TEX7_SZ 6

/* raw mask   */ #define GX_XF_MATRIXINDEX1_TEX7_MASK (((1 << 6) - 1) << 31 - 13)
/* local mask */ #define GX_XF_MATRIXINDEX1_TEX7_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_MATRIXINDEX1_TEX7_SHIFT 18

/* get value  */ #define GX_XF_GET_MATRIXINDEX1_TEX7(reg) GX_BITGET((reg), 8, 6)
/* set value  */ #define GX_XF_SET_MATRIXINDEX1_TEX7(reg, x) ((reg) = GX_BITSET((reg), 8, 6, x))

/******************************************************************************
 * XF structure - Tex
 *****************************************************************************/
// PROJTYPE [30:30] (1)
/* start bit  */ #define GX_XF_TEX_PROJTYPE_B 30
/* end bit    */ #define GX_XF_TEX_PROJTYPE_E 30
/* bit size   */ #define GX_XF_TEX_PROJTYPE_SZ 1

/* raw mask   */ #define GX_XF_TEX_PROJTYPE_MASK (((1 << 1) - 1) << 31 - 30)
/* local mask */ #define GX_XF_TEX_PROJTYPE_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_TEX_PROJTYPE_SHIFT 1

/* get value  */ #define GX_XF_GET_TEX_PROJTYPE(reg) GX_BITGET((reg), 30, 1)
/* set value  */ #define GX_XF_SET_TEX_PROJTYPE(reg, x) ((reg) = GX_BITSET((reg), 30, 1, x))

// INPUTFORM [29:29] (1)
/* start bit  */ #define GX_XF_TEX_INPUTFORM_B 29
/* end bit    */ #define GX_XF_TEX_INPUTFORM_E 29
/* bit size   */ #define GX_XF_TEX_INPUTFORM_SZ 1

/* raw mask   */ #define GX_XF_TEX_INPUTFORM_MASK (((1 << 1) - 1) << 31 - 29)
/* local mask */ #define GX_XF_TEX_INPUTFORM_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_TEX_INPUTFORM_SHIFT 2

/* get value  */ #define GX_XF_GET_TEX_INPUTFORM(reg) GX_BITGET((reg), 29, 1)
/* set value  */ #define GX_XF_SET_TEX_INPUTFORM(reg, x) ((reg) = GX_BITSET((reg), 29, 1, x))

// TEXGENTYPE [25:27] (3)
/* start bit  */ #define GX_XF_TEX_TEXGENTYPE_B 25
/* end bit    */ #define GX_XF_TEX_TEXGENTYPE_E 27
/* bit size   */ #define GX_XF_TEX_TEXGENTYPE_SZ 3

/* raw mask   */ #define GX_XF_TEX_TEXGENTYPE_MASK (((1 << 3) - 1) << 31 - 27)
/* local mask */ #define GX_XF_TEX_TEXGENTYPE_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_XF_TEX_TEXGENTYPE_SHIFT 4

/* get value  */ #define GX_XF_GET_TEX_TEXGENTYPE(reg) GX_BITGET((reg), 25, 3)
/* set value  */ #define GX_XF_SET_TEX_TEXGENTYPE(reg, x) ((reg) = GX_BITSET((reg), 25, 3, x))

// SRCROW [20:24] (5)
/* start bit  */ #define GX_XF_TEX_SRCROW_B 20
/* end bit    */ #define GX_XF_TEX_SRCROW_E 24
/* bit size   */ #define GX_XF_TEX_SRCROW_SZ 5

/* raw mask   */ #define GX_XF_TEX_SRCROW_MASK (((1 << 5) - 1) << 31 - 24)
/* local mask */ #define GX_XF_TEX_SRCROW_LMASK ((1 << 5) - 1)
/* bit shift  */ #define GX_XF_TEX_SRCROW_SHIFT 7

/* get value  */ #define GX_XF_GET_TEX_SRCROW(reg) GX_BITGET((reg), 20, 5)
/* set value  */ #define GX_XF_SET_TEX_SRCROW(reg, x) ((reg) = GX_BITSET((reg), 20, 5, x))

// BUMPSRCTEX [17:19] (3)
/* start bit  */ #define GX_XF_TEX_BUMPSRCTEX_B 17
/* end bit    */ #define GX_XF_TEX_BUMPSRCTEX_E 19
/* bit size   */ #define GX_XF_TEX_BUMPSRCTEX_SZ 3

/* raw mask   */ #define GX_XF_TEX_BUMPSRCTEX_MASK (((1 << 3) - 1) << 31 - 19)
/* local mask */ #define GX_XF_TEX_BUMPSRCTEX_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_XF_TEX_BUMPSRCTEX_SHIFT 12

/* get value  */ #define GX_XF_GET_TEX_BUMPSRCTEX(reg) GX_BITGET((reg), 17, 3)
/* set value  */ #define GX_XF_SET_TEX_BUMPSRCTEX(reg, x) ((reg) = GX_BITSET((reg), 17, 3, x))

// BUMPSRCLIGHT [14:16] (3)
/* start bit  */ #define GX_XF_TEX_BUMPSRCLIGHT_B 14
/* end bit    */ #define GX_XF_TEX_BUMPSRCLIGHT_E 16
/* bit size   */ #define GX_XF_TEX_BUMPSRCLIGHT_SZ 3

/* raw mask   */ #define GX_XF_TEX_BUMPSRCLIGHT_MASK (((1 << 3) - 1) << 31 - 16)
/* local mask */ #define GX_XF_TEX_BUMPSRCLIGHT_LMASK ((1 << 3) - 1)
/* bit shift  */ #define GX_XF_TEX_BUMPSRCLIGHT_SHIFT 15

/* get value  */ #define GX_XF_GET_TEX_BUMPSRCLIGHT(reg) GX_BITGET((reg), 14, 3)
/* set value  */ #define GX_XF_SET_TEX_BUMPSRCLIGHT(reg, x) ((reg) = GX_BITSET((reg), 14, 3, x))

/******************************************************************************
 * XF structure - DualTex
 *****************************************************************************/
// BASEROW [26:31] (6) - Indicates which is the base row of the transform matrix
/* start bit  */ #define GX_XF_DUALTEX_BASEROW_B 26
/* end bit    */ #define GX_XF_DUALTEX_BASEROW_E 31
/* bit size   */ #define GX_XF_DUALTEX_BASEROW_SZ 6

/* raw mask   */ #define GX_XF_DUALTEX_BASEROW_MASK (((1 << 6) - 1) << 31 - 31)
/* local mask */ #define GX_XF_DUALTEX_BASEROW_LMASK ((1 << 6) - 1)
/* bit shift  */ #define GX_XF_DUALTEX_BASEROW_SHIFT 0

/* get value  */ #define GX_XF_GET_DUALTEX_BASEROW(reg) GX_BITGET((reg), 26, 6)
/* set value  */ #define GX_XF_SET_DUALTEX_BASEROW(reg, x) ((reg) = GX_BITSET((reg), 26, 6, x))

// NORMALIZE [23:23] (1) - Normalize texcoord before sending transform
/* start bit  */ #define GX_XF_DUALTEX_NORMALIZE_B 23
/* end bit    */ #define GX_XF_DUALTEX_NORMALIZE_E 23
/* bit size   */ #define GX_XF_DUALTEX_NORMALIZE_SZ 1

/* raw mask   */ #define GX_XF_DUALTEX_NORMALIZE_MASK (((1 << 1) - 1) << 31 - 23)
/* local mask */ #define GX_XF_DUALTEX_NORMALIZE_LMASK ((1 << 1) - 1)
/* bit shift  */ #define GX_XF_DUALTEX_NORMALIZE_SHIFT 8

/* get value  */ #define GX_XF_GET_DUALTEX_NORMALIZE(reg) GX_BITGET((reg), 23, 1)
/* set value  */ #define GX_XF_SET_DUALTEX_NORMALIZE(reg, x) ((reg) = GX_BITSET((reg), 23, 1, x))

#ifdef __cplusplus
}
#endif
#endif
