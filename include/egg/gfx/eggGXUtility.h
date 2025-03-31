#ifndef EGG_GX_UTILITY_H
#define EGG_GX_UTILITY_H

#include "egg/gfx/eggTexture.h"
#include "egg/math/eggVector.h"
#include "nw4r/g3d/res/g3d_restex.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class GXUtility {
public:
    static int getTexMtxID(int);
    static void setScaleOffsetPerspective(f32*, f32, f32, f32, f32);
    static void getTexObj(GXTexObj *, const ResTIMG &);
    static void getTexObj(GXTexObj *, nw4r::g3d::ResTex, GXTexWrapMode, GXTexWrapMode, GXTexFilter, GXTexFilter);

    static void getNormalColor(GXColor &outColor, const Vector3f &vec);

private:
    static const u8 s_texMtxIds[8];
};

} // namespace EGG

#endif
