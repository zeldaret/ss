#include "egg/gfx/eggDrawGX.h"

#include "common.h"
#include "egg/gfx/eggGXUtility.h"
#include "egg/gfx/eggStateGX.h"
#include "math.h"
#include "nw4r/g3d/res/g3d_restex.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXAttr.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXDisplayList.h"
#include "rvl/GX/GXDraw.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtxvec.h"
#include "rvl/MTX/vec.h"
#include "rvl/OS/OSCache.h"

namespace EGG {

const GXColor DrawGX::BLACK = {0x00, 0x00, 0x00, 0xFF};
const GXColor DrawGX::WHITE = {0xFF, 0xFF, 0xFF, 0xFF};
const GXColor DrawGX::RED = {0xFF, 0x00, 0x00, 0xFF};
const GXColor DrawGX::GREEN = {0x00, 0xFF, 0x00, 0xFF};
const GXColor DrawGX::BLUE = {0x00, 0x00, 0xFF, 0xFF};

const DrawGX::DL DrawGX::s_DL0 = DrawGX::DL_0;
const DrawGX::DL DrawGX::s_DL7 = DrawGX::DL_7;
const DrawGX::DL DrawGX::s_DL8 = DrawGX::DL_8;

GXTexMapID DrawGX::sTexMapDefault;
GXLightID DrawGX::sLightMaskDefault = GX_LIGHT0;

nw4r::math::MTX34 DrawGX::s_cameraMtx;
DrawGX::DLData DrawGX::s_DL[DL_MAX];
GXTexObj DrawGX::sDummyTexObj;

u32 DrawGX::s_flag;

}; // namespace EGG

namespace {

static void DrawQuadNormal(u8 x1, u8 x2, u8 x3, u8 x4, u8 y) {
    GXPosition2u8(x1, y);
    GXPosition2u8(x2, y);
    GXPosition2u8(x3, y);
    GXPosition2u8(x4, y);
}

static void DrawQuadLineStripNormal(u8 x1, u8 x2, u8 x3, u8 x4, u8 y) {
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
    GXPosition2u8(x1, y);
    GXPosition2u8(x2, y);
    GXPosition2u8(x3, y);
    GXPosition2u8(x4, y);
    GXPosition2u8(x1, y);
}

static void DrawCircleYPolygonFan(const nw4r::math::MTX34 &mtx, f32 f, u16 numSegments) {
    f32 stepSize = (2.0f * M_PI / numSegments);
    nw4r::math::VEC3 v1(0.0f, 1.0f, 0.0f);
    nw4r::math::VEC3 v2(0.0f, f, 0.0f);

    int seg = numSegments + 1;

    PSMTXMultVec(mtx, v1, v1);
    PSMTXMultVec(mtx, v2, v2);

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, seg + 1);

    f32 signedStepSize = -stepSize;

    GXPosition3f32(v2.x, v2.y, v2.z);
    GXPosition3f32(v1.x, v1.y, v1.z);

    for (int i = 0; i < seg; i++) {
        nw4r::math::VEC3 v3(
            0.5f * nw4r::math::SinRad(signedStepSize * i), 0.0f, 0.5f * nw4r::math::CosRad(signedStepSize * i)
        );
        PSMTXMultVec(mtx, v3, v3);
        GXPosition3f32(v3.x, v3.y, v3.z);

        nw4r::math::VEC3 v4(
            0.5f * nw4r::math::SinRad(signedStepSize * (i - 1)), 0.0f,
            0.5f * nw4r::math::CosRad(signedStepSize * (i - 1))
        );
        nw4r::math::VEC3 v5(
            0.5f * nw4r::math::SinRad(signedStepSize * (i + 1)), 0.0f,
            0.5f * nw4r::math::CosRad(signedStepSize * (i + 1))
        );

        PSMTXMultVec(mtx, v4, v4);
        PSMTXMultVec(mtx, v5, v5);

        nw4r::math::VEC3Sub(&v4, &v4, &v2);
        nw4r::math::VEC3Sub(&v5, &v5, &v2);

        nw4r::math::VEC3 v6;
        PSVECCrossProduct(v5, v4, v6);
        PSVECNormalize(v6, v6);

        GXPosition3f32(v6.x, v6.y, v6.z);
    }
}

} // namespace

