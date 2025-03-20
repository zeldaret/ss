#ifndef EGG_POST_EFFECT_BASE_H
#define EGG_POST_EFFECT_BASE_H

#include "common.h"
#include "egg/egg_types.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

class PostEffectBase {
protected:
    u32 field_0x00;            // at 0x0
    CapTexture *mpCapTexture;  // at 0x4
    GXTexMapID mTexMapId;      // at 0x8
    CapTexture *mpCapTexture2; // at 0xC
    GXTexMapID mTexMapId2;     // at 0x10
    f32 mOffsetX;              // at 0x14
    f32 mOffsetY;              // at 0x18
    f32 mScaleX;               // at 0x1C
    f32 mScaleY;               // at 0x20
    f32 mRotation;             // at 0x24

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
    static void setProjection(const EGG::Screen &screen);

protected:
    virtual void draw(f32 width, f32 height);                                         // at 0xC
    virtual void setMaterialInternal() {}                                             // at 0x10
    virtual void loadTextureInternal();                                               // at 0x14
    virtual void drawScreenInternal(f32 offsetX, f32 offsetY, f32 width, f32 height); // at 0x18
};

} // namespace EGG

#endif
