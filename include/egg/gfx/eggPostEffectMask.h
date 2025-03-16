#ifndef EGG_POST_EFFECT_MASK_H
#define EGG_POST_EFFECT_MASK_H

#include "egg/gfx/eggPostEffectBase.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class PostEffectMask : public PostEffectBase {
public:
    PostEffectMask();

    virtual void setMaterialInternal() override;

    GXTevScale GetTevScale() const {
        return static_cast<GXTevScale>(field_0x1BC);
    }

private:
    /* 0x2C */ int field_0x2C;
    /* 0x30 */ GXColor mColor;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ u8 _0x3C[0x180];
    /* 0x1BC */ u8 field_0x1BC;
};

} // namespace EGG

#endif
