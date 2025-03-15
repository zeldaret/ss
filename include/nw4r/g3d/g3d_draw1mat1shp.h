#ifndef NW4R_G3D_DRAW_1MAT_1SHP_H
#define NW4R_G3D_DRAW_1MAT_1SHP_H
#include <nw4r/types_nw4r.h>

#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resshp.h"

#include "nw4r/math.h" // IWYU pragma: export

namespace nw4r {
namespace g3d {

// Forward declarations
namespace G3DState {
class IndMtxOp;
}

enum Draw1Mat1ShpCtrl {
    DRAW1MAT1SHP_CTRL_NOPPCSYNC = (1 << 0),
    DRAW1MAT1SHP_CTRL_NOSWAPSHP = (1 << 1),
    DRAW1MAT1SHP_CTRL_CULL_FRONT = (1 << 2),
    DRAW1MAT1SHP_CTRL_FORCE_LIGHTOFF = (1 << 3),
};

struct Draw1Mat1ShpSwap {
    ResTexObj texObj;                    // at 0x0
    ResTlutObj tlutObj;                  // at 0x4
    ResGenMode genMode;                  // at 0x8
    ResTev tev;                          // at 0xC
    ResMatPix pix;                       // at 0x10
    ResMatTevColor tevColor;             // at 0x14
    ResMatIndMtxAndScale indMtxAndScale; // at 0x18
    ResMatChan chan;                     // at 0x1C
    ResMatTexCoordGen texCoordGen;       // at 0x20
    ResMatMisc misc;                     // at 0x24
    ResTexSrt texSrt;                    // at 0x28
    ResVtxPosData **vtxPosTable;         // at 0x2C
    ResVtxNrmData **vtxNrmTable;         // at 0x30
    ResVtxClrData **vtxClrTable;         // at 0x34

    Draw1Mat1ShpSwap() : vtxPosTable(NULL), vtxNrmTable(NULL), vtxClrTable(NULL) {}
};

void Draw1Mat1ShpDirectly(
    ResMat mat, ResShp shp, const math::MTX34 *pViewPos, const math::MTX34 *pViewNrm, u32 ctrl, Draw1Mat1ShpSwap *pSwap,
    G3DState::IndMtxOp *pIndMtxOp
);

} // namespace g3d
} // namespace nw4r

#endif
