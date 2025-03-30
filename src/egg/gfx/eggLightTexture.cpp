#include "egg/gfx/eggLightTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "egg/gfx/eggStateGX.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXGeometry.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/GX/GXVert.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtx44.h"

#include <cstring>

namespace EGG {

#define NUM_CPU_TEX 18

static CpuTexture *sCpuTexArray[NUM_CPU_TEX];

u16 LightTexture::sDrawWidth;
u16 LightTexture::sDrawHeight;
u16 LightTexture::sDrawPosX;
u16 LightTexture::sDrawPosY;
u16 LightTexture::sTexWidth;
u16 LightTexture::sTexHeight;
u16 LightTexture::sDrawNumX;
u16 LightTexture::sDrawNumY;

CpuTexture *LightTexture::spNormalEnvironment;

bool LightTexture::sUseDebug;

u16 LightTexture::sTextureSize = 0x20;

static s8 lbl_80574F3A = 0xFE;

template <>
const char *IBinary<LightTexture>::GetBinaryType() {
    return "LTEX";
}

template <>
int IBinary<LightTexture>::GetVersion() {
    return 3;
}

void LightTexture::fn_804AB270() {
    GXSetNumChans(1);
    GXSetCullMode(GX_CULL_BACK);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_SIGN, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(false, GX_LEQUAL, false);
    EGG::DrawGX::SetBlendMode(DrawGX::BLEND_14);
}

void LightTexture::fn_804AB310(const GXColor &color) {
    GXSetNumTexGens(0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevKColor(GX_KCOLOR0, color);
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_KONST);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    EGG::DrawGX::SetBlendMode(DrawGX::BLEND_14);
}

static const GXTevColorArg sTevkColorArg[3] = {GX_CC_C0, GX_CC_C1, GX_CC_C2};
static const GXTevKColorSel sTevkColorSel[3] = {GX_TEV_KCSEL_K0, GX_TEV_KCSEL_K1, GX_TEV_KCSEL_K2};
static const GXIndTexMtxID sIndTexMtxId[3] = {GX_ITM_0, GX_ITM_1, GX_ITM_2};

void LightTexture::fn_804AB440() {
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP0);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetNumTevStages(3);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);

    for (int i = 0; i < 3; i++) {
        GXTevStageID id = static_cast<GXTevStageID>(GX_TEVSTAGE0 + i);
        GXTevColorArg tevColorArg = id == GX_TEVSTAGE0 ? GX_CC_ZERO : GX_CC_CPREV;

        GXSetTevSwapMode(id, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetTevOrder(id, GX_TEXCOORD_NULL, static_cast<GXTexMapID>(GX_TEXMAP1 + i), GX_COLOR_NULL);
        GXSetTevKColorSel(id, sTevkColorSel[i]);
        GXSetTevKAlphaSel(id, GX_TEV_KASEL_1);
        GXSetTevColorIn(id, GX_CC_KONST, sTevkColorArg[i], GX_CC_TEXC, tevColorArg);
        GXSetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevIndirect(
            id, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_STU, sIndTexMtxId[i], GX_ITW_0, GX_ITW_0, false, false, GX_ITBA_OFF
        );
    }
    EGG::DrawGX::SetBlendMode(DrawGX::BLEND_2);
}

