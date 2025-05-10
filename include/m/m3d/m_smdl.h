#ifndef M3D_M_SMDL_H
#define M3D_M_SMDL_H

#include "m/m3d/m_bmdl.h"
#include "m/m3d/m_scnleaf.h"
#include "nw4r/g3d.h" // IWYU pragma: export

class UnkClass2 {};

namespace m3d {

class smdl_c : public bmdl_c, UnkClass2 {
public:
    smdl_c();
    virtual ~smdl_c();

    bool create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize);
    bool create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption) {
        return create(mdl, alloc, bufferOption, 1, nullptr);
    }
};

} // namespace m3d

#endif
