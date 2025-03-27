#ifndef EGG_G3D_UTILITY_H
#define EGG_G3D_UTILITY_H

#include "nw4r/g3d/g3d_scnmdl.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/math/math_types.h"
#include "rvl/MEM/mem_allocator.h"

namespace EGG {

class G3DUtility {
public:
    static MEMAllocator *getAllocator() {
        return sAllocator;
    }

    struct SetTextureResult {
        /* 0x00 */ u8 _0x00[2];
        /* 0x02 */ s8 texMapId;
        /* 0x03 */ s8 texCoordId;
    };

    static u32 SetTexture(
        nw4r::g3d::ResMat resMat, nw4r::g3d::ScnMdl::CopiedMatAccess *param_2, const char *name, GXTexObj *texObj,
        bool copy, SetTextureResult *pResult, int param_7, int param_8
    );

    static void setUpLightSet(nw4r::g3d::LightSetting &, nw4r::g3d::ResAnmScn, int);

    static nw4r::math::MTX34 *GetWorldMtxArray(int, int);

    static int searchStringResNode(nw4r::g3d::ResMdl, const char *, u16 *, u32);
    static int searchStringResMat(nw4r::g3d::ResMdl, const char *, u16 *, u32);
    static int searchStringResTexPlttInfo(nw4r::g3d::ResMdl, const char *, u16 *, u32);

    typedef int (*MdlSearch)(nw4r::g3d::ResMdl, const char *, u16 *, u32);

private:
    static MEMAllocator *sAllocator;
};

} // namespace EGG

#endif
