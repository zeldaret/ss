#ifndef EGG_GX_UTILITY_H
#define EGG_GX_UTILITY_H

#include "egg/gfx/eggTexture.h"
#include "nw4r/g3d/res/g3d_restex.h"
#include "rvl/GX/GXTexture.h"

namespace EGG {

class GXUtility {
public:
    static int getTexMtxID(int);
    static void getTexObj(GXTexObj *, const EGG::ResTIMG &);
    static void getTexObj(_GXTexObj *, nw4r::g3d::ResTex, GXTexWrapMode, GXTexWrapMode, GXTexFilter, GXTexFilter);
};

} // namespace EGG

#endif
