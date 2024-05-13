#ifndef NW4R_G3D_WORKMEM_H
#define NW4R_G3D_WORKMEM_H
#include "common.h"

#define G3D_WORKMEM_SIZE 0x18000

namespace nw4r {
namespace g3d {
namespace detail {
namespace workmem {
void *GetScaleTemporary();
void *GetMtxIDTemporary();
void *GetMdlZTemporary();
void *GetSkinningMtxTemporary();
void *GetBillboardMtxTemporary();
void *GetShpAnmResultBufTemporary();
} // namespace workmem
} // namespace detail
} // namespace g3d
} // namespace nw4r

#endif