namespace EGG {

static DrawGX sDrawGX;

static u8 DummyTextureData[64] = {
    // clang-format off
0x00,  0xFF,  0x00,  0xFF,
0x00,  0xFF,  0x00,  0xFF,
0x00,  0xFF, 0x00, 0xFF,
0x00, 0xFF, 0x00, 0xFF,
0x00, 0xFF, 0x00, 0xFF,
0x00, 0xFF, 0x00, 0xFF,
0x00, 0xFF, 0x00, 0xFF,
0x00, 0xFF, 0x00, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF,
    // clang-format on
};

void DrawGX::Initialize(Heap *pHeap_) {
    2.0f; // cool
    Heap *pHeap = pHeap_ == nullptr ? Heap::getCurrentHeap() : pHeap_;

    GXInitTexObj(&sDummyTexObj, DummyTextureData, 4, 4, GX_TF_Z24X8, GX_REPEAT, GX_REPEAT, false);
    GXInitTexObjLOD(&sDummyTexObj, GX_NEAR, GX_NEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    PSMTXIdentity(s_cameraMtx.m);
    CreateDisplayList(pHeap);
}

GXTexMapID DrawGX::GetTexMapDefault() {
    return sTexMapDefault;
}
GXLightID DrawGX::GetLightMaskDefault() {
    return sLightMaskDefault;
}

void DrawGX::BeginDrawLine(ColorChannel chan, ZMode zMode) {
    SetMat_ColorChannel(chan);
    SetMat_TexGen(TEXGEN_0);
    SetMat_Ind();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetNumTevStages(1);
    SetMat_Tev(GX_TEVSTAGE0, TEV_0);
    SetMat_PE(zMode, BLEND_0);
    SetVtxState(VTX_TYPE_1);
}

void DrawGX::BeginDrawCircleZ(ColorChannel chan, ZMode zMode) {
    SetMat_ColorChannel(chan);
    SetMat_TexGen(TEXGEN_0);
    SetMat_Ind();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetNumTevStages(1);
    SetMat_Tev(GX_TEVSTAGE0, TEV_0);
    SetMat_PE(zMode, BLEND_0);
    SetVtxState(VTX_TYPE_4);
}

void DrawGX::BeginDrawQuad(ColorChannel chan, ZMode zMode, Blend blendMode, bool b1, bool b2) {
    SetMat_ColorChannel(chan);
    SetMat_TexGen(b1 ? TEXGEN_1 : TEXGEN_0);
    SetMat_Ind();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetNumTevStages(1);
    SetMat_Tev(GX_TEVSTAGE0, b1 ? TEV_2 : TEV_0);
    if (b2) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    }
    SetMat_PE(zMode, blendMode);
    SetVtxState(b1 ? VTX_TYPE_7 : VTX_TYPE_8);
}

static const DrawGX::VtxType sVtxTypes[2][2] = {
    {DrawGX::VTX_TYPE_10, DrawGX::VTX_TYPE_12},
    {DrawGX::VTX_TYPE_11, DrawGX::VTX_TYPE_13},
};

void DrawGX::BeginDrawScreen(bool b1, bool b2, bool b3) {
    SetMat_ColorChannel(COLORCHAN_1);
    SetMat_TexGen(b2 ? TEXGEN_1 : TEXGEN_0);
    SetMat_Ind();
    GXSetNumTevStages(1);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    SetMat_Tev(GX_TEVSTAGE0, b2 ? TEV_2 : TEV_0);
    if (b3) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    }
    SetMat_PE(ZMODE_0, BLEND_0);
    SetVtxState(sVtxTypes[b1 ? 1 : 0][b2 ? 0 : 1]);
}

