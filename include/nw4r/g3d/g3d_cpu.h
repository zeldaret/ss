#ifndef NW4R_G3D_CPU_H
#define NW4R_G3D_CPU_H
#include "common.h"

namespace nw4r {
namespace g3d {
namespace detail {
void Copy32ByteBlocks(void *, const void *, u32);
void ZeroMemory32ByteBlocks(void *, u32);
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
