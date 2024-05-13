#ifndef NW4R_G3D_SCN_MDL1_MAT1_SHP_H
#define NW4R_G3D_SCN_MDL1_MAT1_SHP_H
#include "common.h"
#include "g3d_resmat.h"
#include "g3d_resshp.h"
#include "g3d_scnobj.h"

namespace nw4r {
namespace g3d {
class ScnMdl1Mat1Shp : public ScnLeaf {
public:
    static const G3dObj::TypeObj GetTypeObjStatic() {
        return TypeObj(TYPE_NAME);
    }

    ResMat GetResMat() {
        return mResMat;
    }
    ResShp GetResShp() {
        return mResShp;
    }

private:
    ResMat mResMat; // at 0xE8
    ResShp mResShp; // at 0xEC

    NW4R_G3D_TYPE_OBJ_DECL(ScnMdl1Mat1Shp);
};
} // namespace g3d
} // namespace nw4r

#endif
