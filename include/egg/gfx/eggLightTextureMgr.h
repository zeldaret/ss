#ifndef EGG_LIGHT_TEXTURE_MGR_H
#define EGG_LIGHT_TEXTURE_MGR_H


#include "egg/prim/eggBinary.h"
#include "nw4r/g3d/res/g3d_resmdl.h"

namespace EGG {

class LightTextureManager : public IBinary<LightTextureManager> {
public:
    struct BinData {};
    virtual ~LightTextureManager();
    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual size_t GetBinarySize() const override;

    void replaceModelTextures(nw4r::g3d::ResMdl) const;

    void drawAndCaptureTexture(f32, f32, f32, f32);

    u16 getMaxNumLightTextures() const {
        return mMaxNumTextures;
    }
private:
    /* 0x14 */ u16 mMaxNumTextures;
};

} // namespace EGG

#endif
