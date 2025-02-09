#ifndef NW4R_G3D_SCN_MDL_1MAT_1SHP_H
#define NW4R_G3D_SCN_MDL_1MAT_1SHP_H
#include <nw4r/types_nw4r.h>

#include <nw4r/g3d/g3d_scnobj.h>
#include <nw4r/g3d/res/g3d_resmat.h>
#include <nw4r/g3d/res/g3d_resshp.h>

namespace nw4r {
namespace g3d {

class ScnMdl1Mat1Shp : public ScnLeaf {
public:
    ResMat GetResMat() {
        return mResMat;
    }
    ResShp GetResShp() {
        return mResShp;
    }

private:
    ResMat mResMat; // at 0xE8
    ResShp mResShp; // at 0xEC

    NW4R_G3D_RTTI_DECL_DERIVED(ScnMdl1Mat1Shp, ScnLeaf);
};

} // namespace g3d
} // namespace nw4r

#endif
