#ifndef EGG_POST_EFFECT_BLUR_H
#define EGG_POST_EFFECT_BLUR_H

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggPostEffectBase.h"

namespace EGG {

class PostEffectBlur : PostEffectBase {
    struct Kernel {
        /* 0x00 */ u8 field_0x00;
        /* 0x01 */ GXColor field_0x01;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0C;

        void init(GXColor clr) {
            field_0x00 = 16;
            field_0x01 = clr;
            field_0x08 = 1.0f;
            field_0x0C = 0.0f;
        }
    };

public:
    PostEffectBlur();
    virtual ~PostEffectBlur() {}

    virtual void draw(f32 width, f32 height) override; // at 0xC

    void setField0x30(f32 val) {
        field_0x30 = val;
    }

private:
    void drawInternal(u8, u8, f32, f32);

    /* 0x2C */ u8 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ Kernel field_0x38[4];
};

} // namespace EGG

#endif
