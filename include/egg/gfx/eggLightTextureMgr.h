#ifndef EGG_LIGHT_TEXTURE_MGR_H
#define EGG_LIGHT_TEXTURE_MGR_H

#include "egg/gfx/eggLightTexture.h"
#include "egg/prim/eggBinary.h"
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

    int replaceModelTextures(nw4r::g3d::ResMdl) const;
    int replaceModelTexture(u16, nw4r::g3d::ResMdl) const;
    void drawAndCaptureTexture(f32, f32, f32, f32);
    void frameReset();

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

private:
    /* 0x04 */ u8 field_0x04;
    /* 0x06 */ u16 mTextureCount;
    /* 0x08 */ LightTexture **mpTextures;
    /* 0x0C */ const LightManager *mpLightMgr;
    /* 0x10 */ u32 field_0x10;
    /* 0x14 */ u16 mMaxNumTextures;
    /* 0x16 */ u8 field_0x16;
    /* 0x18 */ LightObject **mpObjects;
};

} // namespace EGG

#endif
