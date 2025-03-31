#ifndef EGG_G3D_UTILITY_H
#define EGG_G3D_UTILITY_H

#include "egg/egg_types.h"
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

    struct SearchResult {
        /* 0x00 */ u16 nodeIdx;
        /* 0x04 */ const char *name;
    };

    static u16 SetTexture(
        nw4r::g3d::ResMat resMat, nw4r::g3d::ScnMdl::CopiedMatAccess *param_2, const char *name, const GXTexObj *texObj,
        bool copy, SetTextureResult *pResult, int maxNumResults, int param_8
    );

    static int ApplyLightMat(nw4r::g3d::ResMdl mdl, const char *prefix);

    static bool setUpLightSet(nw4r::g3d::LightSetting &, nw4r::g3d::ResAnmScn, int);

    static void *BumpAlloc(u32 size, u32 align);
    static void ResetBumpAlloc() {
        sOffset = 0;
    }

    static void create(u32 size, Heap *pHeap);

    static int searchStringResNode(nw4r::g3d::ResMdl mdl, const char * name, bool exactMatch, SearchResult *pResults, int maxNumResults);
    static int searchStringResMat(nw4r::g3d::ResMdl mdl, const char * name, bool exactMatch, SearchResult *pResults, int maxNumResults);
    static int searchStringResTexPlttInfo(nw4r::g3d::ResMdl mdl, const char * name, bool exactMatch, SearchResult *pResults, int maxNumResults);

    typedef int (*MdlSearch)(nw4r::g3d::ResMdl mdl, const char * name, bool exactMatch, SearchResult *pResults, int maxNumResults);

private:
    static void defaultTexMtxFunc(nw4r::math::MTX34 *pMtx, s8 camRef, s8 lightRef);

    static MEMAllocator *sAllocator;
    static void *sBuf;
    static u32 sSize;
    static u32 sOffset;
    static u8 sId;
};

} // namespace EGG

#endif