void LightTexture::fn_804AB600() {
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x1e, 0, 0x7d);
    GXSetNumIndStages(0);
    GXSetNumTevStages(2);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    for (int i = 0; i < 2; i++) {
        GXTevStageID id = static_cast<GXTevStageID>(GX_TEVSTAGE0 + i);
        GXTevColorArg tevColorArg = id == GX_TEVSTAGE0 ? GX_CC_ZERO : GX_CC_CPREV;

        GXSetTevOrder(
            id, static_cast<GXTexCoordID>(GX_TEXCOORD0 + i), static_cast<GXTexMapID>(GX_TEXMAP0 + i), GX_COLOR_NULL
        );
        GXSetTevKColorSel(id, sTevkColorSel[i]);
        GXSetTevKAlphaSel(id, GX_TEV_KASEL_1);
        GXSetTevColorIn(id, GX_CC_KONST, sTevkColorArg[i], GX_CC_TEXC, tevColorArg);
        GXSetTevColorOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(id, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
        GXSetTevAlphaOp(id, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
        GXSetTevDirect(id);
    }
    EGG::DrawGX::SetBlendMode(DrawGX::BLEND_2);
}

void LightTexture::initialize(u16 textureSize, Heap *pHeap) {
    static const int sCpuTexGradientOp[NUM_CPU_TEX] = {0, 0, 0, 1, 2, 3, 4, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0};

    if (pHeap == nullptr) {
        pHeap = Heap::getCurrentHeap();
    }
    sTextureSize = textureSize;
    for (int i = 0; i < NUM_CPU_TEX; i++) {
        sCpuTexArray[i] = new (pHeap) CpuTexture(64, 4, i < 10 ? GX_TF_I8 : GX_TF_RGBA8);
        CpuTexture *tex = sCpuTexArray[i];
        tex->configure();
        tex->setWrapS(GX_REPEAT);
        tex->setWrapT(GX_REPEAT);
        u16 a = 32;
        u16 b = 64;
        switch (i) {
            case 1: a = 0; break;
            case 2: a = 16; break;
            case 7:
                a = 4;
                b = 36;
                break;
            case 8:
                a = 10;
                b = 42;
                break;
            case 9:
                a = 13;
                b = 45;
                break;
        }
        tex->allocate(pHeap);
        tex->fillGradient(sCpuTexGradientOp[i], 0x73, a + 1, b - 1, DrawGX::BLACK, DrawGX::WHITE, true, true);
    }
    if (sTextureSize == 0x100) {
        lbl_80574F3A = 1;
    } else if (sTextureSize == 0x80) {
        lbl_80574F3A = 0;
    } else if (sTextureSize == 0x40) {
        lbl_80574F3A = -1;
    } else if (sTextureSize == 0x20) {
        lbl_80574F3A = -2;
    } else if (sTextureSize == 0x10) {
        lbl_80574F3A = -3;
    } else if (sTextureSize == 0x8) {
        lbl_80574F3A = -4;
    } else if (sTextureSize == 0x4) {
        lbl_80574F3A = -5;
    }

    spNormalEnvironment = new (pHeap) CpuTexture(sTextureSize, sTextureSize, GX_TF_RGBA8);
    spNormalEnvironment->configure();
    spNormalEnvironment->allocate(pHeap);
    spNormalEnvironment->fillNormalMapSphere(1.0f, 0.5f);
    spNormalEnvironment->setFilt(GX_NEAR, GX_NEAR);
    spNormalEnvironment->setWrap(GX_REPEAT, GX_REPEAT);
}

LightTexture::LightTexture(const char *name, const LightTextureManager *mgr)
    : CapTexture(sTextureSize, sTextureSize, GX_TF_RGBA8) {
    mpMgr = mgr;
    mNumData = mgr->getMaxNumLightTextures();
    field_0x35 = 0;
    field_0x36 = 0;
    mLightType = 0;
    field_0x38.set(0.0f, 0.0f, 0.0f);
    mpFloatData = nullptr;
    mpByteData1 = nullptr;
    mpByteData2 = nullptr;
    field_0x50 = 1.0f;
    field_0x94 = 0;
    field_0x98 = 1.0f;

    size_t len = std::strlen(name);
    if (len >= 0x1F) {
        len = 0x1E;
    }
    int i = 0;
    for (; i < len; i++) {
        mName1[i] = name[i];
    }
    mName1[len] = '\0';

    for (size_t i = 0; i < sizeof(mName2); i++) {
        mName2[i] = '\0';
    }
    field_0x9C = 0;
    mpFloatData = new float[mNumData];
    mpByteData1 = new u8[mNumData];
    mpByteData2 = new u8[mNumData];
    for (int i = 0; i < mNumData; i++) {
        mpByteData2[i] = 0;
        mpFloatData[i] = 1.0f;
        mpByteData1[i] = 0;
    }
}

LightTexture::~LightTexture() {
    delete[] mpFloatData;
    delete[] mpByteData1;
    delete[] mpByteData2;
}

void LightTexture::configure() {
    CapTexture::configure();
    if (sUseDebug) {
        allocWithHeaderDebug(nullptr);
    } else {
        allocate(nullptr);
    }
    setWrap(GX_CLAMP, GX_CLAMP);
    setPixModeSync(false);
}

void LightTexture::loadTextureData(int index, void *data, GXTexFmt fmt) {
    CpuTexture *pTex = sCpuTexArray[index];
    CpuTexture tmpTex(64, 4, fmt);
    tmpTex.configure();
    tmpTex.setBuffer(data);
    for (u32 y = 0; y < 4; y++) {
        for (int x = 0; x < 32; x++) {
            pTex->setColor(x, y, tmpTex.getColor(x + 32, y));
        }
        for (int x = 0; x < 32; x++) {
            pTex->setColor(x + 32, y, tmpTex.getColor(x, y));
        }
    }
    pTex->flush();
}

void LightTexture::loadTextureFromResTimg(int index, EGG::ResTIMG *img) {
    loadTextureData(index, img + 1, static_cast<GXTexFmt>(img->format));
}

void LightTexture::initDrawSetting(u16 u1, u16 u2, u16 u3, u16 u4) {
    static const u8 ALIGN_DECL(32) sVtxBuf[] = {0, 0, 0, 1, 1, 1, 1, 0};

    nw4r::math::MTX44 mtx;
    C_MTXOrtho(mtx, u4, 0.0f, 0.0f, u3, 0.0f, 1.0f);
    StateGX::GXSetProjection_(mtx, GX_ORTHOGRAPHIC);
    StateGX::GXSetViewport_(u1, u2, u3, u4, 0.0f, 1.0f);
    StateGX::GXSetScissor_(u1, u2, u3, u4);
    StateGX::GXSetScissorBoxOffset_(0, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetArray(GX_VA_POS, sVtxBuf, 2);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGB, GX_RGB565, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);

    sTexWidth = sTextureSize;
    sDrawNumX = u3 / sTextureSize;
    sTexHeight = sTextureSize;
    sDrawNumY = u4 / sTextureSize;
    sDrawWidth = u3;
    sDrawHeight = u4;
    sDrawPosX = u1;
    sDrawPosY = u2;

    fn_804AB270();
    GXSetCurrentMtx(0);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
}

void LightTexture::getTexDimensions(u16 *x, u16 *y, u16 *w, u16 *h, u16 count) {
    *x = sDrawPosX;
    *y = sDrawPosY;

    u16 s1 = count / (sDrawNumX + 1);
    u16 wp = (count - s1 * sDrawNumX);
    *w = wp * sTexWidth;
    *h = (s1 + 1) * sTexHeight;
}

void LightTexture::fn_804AC0A0(int i, int *x, int *y) {
    *y = i / sDrawNumX;
    *x = sTexWidth * (i % sDrawNumX);
    *y *= sTexHeight;
}

static const nw4r::math::MTX34 sIdentityMtx(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

void LightTexture::fn_804AC0E0(int i, const GXColor &color, bool b) {
    field_0x9F = 0;
    field_0x9C = 1;
    int w, h;
    fn_804AC0A0(i, &w, &h);

    nw4r::math::MTX34 mtx = sIdentityMtx;
    mtx._00 *= sTexWidth;
    mtx._11 *= sTexHeight;
    mtx._03 = w;
    mtx._13 = sDrawHeight - (h + sTexHeight);

    GXLoadPosMtxImm(mtx, 0);
    GXLoadTexMtxImm(sIdentityMtx, 0x1E, GX_MTX_2x4);

    GXColor c;
    if ((field_0x36 & 1) != 0) {
        const nw4r::g3d::AmbLightObj &obj = mpMgr->GetAmbientObj(field_0x35);
        c.r = obj.r;
        c.g = obj.g;
        c.b = obj.b;
        c.a = obj.a;

        c.r = c.r * field_0x50;
        c.g = c.g * field_0x50;
        c.b = c.b * field_0x50;
        c.a = 0xFF;
    } else {
        c = DrawGX::BLACK;
    }

    fn_804AB310(c);

    f32 origin = 0.0f;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    f32 d = 0.99f;

    GXPosition1x8(0);
    GXPosition2f32(origin, origin);

    GXPosition1x8(1);
    GXPosition2f32(origin, origin - d);

    GXPosition1x8(2);
    GXPosition2f32(d + origin, origin - d);

    GXPosition1x8(3);
    GXPosition2f32(d + origin, origin);

    switch (mLightType) {
        case 0:
        case 2:
            spNormalEnvironment->load(GX_TEXMAP0);
            fn_804AB440();
            field_0x9D = 3;
            field_0x9E = 1;
            break;

        case 1:
            fn_804AB600();
            field_0x9D = 2;
            field_0x9E = 0;
            break;
    }

    for (int i = 0; i < mpMgr->getMaxNumLightTextures(); i++) {
        if (mpMgr->GetLightObject(i) == nullptr) {
            break;
        }
        addLight(*mpMgr->GetLightObject(i));
    }

    int remainder = field_0x9F % field_0x9D;
    if (remainder > 0) {
        for (; remainder < field_0x9D; remainder++) {
            DrawGX::SetTevColor(
                static_cast<GXTevRegID>(GX_TEVREG0 + remainder), DrawGX::BLACK,
                static_cast<GXTevKColorID>(GX_KCOLOR0 + remainder), DrawGX::BLACK
            );
        }

        f32 origin = GetLightType() != 2 ? 0.0f : 0.5f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        f32 d = 0.99f;
        GXPosition1x8(0);
        GXPosition2f32(origin, origin);

        GXPosition1x8(1);
        GXPosition2f32(origin, origin - d);

        GXPosition1x8(2);
        GXPosition2f32(d + origin, origin - d);

        GXPosition1x8(3);
        GXPosition2f32(d + origin, origin);
    }

    field_0x9C = 2;
    if (b) {
        onCapFlag(1);
        onCapFlag(2);
        setClearColor(color);
    } else {
        offCapFlag(1);
        offCapFlag(2);
    }
    capture(sDrawPosX + w, sDrawPosY + h, false, -1);
    field_0x9C = 0;
}

void LightTexture::draw(int i) {
    if ((field_0x36 & 2) == 0) {
        return;
    }

    LightTexture *other = mpMgr->getTextureByName(mName2);
    if (other == nullptr) {
        return;
    }
    field_0x9F = 0;
    int w, h;
    fn_804AC0A0(i, &w, &h);
    load(GX_TEXMAP0);
    other->load(GX_TEXMAP1);
    DrawGX::SetMat_ColorChannel(DrawGX::COLORCHAN_1);
    DrawGX::SetMat_TexGen(DrawGX::TEXGEN_1);
    DrawGX::SetMat_Ind();
    DrawGX::SetMat_PE(DrawGX::ZMODE_0, DrawGX::BLEND_14);
    GXSetNumTevStages(2);
    u8 val = field_0x98 * 255.0f;
    GXSetTevColor(GX_TEVREG0, (GXColor){val, val, val, val});
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ONE, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);

    nw4r::math::MTX34 mtx = sIdentityMtx;
    mtx._00 *= sTexWidth;
    mtx._11 *= sTexHeight;
    mtx._03 = w;
    mtx._13 = sDrawHeight - (h + sTexHeight);

    GXLoadPosMtxImm(mtx, 0);
    GXLoadTexMtxImm(sIdentityMtx, 0x1E, GX_MTX_2x4);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition1x8(0);
    GXPosition2f32(0.0f, 1.0f);

    GXPosition1x8(1);
    GXPosition2f32(0.0f, 0.0f);

    GXPosition1x8(2);
    GXPosition2f32(1.0f, 0.0f);

    GXPosition1x8(3);
    GXPosition2f32(1.0f, 1.0f);

    capture(sDrawPosX + w, sDrawPosY + h, false, -1);
}

void LightTexture::beginDebugDraw() {
    DrawGX::SetMat_ColorChannel(DrawGX::COLORCHAN_1);
    DrawGX::SetMat_TexGen(DrawGX::TEXGEN_1);
    DrawGX::SetMat_Ind();
    DrawGX::SetMat_PE(DrawGX::ZMODE_0, DrawGX::BLEND_14);
    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, true, GX_TEVPREV);
}

