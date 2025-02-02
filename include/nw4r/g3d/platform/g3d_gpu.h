#ifndef NW4R_G3D_PLATFORM_GPU_H
#define NW4R_G3D_PLATFORM_GPU_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/res/g3d_rescommon.h>

#include <nw4r/math.h>

#include <rvl/GX.h>

namespace nw4r {
namespace g3d {
namespace fifo {

// Swap bits to get hardware representation
static const u8 cm2hw[] = {0b0000, 0b0010, 0b0001, 0b0011};

inline void LoadBPCmd(u32 reg) {
    GXCmd1u8(GX_FIFO_CMD_LOAD_BP_REG);
    GXCmd1u32(reg);
}

inline void LoadCPCmd(u8 addr, u32 value) {
    GXCmd1u8(GX_FIFO_CMD_LOAD_CP_REG);
    GXCmd1u8(addr);
    GXCmd1u32(value);
}

inline void LoadXFCmd(u16 addr, u32 value) {
    GXCmd1u8(GX_FIFO_CMD_LOAD_XF_REG);
    GXCmd1u16(0); // No size (single write)
    GXCmd1u16(addr);
    GXCmd1u32(value);
}

inline void LoadXFCmdHdr(u16 addr, u8 len) {
    GXCmd1u8(GX_FIFO_CMD_LOAD_XF_REG);
    GXCmd1u16(len - 1);
    GXCmd1u16(addr);
}

void GDSetGenMode2(u8 numTexGens, u8 numChans, u8 numTevs, u8 numInds, GXCullMode cullMode);

inline void GDSetGenMode2Ex_BP(u8 numTexGens, u8 numChans, u8 numTevs, u8 numInds, GXCullMode cullMode) {
    // clang-format off
    // @note NUMCOLORS is actually three bits
    LoadBPCmd(GX_BP_REG_SSMASK << GX_BP_OPCODE_SHIFT |
        GX_BP_GENMODE_NUMTEX_MASK                    |
        0b11 << GX_BP_GENMODE_NUMCOLORS_SHIFT        |
        GX_BP_GENMODE_NUMTEVSTAGES_MASK              |
        GX_BP_GENMODE_CULLMODE_MASK                  |
        GX_BP_GENMODE_NUMINDSTAGES_MASK);
    // clang-format on 

    // clang-format off
    LoadBPCmd(
        numTexGens        << GX_BP_GENMODE_NUMTEX_SHIFT       |
        numChans          << GX_BP_GENMODE_NUMCOLORS_SHIFT    |
        numTevs - 1       << GX_BP_GENMODE_NUMTEVSTAGES_SHIFT |
        cm2hw[cullMode]   << GX_BP_GENMODE_CULLMODE_SHIFT     |
        numInds           << GX_BP_GENMODE_NUMINDSTAGES_SHIFT |
        GX_BP_REG_GENMODE << GX_BP_OPCODE_SHIFT);
    // clang-format on
}

void GDSetCullMode(GXCullMode cullMode);

void GDSetTexCoordScale2(
    GXTexCoordID coord, u16 scaleS, GXBool biasS, GXBool wrapS, u16 scaleT, GXBool biasT, GXBool wrapT
);

void GDSetIndTexMtx(u32 id, const math::MTX34 &rMtx);

void GDResetCurrentMtx();
void GDSetCurrentMtx(const u32 *pIdArray);

void GDLoadTexMtxImm3x3(const math::MTX33 &rMtx, u32 id);

inline void GDSetChanCtrl(GXChannelID chan, u32 param, u32 lightMask) {
    param &= ~(GX_XF_COLOR0CNTRL_LMASKLO_MASK | GX_XF_COLOR0CNTRL_LMASKHI_MASK);

    param |= (lightMask & 0b1111) << GX_XF_COLOR0CNTRL_LMASKLO_SHIFT | ((lightMask >> 4) & 0b1111)
                                                                           << GX_XF_COLOR0CNTRL_LMASKHI_SHIFT;

    LoadXFCmd(GX_XF_REG_COLOR0CNTRL + (chan & 3), param);
}

inline void GDSetChanCtrlLightOff(GXChannelID chan, u32 param, u32 lightMask) {
    param &= ~(GX_XF_COLOR0CNTRL_LMASKLO_MASK | GX_XF_COLOR0CNTRL_LMASKHI_MASK | GX_XF_COLOR0CNTRL_LIGHT_MASK);

    param |= (lightMask & 0b1111) << GX_XF_COLOR0CNTRL_LMASKLO_SHIFT | ((lightMask >> 4) & 0b1111)
                                                                           << GX_XF_COLOR0CNTRL_LMASKHI_SHIFT;

    LoadXFCmd(GX_XF_REG_COLOR0CNTRL + (chan & 3), param);
}

inline void GDSetChanAmbColor(GXChannelID chan, GXColor color) {
    LoadXFCmd(GX_XF_REG_AMBIENT0 + chan, *reinterpret_cast<u32 *>(&color));
}

inline void GDSetChanMatColor(GXChannelID chan, GXColor color) {
    LoadXFCmd(GX_XF_REG_MATERIAL0 + chan, *reinterpret_cast<u32 *>(&color));
}

} // namespace fifo
} // namespace g3d
} // namespace nw4r

#endif
