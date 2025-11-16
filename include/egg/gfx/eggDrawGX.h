#ifndef EGG_DRAWGX_H
#define EGG_DRAWGX_H

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggTexture.h"
#include "egg/math/eggMatrix.h"
#include "nw4r/g3d/res/g3d_restex.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"

#include "rvl/GX.h" // IWYU pragma: export

namespace EGG {

class DrawGX {
public:
    DrawGX() {}
    // made up
    struct DLData {
        u8 *mpList;
        u32 mLen;
    };

    // made up, stride 0xC
    struct ZModeConfig {
        bool mTest;
        GXCompare mCompare;
        bool mUpdate;
    };

    // made up, stride 0x10
    struct BlendModeConfig {
        GXBlendMode mBlendMode;
        GXBlendFactor mBlendFactorSrc;
        GXBlendFactor mBlendFactorDst;
        GXLogicOp mLogicOp;
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

    enum ZMode {
        ZMODE_0,
        ZMODE_1,
        ZMODE_2,
        ZMODE_3,

        ZMODE_MAX,
    };

    enum TevSetting {
        TEV_0,
        TEV_1,
        TEV_2,
    };

    enum TexGen {
        TEXGEN_0,
        TEXGEN_1,
    };

    enum ColorChannel {
        COLORCHAN_0,
        COLORCHAN_1,
    };

    static void Initialize(Heap *);

    static GXTexMapID GetTexMapDefault();
    static GXLightID GetLightMaskDefault();

    static void BeginDrawLine(ColorChannel, ZMode);
    static void BeginDrawCircleZ(ColorChannel, ZMode);
    static void BeginDrawQuad(ColorChannel, ZMode, Blend, bool, bool);
    static void BeginDrawScreen(bool, bool, bool);

    static void DrawLineStrip(const EGG::Vector3f *, u16, GXColor, u8);
    static void DrawLine(const EGG::Vector3f *, u16, GXColor, u8);
    static void ClearEfb(const nw4r::math::MTX34 &, bool, bool, bool, GXColor, bool);
    static void ResetMaterial(enum ColorChannel);
    static void SetMat_ColorChannel(enum ColorChannel);
    static void SetMat_TexGen(enum TexGen);
    static void SetMat_Ind();
    static void SetMat_Tev(GXTevStageID, enum TevSetting);
    static void SetMat_PE(enum ZMode, enum Blend);

    static void LoadTexture(const EGG::ResTIMG *, GXTexMapID);
    static void LoadTexture(nw4r::g3d::ResTex, GXTexMapID);

    static void CreateDisplayList(EGG::Heap *);
    static void DrawDL(enum DL, const nw4r::math::MTX34 &, GXColor);
    static void SetVtxState(enum VtxType);
    static void SetZMode(enum ZMode);
    static void SetBlendMode(enum Blend);
    static void DrawDLWorld(DL dl, const EGG::Matrix34f &mtx, GXColor color);

    static void DrawDL16(const nw4r::math::MTX34 &mtx, GXColor clr) {
        DrawDL(DL_16, mtx, clr);
    }
    static void DrawDL17(const nw4r::math::MTX34 &mtx, GXColor clr) {
        DrawDL(DL_17, mtx, clr);
    }
    // TODO MORE

    static void SetTevColor(GXTevRegID regId, GXColor tevClr, GXTevKColorID colorId, GXColor tevkClr) {
        GXSetTevColor(regId, tevClr);
        GXSetTevKColor(colorId, tevkClr);
    }

    // not sure
    static DLData s_DL[DL_MAX];
    static const DL s_DL0;
    static const DL s_DL7;
    static const DL s_DL8;

    static nw4r::math::MTX34 s_cameraMtx;

    static u32 s_flag;

    static const GXColor BLACK;
    static const GXColor WHITE;
    static const GXColor GREEN;
    static const GXColor RED;
    static const GXColor BLUE;
};

} // namespace EGG

#endif
