#include "m/m3d/m_bline.h"

#include "egg/core/eggColorFader.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggDrawGX.h"
#include "m/m3d/m3d.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_state.h"


namespace m3d {

blineMat_c::~blineMat_c() {
    remove();
}

bool blineMat_c::create(
    mAllocator_c *pAllocator, int numLines, u16 numLinePts, f32 width, f32 repeat, nw4r::ut::Color &color,
    EGG::ResTIMG *pTex, bool p9
) {
    if (!proc_c::create(pAllocator, nullptr)) {
        return false;
    }
    nw4r::ut::List_Init(&mLines, 0);
    if (numLines) {
        mpLineArr = new (pAllocator) bline_c[numLines];

        if (!mpLineArr) {
            return false;
        }
        bline_c *pLineArr = mpLineArr;
        EGG::Heap *pHeap = pAllocator->mHeap;
        for (int i = 0; i < numLines; ++i) {
            if (!pLineArr->create(pHeap, numLinePts, width, repeat, color)) {
                remove();
                return false;
            }
            nw4r::ut::List_Append(&mLines, pLineArr);
            pLineArr++;
        }
    }

    mLineArrNum = numLines;
    mpTex = pTex;
    field_0x4A = p9;

    return true;
}

void blineMat_c::remove() {
    if (mpLineArr) {
        delete[] mpLineArr;
        mpLineArr = nullptr;
    }
    scnLeaf_c::remove();
}

void blineMat_c::update() {
    nw4r::math::MTX34 camMtx;
    mVec3_c vec;

    getCurrentCamera().GetCameraMtx(&camMtx);
    PSMTXInverse(camMtx, camMtx);
    PSMTXMultVec(camMtx, mVec3_c::zero, vec);

    for (bline_c *line = (bline_c *)nw4r::ut::List_GetNext(&mLines, 0); line != nullptr;
         line = (bline_c *)nw4r::ut::List_GetNext(&mLines, line)) {
        if ((line->mFlags & 1) == 0) {
            line->update(&vec);
        }
    }
}

bline_c *blineMat_c::getLine(u16 idx) {
    return (bline_c *)nw4r::ut::List_GetNth(&mLines, idx);
}

void blineMat_c::setupGX(bool bTransparent) {
    EGG::DrawGX::LoadTexture(mpTex, GX_TEXMAP0);
    if (field_0x4A) {
        u32 mask_diff_color, mask_diff_alpha, mask_spec_color, mask_spec_alpha;
        nw4r::g3d::AmbLightObj ambObj;

        nw4r::g3d::G3DState::LoadLightSet(
            0, &mask_diff_color, &mask_diff_alpha, &mask_spec_color, &mask_spec_alpha, &ambObj
        );

        nw4r::ut::Color ambColor(0xFF, 0xFF, 0xFF, 0xFF);
        nw4r::ut::Color matColor(0xFF, 0xFF, 0xFF, 0xFF);
        GXSetChanMatColor(GX_COLOR0, matColor);

        ambColor.r = ambObj.r;
        ambColor.g = ambObj.g;
        ambColor.b = ambObj.b;
        ambColor.a = ambObj.a;
        GXSetChanAmbColor(GX_COLOR0, ambColor);

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0, TRUE, GX_SRC_REG, GX_SRC_REG, (GXLightID)mask_diff_color, GX_DF_CLAMP, GX_AF_NONE);
        GXSetChanCtrl(GX_ALPHA0, FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTevStages(2);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_RASC, GX_CC_CPREV, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    } else {
        GXSetNumChans(0);
        GXSetNumTevStages(1);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0, GX_DUALMTX_IDENT);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetZCompLoc(1);
    if (!bTransparent) {
        GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
        GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXSetZMode(TRUE, GX_LEQUAL, FALSE);
    }
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    nw4r::ut::Color fogColor = 0x0;
    GXSetFog(GX_FOG_NONE, fogColor, 0.0f, 0.0f, 0.0f, 0.0f);
    GXSetFogRangeAdj(FALSE, 0, NULL);
    GXSetCullMode(GX_CULL_BACK);
    GXSetDither(1);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_CLR_RGB, GX_RGB8, 6);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);

    nw4r::math::MTX34 camMtx;
    getCurrentCamera().GetCameraMtx(&camMtx);
    GXLoadPosMtxImm(camMtx, 0);
    GXLoadNrmMtxImm(camMtx, 0);
    GXSetCurrentMtx(0);
}

