#include "egg/gfx/eggLightTextureMgr.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggG3DUtility.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/gfx/eggLightTexture.h"
#include "egg/gfx/eggStateGX.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/math/math_types.h"
#include "nw4r/ut/ut_Color.h"
#include "rvl/GX/GXTexture.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "rvl/MTX/mtx44.h"

#include <cstring>

namespace EGG {

LightTextureManager::LightTextureManager(const LightManager *lightMgr) {
    mFlags = 0x78;
    mTextureCount = 0;
    mpTextures = nullptr;
    mpLightMgr = lightMgr;
    field_0x10 = 0;
    mMaxNumTextures = 0x80;
    field_0x16 = 1;
    mpObjects = nullptr;
    mpTextures = new LightTexture *[0x80];
    for (int i = 0; i < 0x80; i++) {
        mpTextures[i] = nullptr;
    }

    mpObjects = new const LightObject *[mMaxNumTextures];
    for (int i = 0; i < mMaxNumTextures; i++) {
        mpObjects[i] = nullptr;
    }

    if ((StateGX::s_flag & 0x40) != 0) {
        mFlags |= 0x80;
        mFlags &= ~0x40;
        field_0x16 = 3;
    }
}

LightTextureManager::~LightTextureManager() {
    for (int i = 0; i < 0x80; i++) {
        deleteTexture(i);
    }
    delete[] mpTextures;
    delete[] mpObjects;
}

u16 LightTextureManager::createTexture(const char *name) {
    // Check if one already exists with the same name
    for (int i = 0; i < getNumLightTextures(); i++) {
        if (mpTextures[i] != nullptr && !strcmp(name, mpTextures[i]->getName())) {
            return i;
        }
    }

    // Find a hole to fill
    int targetPosition = getNumLightTextures();
    for (int i = 0; i < getNumLightTextures(); i++) {
        if (mpTextures[i] == nullptr) {
            targetPosition = i;
        }
    }
    if (targetPosition == getNumLightTextures()) {
        mTextureCount++;
    }
    LightTexture *tex = new LightTexture(name, this);
    mpTextures[targetPosition] = tex;
    tex->configure();
    return targetPosition;
}

u16 LightTextureManager::createTextureFromBin(const void *bin) {
    const LightTexture::Bin *data = reinterpret_cast<const LightTexture::Bin *>(bin);
    u16 idx = createTexture(data->mData.mName);
    mpTextures[idx]->SetBinary(data);
    return idx;
}

int LightTextureManager::createTexturesFromBin(const void *bin) {
    const LightTextureManager::Bin *data = reinterpret_cast<const LightTextureManager::Bin *>(bin);
    // Moderate inefficiency here - this algorithm has quadratic runtime
    // since we always search from the beginning
    for (int i = 0; i < data->mData.mDataCount; i++) {
        const void *subData = getLtexFromLmap(data, (u16)i);
        createTextureFromBin(subData);
    }
    return data->mData.mDataCount;
}

bool LightTextureManager::deleteTexture(int idx) {
    if (0 <= idx && idx < 0x80) {
        if (mpTextures[idx] != nullptr) {
            delete mpTextures[idx];
            mpTextures[idx] = nullptr;
            return true;
        }
    }
    return false;
}

u16 LightTextureManager::replaceModelTextures(nw4r::g3d::ResMdl mdl) const {
    u16 count = 0;
    for (u16 i = 0; i < mTextureCount; i++) {
        count += replaceModelTexture(i, mdl);
    }
    return count;
}

u16 LightTextureManager::replaceModelTexture(int tex, nw4r::g3d::ResMdl mdl) const {
    if (mpTextures[tex] == nullptr) {
        return 0;
    }

    G3DUtility::SetTextureResult buf[256];
    GXTexObj obj;
    mpTextures[tex]->getTexObj(&obj);
    u16 count = 0;
    for (int i = 0; i < mdl.GetResMatNumEntries(); i++) {
        nw4r::g3d::ResMat mat = mdl.GetResMat(i);

        u16 res = G3DUtility::SetTexture(mat, nullptr, mpTextures[tex]->getName(), &obj, false, buf, 0xFF, 2);
        for (int j = 0; j < res; j++) {
            if (buf[j].texCoordId != -1) {
                setupEnvmap(mat, static_cast<GXTexCoordID>(buf[j].texCoordId));
            }
        }
        count += res;
    }
    return count;
}

int LightTextureManager::getTextureIndex(const char *name) const {
    for (int i = 0; i < mTextureCount; i++) {
        if (!strcmp(name, mpTextures[i]->getName())) {
            return i;
        }
    }

    return -1;
}

bool LightTextureManager::setBinaryToTexture(const void *data) {
    const LightTexture::Bin *bin = reinterpret_cast<const LightTexture::Bin *>(data);
    for (int i = 0; i < getNumLightTextures(); i++) {
        if (!std::strcmp(bin->mData.mName, mpTextures[i]->getName())) {
            mpTextures[i]->SetBinary(bin);
            return true;
        }
    }
    return false;
}

void LightTextureManager::correctLightObject() {
    int j = 0;
    for (int i = 0; i < mpLightMgr->GetNumLightData(); i++) {
        const LightObject *o = mpLightMgr->GetLightObject((u16)i);
        if (o->CheckFlag1() && o->CheckFlag0x20()) {
            mpObjects[j] = o;
            j++;
            if (j >= mMaxNumTextures) {
                break;
            }
        }
    }

    for (int i = j; i < mMaxNumTextures; i++) {
        mpObjects[i] = nullptr;
    }
}

void LightTextureManager::frameReset() {
    correctLightObject();
    field_0x10 = 0;
}

void LightTextureManager::drawAndCaptureTexture(f32 ox, f32 oy, f32 sx, f32 sy) {
    bool b1 = getSomeTfRelatedBool();
    u32 mask1 = 1 << (mpLightMgr->GetField0x1D());
    if ((mFlags & 0x10) == 0 || (field_0x10 & mask1) != 0 || mTextureCount == 0) {
        return;
    }

    StateGX::resetGXCache();
    if ((mFlags & 0x40) != 0) {
        ox = 0.0f;
        oy = StateGX::s_heightEfb;
        sx = StateGX::s_widthEfb;
        sy = 528 - StateGX::s_heightEfb;
    }
    field_0x10 |= mask1;

    StateGX::ScopedColor colorGuard(true);
    StateGX::ScopedAlpha alphaGuard(false);

    LightTexture::initDrawSetting(ox + ((u16)ox & 1), oy + ((u16)oy & 1), sx, sy);

    u16 x1, y1, x2, y2;
    TextureBuffer *buf = nullptr;
    LightTexture::getRequiredPosAndSize(&x1, &y1, &x2, &y2, mTextureCount);
    x2 += x2 & 3;
    y2 += y2 & 3;

    if ((mFlags & 0x40) == 0 && (mFlags & 4) != 0) {
        buf = TextureBuffer::alloc(x2, y2, GX_TF_RGBA8);
        buf->capture(x1, y1, false, -1);
    }

    if (b1) {
        StateGX::setPixelFormatGX(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }

    nw4r::ut::Color c(StateGX::s_clearEfb);
    if (b1 && StateGX::s_pixFormat == GX_PF_RGBA6_Z24) {
        // convert GX_PF_RGBA6_Z24 -> GX_PF_RGB8_Z24
        u8 tmp = c.a >> 2;
        c = ((c.r >> 2) << 26) | (c.g & 0x3FFC) << 18 | (c.b & 0xFC) << 12 | (tmp & 0x3F) << 8;
    }

    bool b2 = b1 && (mFlags & 8) != 0;
    for (int i = 0; i < mTextureCount; i++) {
        mpTextures[i]->fn_804AC0E0(i, c, b2);
    }

    for (int i = 0; i < mTextureCount; i++) {
        mpTextures[i]->draw(i);
    }

    if (b1) {
        StateGX::setPixelFormatGX(StateGX::s_pixFormat, StateGX::s_zFmt16);
    }

    if ((mFlags & 0x40) == 0 && buf != nullptr) {
        StateGX::ScopedColor colorGuard(true);
        StateGX::ScopedAlpha alphaGuard(true);
        nw4r::math::MTX44 orthMtx;
        C_MTXOrtho(orthMtx, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        StateGX::GXSetProjection_(orthMtx, GX_ORTHOGRAPHIC);
        StateGX::GXSetViewport_(x1, y1, x2, y2, 0.0f, 1.0f);
        StateGX::GXSetScissor_(x1, y1, x2, y2);
        StateGX::GXSetScissorBoxOffset_(0, 0);
        nw4r::math::MTX34 identity;
        PSMTXIdentity(identity);

        DrawGX::BeginDrawScreen(1, 1, 0);
        DrawGX::SetBlendMode(DrawGX::BLEND_14);
        GXSetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        buf->load(GX_TEXMAP0);
        DrawGX::DrawDL(DrawGX::DL_16, identity, DrawGX::WHITE);
        buf->free();
    }

    StateGX::invalidateTexAllGX();
    if ((mFlags & 0x80) != 0) {
        LightTexture::beginDebugDraw();
        for (int i = 0; i < mTextureCount; i++) {
            mpTextures[i]->debugDraw(i);
        }
    }
}

void LightTextureManager::SetBinaryInner(const Bin &bin) {
    if (bin.mHeader.mVersion != 0) {
        return;
    }
    const char *buf = reinterpret_cast<const char *>(bin.mData.mSubData);
    for (int i = 0; i < bin.mData.mDataCount; i++) {
        const BinHeader *hdr = reinterpret_cast<const BinHeader *>(buf);
        setBinaryToTexture(buf);
        buf += hdr->mSize;
    }
}

void LightTextureManager::GetBinaryInner(Bin *pOutBin) const {
    int count = 0;
    char *buf = reinterpret_cast<char *>(pOutBin->mData.mSubData);
    for (int i = 0; i < getNumLightTextures(); i++) {
        mpTextures[i]->GetBinary(buf);
        BinHeader *hdr = reinterpret_cast<BinHeader *>(buf);
        // This is where the actually written size is added to the whole object
        pOutBin->mHeader.mSize += hdr->mSize;
        buf += hdr->mSize;
        count++;
    }
    pOutBin->mData.mDataCount = count;
}

// Performs a linear search through the data to find the offset for the given index
const void *LightTextureManager::getLtexFromLmap(const void *lmap, u16 index) {
    // A bit awkward but it matches
    const BinHeader *buf = nullptr;
    const Bin *bin = reinterpret_cast<const Bin *>(lmap);
    if (bin->mHeader.mVersion == 0 && index < bin->mData.mDataCount) {
        buf = bin->mData.mSubData;
        for (int i = 0; i < index; i++) {
            buf = reinterpret_cast<const BinHeader *>(reinterpret_cast<const char *>(buf) + buf->mSize);
        }
    }

    return buf;
}

void LightTextureManager::setupEnvmap(nw4r::g3d::ResMat mat, GXTexCoordID id) {
    nw4r::g3d::ResTexSrt dat = mat.GetResTexSrt();
    dat.SetMapMode(id, 1, -1, -1);
    nw4r::g3d::ResMatTexCoordGen coordGen = mat.GetResMatTexCoordGen();

    GXTexGenType type;
    GXTexGenSrc src;
    GXBool normalize;
    u32 postMtx;
    coordGen.GXGetTexCoordGen2(id, &type, &src, &normalize, &postMtx);
    coordGen.GXSetTexCoordGen2(id, GX_TG_MTX3x4, GX_TG_NRM, true, postMtx);
    coordGen.DCStore(false);
}

size_t LightTextureManager::GetBinarySize() const {
    // Contrary to the other dynamically-sized Bins, this only
    // returns the fixed size. GetBinaryInner will add the dynamic
    // parts when writing.
    return sizeof(Bin) - sizeof(BinHeader);
}

} // namespace EGG
