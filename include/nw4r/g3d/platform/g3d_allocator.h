#ifndef NW4R_G3D_PLATFORM_ALLOCATOR_H
#define NW4R_G3D_PLATFORM_ALLOCATOR_H
#include <nw4r/types_nw4r.h>

#include <rvl/MEM.h>

namespace nw4r {
namespace g3d {
namespace detail {

inline void *AllocFromAllocator(MEMAllocator *pAllocator, u32 size) {
    return MEMAllocFromAllocator(pAllocator, size);
}
inline void FreeToAllocator(MEMAllocator *pAllocator, void *pBlock) {
    return MEMFreeToAllocator(pAllocator, pBlock);
}

} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
