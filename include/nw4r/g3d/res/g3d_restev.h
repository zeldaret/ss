#ifndef NW4R_G3D_RES_RES_TEV_H
#define NW4R_G3D_RES_RES_TEV_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {

// TODO: Why?
static const int TEV_STAGES_PER_DL = 2;

struct ResTevCommonDL {
    union {
        struct {
            u8 swapModeTable[GX_MAX_TEVSWAP][GX_BP_CMD_SZ * 4]; // at 0x0
            u8 indTexOrder[1][GX_BP_CMD_SZ];                    // at 0x50
            u8 PADDING_0x55[0x60 - 0x55];                       // at 0x55
        } dl;

        u8 data[0x60]; // at 0x0
    };
};

struct ResTevVariableDL {
    union {
        struct {
            u8 tevKonstantSel[GX_BP_CMD_SZ * 2];                  // at 0x0
            u8 tevOrder[GX_BP_CMD_SZ];                            // at 0xA
            u8 tevColorCalc[TEV_STAGES_PER_DL][GX_BP_CMD_SZ];     // at 0xF
            u8 alphaCalcAndSwap[TEV_STAGES_PER_DL][GX_BP_CMD_SZ]; // at 0x19
            u8 tevIndirect[TEV_STAGES_PER_DL][GX_BP_CMD_SZ];      // at 0x23
            u8 PADDING_0x2D[0x30 - 0x2D];                         // at 0x2D
        } dl;

        u8 data[0x30]; // at 0x0
    };
};

struct ResTevDL {
    union {
        struct {
            ResTevCommonDL common;                                     // at 0x0
            ResTevVariableDL var[GX_MAX_TEVSTAGE / TEV_STAGES_PER_DL]; // at 0x60
        } dl;

        u8 data[0x1E0]; // at 0x0
    };
};

struct ResTevData {
    u32 size;                               // at 0x0
    s32 toResMdlData;                       // at 0x4
    u32 id;                                 // at 0x8
    u8 nStages;                             // at 0xC
    u8 dummy_[0x10 - 0xD];                  // at 0xD
    u8 texCoordToTexMapID[GX_MAX_TEXCOORD]; // at 0x10
    u8 dummy[0x20 - 0x18];                  // at 0x18
    ResTevDL dl;                            // at 0x20
};

class ResTev : public ResCommon<ResTevData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResTev);

    bool GXGetTevSwapModeTable(
        GXTevSwapSel swap, GXTevColorChan *pR, GXTevColorChan *pG, GXTevColorChan *pB, GXTevColorChan *pA
    ) const;
    void
    GXSetTevSwapModeTable(GXTevSwapSel swap, GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a);

    bool GXGetTevOrder(GXTevStageID stage, GXTexCoordID *pCoord, GXTexMapID *pMap, GXChannelID *pChannel) const;

    void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d);

    void CallDisplayList(bool sync) const;

    ResTev CopyTo(void *pDst);

    void DCStore(bool sync);

    u8 GetNumTevStages() const {
        return ref().nStages;
    }

    void EndEdit() {
        DCStore(false);
    }
};

} // namespace g3d
} // namespace nw4r

#endif
