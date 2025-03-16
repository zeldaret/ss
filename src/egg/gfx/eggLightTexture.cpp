#include "egg/gfx/eggLightTexture.h"

#include "common.h"
#include "egg/core/eggHeap.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "rvl/GX/GXTypes.h"

#include <cstring>

namespace EGG {

#define NUM_CPU_TEX 18

static CpuTexture *sCpuTexArray[NUM_CPU_TEX];
static const int sCpuTexGradientOp[NUM_CPU_TEX] = {0, 0, 0, 1, 2, 3, 4, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0};

static nw4r::math::MTX34 sMtx(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

u16 LightTexture::sTextureSize;
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
    mpByteData2 = new char[mNumData];
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
