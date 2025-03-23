#ifndef EGG_G3D_UTILITY_H
#define EGG_G3D_UTILITY_H

#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "rvl/MEM/mem_allocator.h"

namespace EGG {

class G3DUtility {
public:
    static MEMAllocator *getAllocator() {
        return sAllocator;
    }

    static u16 SetTexture(
        nw4r::g3d::ResMat resMat, nw4r::g3d::ScnMdl::CopiedMatAccess param_2, char *name, GXTexObj *texObj, bool copy,
        void *param_6, int param_7, int param_8
    );

    static void setUpLightSet(nw4r::g3d::LightSetting&, nw4r::g3d::ResAnmScn, int);

private:
    static MEMAllocator *sAllocator;
};

} // namespace EGG

#endif
