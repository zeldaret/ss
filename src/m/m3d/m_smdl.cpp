#include "m/m3d/m_smdl.h"

#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/g3d_scnmdlsmpl.h"


namespace m3d {

smdl_c::smdl_c() {}
smdl_c::~smdl_c() {}

bool smdl_c::create(nw4r::g3d::ResMdl mdl, mAllocator_c *alloc, u32 bufferOption, int nView, u32 *pSize) {
    if (alloc == nullptr) {
        alloc = internal::l_allocator_p;
    }

    u32 tmp;
    if (pSize == nullptr) {
        pSize = &tmp;
    }

    if (bufferOption != 0) {
        mpScnLeaf = nw4r::g3d::ScnMdl::Construct(alloc, pSize, mdl, bufferOption, nView);
    } else {
        mpScnLeaf = nw4r::g3d::ScnMdlSimple::Construct(alloc, pSize, mdl, nView);
    }

    if (mpScnLeaf == nullptr) {
        return false;
    }
    mpScnLeaf->SetPriorityDrawOpa(0x7f);
    mpScnLeaf->SetPriorityDrawXlu(0x7f);
    return true;
}

} // namespace m3d
