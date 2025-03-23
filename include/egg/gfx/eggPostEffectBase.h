#ifndef EGG_POST_EFFECT_BASE_H
#define EGG_POST_EFFECT_BASE_H

#include "common.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggScreen.h"
#include "egg/math/eggMath.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class PostEffectBase {
public:
    struct CapTextureWrapper {
        CapTexture *mpTex;
        GXTexMapID mTexMapID;
    };
    void setField_0x00(u32 val) {
        field_0x00 = val;
    }

    static void setProjection(const EGG::Screen &screen);

protected:
    u32 field_0x00;          // at 0x0
    CapTextureWrapper mTex1; // at 0x4
    CapTextureWrapper mTex2; // at 0xC
    f32 mOffsetX;            // at 0x14
    f32 mOffsetY;            // at 0x18
    f32 mScaleX;             // at 0x1C
    f32 mScaleY;             // at 0x20
    f32 mRotation;           // at 0x24

    PostEffectBase();
    virtual ~PostEffectBase() {} // at 0x8

    void fn_804AED20();

    void setVtxState();
    void setMatColorChannel();
    void setMatInd();
    void setMatPE();
    void setBlendModeInternal();
    static void scaleColor(GXColor &, const GXColor &, bool, f32);
    static void lerpColor(GXColor &, const GXColor &, const GXColor &, f32);
    void getBaseTexMtx(nw4r::math::MTX34 *mtx) const;

protected:
    virtual void draw(f32 width, f32 height);                                         // at 0xC
    virtual void setMaterialInternal() {}                                             // at 0x10
    virtual void loadTextureInternal();                                               // at 0x14
    virtual void drawScreenInternal(f32 offsetX, f32 offsetY, f32 width, f32 height); // at 0x18
};

} // namespace EGG

#endif
