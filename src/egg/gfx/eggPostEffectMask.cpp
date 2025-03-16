#include "egg/gfx/eggPostEffectMask.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "rvl/GX/GXAttr.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

PostEffectMask::PostEffectMask()
    : mColor(DrawGX::WHITE), field_0x2C(0), field_0x34(0.0f), field_0x38(1.0f), field_0x1BC(1) {}

void PostEffectMask::setMaterialInternal() {
    bool hasTex2 = mpCapTexture2 != nullptr;
    loadTextureInternal();
    GXColor resColor;
    fn_804AF040(resColor, mColor, false, field_0x34);
    setMatColorChannel();
    GXSetNumTexGens(2);

    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    setMatInd();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXTevScale scale = field_0x2C == 0 ? GetTevScale() : GX_CS_SCALE_1;

    GXColor c = resColor;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, mTexMapId, GX_COLOR_NULL);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, c);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, scale, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, scale, 1, GX_TEVPREV);

    switch (field_0x2C) {
        case 0: {
            if (hasTex2) {
                GXSetNumTevStages(2);
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, mTexMapId2, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
                field_0x00 = 0;
            } else {
                GXSetNumTevStages(1);
                field_0x00 = 10;
            }
            break;
        }
        case 1: {
            if (hasTex2) {
                GXSetNumTevStages(3);
                scale = GetTevScale();
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, mTexMapId, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, scale, 1, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, mTexMapId2, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                field_0x00 = 0;
            } else {
                GXSetNumTevStages(2);
                scale = GetTevScale();
                GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, mTexMapId, GX_COLOR_NULL);
                GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXA, GX_CC_ZERO);
                GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, scale, 1, GX_TEVPREV);
                GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
                GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                field_0x00 = 10;
            }
            break;
        }
    }

    setMatPE();
}

} // namespace EGG
