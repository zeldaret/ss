#ifndef EGG_LIGHT_TEXTURE_MGR_H
#define EGG_LIGHT_TEXTURE_MGR_H

#include "common.h"
#include "egg/gfx/eggLightManager.h"
#include "egg/gfx/eggLightTexture.h"
#include "egg/prim/eggBinary.h"
#include "nw4r/g3d/res/g3d_resmat.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

namespace EGG {

class LightManager;
class LightObject;

class LightTextureManager : public IBinary<LightTextureManager> {
public:
    struct BinData {
        u16 mDataCount;
        u8 _0x02[14];
        BinHeader mSubData[1];
    };
    LightTextureManager(const LightManager *lightMgr);
    virtual ~LightTextureManager();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;

    u16 createTexture(const char *name);
    bool setBinaryToTexture(const void *data);
    bool deleteTexture(int idx);

    u16 replaceModelTextures(nw4r::g3d::ResMdl) const;
    u16 replaceModelTexture(int, nw4r::g3d::ResMdl) const;
    void drawAndCaptureTexture(f32, f32, f32, f32);
    void frameReset();
    void correctLightObject();

    // Inofficial
    static const void *getLtexFromLmap(const void *lmap, u16 index);
    int createTexturesFromBin(const void *bin);
    u16 createTextureFromBin(const void *bin);

    u16 getMaxNumLightTextures() const {
        return mMaxNumTextures;
    }

    u16 getNumLightTextures() const {
        return mTextureCount;
    }

    LightTexture *getTextureByName(const char *name) const {
        int idx = getTextureIndex(name);
        if (idx != -1) {
            return mpTextures[idx];
        } else {
            return nullptr;
        }
    }

    const nw4r::g3d::AmbLightObj &GetAmbientObj(int idx) const {
        return mpLightMgr->GetAmbientObject(idx)->mLightObj;
    }

    const LightObject *GetLightObject(u16 i) const {
        return mpObjects[i];
    }

    const LightObject *GetLightObject(u16 i) {
        return mpObjects[i];
    }

private:
    static void fn_804AE340(nw4r::g3d::ResMat, GXTexCoordID);
    int getTextureIndex(const char *name) const;

    bool getSomeTfRelatedBool() const {
        return (mFlags >> 5) & 1;
    }

    /* 0x04 */ u8 mFlags;
    /* 0x06 */ u16 mTextureCount;
    /* 0x08 */ LightTexture **mpTextures;
    /* 0x0C */ const LightManager *mpLightMgr;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u16 mMaxNumTextures;
    /* 0x16 */ u8 field_0x16;
    /* 0x18 */ const LightObject **mpObjects;
};

} // namespace EGG

#endif
