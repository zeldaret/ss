#ifndef EGG_POST_EFFECT_MASK_DOF_H
#define EGG_POST_EFFECT_MASK_DOF_H

#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "egg/math/eggVector.h"
#include "rvl/GX/GXTypes.h"
namespace EGG {

class PostEffectMaskDOF : public PostEffectBase {
public:
    PostEffectMaskDOF();

    void setUpGradation();
    virtual void draw(f32 width, f32 height) override;
    virtual void setMaterialInternal() override;
    virtual ~PostEffectMaskDOF() {}

    const GXColor &getNearColor();
    const GXColor &getFarColor();
    const GXColor &getCenterColor();

private:
    /* 0x2C */ u8 field_0x2C;
    /* 0x30 */ CpuTexture *mpCpuTexArr[3];
    /* 0x3C */ CpuTexture *mpCpuTex;
    /* 0x40 */ u8 _40[4];
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4C;
    /* 0x50 */ Vector2f field_0x50;
    /* 0x58 */ u8 _58[4];
    /* 0x5C */ int mCpuTexArrIdx;
    /* 0x60 */ int field_0x60;
};

} // namespace EGG

#endif
