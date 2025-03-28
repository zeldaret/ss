#include "egg/gfx/eggPostEffectBlurGather.h"

#include "common.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggGXUtility.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXAttr.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

PostEffectBlurGather::PostEffectBlurGather() {
    field_0x2C = 1;
    field_0x2D = 1;
    field_0x30 = 0.5f;
    mBlurGatherData[0].reset();
    mBlurGatherData[1].reset();
    mBlurGatherData[2].reset();
}

void PostEffectBlurGather::setMaterialInternal() {
    u8 hasExtraStage = (mTex1.mpTex != nullptr && (field_0x2D & 1) == 0) ? 1 : 0;
    u8 numStages = field_0x2C + hasExtraStage * 2;
    setMatColorChannel();
    nw4r::math::MTX34 mtx;
    getBaseTexMtx(&mtx);
    u8 numStagesFirst = field_0x2C + hasExtraStage;
    GXSetNumTexGens(numStagesFirst);
    for (int i = 0; i < numStagesFirst; i++) {
        int texMtxIdx = GXUtility::getTexMtxID(i);
        GXSetTexCoordGen2(static_cast<GXTexCoordID>(i), GX_TG_MTX2x4, GX_TG_TEX0, texMtxIdx, 0, 0x7D);
        if (i < field_0x2C) {
            mBlurGatherData[i].mpCapTexture->load(mBlurGatherData[i].mTexMapId);
        } else {
            loadTextureInternal();
        }
        GXLoadTexMtxImm(mtx, texMtxIdx, GX_MTX_2x4);
    }

    f32 v = field_0x30 * 255.0f;
    GXColor c = {v, v, v, v};
    setMatInd();
    GXSetNumTevStages(numStages);

    for (int i = 0; i < numStages; i++) {
        BlurGatherData &k = mBlurGatherData[i];
        GXTevStageID stage = static_cast<GXTevStageID>(i);
        GXSetTevDirect(stage);
        GXSetTevSwapMode(stage, GX_TEV_SWAP0, GX_TEV_SWAP0);
        if (i < field_0x2C) {
            GXColor scaledColor;
            scaleColor(scaledColor, k.mColor, false, k.mColorScale);

            GXSetTevKColorSel(stage, static_cast<GXTevKColorSel>(GX_TEV_KCSEL_K0 + i));
            GXSetTevKAlphaSel(stage, static_cast<GXTevKAlphaSel>(GX_TEV_KASEL_K0_A + i));

            GXSetTevKColor(static_cast<GXTevKColorID>(i), scaledColor);
            GXSetTevOrder(stage, static_cast<GXTexCoordID>(i), k.mTexMapId, GX_COLOR_NULL);
            GXSetTevColorIn(stage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, stage == 0 ? GX_CC_ZERO : GX_CC_CPREV);
            GXSetTevColorOp(
                stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, i == field_0x2C - 1 ? GX_TEVREG0 : GX_TEVPREV
            );
            GXSetTevAlphaIn(stage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, stage == 0 ? GX_CA_ZERO : GX_CA_APREV);
            GXSetTevAlphaOp(
                stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, i == field_0x2C - 1 ? GX_TEVREG0 : GX_TEVPREV
            );
        } else if (stage == field_0x2C) {
            GXSetTevKColorSel(stage, GX_TEV_KCSEL_K2);
            GXSetTevKAlphaSel(stage, GX_TEV_KASEL_K2_A);
            GXSetTevKColor(GX_KCOLOR2, c);
            GXSetTevOrder(stage, static_cast<GXTexCoordID>(i), mTex1.mTexMapID, GX_COLOR_NULL);
            GXSetTevColorIn(stage, GX_CC_KONST, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(stage, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_4, 1, GX_TEVPREV);
            GXSetTevAlphaIn(stage, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(stage, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_4, true, GX_TEVPREV);
        } else {
            GXSetTevOrder(stage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
            GXSetTevColorIn(stage, GX_CC_C0, GX_CC_ZERO, GX_CC_CPREV, GX_CC_ZERO);
            GXSetTevColorOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(stage, GX_CA_A0, GX_CA_ZERO, GX_CA_APREV, GX_CA_ZERO);
            GXSetTevAlphaOp(stage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        }
    }
    setMatPE();
}

} // namespace EGG