void DrawGX::DrawLine(const EGG::Vector3f *pPoints, u16 numPoints, GXColor color, u8 thickness) {
    GXLoadPosMtxImm(s_cameraMtx.m, 0);
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetLineWidth(thickness, 0);
    GXBegin(GX_LINES, GX_VTXFMT0, numPoints);
    for (int i = 0; i < numPoints; i++) {
        GXPosition3f32(pPoints[i].x, pPoints[i].y, pPoints[i].z);
    }
}

void DrawGX::DrawLineStrip(const EGG::Vector3f *pPoints, u16 numPoints, GXColor color, u8 thickness) {
    GXLoadPosMtxImm(s_cameraMtx.m, 0);
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetLineWidth(thickness, 0);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, numPoints);
    for (int i = 0; i < numPoints; i++) {
        GXPosition3f32(pPoints[i].x, pPoints[i].y, pPoints[i].z);
    }
}

void DrawGX::ClearEfb(
    const nw4r::math::MTX34 &mat, bool bColorUpdate, bool bAlphaUpdate, bool b3, GXColor clr, bool b4
) {
    StateGX::ScopedColor colorUpdate(bColorUpdate);
    StateGX::ScopedAlpha alphaUpdate(bAlphaUpdate);
    SetMat_ColorChannel(COLORCHAN_1);
    GXSetCullMode(GX_CULL_NONE);
    if (b3) {
        SetMat_TexGen(TEXGEN_1);
        GXLoadTexObj(&sDummyTexObj, sTexMapDefault);
    } else {
        SetMat_TexGen(TEXGEN_0);
    }

    SetMat_Ind();
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevColor(GX_TEVREG0, clr);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    if (b3) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, sTexMapDefault, GX_COLOR_NULL);
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    }
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    SetBlendMode(BLEND_14);
    if (b3) {
        GXSetZTexture(2, GX_TF_Z24X8, 0);
        SetZMode(ZMODE_3);
        GXSetZCompLoc(0);
        SetVtxState(b4 ? VTX_TYPE_11 : VTX_TYPE_10);
        GXLoadPosMtxImm(mat, 0);
        GXCallDisplayList(s_DL[DL_16].mpList, s_DL[DL_16].mLen);
        GXSetZTexture(0, GX_TF_Z24X8, 0);
        GXSetZCompLoc(1);
    } else {
        SetZMode(ZMODE_0);
        SetVtxState(b4 ? VTX_TYPE_13 : VTX_TYPE_12);
        GXLoadPosMtxImm(mat, 0);
        GXCallDisplayList(s_DL[DL_17].mpList, s_DL[DL_17].mLen);
    }
}

void DrawGX::ResetMaterial(enum ColorChannel chan) {
    SetMat_ColorChannel(chan);
    SetMat_TexGen(TEXGEN_0);
    SetMat_Ind();
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetNumTevStages(1);
    SetMat_Tev(GX_TEVSTAGE0, TEV_0);
    SetMat_PE(ZMODE_1, BLEND_0);
}

void DrawGX::SetMat_ColorChannel(enum ColorChannel chan) {
    switch (chan) {
        case COLORCHAN_0:
            GXSetNumChans(1);
            GXSetCullMode(GX_CULL_BACK);
            GXSetChanCtrl(GX_COLOR0A0, true, GX_SRC_REG, GX_SRC_REG, GetLightMaskDefault(), GX_DF_CLAMP, GX_AF_SPOT);
            GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            break;
        case COLORCHAN_1:
            GXSetNumChans(1);
            GXSetCullMode(GX_CULL_BACK);
            GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
            GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            break;
    }
}

void DrawGX::SetMat_TexGen(enum TexGen texGen) {
    switch (texGen) {
        case TEXGEN_0:
            GXSetNumTexGens(0);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
            break;
        case TEXGEN_1:
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
            break;
    }
}

void DrawGX::SetMat_Ind() {
    GXSetNumIndStages(0);
}

