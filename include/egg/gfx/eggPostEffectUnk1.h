#ifndef EGG_POST_EFFECT_UNK1_H
#define EGG_POST_EFFECT_UNK1_H

#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

class PostEffectUnk1 : public PostEffectBase {
    // Size 0x18
    struct Kernel {
        /* 0x00 */ EGG::CpuTexture *field_0x00;
        /* 0x04 */ GXTexMapID mTexMapId;
        /* 0x08 */ GXColor field_0x08;
        /* 0x0C */ f32 field_0x0C;
        /* 0x10 */ u8 field_0x10;
    };
public:
    PostEffectUnk1();
    virtual void setMaterialInternal() override;


    /* 0x2C */ u8 field_0x2C;
    /* 0x2D */ u8 field_0x2D;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ Kernel field_0x34[3];
};

} // namespace EGG

#endif
