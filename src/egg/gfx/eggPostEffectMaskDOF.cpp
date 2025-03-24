#include "egg/gfx/eggPostEffectMaskDOF.h"

#include "common.h"
#include "egg/gfx/eggCpuTexture.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

static const GXColor sColorNear = (GXColor){0x00, 0xFF, 0x00, 0xFF};
static const GXColor sColorFar = (GXColor){0xFF, 0x00, 0x00, 0x00};
static const GXColor sColorCenter = (GXColor){0x00, 0x00, 0x00, 0x00};

PostEffectMaskDOF::PostEffectMaskDOF()
    : field_0x2C(8),
      field_0x44(0.5),
      field_0x48(0.f),
      field_0x4C(1.f),
      field_0x50(0.f, 0.f),
      mCpuTexArrIdx(2),
      field_0x60(2) {
    for (int i = 0; i < 3; i++) {
        mpCpuTexArr[i] = new CpuTexture(64, 2, GX_TF_RGBA8);
        mpCpuTexArr[i]->configure();
        mpCpuTexArr[i]->allocate(nullptr);
        mpCpuTexArr[i]->setWrapS(GX_CLAMP);
        mpCpuTexArr[i]->setWrapT(GX_CLAMP);
    }

    mpCpuTex = new CpuTexture(64, 64, GX_TF_RGBA8);
    mpCpuTex->configure();
    mpCpuTex->allocate(nullptr);
    mpCpuTex->setWrapS(GX_CLAMP);
    mpCpuTex->setWrapT(GX_CLAMP);
    setUpGradation();
    field_0x00 = 0;
}

void PostEffectMaskDOF::setUpGradation() {
    for (int i = 0; i < 3; i++) {
        const u16 width = mpCpuTexArr[i]->getWidth();
        const int mid = width / 2;
        for (u16 x = 0; x < mid; x++) {
            f32 val = 1.f - (f32)x / (mid - 1);

            switch (i) {
                case 1: val *= val; break;
                case 2:
                    val *= val;
                    val *= val;
                    break;
            }

            GXColor clr1, clr2;
            lerpColor(clr1, getCenterColor(), getNearColor(), val);
            lerpColor(clr2, getCenterColor(), getFarColor(), val);

            mpCpuTexArr[i]->setColor(x, 0, clr1);
            mpCpuTexArr[i]->setColor(x, 1, clr1);
            mpCpuTexArr[i]->setColor((width - 1) - x, 0, clr2);
            mpCpuTexArr[i]->setColor((width - 1) - x, 1, clr2);
        }
        mpCpuTexArr[i]->flush();
    }

    const u16 width = mpCpuTex->getWidth() / 2;
    for (u16 y = 0; y < mpCpuTex->getHeight(); y++) {
        const f32 fy = ((f32)y / mpCpuTex->getHeight());
        for (u16 x = 0; x < width; x++) {
            f32 fx = (f32)x / width;
            f32 ratio = fx / (1.f - fy);

            if (ratio > 1.f) {
                ratio = 1.f;
            } else if (ratio < 0.f) {
                ratio = 0.f;
            }
            ratio *= ratio;
            ratio *= ratio;
            GXColor clr1, clr2;
            lerpColor(clr1, getCenterColor(), getNearColor(), ratio);
            lerpColor(clr2, getCenterColor(), getFarColor(), ratio);
            mpCpuTex->setColor((width - 1) - x, y, clr1);
            mpCpuTex->setColor(width + x, y, clr2);
        }
    }
    mpCpuTex->flush();
}

void PostEffectMaskDOF::draw(f32 width, f32 height) {
    setMaterialInternal();
    setVtxState();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    nw4r::math::MTX34 mtx;
    PSMTXScale(mtx, width * mScaleX, height * mScaleY, 1.f);
    PSMTXTransApply(mtx, mtx, mOffsetX, mOffsetY, 0.f);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2u8(0, 0);
    GXPosition2f32(field_0x50.x, field_0x50.y);
    GXPosition2u8(1, 1);
    GXPosition2f32(field_0x50.x, field_0x50.y);
    GXPosition2u8(2, 2);
    GXPosition2f32(field_0x50.x, field_0x50.y);
    GXPosition2u8(3, 3);
    GXPosition2f32(field_0x50.x, field_0x50.y);
    GXSetTexCoordScaleManually(GX_TEXCOORD1, 0, 0x40, 0);
    GXSetTexCoordBias(GX_TEXCOORD1, 0, 0);
}

