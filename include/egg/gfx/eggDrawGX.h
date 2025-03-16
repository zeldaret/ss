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
    // made up
    struct DLData {
        void *mpList;
        u32 mLen;
    };

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

    enum VtxType {
        VTX_TYPE_0,
        VTX_TYPE_1,
        VTX_TYPE_2,
        VTX_TYPE_3,
        VTX_TYPE_4,
        VTX_TYPE_5,
        VTX_TYPE_6,
        VTX_TYPE_7,
        VTX_TYPE_8,
        VTX_TYPE_9,
        VTX_TYPE_10,
        VTX_TYPE_11,
        VTX_TYPE_12,
        VTX_TYPE_13
    };

    enum Blend {
        BLEND_0,
        BLEND_1,
        BLEND_2,
        BLEND_3,
        BLEND_4,
        BLEND_5,
        BLEND_6,
        BLEND_7,
        BLEND_8,
        BLEND_9,
        BLEND_10,
        BLEND_11,
        BLEND_12,
        BLEND_13,
        BLEND_14,

        BLEND_MAX
    };

    static void Initialize(Heap *);

    static GXTexMapID GetTexMapDefault();
    static void LoadTexture(const EGG::ResTIMG *, GXTexMapID);
    static void BeginDrawScreen(bool, bool, bool);
    static void DrawDL(enum DL, const nw4r::math::MTX34 &, GXColor);
    static void SetVtxState(enum VtxType);
    static void SetBlendMode(enum Blend);

    // TODO MORE

    static DLData s_DL[DL_MAX];

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
