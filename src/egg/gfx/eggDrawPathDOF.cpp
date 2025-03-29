#include "egg/gfx/eggDrawPathDOF.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGlobalDrawState.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "egg/gfx/eggPostEffectBlur.h"
#include "egg/gfx/eggPostEffectMask.h"
#include "egg/gfx/eggPostEffectMaskDOF.h"
#include "egg/gfx/eggPostEffectSimple.h"
#include "egg/gfx/eggScreen.h"
#include "egg/gfx/eggStateEfb.h"
#include "egg/gfx/eggStateGX.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

DrawPathDOF::DrawPathDOF()
    : field_0x1C(1),
      field_0x20(1),
      field_0x24(30.0f),
      field_0x28(20.0f),
      field_0x2C(20.0f),
      field_0x30(1.6f),
      field_0x34(0),
      field_0x38(2),
      field_0x5C(0.0f),
      field_0x60(0.0f),
      field_0x64(0.4f),
      field_0x68(0.2f),
      field_0x6C(-0.0075f),
      field_0x70(-0.0075f),
      field_0x74(10.0f),
      field_0x78(15.0f),
      field_0x7C(1.0f),
      field_0x80(21),
      mpBlur(nullptr),
      mpMask(nullptr),
      mpTextureBuffer(nullptr),
      mpMaskDof(nullptr) {
    mpMaskDof = new PostEffectMaskDOF();
    mpBlur = new PostEffectBlur();
    mpBlur->setField0x2C(1);
    mpMask = new PostEffectMask();

    mpMask->setField_0x00(0);
    mpMask->setField_0x34(0.0f);
    mpMask->setField_0x2C(0);
    field_0x1E = 0xFF;
    field_0x1F = 0xFF;
}

DrawPathDOF::~DrawPathDOF() {
    delete mpMaskDof;
    mpMaskDof = nullptr;
    delete mpBlur;
    mpBlur = nullptr;
    delete mpMask;
    mpMask = nullptr;
}

void DrawPathDOF::execute() {
    field_0x5C += field_0x6C * field_0x7C;
    field_0x60 += field_0x70 * field_0x7C;

    if (field_0x5C < -1.0f) {
        field_0x5C += 2.0f;
    } else if (field_0x5C > 1.0f) {
        // @bug typo?
        field_0x5C -= -2.0f;
    }

    if (field_0x60 < -1.0f) {
        field_0x60 += 2.0f;
    } else if (field_0x60 > 1.0f) {
        field_0x60 -= 2.0f;
    }
}

static int sMaskDofArray[] = {0, 1, 2};
static u8 sBlurArray[] = {4, 2, 1, 0};

void DrawPathDOF::internalResetForDraw() {
    // Tons of regswaps...
    f32 a = GlobalDrawState::getScreen().TranslateDepthToViewSpace(field_0x24);
    f32 b = GlobalDrawState::getScreen().TranslateDepthToViewSpace(field_0x24 - field_0x28 / 2.0f);
    f32 c = GlobalDrawState::getScreen().TranslateDepthToViewSpace(field_0x24 + field_0x28 / 2.0f);

    // TODO
    if (c < b + 1e-6f) {
        c = b + 1e-6f;

        if (b < 1.0f) {
            c = 1.0f;
            b -= 1e-6f;
        }
    }
    mpMaskDof->setParam0x48(b);
    mpMaskDof->setParam0x44(a);
    mpMaskDof->setParam0x4C(c);

    if (field_0x38 >= 3) {
        field_0x38 = 2;
    }

    mpMaskDof->setCpuTexArrIdx(sMaskDofArray[field_0x38]);
    mpBlur->setStage0Field0(sBlurArray[field_0x34]);
    mpBlur->setStage0F(field_0x34 != 2 ? field_0x30 : 0.0f);
    // HELP
    mpBlur->setStage0ColorThing(field_0x1E);
}

void DrawPathDOF::internalDraw(u16 idx) {
    if (idx == 0) {
        if (StateEfb::CheckFlag0x2() == 0) {
            StateEfb::fn_804B4270(
                static_cast<StateEfb::BufferType>((field_0x1C >> 2) & 1), reinterpret_cast<u32>(this)
            );
        }
        StateGX::invalidateTexAllGX();
    }

    if (field_0x20 == 2) {
        internalDrawVariant2(idx);
    } else {
        internalDrawVariant01(idx);
    }

    switch (idx) {
        case 1:
            StateEfb::fn_804B4310(StateEfb::BUFFER_0, reinterpret_cast<u32>(this));
            StateEfb::releaseEfb(StateEfb::BUFFER_3, reinterpret_cast<u32>(this));
            break;
        case 2:
            if (mpTextureBuffer != nullptr) {
                mpTextureBuffer->free();
                mpTextureBuffer = nullptr;
            }
    }
}

