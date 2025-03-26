#include "egg/gfx/eggLightTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "egg/gfx/eggStateGX.h"
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
static const int sCpuTexGradientOp[NUM_CPU_TEX] = {0, 0, 0, 1, 2, 3, 4, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0};

static nw4r::math::MTX34 sMtx(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

u16 LightTexture::sTextureSize = 0x20;
CpuTexture *LightTexture::spNormalEnvironment;

static s8 lbl_80574F3A = 0xFE;

template <>
const char *IBinary<LightTexture>::GetBinaryType() {
    return "LTEX";
}

template <>
int IBinary<LightTexture>::GetVersion() {
    return 3;
}

void LightTexture::initialize(u16 textureSize, Heap *pHeap) {
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
    mpByteData1 = new char[mNumData];
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

extern "C" bool lbl_8057685C;

void LightTexture::configure() {
    CapTexture::configure();
    if (lbl_8057685C) {
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
    static const u8 sVtxBuf[] = {0, 0, 0, 1, 1, 1, 1, 0};

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

void LightTexture::draw(int i) {}

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
    GXLoadTexMtxImm(sMtx, 0x1E, GX_MTX_2x4);

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
    // NONMATCHING
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

    // TODO
    GXColor blackColor = obj.GetBlack();
    // Sorry clang-format, what is this formatting???
    blackColor = (GXColor
    ){blackColor.r * obj.getField0x30(), blackColor.g * obj.getField0x30(), blackColor.b * obj.getField0x30(),
      blackColor.a};
    GXSetTevColor(static_cast<GXTevRegID>(GX_TEVREG0 + remainder), color);
    GXSetTevKColor(static_cast<GXTevKColorID>(GX_KCOLOR0 + remainder), blackColor);
    if (GetLightType() == 0 || GetLightType() == 2) {
        f32 mtx[2][3];
        mtx[0][0] = -vec.x * 0.485f;
        mtx[0][1] = -vec.y * 0.485f;
        mtx[0][2] = -vec.z * 0.485f;
        mtx[1][0] = 0.0f;
        mtx[1][1] = 0.0f;
        mtx[1][2] = 0.0f;

        static const GXIndTexMtxID sIds[] = {GX_ITM_0, GX_ITM_1, GX_ITM_2};

        GXSetIndTexMtx(sIds[remainder], mtx, -1);
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
    // Not bothering with version differences right now,
    // since the game uses V3 and GetBinaryInner is enough
    // for the structure
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
