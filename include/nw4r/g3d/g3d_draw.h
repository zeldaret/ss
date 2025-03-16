#ifndef NW4R_G3D_DRAW_H
#define NW4R_G3D_DRAW_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"
#include "nw4r/g3d/res/g3d_resnode.h"
#include "nw4r/g3d/res/g3d_resshp.h"

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

// Forward declarations
namespace G3DState {
class IndMtxOp;
}

namespace detail {

G3DState::IndMtxOp *GetIndMtxOp(ResMat mat, ResNode node, ResShp shp);

} // namespace detail

struct DrawResMdlReplacement {
    u32 flag;                                   // at 0x0
    u8 *visArray;                               // at 0x4
    ResTexObjData *texObjDataArray;             // at 0x8
    ResTlutObjData *tlutObjDataArray;           // at 0xC
    ResTexSrtData *texSrtDataArray;             // at 0x10
    ResChanData *chanDataArray;                 // at 0x14
    ResGenModeData *genModeDataArray;           // at 0x18
    ResMatMiscData *matMiscDataArray;           // at 0x1C
    ResPixDL *pixDLArray;                       // at 0x20
    ResTevColorDL *tevColorDLArray;             // at 0x24
    ResIndMtxAndScaleDL *indMtxAndScaleDLArray; // at 0x28
    ResTexCoordGenDL *texCoordGenDLArray;       // at 0x2C
    ResTevData *tevDataArray;                   // at 0x30
    ResVtxPosData **vtxPosTable;                // at 0x34
    ResVtxNrmData **vtxNrmTable;                // at 0x38
    ResVtxClrData **vtxClrTable;                // at 0x3C
};

void DrawResMdlDirectly(
    const ResMdl mdl, const math::MTX34 *pViewPosMtxArray, const math::MTX33 *pViewNrmMtxArray,
    const math::MTX34 *pViewEnvMtxArray, const u8 *pByteCodeOpa, const u8 *pByteCodeXlu,
    DrawResMdlReplacement *pReplacement, u32 drawMode
);

} // namespace g3d
} // namespace nw4r

#endif
