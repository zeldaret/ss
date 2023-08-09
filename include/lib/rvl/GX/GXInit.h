#ifndef RVL_SDK_GX_INIT_H
#define RVL_SDK_GX_INIT_H
#include <lib/rvl/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXData {
    union {
        UNKWORD WORD_0x0;
        struct {
            u16 SHORT_0x0;      // at 0x0
            u16 lastWriteWasXF; // at 0x2
        };
    }; // at 0x0
    u16 SHORT_0x4;
    u16 vlim;      // at 0x6
    u32 cpCtrlReg; // at 0x8
    u32 cpStatReg; // at 0xC
    char UNK_0x10[0x4];
    u32 vcdLoReg;            // at 0x14
    u32 vcdHiReg;            // at 0x18
    u32 vatA[GX_MAX_VTXFMT]; // at 0x1C
    u32 vatB[GX_MAX_VTXFMT]; // at 0x3C
    u32 vatC[GX_MAX_VTXFMT]; // at 0x5C
    u32 linePtWidth;         // at 0x7C
    u32 matrixIndex0;        // at 0x80
    u32 matrixIndex1;        // at 0x84
    char UNK_0x88[0xA8 - 0x88];
    GXColor ambColors[2];             // at 0xA8
    GXColor matColors[2];             // at 0xB0
    u32 colorControl[4];              // at 0xB8
    u32 texRegs[GX_MAX_TEXCOORD];     // at 0xC8
    u32 dualTexRegs[GX_MAX_TEXCOORD]; // at 0xE8
    u32 txcRegs[GX_MAX_TEXCOORD];     // at 0x108
    char UNK_0x128[0x148 - 0x128];
    u32 scissorTL; // at 0x148
    u32 scissorBR; // at 0x14C
    char UNK_0x150[0x170 - 0x150];
    u32 ras1_iref; // at 0x170
    u32 ind_imask; // at 0x174
    u32 ras1_ss0;  // at 0x178
    u32 ras1_ss1;  // at 0x17C
    char UNK_0x180[0x220 - 0x180];
    u32 blendMode; // at 0x220
    u32 dstAlpha;  // at 0x224
    u32 zMode;     // at 0x228
    u32 zControl;  // at 0x22C
    char UNK_0x230[0x254 - 0x230];
    u32 genMode; // at 0x254
    char UNK_0x258[0x520 - 0x258];
    GXAttrType normalType;  // at 0x520
    GXBool normal;          // at 0x524
    GXBool binormal;        // at 0x525
    GXProjMtxType projType; // at 0x528
    f32 proj[6];            // at 0x52C
    union {
        struct {
            f32 vpOx;   // at 0x544
            f32 vpOy;   // at 0x548
            f32 vpSx;   // at 0x54C
            f32 vpSy;   // at 0x550
            f32 vpNear; // at 0x554
            f32 vpFar;  // at 0x558
        };
        f32 view[6];
    };           // at 0x544
    f32 offsetZ; // at 0x55C
    f32 scaleZ;  // at 0x560
    char UNK_0x564[0x5F8 - 0x564];
    GXBool dlistActive; // at 0x5F8
    GXBool dlistSave;   // at 0x5F9
    u8 BYTE_0x5FA;
    u8 vatDirtyFlags; // at 0x5FB
    u32 gxDirtyFlags; // at 0x5FC
} GXData;

extern GXData* const __GXData;

// I hate typing this name out
#define gxdt __GXData

#ifdef __cplusplus
}
#endif
#endif