void LightTexture::debugDraw(int i) {
    StateGX::ScopedDither ditherGuard(false);
    field_0x9F = 0;
    int u1, u2;
    fn_804AC0A0(i, &u1, &u2);
    load(GX_TEXMAP0);
    nw4r::math::MTX34 mtx;
    PSMTXScale(mtx, sTexWidth, sTexHeight, 1.0f);

    mtx._03 = u1;
    mtx._13 = sDrawHeight - (u2 + sTexHeight);

    GXLoadPosMtxImm(mtx, 0);
    GXLoadTexMtxImm(sIdentityMtx, 0x1E, GX_MTX_2x4);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition1x8(0);
    GXPosition2f32(0.0f, 1.0f);

    GXPosition1x8(1);
    GXPosition2f32(0.0f, 0.0f);

    GXPosition1x8(2);
    GXPosition2f32(1.0f, 0.0f);

    GXPosition1x8(3);
    GXPosition2f32(1.0f, 1.0f);
}

void LightTexture::addLight(const EGG::LightObject &obj) {
    if (!(obj.CheckFlag1() && obj.CheckFlag0x20() && (mpByteData1[obj.GetIndex()] & 1) != 0 &&
          (field_0x9F < 2 || mLightType != 1))) {
        return;
    }

    nw4r::math::VEC3 vec;
    GXColor color;
    obj.fn_804A9C30(this, &vec, &color);
    int remainder = field_0x9F % field_0x9D;
    if (field_0x9F == field_0x9D) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
    }
    sCpuTexArray[mpByteData2[obj.GetIndex()]]->load(static_cast<GXTexMapID>(field_0x9E + remainder));

    GXColor blk = obj.GetBlack();
    f32 mult = obj.getField0x30();
    blk = (GXColor){blk.r * mult, blk.g * mult, blk.b * mult, obj.GetBlack().a};

    DrawGX::SetTevColor(
        static_cast<GXTevRegID>(GX_TEVREG0 + remainder), color, static_cast<GXTevKColorID>(GX_KCOLOR0 + remainder), blk
    );
    if (GetLightType() == 0 || GetLightType() == 2) {
        f32 mtx[2][3];
        mtx[0][0] = -vec.x * 0.485f;
        mtx[0][1] = -vec.y * 0.485f;
        mtx[0][2] = -vec.z * 0.485f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.0f;
        mtx[1][2] = 0.0f;

        GXSetIndTexMtx(sIndTexMtxId[remainder], mtx, -1);
    }

    if (remainder == field_0x9D - 1) {
        f32 v = GetLightType() != 2 ? 0.0f : 0.5f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        GXPosition1x8(0);
        GXPosition2f32(v, v);

        GXPosition1x8(1);
        GXPosition2f32(v, v - 0.99f);

        GXPosition1x8(2);
        GXPosition2f32(v + 0.99f, v - 0.99f);

        GXPosition1x8(3);
        GXPosition2f32(v + 0.99f, v);
    }

    field_0x9F++;
}
void LightTexture::SetBinaryInner(const Bin &bin) {
    switch (bin.mHeader.mVersion) {
        case 0: {
            const struct Data {
                /* 0x00 */ u16 mNumEntries;
                /* 0x02 */ u8 _pad_02[2];
                /* 0x04 */ char mName[32];
                /* 0x24 */ u8 mType;
                /* 0x25 */ u8 _pad_25[3];
                /* 0x28 */ u32 field_0x28;
                /* 0x2C */ SubData mSubData[1];
            } &data = *(const Data *)&bin.mData;
            mLightType = data.mType;
            int numData = (u16)MIN(data.mNumEntries, (u16)mNumData);

            const SubData *pSub = data.mSubData;
            for (int i = 0; numData > 0; numData--, i++, pSub++) {
                if (data.field_0x28 & (1 << i)) {
                    onByte1(i);
                } else {
                    offByte1(i);
                }
                setFloat(i, pSub->mIntensity);
                setByte2(i, pSub->mGradientUsed);
            }

            field_0x35 = 0;
            field_0x50 = 1.f;
            field_0x36 &= ~1;
            field_0x36 &= ~2;
            mName2[0] = '\0';
            field_0x94 = -1;

        } break;
        case 1:
        case 2: {
            const struct Data {
                /* 0x00 */ u16 mNumEntries;
                /* 0x02 */ u8 field_0x02;
                /* 0x03 */ u8 field_0x03;
                /* 0x04 */ char mName[32];
                /* 0x24 */ u8 mType;
                /* 0x25 */ u8 _pad_25[3];
                /* 0x28 */ u32 field_0x28;
                /* 0x2C */ f32 field_0x2C;
                /* 0x30 */ u8 _30[8];
                /* 0x38 */ SubData mSubData[1];
            } &data = *(const Data *)&bin.mData;

            mLightType = data.mType;
            int numData = (u16)MIN(data.mNumEntries, (u16)mNumData);

            const SubData *pSub = data.mSubData;
            for (int i = 0; numData > 0; numData--, i++, pSub++) {
                if (bin.mHeader.mVersion == 1) {
                    if (data.field_0x28 & (1 << i)) {
                        onByte1(i);
                    } else {
                        offByte1(i);
                    }
                } else if (pSub->field_0x05 & 1) {
                    onByte1(i);
                } else {
                    offByte1(i);
                }
                setFloat(i, pSub->mIntensity);
                setByte2(i, pSub->mGradientUsed);
            }

            if (data.field_0x02 & 1) {
                field_0x36 |= 1;
            } else {
                field_0x36 &= ~1;
            }

            field_0x35 = data.field_0x03;
            field_0x50 = data.field_0x2C;
            field_0x36 &= ~2;
            mName2[0] = '\0';
            field_0x94 = -1;
            field_0x98 = 1.f;

        } break;
        case 3: {
            const BinData &data = bin.mData;
            mLightType = data.mType;
            int numData = (u16)MIN(data.mNumEntries, (u16)mNumData);

            const SubData *pSub = data.mSubData;
            for (int i = 0; numData > 0; numData--, i++, pSub++) {
                if (bin.mHeader.mVersion == 1) {
                    if (data.field_0x28 & (1 << i)) {
                        onByte1(i);
                    } else {
                        offByte1(i);
                    }
                } else if (pSub->field_0x05 & 1) {
                    onByte1(i);
                } else {
                    offByte1(i);
                }
                setFloat(i, pSub->mIntensity);
                setByte2(i, pSub->mGradientUsed);
            }

            if (data.field_0x02 & 1) {
                field_0x36 |= 1;
            } else {
                field_0x36 &= ~1;
            }
            field_0x35 = data.field_0x03;
            field_0x50 = data.field_0x2C;

            if (data.field_0x02 & 2) {
                field_0x36 |= 2;
            } else {
                field_0x36 &= ~2;
            }
            std::strcpy(mName2, data.mName2);
            field_0x94 = -1;
            field_0x98 = data.field_0x54;
        } break;
    }
}

