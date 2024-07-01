#ifndef NW4R_G3D_GPU_H
#define NW4R_G3D_GPU_H
#include "common.h"
#include "nw4r/g3d/g3d_rescommon.h"
#include "nw4r/math/math_types.h"
#include <rvl/GX.h>


namespace nw4r {
namespace g3d {
namespace fifo {
const u8 cm2hw[] = {0x00, 0x02, 0x01, 0x03};

inline void LoadXFCmd(u16 us, u32 ul) {
    GXCmd1u8(FIFO_ACCESS_XF);
    GXCmd1u16(0);
    GXCmd1u16(us);
    GXCmd1u32(ul);
}

inline void LoadBPCmd(u32 ul) {
    GXCmd1u8(FIFO_ACCESS_BP);
    GXCmd1u32(ul);
}

inline void LoadXFCmdHdr(u16 us, u8 uc) {
    GXCmd1u8(FIFO_ACCESS_XF);
    GXCmd1u16((u16)(uc - 1));
    GXCmd1u16(us);
}

inline void LoadCPCmd(u8 uc, u32 ul) {
    GXCmd1u8(FIFO_ACCESS_CP);
    GXCmd1u8(uc);
    GXCmd1u32(ul);
}

void GDSetGenMode2(u8, u8, u8, u8, GXCullMode);
void GDSetCullMode(GXCullMode);
void GDSetTexCoordScale2(GXTexCoordID, u16, u8, u8, u16, u8, u8);
void GDSetIndTexMtx(u32, const math::MTX34 &);
void GDResetCurrentMtx();
void GDSetCurrentMtx(const u32 *);
void GDLoadTexMtxImm3x3(const math::MTX33 &, u32);
} // namespace fifo
} // namespace g3d
} // namespace nw4r

#endif
