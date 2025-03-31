#include "egg/gfx/eggLightManager.h"

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggG3DUtility.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "nw4r/g3d/g3d_light.h"
#include "nw4r/g3d/g3d_scnroot.h"
#include "nw4r/g3d/g3d_state.h"
#include "nw4r/g3d/res/g3d_resanmamblight.h"
#include "nw4r/g3d/res/g3d_resanmscn.h"
#include "nw4r/g3d/res/g3d_reslightset.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"
#include "rvl/OS/OSCache.h"

namespace EGG {

LightManager::LightManager(u32 numEggObjects, u32 numAmbientObject, u8 numGxObjsPerEggObj) {
    Counts c;
    c.mNumEggLightObjects = numEggObjects;
    c.mNumAmbientObjects = numAmbientObject;

    mCounts = c;
    mSavedCounts = c;
    mpLightData = nullptr;
    mpAmbientObjects = nullptr;
    mpTextureMgr = nullptr;

    mNumGxObjsPerEggObj = numGxObjsPerEggObj;
    mFlags = 0x40;
    mResAnmScn = nw4r::g3d::ResAnmScn(nullptr);
    mRefNumber = -1;

    mpTextureMgr = new LightTextureManager(this);
    mpLightObjs = new GXLightObj[mNumGxObjsPerEggObj * CheckedNumLightObjs(mCounts.mNumEggLightObjects)]();
    mpLightData = new LightData[mCounts.mNumEggLightObjects]();
    mpAmbientObjects = new AmbientObject[mCounts.mNumAmbientObjects]();

    for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
        GetLightObject(i)->SetIndex(i);
        GetLightObject(i)->ClearField0xA0();
        BecomeInvalidLight(i);
    }

    for (u16 i = 0; i < (u32)mCounts.mNumAmbientObjects; i++) {
        mpAmbientObjects[i].field_0x08 = 0;
    }

    Reset();
}

LightManager::~LightManager() {
    delete mpTextureMgr;
    delete[] mpLightObjs;
    delete[] mpLightData;
    delete[] mpAmbientObjects;
}

static const char *ambientLightPlaceholderName = "--";

void LightManager::Reset() {
    for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
        LightObject *obj = GetLightObject(i);
        obj->Reset();
        if (i > 0) {
            obj->ClearFlag2();
        }
    }

    for (u16 i = 0; i < (u32)mCounts.mNumAmbientObjects; i++) {
        mpAmbientObjects[i].mLightObj = (nw4r::g3d::AmbLightObj){0x64, 0x64, 0x64, 0xFF};
        mpAmbientObjects[i].mLightName = ambientLightPlaceholderName;
    }
    mColor = DrawGX::BLACK;
    field_0x1D = 0;
    mFlags = mFlags & 0xFFE0;
}

void LightManager::Calc(nw4r::g3d::ScnRoot *root) {
    if ((mFlags & 4) == 0) {
        for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
            LightObject *obj = GetLightObject(i);
            if (obj->CheckFlag2()) {
                u16 u = obj->GetField0x06();
                if (i != u) {
                    obj->UpdatePosAt(*GetLightObject(u));
                }
            }
            obj->Calc();
        }
        mFlags |= 4;
        mpTextureMgr->frameReset();
    }

    if (root != nullptr) {
        CopyToG3D_World(root);
    }
}

void LightManager::CalcView(const nw4r::math::MTX34 &viewMtx, u8 lightObjOffset, nw4r::g3d::ScnRoot *root) {
    // NONMATCHING - regswaps
    field_0x1D = lightObjOffset;
    mFlags |= 8;

    GXLightObj *gxObj = mpLightObjs + lightObjOffset * CheckedNumLightObjs(mCounts.mNumEggLightObjects);
    for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
        GetLightObject(i)->CalcView(viewMtx);
        if (i < CheckedNumLightObjs(mCounts.mNumEggLightObjects)) {
            GetLightObject(i)->InitGX(&gxObj[i]);
        }
    }
    DCFlushRange(gxObj, CheckedNumLightObjs(mCounts.mNumEggLightObjects) * sizeof(GXLightObj));
    if (root != nullptr) {
        nw4r::math::MTX34 tmp;
        PSMTXInverse(viewMtx, tmp);
        CopyToG3D_View(root, tmp);
    }
}