void DrawGX::SetMat_Tev(GXTevStageID stageId, enum TevSetting setting) {
    switch (setting) {
        case TEV_0:
            GXSetTevDirect(stageId);
            GXSetTevOrder(stageId, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevSwapMode(stageId, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevColorIn(stageId, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
            GXSetTevColorOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(stageId, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            GXSetTevAlphaOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            break;
        case TEV_2:
            GXSetTevDirect(stageId);
            GXSetTevOrder(stageId, GX_TEXCOORD0, sTexMapDefault, GX_COLOR0A0);
            GXSetTevSwapMode(stageId, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevColorIn(stageId, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevColorOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(stageId, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
            GXSetTevAlphaOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            break;
        case TEV_1:
            GXSetTevDirect(stageId);
            GXSetTevOrder(stageId, GX_TEXCOORD0, sTexMapDefault, GX_COLOR0A0);
            GXSetTevSwapMode(stageId, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevColorIn(stageId, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
            GXSetTevColorOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(stageId, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            GXSetTevAlphaOp(stageId, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
            break;
    }
}

void DrawGX::SetMat_PE(enum ZMode zMode, enum Blend blendMode) {
    GXSetZCompLoc(true);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    SetZMode(zMode);
    SetBlendMode(blendMode);
}

void DrawGX::LoadTexture(const EGG::ResTIMG *tex, GXTexMapID texMapId) {
    GXTexObj obj;
    GXUtility::getTexObj(&obj, *tex);
    GXLoadTexObj(&obj, texMapId);
}

void DrawGX::LoadTexture(nw4r::g3d::ResTex tex, GXTexMapID texMapId) {
    GXTexObj obj;
    GXUtility::getTexObj(&obj, tex, GX_CLAMP, GX_CLAMP, GX_LINEAR, GX_LINEAR);
    GXLoadTexObj(&obj, texMapId);
}

void DrawGX::SetVtxState(EGG::DrawGX::VtxType type) {
    GXClearVtxDesc();
    switch (type) {
        case VTX_TYPE_0:
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0xe);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 0xe);
            // clang-format off
            static const ALIGN_DECL(32) s16 sVtxDataType0[][3] = {
                {0xE000, 0x2000, 0xE000},
                {0xE000, 0x2000, 0x2000},
                {0x2000, 0x2000, 0x2000},
                {0x2000, 0x2000, 0xE000},
                {0xE000, 0xE000, 0xE000},
                {0xE000, 0xE000, 0x2000},
                {0x2000, 0xE000, 0x2000},
                {0x2000, 0xE000, 0xE000},
            };
            static const ALIGN_DECL(32) s16 sNrmDataType0[][3] = {
                {0xC000, 0x0000, 0x0000},
                {0x4000, 0x0000, 0x0000},
                {0x0000, 0xC000, 0x0000},
                {0x0000, 0x4000, 0x0000},
                {0x0000, 0x0000, 0xC000},
                {0x0000, 0x0000, 0x4000},
            };
            // clang-format on
            GXSetArray(GX_VA_POS, sVtxDataType0, 6);
            GXSetArray(GX_VA_NRM, sNrmDataType0, 6);
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            break;
        case VTX_TYPE_2:
        case VTX_TYPE_3:
        case VTX_TYPE_4:
        case VTX_TYPE_5:
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
            break;
        case VTX_TYPE_6:
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            break;
        case VTX_TYPE_1:
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            break;
        case VTX_TYPE_7:
        case VTX_TYPE_8:
            // clang-format off
            static const ALIGN_DECL(32) s16 sVtxDataType7[][2] = {
                {0xE000, 0x2000},
                {0x2000, 0x2000},
                {0x2000, 0xE000},
                {0xE000, 0xE000},
            };
            static const ALIGN_DECL(32) s16 sNrmDataType7[][3] = {
                {0x0000, 0x0000, 0x4000}
            };
            static const ALIGN_DECL(32) u8 sTexDataType7[][2] = {
                {0x00, 0x00},
                {0x01, 0x00},
                {0x01, 0x01},
                {0x00, 0x01},
            };
            // clang-format on
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            if (type == VTX_TYPE_7) {
                GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            }
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_S16, 0xe);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 0xe);
            if (type == VTX_TYPE_7) {
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
            }
            GXSetArray(GX_VA_POS, sVtxDataType7, 4);
            GXSetArray(GX_VA_NRM, sNrmDataType7, 6);
            if (type == VTX_TYPE_7) {
                GXSetArray(GX_VA_TEX0, sTexDataType7, 2);
            }
            break;
        case VTX_TYPE_9:
            // clang-format off
            static const ALIGN_DECL(32) s16 sVtxDataType9[][3] = {
                {0xE000, 0x0000, 0xE000},
                {0x2000, 0x0000, 0xE000},
                {0x2000, 0x0000, 0x2000},
                {0xE000, 0x0000, 0x2000},
            };
            static const ALIGN_DECL(32) s16 sNrmDataType9[][3] = {
                {0x0000, 0x4000, 0x0000}  
            };
            // clang-format on
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0xe);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S16, 0xe);
            GXSetArray(GX_VA_POS, sVtxDataType9, 6);
            GXSetArray(GX_VA_NRM, sNrmDataType9, 6);
            break;
        case VTX_TYPE_10:
        case VTX_TYPE_11:
        case VTX_TYPE_12:
        case VTX_TYPE_13: {
            // clang-format off
            static const ALIGN_DECL(32) u8 sPosDataType10[][2] = {
                {0x00, 0x01},
                {0x01, 0x01},
                {0x01, 0x00},
                {0x00, 0x00},
            };
            static const ALIGN_DECL(32) u8 sTexDataType10[][2] = {
                {0x00, 0x00},
                {0x01, 0x00},
                {0x01, 0x01},
                {0x00, 0x01},
            };
            // clang-format on
            bool bVar1 = type == VTX_TYPE_11 || type == VTX_TYPE_13;
            bool bVar2 = type == VTX_TYPE_10 || type == VTX_TYPE_11;
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            if (bVar2) {
                GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            }
            GXSetArray(GX_VA_POS, bVar1 ? sTexDataType10 : sPosDataType10, 2);
            if (bVar2) {
                GXSetArray(GX_VA_TEX0, sTexDataType10, 2);
            }
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U8, 0);
            if (bVar2) {
                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U8, 0);
            }
            break;
        }
    }
}

const u8 sDetailLevels[] = {0x10, 0x20};

void DrawGX::CreateDisplayList(EGG::Heap *pHeap) {
    u8 ALIGN_DECL(32) tmpDisplayList[16 * 1024];

    OSInitFastCast();

    for (int i = 0; i < DL_MAX; i++) {
        DCInvalidateRange(tmpDisplayList, sizeof(tmpDisplayList));
        GXBeginDisplayList(tmpDisplayList, sizeof(tmpDisplayList));
        switch (static_cast<DL>(i)) {
            case DL_0:
                GXSetChanMatColor(GX_COLOR0A0, RED);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(1.0f, 0.0f, 0.0f);
                GXSetChanMatColor(GX_COLOR0A0, GREEN);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(0.0f, 1.0f, 0.0f);
                GXSetChanMatColor(GX_COLOR0A0, BLUE);
                GXBegin(GX_LINES, GX_VTXFMT0, 2);
                GXPosition3f32(0.0f, 0.0f, 0.0f);
                GXPosition3f32(0.0f, 0.0f, 1.0f);
                break;
            case DL_1:
                GXBegin(GX_QUADS, GX_VTXFMT0, 6 * 4);
                DrawQuadNormal(1, 5, 4, 0, 0);
                DrawQuadNormal(3, 7, 6, 2, 1);
                DrawQuadNormal(5, 6, 7, 4, 2);
                DrawQuadNormal(3, 2, 1, 0, 3);
                DrawQuadNormal(0, 4, 7, 3, 4);
                DrawQuadNormal(2, 6, 5, 1, 5);
                break;
            case DL_2:
                DrawQuadLineStripNormal(1, 5, 4, 0, 0);
                DrawQuadLineStripNormal(3, 7, 6, 2, 1);
                DrawQuadLineStripNormal(5, 6, 7, 4, 2);
                DrawQuadLineStripNormal(3, 2, 1, 0, 3);
                break;
            case DL_3: GXDrawSphere(4, 8); break;
            case DL_4: GXDrawSphere(8, 16); break;
            case DL_5:
            case DL_6: {
                u16 numSegments = sDetailLevels[i - DL_5] + 1;
                f32 radPerSegment = 2.0f * M_PI / (numSegments - 1);
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, numSegments);
                for (int i = 0; i < numSegments; i++) {
                    GXPosition3f32(
                        0.5f * nw4r::math::CosRad(radPerSegment * i), 0.5f * nw4r::math::SinRad(radPerSegment * i), 0.0f
                    );
                }
                break;
            }
            case DL_9:
            case DL_10: {
                u16 numSegments = sDetailLevels[i - DL_9] + 1;
                f32 radPerSegment = 2.0f * M_PI / (numSegments - 1);
                GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, numSegments * 2);
                for (int i = 0; i < numSegments; i++) {
                    f32 x = 0.5f * nw4r::math::SinRad(radPerSegment * i);
                    f32 z = 0.5f * nw4r::math::CosRad(radPerSegment * i);
                    GXPosition3f32(x, -0.5f, z);
                    GXPosition3f32(2.0f * x, 0.0f, 2.0f * z);
                    GXPosition3f32(x, 0.5f, z);
                    GXPosition3f32(2.0f * x, 0.0f, 2.0f * z);
                }
                nw4r::math::MTX34 mtx;
                PSMTXTrans(mtx, 0.0f, 0.5f, 0.0f);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_9]);

                PSMTXIdentity(mtx);
                PSMTXRotRad(mtx, M_PI, 0x7A);
                PSMTXTransApply(mtx, mtx, 0.0f, -0.5f, 0.0f);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_9]);
                break;
            }
            case DL_11:
            case DL_12: {
                nw4r::math::MTX34 mtx;
                PSMTXIdentity(mtx);
                DrawCircleYPolygonFan(mtx, 1.0f, sDetailLevels[i - DL_11]);
                PSMTXRotRad(mtx, M_PI, 0x7A);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_11]);
                break;
            }
            case DL_7:
            case DL_8: {
                nw4r::math::MTX34 mtx;
                PSMTXIdentity(mtx);
                PSMTXRotRad(mtx, M_PI / 2.0f, 0x78);
                DrawCircleYPolygonFan(mtx, 0.0f, sDetailLevels[i - DL_7]);
                break;
            }

            case DL_13:
            case DL_14: {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition2u8(0, 0);
                if (i == DL_13) {
                    GXPosition1x8(0);
                }
                GXPosition2u8(1, 0);
                if (i == DL_13) {
                    GXPosition1x8(1);
                }
                GXPosition2u8(2, 0);
                if (i == DL_13) {
                    GXPosition1x8(2);
                }
                GXPosition2u8(3, 0);
                if (i == DL_13) {
                    GXPosition1x8(3);
                }
                break;
            }
            case DL_15:
                GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
                GXPosition3f32(-0.5f, 0.5f, 0.0f);
                GXPosition3f32(0.5f, 0.5f, 0.0f);
                GXPosition3f32(0.5f, -0.5f, 0.0f);
                GXPosition3f32(-0.5f, -0.5f, 0.0f);
                GXPosition3f32(-0.5f, 0.5f, 0.0f);
                break;
            case DL_16:
            case DL_17:
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition1x8(0);
                if (i == DL_16) {
                    GXPosition1x8(0);
                }
                GXPosition1x8(1);
                if (i == DL_16) {
                    GXPosition1x8(1);
                }
                GXPosition1x8(2);
                if (i == DL_16) {
                    GXPosition1x8(2);
                }
                GXPosition1x8(3);
                if (i == DL_16) {
                    GXPosition1x8(3);
                }
                break;
            default: break;
        }

        u32 size = GXEndDisplayList();
        s_DL[i].mLen = size;
        u8 *data = new (pHeap, 0x20) u8[size];
        s_DL[i].mpList = data;
        for (int j = 0; j < size; j++) {
            data[j] = tmpDisplayList[j];
        }
        DCFlushRange(data, size);
    }
    s_flag |= 1;
}

