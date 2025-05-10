#include "nw4r/g3d.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {
namespace detail {

void Copy32ByteBlocks(register void *pDst, register const void *pSrc, register u32 size) {
    register f32 work0, work1, work2, work3;

    for (size /= 32; size > 0; size--) {
        // clang-format off
        asm {
            lfd  work0, 0(pSrc)
            stfd work0, 0(pDst)

            lfd  work1, 8(pSrc)
            stfd work1, 8(pDst)

            lfd  work2, 16(pSrc)
            stfd work2, 16(pDst)
            
            lfd  work3, 24(pSrc)
            stfd work3, 24(pDst)
        }
        // clang-format on

        pDst = static_cast<u8 *>(pDst) + 32;
        pSrc = static_cast<const u8 *>(pSrc) + 32;
    }
}

void ZeroMemory32ByteBlocks(register void *pDst, register u32 size) {
    register f32 zero = 0.0f;

    for (size /= 32; size > 0; size--) {
        // clang-format off
        asm {
            psq_st zero,  0(pDst), 0, 0
            psq_st zero,  8(pDst), 0, 0
            psq_st zero, 16(pDst), 0, 0
            psq_st zero, 24(pDst), 0, 0
        }
        // clang-format on

        pDst = static_cast<u8 *>(pDst) + 32;
    }
}

} // namespace detail
} // namespace g3d
} // namespace nw4r