void blineMat_c::drawOpa() {
    update();
    setupGX(false);
    for (bline_c *line = (bline_c *)nw4r::ut::List_GetNext(&mLines, 0); line != nullptr;
         line = (bline_c *)nw4r::ut::List_GetNext(&mLines, line)) {
        if ((line->mFlags & 1) == 0 && line->mColor.a == 0xFF) {
            line->draw();
        }
    }
}

void blineMat_c::drawXlu() {
    update();
    setupGX(true);
    for (bline_c *line = (bline_c *)nw4r::ut::List_GetNext(&mLines, 0); line != nullptr;
         line = (bline_c *)nw4r::ut::List_GetNext(&mLines, line)) {
        if ((line->mFlags & 1) == 0 && line->mColor.a != 0xFF) {
            line->draw();
        }
    }
}

bline_c::~bline_c() {
    remove();
}

bool bline_c::create(EGG::Heap *pHeap, u16 numPts, f32 width, f32 repeat, const nw4r::ut::Color &color) {
    mpPathArr = new (pHeap) mVec3_c[numPts];
    if (!mpPathArr) {
        return false;
    }

    mpVtxPosArr = new (pHeap) VtxPos[numPts];
    if (!mpVtxPosArr) {
        remove();
        return false;
    }

    mpVtxNrmArr = new (pHeap) VtxNrm[numPts];
    if (!mpVtxNrmArr) {
        remove();
        return false;
    }

    mpVtxTexArr = new (pHeap) VtxTex[numPts];
    if (!mpVtxTexArr) {
        remove();
        return false;
    }

    mPathNum = numPts;
    mVtxNum = numPts;
    mWidth = width;
    mColor = color;
    mTexRepeat = repeat;
    field_0x2C = 0;

    return true;
}

void bline_c::remove() {
    if (mpPathArr) {
        delete[] mpPathArr;
        mpPathArr = nullptr;
    }
    if (mpVtxPosArr) {
        delete[] mpVtxPosArr;
        mpVtxPosArr = nullptr;
    }
    if (mpVtxNrmArr) {
        delete[] mpVtxNrmArr;
        mpVtxNrmArr = nullptr;
    }
    if (mpVtxTexArr) {
        delete[] mpVtxTexArr;
        mpVtxTexArr = nullptr;
    }
}

void bline_c::update(mVec3_c *startPos) {}

void bline_c::draw() {
    GXSetTevColor(GX_TEVREG0, mColor);

    VtxPos *vtxPos = mpVtxPosArr;
    VtxNrm *vtxNrm = mpVtxNrmArr;
    VtxTex *vtxTex = mpVtxTexArr;

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, mVtxNum * 2);

    for (int i = 0; i < mVtxNum; i++) {
        // First Vtx
        GXPosition3f32(vtxPos->pos1.x, vtxPos->pos1.y, vtxPos->pos1.z);
        GXNormal3s8(vtxNrm->nrm1.x, vtxNrm->nrm1.y, vtxNrm->nrm1.z);
        GXTexCoord2f32(0.0f, vtxTex->tex);

        // Second Vtx
        GXPosition3f32(vtxPos->pos2.x, vtxPos->pos2.y, vtxPos->pos2.z);
        GXNormal3s8(vtxNrm->nrm2.x, vtxNrm->nrm2.y, vtxNrm->nrm2.z);
        GXTexCoord2f32(0.0f, vtxTex->tex);

        vtxPos++;
        vtxNrm++;
        vtxTex++;
    }
}

} // namespace m3d
