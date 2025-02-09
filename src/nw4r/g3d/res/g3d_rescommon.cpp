#include <nw4r/g3d.h>

#include <rvl/GX.h>

#include <cstring>

namespace nw4r {
namespace g3d {

bool ResName::operator==(const ResName rhs) const {
    if (GetLength() == rhs.GetLength()) {
        return std::strcmp(GetName(), rhs.GetName()) == 0;
    }

    return false;
}

namespace detail {

void ResWriteBPCmd(u8 *pPtr, u32 reg) {
    ResWrite_u8(pPtr + 0, GX_FIFO_CMD_LOAD_BP_REG);
    ResWrite_u32(pPtr + 1, reg);
}

void ResWriteBPCmd(u8 *pPtr, u32 reg, u32 mask) {
    ResWrite_u8(pPtr + 0, GX_FIFO_CMD_LOAD_BP_REG);

    u32 orig;
    ResReadBPCmd(pPtr + 0, &orig);

    // Insert register value into the original command using the mask
    orig &= ~mask;
    reg &= mask;

    ResWrite_u32(pPtr + 1, reg | orig);
}

void ResWriteCPCmd(u8 *pPtr, u8 addr, u32 value) {
    ResWrite_u8(pPtr + 0, GX_FIFO_CMD_LOAD_CP_REG);
    ResWrite_u8(pPtr + 1, addr);
    ResWrite_u32(pPtr + 2, value);
}

void ResWriteXFCmd(u8 *pPtr, u16 addr, u32 value) {
    ResWrite_u8(pPtr + 0, GX_FIFO_CMD_LOAD_XF_REG);
    ResWrite_u16(pPtr + 1, 0x0000); // No size (single write)
    ResWrite_u16(pPtr + 3, addr);
    ResWrite_u32(pPtr + 5, value);
}

void ResWriteSSMask(u8 *pPtr, u32 value) {
    u32 orig = ResRead_u32(pPtr + 1);

    // Overwrite BP register ID
    orig |= value;
    orig |= GX_BP_REG_SSMASK << GX_BP_OPCODE_SHIFT;

    ResWriteBPCmd(pPtr + 0, orig);
}

} // namespace detail
} // namespace g3d
} // namespace nw4r
