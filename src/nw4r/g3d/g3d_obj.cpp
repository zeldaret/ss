#include <nw4r/g3d.h>

namespace nw4r {
namespace g3d {

NW4R_G3D_RTTI_DEF(G3dObj);

G3dObj::~G3dObj() {
    Dealloc(mpHeap, this);
}

void G3dObj::Destroy() {
    G3dObj* pParent = GetParent();

    if (pParent != NULL) {
        pParent->G3dProc(G3DPROC_CHILD_DETACHED, 0, this);
    }

    delete this;
}

// clang-format off
DECOMP_FORCEACTIVE(g3d_obj_cpp,
                   G3dObj::IsDerivedFrom);
// clang-format on

} // namespace g3d
} // namespace nw4r