void PostEffectMaskDOF::setMaterialInternal() {
    loadTextureInternal();
    setMatColorChannel();
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, false, GX_DUALMTX_IDENT);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_TEXMTX_IDENT, false, GX_DUALMTX_IDENT);
    nw4r::math::MTX34 baseTexMtx;
    getBaseTexMtx(&baseTexMtx);
    GXLoadTexMtxImm(baseTexMtx, GX_TEXMTX0, GX_MTX_2x4);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, mTex1.mTexMapID);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);

    CpuTexture *pCpuTex = mpCpuTexArr[mCpuTexArrIdx];
    switch (field_0x60) {
        case 0: {
            GXSetNumTevStages(1);
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            GXSetTevIndirect(
                GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_0, GX_ITW_OFF, GX_ITW_OFF, false, false,
                GX_ITBA_OFF
            );

            s8 scaleExp = 0;
            f32 fVar2 = scaleExp + 1;
            f32 fVar3 = 1.f / (field_0x4C - field_0x48);

            f32 fVar1 = fVar3 / fVar2;

            while (fVar1 > (1023.f / 1024.f)) {
                fVar1 /= 2.f;
                fVar2 *= 2.f;

                scaleExp++;
            }
            while (0.f < fVar1 && fVar1 < 0.5f) {
                fVar1 *= 2.f;
                fVar2 /= 2.f;

                scaleExp--;
            }
            f32 indTexMtx[2][3] = {
                {fVar1 / 256.f, fVar1, 0.f},
                {          0.f,   0.f, 0.f}
            };
            GXSetIndTexMtx(GX_ITM_0, indTexMtx, scaleExp - 2);
            field_0x50.x = -(field_0x44 * fVar1 * fVar2) + 0.5f;
            if (field_0x2C & 8) {
                field_0x50.x = (s32)(field_0x50.x * 128.f) / 128.f;
            }
            field_0x50.y = 0.f;
            GXSetTexCoordScaleManually(GX_TEXCOORD1, true, 0x40, 0);
            GXSetTexCoordBias(GX_TEXCOORD1, 0, 0);
        } break;
        case 1: {
            pCpuTex = mpCpuTex;
            GXSetNumTevStages(2);
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevDirect(GX_TEVSTAGE0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD1, GX_TEXMAP2, GX_COLOR_NULL);
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP2, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            GXSetTevIndirect(
                GX_TEVSTAGE1, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_0, GX_ITW_0, GX_ITW_0, true, false,
                GX_ITBA_OFF
            );
            field_0x50.y = field_0x4C;

            f32 val = 1023.f / 1024.f;
            f32 indTexMtx[2][3] = {
                {val / 256.f, val, 0.f},
                {        0.f, 0.f, 0.f}
            };

            GXSetIndTexMtx(GX_ITM_0, indTexMtx, 8);

            int unk = 1024;
            field_0x50.x = -field_0x44 + 0.5f / unk;

            GXSetTexCoordScaleManually(GX_TEXCOORD1, GX_TEXCOORD1, 0, 0x40);
            GXSetTexCoordBias(GX_TEXCOORD1, 0, 0);
        } break;
        case 2: {
            GXSetNumTevStages(1);
            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP2, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            GXSetTevIndirect(
                GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_0, GX_ITW_0, GX_ITW_0, false, false,
                GX_ITBA_OFF
            );

            f32 indTexMtx[2][3] = {
                {0.f, 0.5f, 0.f},
                {0.f,  0.f, 0.f}
            };
            GXSetIndTexMtx(GX_ITM_0, indTexMtx, -1);

            field_0x50.y = 0.f;
            field_0x50.x = 0.f;
        } break;
    }
    pCpuTex->load(GX_TEXMAP2);
    setMatPE();
}

const GXColor &PostEffectMaskDOF::getNearColor() {
    return sColorNear;
}
const GXColor &PostEffectMaskDOF::getFarColor() {
    return sColorFar;
}
const GXColor &PostEffectMaskDOF::getCenterColor() {
    return sColorCenter;
}

} // namespace EGG
