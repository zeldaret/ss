#ifndef EGG_POST_EFFECT_BLUR_H
#define EGG_POST_EFFECT_BLUR_H

#include "common.h"
namespace EGG {

class PostEffectBlur {
public:
    PostEffectBlur();
    ~PostEffectBlur() {}

    void setField0x30(f32 val) {
        field_0x30 = val;
    }

private:
    u8 _0x00[0x30 - 0x00];
    f32 field_0x30;
    u8 _0x03[0x78 - 0x34];
};

} // namespace EGG

#endif
