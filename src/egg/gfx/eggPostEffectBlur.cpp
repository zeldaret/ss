#include "egg/gfx/eggPostEffectBlur.h"

#include "common.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGXUtility.h"
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

PostEffectBlur::PostEffectBlur() {
    // NONMATCHING
    static const GXColor c = {0xD7, 0xD7, 0xD7, 0xD7};
    field_0x2C = 1;
    field_0x30 = 1.0f;
    field_0x34 = 1.0f;
    for (int i = 0; i < ARRAY_LENGTH(field_0x38); i++) {
        field_0x38[i].init(c);
    }
}

void PostEffectBlur::draw(f32 width, f32 height) {
    // NONMATCHING
    loadTextureInternal();
    setMatColorChannel();
    setMatInd();
    setMatPE();
    setVtxState();
    f32 f1 = 1.0f / mpCapTexture->getWidth();
    f32 f2 = 1.0f / mpCapTexture->getHeight();
    for (u8 i = 0; i < field_0x2C; i++) {
        // Regswap
        int maxNum = field_0x38[i].field_0x00 - 1;
        for (u8 b = 0; b <= maxNum / 8; b++) {
            drawInternal(i, b, f1, f2);
            drawScreenInternal(mOffsetX, mOffsetY, width * mScaleX, height * mScaleY);
            DrawGX::SetBlendMode(DrawGX::BLEND_2);
        }
    }
}

void PostEffectBlur::drawInternal(u8 kernelIdx, u8 p2, f32 f1, f32 f2) {
    // NONMATCHING
    nw4r::math::MTX34 mtx;
    Stage &k = field_0x38[kernelIdx];

    // Some regswaps and instruction swaps only at the start of the function
    f32 unk_00 = k.field_0x00;
    int unk_00_scale = (p2 & 0x1F) * 8;

    f32 rad1 = (k.field_0x08 - M_PI * mRotation);
    f32 abc = (2.0f * M_PI / unk_00);

    f32 f3 = k.field_0x0C * f1;
    f32 f4 = k.field_0x0C * f2;

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
            1.0f, 0.0f, cos * f3, 0.0f,
            0.0f, 1.0f, sin * f4, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
            // clang-format on
        );
        // OK from here
        PSMTXConcat(m, mtx, m);
        GXLoadTexMtxImm(m, texMtxId, GX_MTX_2x4);
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
        GXSetTevOrder(stage, static_cast<GXTexCoordID>(id), mTexMapId, GX_COLOR_NULL);
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
