#include "egg/gfx/eggPostEffectBase.h"

#include "common.h"
#include "egg/gfx/eggCapTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggScreen.h"
#include "egg/math/eggVector.h"
#include "math.h"
#include "nw4r/math/math_triangular.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXBump.h"
#include "rvl/GX/GXDisplayList.h"
#include "rvl/GX/GXLight.h"
#include "rvl/GX/GXTransform.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

namespace EGG {

PostEffectBase::PostEffectBase()
    : field_0x00(0),
      mTex1((CapTextureWrapper){nullptr, GX_TEXMAP0}),
      mTex2((CapTextureWrapper){nullptr, GX_TEXMAP0}),
      mOffsetX(0.0f),
      mOffsetY(0.0f),
      mScaleX(1.0f),
      mScaleY(1.0f),
      mRotation(0.0f) {}

void PostEffectBase::draw(f32 width, f32 height) {
    setMaterialInternal();
    setVtxState();
    drawScreenInternal(mOffsetX, mOffsetY, mScaleX * width, mScaleY * height);
}

void PostEffectBase::fn_804AED20() {
    draw(mTex1.mpTex->getWidth(), mTex1.mpTex->getHeight());
}

void PostEffectBase::setVtxState() {
    DrawGX::SetVtxState(DrawGX::VTX_TYPE_11);
}

void PostEffectBase::drawScreenInternal(f32 offsetX, f32 offsetY, f32 width, f32 height) {
    nw4r::math::MTX34 mtx;
    PSMTXScale(mtx, width, height, 1.0f);
    mtx._03 = offsetX;
    mtx._13 = offsetY;
    mtx._23 = 0.0f;
    GXLoadPosMtxImm(mtx, 0);
    GXCallDisplayList(DrawGX::s_DL[DrawGX::DL_16].mpList, DrawGX::s_DL[DrawGX::DL_16].mLen);
}

void PostEffectBase::loadTextureInternal() {
    mTex1.mpTex->load(mTex1.mTexMapID);
    if (mTex2.mpTex != nullptr) {
        mTex2.mpTex->load(mTex2.mTexMapID);
    }
}

void PostEffectBase::setMatColorChannel() {
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_SIGN, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_SIGN, GX_AF_NONE);
    GXSetCullMode(GX_CULL_BACK);
}

void PostEffectBase::setMatInd() {
    GXSetNumIndStages(0);
}

void PostEffectBase::setMatPE() {
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(false, GX_ALWAYS, false);
    static const GXColor black = {0, 0, 0, 0xFF};
    GXSetFog(GX_FOG_NONE, black, 0.0, 1.0, 0.0, 1.0);
    setBlendModeInternal();
}

void PostEffectBase::setBlendModeInternal() {
    switch (field_0x00) {
        case 0:  DrawGX::SetBlendMode(DrawGX::BLEND_14); break;
        case 1:  DrawGX::SetBlendMode(DrawGX::BLEND_0); break;
        case 2:  DrawGX::SetBlendMode(DrawGX::BLEND_2); break;
        case 3:  DrawGX::SetBlendMode(DrawGX::BLEND_4); break;
        case 4:  GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTCLR, GX_BL_INVDSTCLR, GX_LO_CLEAR); break;
        case 5:  DrawGX::SetBlendMode(DrawGX::BLEND_5); break;
        case 6:  GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR); break;
        case 7:  DrawGX::SetBlendMode(DrawGX::BLEND_3); break;
        case 8:  GXSetBlendMode(GX_BM_BLEND, GX_BL_INVSRCALPHA, GX_BL_ONE, GX_LO_CLEAR); break;
        case 9:  GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR); break;
        case 10: DrawGX::SetBlendMode(DrawGX::BLEND_7); break;
        case 11: GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ZERO, GX_LO_CLEAR); break;
    }
}

void PostEffectBase::scaleColor(GXColor &out, const GXColor &c, bool bClamp, f32 amount) {
    f32 fs[4];
    fs[0] = c.r * amount;
    fs[1] = c.g * amount;
    fs[2] = c.b * amount;
    fs[3] = c.a * amount;

    if (bClamp) {
        out.r = fs[0] < 255.0f ? fs[0] : 255.0f;
        out.g = fs[1] < 255.0f ? fs[1] : 255.0f;
        out.b = fs[2] < 255.0f ? fs[2] : 255.0f;
        out.a = fs[3] < 255.0f ? fs[3] : 255.0f;
    } else {
        out.r = fs[0];
        out.g = fs[1];
        out.b = fs[2];
        out.a = fs[3];
    }
}

void PostEffectBase::lerpColor(GXColor &out, const GXColor &c1, const GXColor &c2, f32 amount) {
    out.r = c1.r + amount * (c2.r - c1.r);
    out.g = c1.g + amount * (c2.g - c1.g);
    out.b = c1.b + amount * (c2.b - c1.b);
    out.a = c1.a + amount * (c2.a - c1.a);
}

void PostEffectBase::getBaseTexMtx(nw4r::math::MTX34 *mtx) const {
    // Feels wrong -> maybe an inline with rotation and pi
    f32 rot = Math<f32>::pi();
    f32 sin, cos;
    nw4r::math::SinCosRad(&sin, &cos, rot * mRotation);

    f32 zero = 0.0f;
    f32 scale = 1.0f;
    f32 f8 = zero - 0.5f;

    mtx->_00 = scale * cos;
    mtx->_01 = scale * sin;
    mtx->_02 = ((f8 * cos) - (sin * (f8 + scale))) + 0.5f;

    mtx->_10 = -scale * sin;
    mtx->_11 = scale * cos;
    mtx->_12 = ((-f8 * sin) - (cos * (f8 + scale))) + 0.5f;

    mtx->_23 = zero;
    mtx->_21 = zero;
    mtx->_20 = zero;
    mtx->_13 = zero;
    mtx->_03 = zero;
    mtx->_22 = scale;
}

void PostEffectBase::setProjection(const Screen &screen) {
    static Vector3f sScale(1.0f, 1.0f, 1.0f);
    static Vector2f sOffset(0.0f, 0.0f);

    Screen screenCpy(screen);
    screenCpy.SetFlag(Screen::FLAG_0x40);
    screenCpy.SetCanvasMode(Screen::CANVASMODE_1);
    screenCpy.SetProjectionType(Screen::PROJ_ORTHO);
    screenCpy.SetNearFar(0.0f, 1.0f);
    screenCpy.SetScale(sScale);
    screenCpy.SetOffset(sOffset);
    screenCpy.SetProjectionGX();
}

} // namespace EGG
