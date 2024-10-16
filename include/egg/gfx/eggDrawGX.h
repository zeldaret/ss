#ifndef EGG_DRAWGX_H
#define EGG_DRAWGX_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggTexture.h"
#include "egg/math/eggMatrix.h"
#include "rvl/GX.h" // IWYU pragma: export


namespace EGG {

class DrawGX {
public:
    enum DL {
        DL_0,
        DL_1,
        DL_2,
        DL_3,
        DL_4,
        DL_5,
        DL_6,
        DL_7,
        DL_8,
        DL_9,
        DL_10,
        DL_11,
        DL_12,
        DL_13,
        DL_14,
        DL_15,
        DL_16,
        DL_17,

        DL_MAX
    };

    static void Initialize(Heap *);

    static GXTexMapID GetTexMapDefault();
    static void LoadTexture(const EGG::ResTIMG *, GXTexMapID);
    static void BeginDrawScreen(bool, bool, bool);
    static void DrawDL(enum DL, const nw4r::math::MTX34 &, GXColor);

    // TODO MORE

    /* static ?? s_DL */

    static Matrix34f s_cameraMtx;

    /* static ?? s_flag */

    static GXColor BLACK;
    static GXColor WHITE;
    static GXColor GREEN;
    static GXColor RED;
    static GXColor BLUE;
};

} // namespace EGG

#endif
