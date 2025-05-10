#include "egg/gfx/eggPostEffectSimple.h"

#include "egg/gfx/eggDrawGX.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

PostEffectSimple::PostEffectSimple()
    : field_0x2C(DrawGX::WHITE), field_0x30(1.0f), field_0x34(GX_CS_SCALE_1), field_0x38(1) {}

void PostEffectSimple::setMaterialInternal() {
    loadTextureInternal();
    GXColor c = field_0x2C;
    if (field_0x30 < 1.0f) {
        scaleColor(c, c, true, field_0x30);
    }
    setMatColorChannel();
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, 0, 0x7d);
    nw4r::math::MTX34 mtx;
    getBaseTexMtx(&mtx);
    GXLoadTexMtxImm(mtx, 0x1e, GX_MTX2x4);
    setMatInd();
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevKColor(GX_KCOLOR0, c);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, mTex1.mTexMapID, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, field_0x34, 1, GX_TEVPREV);
    switch (field_0x38) {
        case 0: GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST); break;
        case 1: GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO); break;
        case 2:
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXA, GX_CC_KONST, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
            break;
    }

    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, field_0x34, true, GX_TEVPREV);
    setMatPE();
}

} // namespace EGG
