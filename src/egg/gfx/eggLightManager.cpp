#include "egg/gfx/eggLightManager.h"

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggLightObject.h"
#include "egg/gfx/eggLightTextureMgr.h"
#include "rvl/GX/GXLight.h"

namespace EGG {

LightManager::LightManager(u32 p1, u32 p2, u8 p3) {
    Counts c;
    c.mNumEggLightObjects = p1;
    c.mNumUnks = p2;

    mCounts0x04 = c;
    mCounts0x06 = c;
    mpLightData = nullptr;
    mpUnk1 = nullptr;
    mpTextureMgr = nullptr;

    field_0x1C = p3;
    mFlags = 0x40;
    field_0x20 = 0;
    field_0x24 = -1;

    mpTextureMgr = new LightTextureManager(this);
    u16 numGxObjs = mCounts0x04.mNumEggLightObjects < 8 ? mCounts0x04.mNumEggLightObjects : 8;
    mpLightObjs = new GXLightObj[field_0x1C * numGxObjs]();
    mpLightData = new LightData[mCounts0x04.mNumEggLightObjects]();
    mpUnk1 = new Unk1[mCounts0x04.mNumUnks]();

    for (int i = 0; i < mCounts0x04.mNumEggLightObjects; i++) {
        GetLightObject(i)->SetIndex(i);
        GetLightObject(i)->ClearField0xA0();
        BecomeInvalidLight(i);
    }

    for (u16 i = 0; i < (u32)mCounts0x04.mNumUnks; i++) {
        mpUnk1[i].field_0x08 = 0;
    }

    Reset();
}

LightManager::~LightManager() {
    delete mpTextureMgr;
    delete[] mpLightObjs;
    delete[] mpLightData;
    delete[] mpUnk1;
}

static const char *resetPattern = "--";

void LightManager::Reset() {
    for (int i = 0; i < mCounts0x04.mNumEggLightObjects; i++) {
        LightObject *obj = GetLightObject(i);
        obj->Reset();
        if (i > 0) {
            obj->ClearFlag4();
        }
    }

    for (u16 i = 0; i < (u32)mCounts0x04.mNumUnks; i++) {
        mpUnk1[i].mColor = (GXColor){0x64, 0x64, 0x64, 0xFF};
        mpUnk1[i].mStr = resetPattern;
    }
    mColor = DrawGX::BLACK;
    field_0x1D = 0;
    mFlags = mFlags & 0xFFE0;
}

} // namespace EGG