void LightManager::LoadLightSet(
    int id, u32 *pDiffColorMask, u32 *pDiffAlphaMask, u32 *pSpecColorMask, u32 *pSpecAlphaMask, GXColor *pColor
) {
    nw4r::g3d::AmbLightObj o;
    nw4r::g3d::G3DState::LoadLightSet(id, pDiffColorMask, pDiffAlphaMask, pSpecColorMask, pSpecAlphaMask, &o);
    if (pColor != nullptr) {
        pColor->r = o.r;
        pColor->g = o.g;
        pColor->b = o.b;
        pColor->a = o.a;
    }
}

void LightManager::DoneDraw() {
    if (((mFlags & 8) != 0) && ((mFlags & 4) != 0)) {
        mFlags = mFlags & 0xffef;
    }
    mFlags = mFlags & 0xfffb;
    mFlags = mFlags & 0xfff7;
}

void LightManager::LoadScnLightInner(nw4r::g3d::ResAnmScn scn, f32 frame, s16 refNumber, u32 u2) {
    // NONMATCHING - small problems around lightSet.GetLightID
    if (!scn.IsValid()) {
        return;
    }

    if ((u2 & 2) != 0) {
        mFlags |= 0x10;
        mResAnmScn = scn;
        mRefNumber = refNumber;
    }

    if ((u2 & 1) == 0) {
        return;
    }

    for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
        BecomeInvalidLight(i);
    }

    nw4r::g3d::ResLightSet lightSet = scn.GetResLightSetByRefNumber(refNumber);
    s32 lightId = -1;
    if (lightSet.IsValid()) {
        lightId = lightSet.GetAmbLightID();
    }
    u32 loopMax = mCounts.mNumAmbientObjects;
    if (scn.GetResAnmAmbLightMaxRefNumber() < loopMax) {
        loopMax = scn.GetResAnmAmbLightMaxRefNumber();
    }
    for (u32 i = 0; i < loopMax; i++) {
        AmbientObject *ambient = &mpAmbientObjects[i];
        ambient->field_0x08 = ambient->field_0x08 & 0xFE;
        if (lightId == -1 || i == lightId) {
            nw4r::g3d::ResAnmAmbLight light = scn.GetResAnmAmbLightByRefNumber(i);
            AnmScnRes_GetAmbLightColor(ambient, light, frame);
            ambient->field_0x08 |= 1;
        }
    }

    u32 numObjs = mCounts.mNumEggLightObjects;
    u32 numAmb = scn.GetResAnmLightMaxRefNumber();
    u32 numTotal = numAmb + scn.GetSpecularLightNumber();
    u32 numLight = numAmb <= numObjs ? numAmb : numObjs;
    u32 maxIdx = numTotal <= numObjs ? numTotal : numObjs;

    if (lightSet.IsValid()) {
        numLight = lightSet.GetNumLight();
    }

    for (u32 i = 0; i < numLight; i++) {
        u32 idx = lightSet.IsValid() ? lightSet.GetLightID(i) : i;
        GetLightObject(idx)->ClearFlag2();
    }

    for (u32 i = 0; i < numLight; i++) {
        u32 idx = lightSet.IsValid() ? lightSet.GetLightID(i) : i;
        nw4r::g3d::ResAnmLight light = scn.GetResAnmLightByRefNumber(idx);
        if (light.IsValid()) {
            LightObject *obj = nullptr;
            LightData *dat = &mpLightData[idx];
            if (!GetLightObject(idx)->CheckFlag1()) {
                if (light.HasSpecularLight() && light.GetSpecularLightIdx() < maxIdx) {
                    obj = GetLightObject(light.GetSpecularLightIdx());
                }
                dat->mLightObject.CopyFromG3D(light, frame, obj, false);
                mpLightData[idx].mLightObject.SetOtherFlag1();
            }
        }
    }
}

void LightManager::CopyToG3D_Ambient(nw4r::g3d::ScnRoot *root) const {
    // NONMATCHING - word copy
    nw4r::g3d::LightSetting &setting = root->GetLightSetting();
    for (int i = 0; i < mCounts.mNumAmbientObjects; i++) {
        setting.GetAmbLightObjArray()[i] = mpAmbientObjects[i].mLightObj;
    }
}