void DrawPathDOF::internalDrawVariant01(u16 idx) {}

void DrawPathDOF::internalDrawVariant2(u16 idx) {
    const Screen &screen = GlobalDrawState::getScreen();
    const Screen::DataEfb &efb = screen.GetDataEfb();
    switch (idx) {
        case 0: {
            TextureBuffer *capturedBuf = StateEfb::captureEfb(StateEfb::BUFFER_3, false, reinterpret_cast<u32>(this));
            StateGX::GXSetPixelFmt_(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
            StateGX::ScopedColor colorGuard(true);
            StateGX::ScopedAlpha alphaGuard(false);
            StateGX::ScopedDither ditherGuard(false);
            PostEffectBase::setProjection(screen);
            f32 *floats = StateEfb::fn_804B4550();
            f32 scale = (field_0x1C & 4) != 0 ? 0.25f : 0.5f;
            f32 width1 = screen.GetWidth() * scale;
            f32 height1 = screen.GetHeight() * scale;

            f32 rot = 0.5f;

            u16 width = efb.vp.x2 * scale;
            u16 height = efb.vp.y2 * scale;

            PostEffectSimple effect;
            effect.setCapTexMapId(GX_TEXMAP0);
            effect.setField0x38(0);
            effect.setField_0x00(0);
            effect.setRot(rot);

            effect.setCapTexture(capturedBuf);
            // TODO tons of problems in these UnscaleByX inlines that probably exist
            effect.draw(screen.UnscaleByX(height), screen.UnscaleByY(width));
            TextureBuffer *pTex2 = TextureBuffer::alloc(height, width, GX_TF_RGBA8);
            pTex2->fn_8049D9D0(field_0x80);
            pTex2->capture(floats[0], floats[1], false, -1);
            mpBlur->setCapTexture(pTex2);
            mpBlur->setCapTexMapId(GX_TEXMAP0);
            mpBlur->setRot(-rot);
            mpBlur->draw(width1, height1);

            if (pTex2 != nullptr) {
                pTex2->free();
            }
            afterDraw(floats[0], floats[1], width, height);
            StateGX::GXSetPixelFmt_(StateGX::s_pixFormat, StateGX::s_zFmt16);
            break;
        }
        case 2: {
            StateGX::ScopedColor colorGuard(true);
            StateGX::ScopedAlpha alphaGuard(false);
            StateGX::ScopedDither ditherGuard(true);

            Screen screenCpy(screen);
            screenCpy.SetOffset(nw4r::math::VEC2(0.0f, 0.0f));
            screenCpy.SetScale(nw4r::math::VEC3(1.0f, 1.0f, 1.0f));
            screenCpy.SetFlag(Screen::FLAG_0x40);
            screenCpy.SetProjectionGX();
            DrawGX::SetMat_ColorChannel(DrawGX::COLORCHAN_1);
            DrawGX::SetMat_TexGen(DrawGX::TEXGEN_1);
            DrawGX::SetMat_Ind();
            DrawGX::SetMat_Tev(GX_TEVSTAGE0, DrawGX::TEV_2);
            GXSetTevAlphaIn(GX_TEVSTAGE0,GX_CA_ZERO,GX_CA_ZERO,GX_CA_ZERO,GX_CA_RASA);
            DrawGX::SetMat_PE(DrawGX::ZMODE_2, DrawGX::BLEND_0);
            DrawGX::SetVtxState(DrawGX::VTX_TYPE_7);
            mpTextureBuffer->load(GX_TEXMAP0);

            for (int i = 0; i < 2; i++) {
                // TODO
            }

            break;
        }
    }
}

void DrawPathDOF::SetBinaryInner(const Bin &bin) {}

void DrawPathDOF::GetBinaryInner(Bin *pBin) const {}

void DrawPathDOF::SetBinaryInner(const Bin &, const Bin &, f32) {}

} // namespace EGG
