#ifndef EGG_POST_EFFECT_SIMPLE_H
#define EGG_POST_EFFECT_SIMPLE_H

#include "egg/gfx/eggPostEffectBase.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

class PostEffectSimple : public PostEffectBase {
public:
    PostEffectSimple();
    virtual void setMaterialInternal() override;

private:
    /* 0x2C */ GXColor field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ GXTevScale field_0x34;
    /* 0x38 */ s32 field_0x38;
};

} // namespace EGG

#endif