void LightManager::CopyToG3D_World(nw4r::g3d::ScnRoot *root) const {
    // NONMATCHING - loop registers, variables
    nw4r::g3d::LightSetting &setting = root->GetLightSetting();
    if ((mFlags & 0x10) != 0) {
        G3DUtility::setUpLightSet(setting, mResAnmScn, mRefNumber);
    }
    CopyToG3D_Ambient(root);
    for (int i = 0; i < mCounts.mNumAmbientObjects; i++) {
        GetLightObject(i)->CopyToG3D_World(setting.GetLightObjArray()[i]);
    }
}

void LightManager::CopyToG3D_View(nw4r::g3d::ScnRoot *root, const nw4r::math::MTX34 &viewMtx) const {
    nw4r::g3d::LightSetting &setting = root->GetLightSetting();
    // NONMATCHING - loop registers, variables
    for (int i = 0; i < mCounts.mNumEggLightObjects; i++) {
        GetLightObject(i)->CopyToG3D_View(setting.GetLightObjArray()[i], viewMtx);
    }
}

void LightManager::AnmScnRes_GetAmbLightColor(
    EGG::LightManager::AmbientObject *obj, nw4r::g3d::ResAnmAmbLight light, f32 frame
) const {
    nw4r::g3d::AmbLightAnmResult res;
    if (!light.IsValid()) {
        res.flags = 0;
        res.color = -1;
    } else {
        light.GetAnmResult(&res, frame);
        obj->mLightName = light.GetName();
    }

    if ((res.flags & nw4r::g3d::AmbLightAnmResult::FLAG_COLOR_ENABLE) == 0) {
        res.color |= 0xFFFFFF00;
    }

    if ((res.flags & nw4r::g3d::AmbLightAnmResult::FLAG_ALPHA_ENABLE) == 0) {
        res.color |= 0x000000FF;
    }

    u32 color = res.color;

    obj->mLightObj.r = (color >> 24) & 0xFF;
    obj->mLightObj.g = (color >> 16) & 0xFF;
    obj->mLightObj.b = (color >> 8) & 0xFF;
    obj->mLightObj.a = (color >> 0) & 0xFF;
}

void LightManager::SetBinaryInner(const Bin &) {
    // TODO
}

void LightManager::GetBinaryInner(Bin *bin) const {
    // NONMATCHING - regswaps, loop increments
    u16 numObjs = mSavedCounts.mNumEggLightObjects;
    u16 numAmbient = mSavedCounts.mNumAmbientObjects;
    bin->mData.mNumLightObjects = numObjs;
    bin->mData.mNumAmbientObjects = mSavedCounts.mNumAmbientObjects;

    LightObject::Bin *pObj = reinterpret_cast<LightObject::Bin *>(&bin->mData.mObjData);
    for (int i = 0; i < numObjs; i++) {
        GetLightObject(i)->GetBinary(&pObj[i]);
    }

    BinAmbient *pAmp = reinterpret_cast<BinAmbient *>(&pObj[numObjs]);
    for (u16 i = 0; i < numAmbient; i++) {
        AmbientObject &o = mpAmbientObjects[i];
        pAmp[i].field_0x00 = o.mLightObj.r;
        pAmp[i].field_0x01 = o.mLightObj.g;
        pAmp[i].field_0x02 = o.mLightObj.b;
        pAmp[i].field_0x03 = o.mLightObj.a;
        pAmp[i].field_0x07 = 0;
        pAmp[i].field_0x06 = 0;
        pAmp[i].field_0x05 = 0;
        pAmp[i].field_0x04 = 0;
    }

    bin->mData.mColor = mColor;
}

void LightManager::SetBinaryInner(const Bin &, const Bin &, f32) {
    // TODO
}

void LightManager::BecomeInvalidLight(int idx) {
    mpLightData[idx].mLightObject.ClearOtherFlag1();
}

size_t LightManager::GetBinarySize() const {
    return sizeof(Bin) + sizeof(LightObject::Bin) * (mSavedCounts.mNumEggLightObjects - 1) +
           sizeof(BinAmbient) * mSavedCounts.mNumAmbientObjects;
}

} // namespace EGG
