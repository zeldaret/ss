#include "egg/gfx/eggPostEffectUnk2.h"

#include "common.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggGXUtility.h"
#include "egg/math/eggVector.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

PostEffectUnk2::PostEffectUnk2() {
    // NONMATCHING
    field_0x2C = 0;
    field_0x30 = 0;
    field_0x34 = 1;
    field_0x35 = 0xFE;
    field_0x36 = 7;
    field_0x38 = 0.0f;
    field_0x3C = 0.0f;
    field_0x40 = 1.0f;
    field_0x44 = 1.0f;
    field_0x48 = 1.0f;
    mpTexture = new CpuTexture(32, 4, GX_TF_RGBA8);
    mpTexture->configure();
    mpTexture->allocate(nullptr);
    mpTexture->setWrapS(GX_CLAMP);
    mpTexture->setWrapT(GX_CLAMP);
    for (u16 y = 0; y < mpTexture->getHeight(); y++) {
        for (u16 x = 0; x < mpTexture->getWidth(); x++) {
            // The conversion constant 0x43000000 needs to
            // be stored at the start of the function...
            f32 xF = x;
            f32 widF = mpTexture->getWidth() - 1;
            f32 ratio = nw4r::ut::Min(xF / widF, 1.0f);
            u8 val = ratio * 255.0f;
            GXColor color = {val, val, val, val};
            mpTexture->setColor(x, y, color);
        }
    }

    mpTexture->flush();
    field_0x00 = 0;
}

PostEffectUnk2::~PostEffectUnk2() {
    delete mpTexture;
}

void PostEffectUnk2::draw(f32 width, f32 height) {
    // NONMATCHING
    setMaterialInternal();
    setVtxState();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);

    nw4r::math::MTX34 mtx;
    PSMTXScale(mtx, mScaleX * width, mScaleY * height, 1.0f);
    PSMTXTransApply(mtx, mtx, mOffsetX, mOffsetY, 0.0f);
    GXLoadPosMtxImm(mtx, 0);

    static const f32 floats1[2][2] = {
        {-1.0f, 1.0f},
        { 0.0f, 0.0f},
    };
    static const f32 floats2[2][2] = {
        { 0.0f, 0.0f},
        {-1.0f, 1.0f},
    };

    // TODO
    f32 s1 = field_0x40 / mpCapTexture->getWidth();
    f32 s2 = field_0x40 / mpCapTexture->getHeight();
    f32 f = 0.0f;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EGG::Vector3f(0.0f, 0.0f, 0.0f);
            f32 sin, cos;
            nw4r::math::SinCosRad(&sin, &cos, f);
            nw4r::math::MTX34 mtx2;
            mtx2._00 = cos;
            mtx2._01 = -sin;
            mtx2._02 = -cos * f + sin * f + (s1 * floats1[i][j]);
            mtx2._03 = f;
            mtx2._10 = sin;
            mtx2._11 = cos;
            mtx2._12 = -sin * f - cos * f + (s2 * floats2[i][j]);
            mtx2._13 = f;
            GXLoadTexMtxImm(mtx2, GXUtility::getTexMtxID(j), GX_MTX_2x4);
        }
        if (i == 1 && field_0x2C <= 1) {
            GXSetBlendMode(GX_BM_LOGIC, GX_BL_ZERO, GX_BL_ZERO, GX_LO_OR);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2u8(0, 0);
        GXPosition2f32(field_0x38, field_0x3C);
        GXPosition2u8(1, 1);
        GXPosition2f32(field_0x38, field_0x3C);
        GXPosition2u8(2, 2);
        GXPosition2f32(field_0x38, field_0x3C);
        GXPosition2u8(3, 3);
        GXPosition2f32(field_0x38, field_0x3C);
    }
}

void PostEffectUnk2::setMaterialInternal() {
    loadTextureInternal();
    setMatColorChannel();
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3c, 0, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, 0, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD3, GX_TG_MTX2x4, GX_TG_TEX0, 0x21, 0, 0x7d);
    GXSetNumIndStages(3);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, mTexMapId);
    GXSetIndTexOrder(GX_INDTEXSTAGE1, GX_TEXCOORD2, mTexMapId);
    GXSetIndTexOrder(GX_INDTEXSTAGE2, GX_TEXCOORD3, mTexMapId);
    setMatPE();
    mpTexture->load(GX_TEXMAP1);

    GXSetNumTevStages(4);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);

    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);

    GXSetTevIndirect(
        GX_TEVSTAGE1, GX_INDTEXSTAGE1, GX_ITF_8, GX_ITB_NONE, GX_ITM_1, GX_ITW_0, GX_ITW_0, true, false, GX_ITBA_OFF
    );

    GXSetTevSwapMode(GX_TEVSTAGE2, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevIndirect(
        GX_TEVSTAGE2, GX_INDTEXSTAGE2, GX_ITF_8, GX_ITB_NONE, GX_ITM_2, GX_ITW_0, GX_ITW_0, true, false, GX_ITBA_OFF
    );

    GXSetTevSwapMode(GX_TEVSTAGE3, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP1, GX_COLOR_NULL);
    if (field_0x2C == 2) {
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_GREATER, field_0x35);
    } else {
        GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    }
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevIndirect(
        GX_TEVSTAGE3, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_0, GX_ITW_0, GX_ITW_0, true, false, GX_ITBA_OFF
    );
    for (int i = 0; i < 3; i++) {
        static const f32 sFloats[2][3] = {
            { 1.0f, -1.0f, -1.0f},
            {-1.0f,  1.0f,  1.0f},
        };

        s16 scale = field_0x36;
        f32 f1 = (i != 0 ? 0.5f : 1.0f) * (sFloats[field_0x30][i] * 0.5f);
        f32 f2 = (i == 0 ? field_0x44 : 0.0f) * 0.5f;

        f32 v[2][3] = {
            {f1 / 256.0f, f1, 0.0f},
            {f2 / 256.0f, f2, 0.0f},
        };

        GXSetIndTexMtx(static_cast<GXIndTexMtxID>(i + 1), v, scale - 2);
    }
}

} // namespace EGG
