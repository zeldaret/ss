#ifndef EGG_POST_EFFECT_BLUR_GATHER_H
#define EGG_POST_EFFECT_BLUR_GATHER_H

#include "common.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class PostEffectBlurGather : public PostEffectBase {
    // Size 0x18
    struct BlurGatherData {
        void reset() {
            mpCapTexture = nullptr;
            mColor = DrawGX::WHITE;
            mColorScale = 1.f;
            field_0x10 = 0;
        }
        /* 0x00 */ EGG::CpuTexture *mpCapTexture;
        /* 0x04 */ GXTexMapID mTexMapId;
        /* 0x08 */ GXColor mColor;
        /* 0x0C */ f32 mColorScale;
        /* 0x10 */ u8 field_0x10;
    };

public:
    PostEffectBlurGather();
    virtual void setMaterialInternal() override;

    /* 0x2C */ u8 field_0x2C;
    /* 0x2D */ u8 field_0x2D;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ BlurGatherData mBlurGatherData[3];
};

} // namespace EGG

#endif
