#ifndef EGG_POST_EFFECT_BLUR_H
#define EGG_POST_EFFECT_BLUR_H

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class PostEffectBlur : public PostEffectBase {
    struct Stage {
        /* 0x00 */ u8 field_0x00;
        /* 0x01 */ GXColor field_0x01;
        /* 0x08 */ f32 field_0x08;
        /* 0x0C */ f32 field_0x0C;

        void init(const GXColor &clr) {
            field_0x00 = 16;
            field_0x01 = clr;
            field_0x08 = 0.0f;
            field_0x0C = 1.0f;
        }
    };

public:
    PostEffectBlur();
    virtual ~PostEffectBlur() {}

    virtual void draw(f32 width, f32 height) override; // at 0xC

    void setField0x30(f32 val) {
        field_0x30 = val;
    }

    void setField0x2C(int i) {
        field_0x2C = i;
    }

    void setStage0Field0(u8 u1) {
        field_0x38[0].field_0x00 = u1;
    }

    void setStage0Color(GXColor clr) {
        field_0x38[0].field_0x01 = clr;
    }

    void setStage0F(f32 f) {
        field_0x38[0].field_0x0C = f;
    }

    void setStage0ColorThing(u8 i) {
        f32 color = (255.0f / field_0x38[0].field_0x00) + 0.5f;
        f32 f = (i * 1.75f) / field_0x38[0].field_0x00;
        f32 alpha = f <= 255.0f ? f : 255.0f;
        field_0x38[0].field_0x01 = (GXColor){color, color, color, alpha};
    }

private:
    void drawInternal(u8, u8, f32, f32);

    /* 0x2C */ u8 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ Stage field_0x38[4];
};

} // namespace EGG

#endif