void LightTexture::GetBinaryInner(Bin *pOutBin) const {
    pOutBin->mData.mType = mLightType;
    pOutBin->mData.mNumEntries = mNumData;
    std::strncpy(pOutBin->mData.mName, mName1, sizeof(mName1));
    SubData *dat = pOutBin->mData.mSubData;
    for (int i = 0; i < mNumData; i++) {
        dat[i].field_0x05 = 0;
        if (getByte1(i) & 1) {
            dat[i].field_0x05 |= 1;
        }
        dat[i].mIntensity = getFloat(i);
        dat[i].mGradientUsed = getByte2(i);
    }
    pOutBin->mData.field_0x02 = 0;
    if (field_0x36 & 1) {
        pOutBin->mData.field_0x02 |= 1;
    }
    if (field_0x36 & 2) {
        pOutBin->mData.field_0x02 |= 2;
    }
    pOutBin->mData.field_0x03 = field_0x35;
    pOutBin->mData.field_0x2C = field_0x50;
    std::strcpy(pOutBin->mData.mName2, mName2);
    pOutBin->mData.field_0x54 = field_0x98;
}

size_t LightTexture::GetBinarySize() const {
    return sizeof(Bin) + (mNumData - 1) * sizeof(SubData);
}

} // namespace EGG
