#include "egg/gfx/eggPostEffectBase.h"

#include "common.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

PostEffectBase::PostEffectBase() {
    field_0x00 = 0;
    mpCapTexture = nullptr;
    mTexMapId = GX_TEXMAP0;
    mpCapTexture2 = nullptr;
    mTexMapId2 = GX_TEXMAP0;
    mOffsetX = 0.0f;
    mOffsetY = 0.0f;
    mScaleX = 1.0f;
    mScaleY = 1.0f;
    field_0x24 = 0.0f;
}

void PostEffectBase::loadTextureInternal() {
    mpCapTexture;
}

} // namespace EGG
