#ifndef EGG_POST_EFFECT_UNK2_H
#define EGG_POST_EFFECT_UNK2_H

#include "egg/egg_types.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "egg/math/eggVector.h"

namespace EGG {

class PostEffectUnk2 : public PostEffectBase {
public:
    PostEffectUnk2();
    virtual ~PostEffectUnk2();
    virtual void draw(f32 width, f32 height) override;
    virtual void setMaterialInternal() override;

private:
    /* 0x2C */ u32 field_0x2C;
    /* 0x30 */ u32 field_0x30;
    /* 0x34 */ u8 field_0x34;
    /* 0x35 */ u8 field_0x35;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ Vector2f field_0x38;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ EGG::CpuTexture *mpTexture;
};

} // namespace EGG

#endif