void DrawGX::DrawDL(enum DL dl, const nw4r::math::MTX34 &mtx, GXColor clr) {
    GXLoadPosMtxImm(mtx, 0);
    GXLoadNrmMtxImm(mtx, 0);
    GXSetChanMatColor(GX_COLOR0A0, clr);
    GXCallDisplayList(s_DL[dl].mpList, s_DL[dl].mLen);
}

const DrawGX::ZModeConfig DrawGX::s_ZMode[ZMODE_MAX] = {
    {false, GX_ALWAYS, false},
    { true, GX_LEQUAL,  true},
    { true, GX_LEQUAL, false},
    { true, GX_ALWAYS,  true},
};

void DrawGX::SetZMode(enum ZMode mode) {
    GXSetZMode(s_ZMode[mode].mTest, s_ZMode[mode].mCompare, s_ZMode[mode].mUpdate);
}

const DrawGX::BlendModeConfig DrawGX::s_Blend[BLEND_MAX] = {
    {   GX_BM_BLEND,    GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR},
    {   GX_BM_BLEND, GX_BL_INVSRCALPHA,    GX_BL_SRCALPHA, GX_LO_CLEAR},
    {   GX_BM_BLEND,         GX_BL_ONE,         GX_BL_ONE, GX_LO_CLEAR},
    {   GX_BM_BLEND,    GX_BL_SRCALPHA,         GX_BL_ONE, GX_LO_CLEAR},
    {   GX_BM_BLEND,      GX_BL_SRCCLR,         GX_BL_ONE, GX_LO_CLEAR},
    {   GX_BM_BLEND,   GX_BL_INVSRCCLR,         GX_BL_ONE, GX_LO_CLEAR},
    {GX_BM_SUBTRACT,         GX_BL_ONE,         GX_BL_ONE, GX_LO_CLEAR},
    {   GX_BM_BLEND,        GX_BL_ZERO,      GX_BL_SRCCLR, GX_LO_CLEAR},
    {   GX_BM_BLEND,        GX_BL_ZERO,   GX_BL_INVSRCCLR, GX_LO_CLEAR},
    {   GX_BM_BLEND,         GX_BL_ONE,        GX_BL_ZERO, GX_LO_CLEAR},
    {   GX_BM_BLEND,    GX_BL_DSTALPHA, GX_BL_INVDSTALPHA, GX_LO_CLEAR},
    {   GX_BM_BLEND, GX_BL_INVDSTALPHA,    GX_BL_DSTALPHA, GX_LO_CLEAR},
    {   GX_BM_BLEND,    GX_BL_DSTALPHA,         GX_BL_ONE, GX_LO_CLEAR},
    {   GX_BM_BLEND, GX_BL_INVDSTALPHA,         GX_BL_ONE, GX_LO_CLEAR},
    {    GX_BM_NONE,         GX_BL_ONE,         GX_BL_ONE, GX_LO_CLEAR},
};

void DrawGX::SetBlendMode(enum Blend mode) {
    GXSetBlendMode(
        s_Blend[mode].mBlendMode, s_Blend[mode].mBlendFactorSrc, s_Blend[mode].mBlendFactorDst, s_Blend[mode].mLogicOp
    );
}

void DrawGX::DrawDLWorld(DL dl, const EGG::Matrix34f &mtx, GXColor color) {
    nw4r::math::MTX34 resMtx;

    PSMTXConcat(s_cameraMtx.m, mtx.m, resMtx.m);
    DrawDL(dl, resMtx, color);
}

} // namespace EGG
