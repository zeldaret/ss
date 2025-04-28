#include "egg/gfx/eggPostEffectBlur.h"

#include "common.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGXUtility.h"
#include "egg/math/eggMath.h"
#include "egg/math/eggVector.h"
#include "math.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXAttr.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

PostEffectBlur::PostEffectBlur() : field_0x2C(1), field_0x30(1.0f), field_0x34(1.0f) {
    const GXColor clr = (GXColor){0xD7, 0xD7, 0xD7, 0xD7};
    field_0x38[0].init(clr);
    field_0x38[1].init(clr);
    field_0x38[2].init(clr);
    field_0x38[3].init(clr);
}

void PostEffectBlur::draw(f32 width, f32 height) {
    // NONMATCHING
    loadTextureInternal();
    setMatColorChannel();
    setMatInd();
    setMatPE();
    setVtxState();
    f32 f1 = 1.0f / mTex1.mpTex->getWidth();
    f32 f2 = 1.0f / mTex1.mpTex->getHeight();
    for (u8 i = 0; i < field_0x2C; i++) {
        int maxNum = (field_0x38[i].field_0x00 - 1) / 8;
        for (u8 b = 0; b <= maxNum; b++) {
            drawInternal(i, b, f1, f2);
            drawScreenInternal(mOffsetX, mOffsetY, width * mScaleX, height * mScaleY);
            DrawGX::SetBlendMode(DrawGX::BLEND_2);
        }
    }
}

void PostEffectBlur::drawInternal(u8 kernelIdx, u8 p2, f32 f1, f32 f2) {
    // NONMATCHING
    nw4r::math::MTX34 mtx;
    const Stage &k = field_0x38[kernelIdx];

    f32 tx = k.field_0x0C * f1;
    f32 ty = k.field_0x0C * f2;
    f32 rad1 = k.field_0x08 - Math<f32>::pi() * mRotation;
    f32 abc = 2.f * Math<f32>::pi() / k.field_0x00;

    int unk_00_scale = (p2 & 0x1F) * 8;

    u8 numTexGens = k.field_0x00 - unk_00_scale;
    if (numTexGens > 8) {
        numTexGens = 8;
    }

    getBaseTexMtx(&mtx);
    GXSetNumTexGens(numTexGens);
    for (int id = 0; id < numTexGens; id++) {
        int texMtxId = GXUtility::getTexMtxID(id);
        GXSetTexCoordGen2(static_cast<GXTexCoordID>(id), GX_TG_MTX2x4, GX_TG_TEX0, texMtxId, false, 0x7D);
        f32 sin, cos;
        nw4r::math::SinCosRad(&sin, &cos, rad1 + abc * unk_00_scale);
        nw4r::math::MTX34 m(
            // clang-format off
            1.0f, 0.0f, cos * tx, 0.0f,
            0.0f, 1.0f, sin * ty, 0.0f,
            0.0f, 0.0f,     0.0f, 0.0f
            // clang-format on
        );
        PSMTXConcat(m, mtx, m);
        GXLoadTexMtxImm(m, texMtxId, GX_MTX2x4);
        unk_00_scale++;
    }

    bool isSmallColor = k.field_0x01.r < 0x80 && k.field_0x01.g < 0x80 && k.field_0x01.b < 0x80;
    bool isSmallAlpha = k.field_0x01.a < 0x80;
    f32 colorScale = (isSmallColor ? 2.0f : 1.0f) * field_0x30;
    f32 alphaScale = (isSmallAlpha ? 2.0f : 1.0f) * field_0x30;

    f32 clrs[] = {
        k.field_0x01.r * colorScale,
        k.field_0x01.g * colorScale,
        k.field_0x01.b * colorScale,
        k.field_0x01.a * alphaScale,
    };
    GXColor clr;
    clr.r = clrs[0] < 255.0f ? clrs[0] : 255.0f;
    clr.g = clrs[1] < 255.0f ? clrs[1] : 255.0f;
    clr.b = clrs[2] < 255.0f ? clrs[2] : 255.0f;
    clr.a = clrs[3] < 255.0f ? clrs[3] : 255.0f;

    GXSetTevKColor(GX_KCOLOR0, clr);
    GXSetNumTevStages(numTexGens);
    for (int id = 0; id < numTexGens; id++) {
        GXTevStageID stage = static_cast<GXTevStageID>(id);
        GXSetTevDirect(stage);
        GXSetTevSwapMode(stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXTevColorArg a1 = id > 0 ? GX_CC_CPREV : GX_CC_ZERO;
        GXTevAlphaArg a2 = id > 0 ? GX_CA_APREV : GX_CA_ZERO;

        GXSetTevKColorSel(stage, GX_TEV_KCSEL_K0);
        GXSetTevKAlphaSel(stage, GX_TEV_KASEL_K0_A);
        GXSetTevOrder(stage, static_cast<GXTexCoordID>(id), mTex1.mTexMapID, GX_COLOR_NULL);
        GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, a1);
        GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, a2);

        bool isLast = stage == numTexGens - 1;
        bool colorArg = isLast;

        GXTevScale colorScale = isSmallColor && isLast ? GX_CS_DIVIDE_2 : GX_CS_SCALE_1;
        GXTevScale alphaScale = isSmallAlpha && colorArg ? GX_CS_DIVIDE_2 : GX_CS_SCALE_1;
        GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, colorScale, colorArg, GX_TEVPREV);
        GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, alphaScale, isLast, GX_TEVPREV);
    }
}

} // namespace EGG
