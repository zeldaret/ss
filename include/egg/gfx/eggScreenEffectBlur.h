#ifndef EGG_SCREEN_EFFECT_BLUR_H
#define EGG_SCREEN_EFFECT_BLUR_H

#include "common.h"
#include "egg/egg_types.h"
#include "egg/gfx/eggScreenEffectBase.h"
#include "egg/prim/eggBinary.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class ScreenEffectBlur : public ScreenEffectBase, IBinary<ScreenEffectBlur> {
public:
    struct BinData {
        u8 field_0x00;
        u8 field_0x01;
        u8 field_0x02;
        f32 field_0x04;
        f32 field_0x08;
        f32 field_0x0C;
        f32 field_0x10;
        f32 field_0x14;
        f32 field_0x18;
        u8 field_0x1C;
        u8 field_0x1D;
        u8 field_0x1E;
        u8 field_0x1F;
    };

    ScreenEffectBlur();
    virtual ~ScreenEffectBlur();

    virtual void SetBinaryInner(const Bin &) override;
    virtual void GetBinaryInner(Bin *) const override;
    virtual void SetBinaryInner(const Bin &, const Bin &, f32) override;

    void fn_804B32B0();
    void fn_804B3710();

private:
    /* 0x0C */ u8 field_0x0C;
    /* 0x0D */ u8 field_0x0D;
    /* 0x0E */ u8 field_0x0E;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1C;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ GXColor field_0x28;
    /* 0x2C */ PostEffectUnk2 *mpUnk2;
    /* 0x30 */ PostEffectBlur *mpBlur;
    /* 0x34 */ TextureBuffer *mpCpuTexture;
};

} // namespace EGG

#endif
