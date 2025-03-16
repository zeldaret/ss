#include "egg/gfx/eggLightTextureMgr.h"

#include "common.h"
#include "egg/gfx/eggLightTexture.h"
#include "egg/gfx/eggStateGX.h"

#include <cstring>

namespace EGG {

LightTextureManager::LightTextureManager(const LightManager *lightMgr) {
    field_0x04 = 0x78;
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

    mpObjects = new LightObject *[mMaxNumTextures];
    for (int i = 0; i < mMaxNumTextures; i++) {
        mpObjects[i] = nullptr;
    }

    if ((StateGX::s_flag & 0x40) != 0) {
        field_0x04 |= 0x80;
        field_0x04 &= ~0x40;
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

size_t LightTextureManager::GetBinarySize() const {
    // Contrary to the other dynamically-sized Bins, this only
    // returns the fixed size. GetBinaryInner will add the dynamic
    // parts when writing.
    return sizeof(Bin) - sizeof(BinHeader);
}

} // namespace EGG
