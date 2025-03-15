#ifndef NW4R_G3D_RES_RES_ANM_FOG_H
#define NW4R_G3D_RES_RES_ANM_FOG_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resanm.h"
#include "nw4r/g3d/res/g3d_rescommon.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "rvl/GX.h"  // IWYU pragma: export

namespace nw4r {
namespace g3d {

/******************************************************************************
 *
 * FogAnmResult
 *
 ******************************************************************************/
struct FogAnmResult {
    GXFogType type;  // at 0x0
    f32 startz;      // at 0x4
    f32 endz;        // at 0x8
    ut::Color color; // at 0xC
};

/******************************************************************************
 *
 * ResAnmFog
 *
 ******************************************************************************/
struct ResAnmFogData {
    enum Flag {
        FLAG_START_CONST = (1 << 29),
        FLAG_END_CONST = (1 << 30),
        FLAG_COLOR_CONST = (1 << 31)
    };

    u32 size;              // at 0x0
    s32 toResAnmScnData;   // at 0x4
    s32 name;              // at 0x8
    u32 id;                // at 0xC
    u32 refNumber;         // at 0x10
    u32 flags;             // at 0x14
    GXFogType type;        // at 0x18
    ResAnmData startz;     // at 0x1C
    ResAnmData endz;       // at 0x20
    ResColorAnmData color; // at 0x24
};

class ResAnmFog : public ResCommon<ResAnmFogData> {
public:
    NW4R_G3D_RESOURCE_FUNC_DEF(ResAnmFog);

    void GetAnmResult(FogAnmResult *pResult, f32 frame) const;

    u32 GetID() const {
        return ref().id;
    }
};

} // namespace g3d
} // namespace nw4r

#endif
