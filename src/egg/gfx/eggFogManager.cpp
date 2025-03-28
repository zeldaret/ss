#include "egg/gfx/eggFogManager.h"

#include "common.h"
#include "nw4r/g3d/g3d_scnroot.h"


namespace EGG {

FogManager::FogManager(const u16 num) : mFlag(0), mCount(num), mCount2(num) {
    mpFog = new Fog[num];
}

FogManager::~FogManager() {
    delete[] mpFog;
}

void FogManager::ResetFog() {
    for (int i = 0; i < mCount; i++) {
        mpFog[i].Reset();
    }
}

void FogManager::Calc() {
    if (!(mFlag & 4)) {
        mFlag |= 4;
    }
}

void FogManager::CopyToG3D(nw4r::g3d::ScnRoot *root) const {
    for (int i = 0; i < mCount; i++) {
        mpFog[i].CopyToG3D(root->GetFog(i));
    }
}

void FogManager::SetBinaryInner(const Bin &bin) {
    if (bin.mHeader.mVersion == 0) {
        u32 loopMax = bin.mData.mCount >= mCount2 ? mCount2 : bin.mData.mCount;
        const IBinary<Fog>::Bin *pBinData = bin.mData.mFogData;
        for (u16 i = 0; i < loopMax; i++) {
            mpFog[i].SetBinary(&pBinData[i]);
        }
    }
}

void FogManager::SetBinaryInner(const Bin &bin1, const Bin &bin2, f32 blend) {
    if (bin1.mHeader.mVersion == 0) {
        u32 loopMax = bin1.mData.mCount >= mCount2 ? mCount2 : bin1.mData.mCount;
        const IBinary<Fog>::Bin *pBinData1 = bin1.mData.mFogData;
        const IBinary<Fog>::Bin *pBinData2 = bin2.mData.mFogData;
        for (u16 i = 0; i < loopMax; i++) {
            mpFog[i].SetBinaryBlend(&pBinData1[i], &pBinData2[i], blend);
        }
    }
}

void FogManager::GetBinaryInner(Bin *pOutBin) const {
    pOutBin->mData.mCount = mCount2;
    u16 max = mCount2;
    IBinary<Fog>::Bin *pOutData = pOutBin->mData.mFogData;
    for (u16 i = 0; i < max; i++) {
        mpFog[i].GetBinary(&pOutData[i]);
    }
}

size_t FogManager::GetBinarySize() const {
    return sizeof(Bin) + (mCount2 - 1) * sizeof(Fog::Bin);
}

} // namespace EGG
