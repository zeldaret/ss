#include "egg/gfx/eggScreenEffectBlur.h"

#include "common.h"
#include "egg/gfx/eggCpuTexture.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "egg/gfx/eggPostEffectBlur.h" // IWYU pragma: keep
#include "egg/gfx/eggPostEffectUnk2.h" // IWYU pragma: keep
#include "egg/gfx/eggStateGX.h"
#include "egg/gfx/eggTextureBuffer.h"
#include "nw4r/math/math_types.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTypes.h"
#include "rvl/MTX/mtx.h"

#include <cstddef>

namespace EGG {

static const GXColor sDefaultColor = {0, 0, 0, 0xFF};
static const GXColor sDefaultColor2 = {0x80, 0x80, 0x80, 0x80};

ScreenEffectBlur::ScreenEffectBlur()
    : field_0x0C(3),
      field_0x0D(0),
      field_0x0E(0),
      field_0x10(1.0f),
      field_0x14(1.0f),
      field_0x18(0.5f),
      field_0x1C(1.0f),
      field_0x20(0.5f),
      field_0x24(0.0f),
      field_0x28(sDefaultColor),
      mpCpuTexture(0) {
    mpUnk2 = new PostEffectUnk2();
    mpBlur = new PostEffectBlur();
    mpBlur->setField_0x00(2);
    mpBlur->setStage0Field0(4);
    mpBlur->setStage0Color(sDefaultColor2);
    mpBlur->setStage0F(field_0x1C);
}

ScreenEffectBlur::~ScreenEffectBlur() {
    delete mpUnk2;
    delete mpBlur;
}
static const CopyFilter copyFilter = {
    {0x15, 0, 0, 0x16, 0, 0x15, 0}
};
void ScreenEffectBlur::fn_804B32B0() {
    if (mFlag & 1) {
        StateGX::invalidateTexAllGX();
        PostEffectBase::setProjection(GetScreen());
        const Screen::DataEfb &efb = GetScreen().GetDataEfb();
        u16 x1 = efb.vp.x1;
        u16 y1 = efb.vp.y1;
        f32 sizeX = (u16)sScreen.GetSize().x * field_0x10;
        f32 sizeY = (u16)sScreen.GetSize().y * field_0x10;
        u16 x2 = efb.vp.x2 * field_0x10;
        u16 y2 = efb.vp.y2 * field_0x10;

        StateGX::ScopedColor _clr(true);
        StateGX::ScopedAlpha _alpha(false);
        StateGX::ScopedDither _dither(false);
        TextureBuffer *pTex0 = NULL;
        if (field_0x0C & 1) {
            pTex0 = TextureBuffer::alloc(x2, y2, GX_TF_RGBA8);
            pTex0->capture(x1, y1, 0, GX_TF_RGBA8);
        }
        TextureBuffer *pTex1 = TextureBuffer::alloc(x2, y2, GX_TF_IA8);
        pTex1->setMinFilt(GX_NEAR);
        pTex1->setMagFilt(GX_NEAR);
        pTex1->capture(x1, y1, false, GX_TF_Z16);

        // . . .
        if (field_0x0C & 2) {
            TextureBuffer *pTex2 = TextureBuffer::alloc(x2, y2, GX_TF_RGBA8);
            pTex2->onCapFlag(0x8);
            pTex2->setCopyFilter(copyFilter);
            pTex2->capture(x1, y1, false, GX_CTF_R8);
            u8 val = field_0x18 * 255.f;
            GXColor clr = {val, val, val, val};
            mpBlur->setStage0Color(clr);
            mpBlur->setStage0F(field_0x1C);
            mpBlur->mTex1 = (PostEffectBase::CapTextureWrapper){pTex2, GX_TEXMAP1};
            mpBlur->draw(sizeX * 1.0f, sizeY * 1.0f);
            pTex2->free();
        }
        // . . .

        mpCpuTexture = TextureBuffer::alloc(x2, y2, GX_TF_I8);
        mpCpuTexture->onCapFlag(0x8);
        mpCpuTexture->setCopyFilter(copyFilter);
        mpCpuTexture->capture(x1, y1, false, GX_CTF_R8);
        pTex1->free();

        if (field_0x0C & 1) {
            nw4r::math::MTX34 m;
            PSMTXScale(m, sizeY, sizeY, 1.f);
            pTex0->load(GX_TEXMAP0);
            DrawGX::BeginDrawScreen(true, true, false);
            DrawGX::SetBlendMode(DrawGX::BLEND_14);
            DrawGX::DrawDL16(m, DrawGX::WHITE);
            pTex0->free();
        }
    }
}

void ScreenEffectBlur::fn_804B3710() {
    if ((mFlag & 1) != 0) {
        StateGX::invalidateTexAllGX();
        PostEffectBase::setProjection(sScreen);
        nw4r::math::MTX34 mtx;
        // Have to perform a float -> u16 -> float round trip here...
        u16 sizeY = sScreen.GetSize().y;
        u16 sizeX = sScreen.GetSize().x;
        PSMTXScale(mtx, sizeX, sizeY, 0.0f);
        mpCpuTexture->load(GX_TEXMAP0);
        DrawGX::BeginDrawScreen(true, true, true);
        GXSetTevKColor(GX_KCOLOR0, field_0x28);
        GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_KONST);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_KONST, GX_CA_TEXA, GX_CA_ZERO);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_GREATER, field_0x0D);
        DrawGX::SetBlendMode((field_0x0C & 4) != 0 ? DrawGX::BLEND_14 : DrawGX::BLEND_0);
        DrawGX::DrawDL(DrawGX::DL_16, mtx, DrawGX::WHITE);
    }
    if (mpCpuTexture != nullptr) {
        mpCpuTexture->free();
        mpCpuTexture = nullptr;
    }
}

void ScreenEffectBlur::SetBinaryInner(const Bin &bin) {
    if (bin.mHeader.mVersion != 0) {
        return;
    }
    field_0x0C = bin.mData.field_0x00;
    field_0x0D = bin.mData.field_0x01;
    field_0x0E = bin.mData.field_0x02;
    field_0x10 = bin.mData.field_0x04;
    field_0x14 = bin.mData.field_0x08;
    field_0x18 = bin.mData.field_0x0C;
    field_0x1C = bin.mData.field_0x10;
    field_0x20 = bin.mData.field_0x14;
    field_0x24 = bin.mData.field_0x18;
    field_0x28.r = bin.mData.field_0x1C;
    field_0x28.g = bin.mData.field_0x1D;
    field_0x28.b = bin.mData.field_0x1E;
    field_0x28.a = bin.mData.field_0x1F;
}

void ScreenEffectBlur::GetBinaryInner(Bin *bin) const {
    bin->mData.field_0x00 = field_0x0C;
    bin->mData.field_0x01 = field_0x0D;
    bin->mData.field_0x02 = field_0x0E;
    bin->mData.field_0x04 = field_0x10;
    bin->mData.field_0x08 = field_0x14;
    bin->mData.field_0x0C = field_0x18;
    bin->mData.field_0x10 = field_0x1C;
    bin->mData.field_0x14 = field_0x20;
    bin->mData.field_0x18 = field_0x24;
    bin->mData.field_0x1C = field_0x28.r;
    bin->mData.field_0x1D = field_0x28.g;
    bin->mData.field_0x1E = field_0x28.b;
    bin->mData.field_0x1F = field_0x28.a;
}

void ScreenEffectBlur::SetBinaryInner(const Bin &bin1, const Bin &bin2, f32 blend) {
    // TODO
}

} // namespace EGG
