#include "egg/gfx/eggDrawPathLightMap.h"

#include "common.h"
#include "egg/gfx/eggDrawGX.h"
#include "egg/gfx/eggGlobalDrawState.h"
#include "egg/gfx/eggPostEffectBase.h"
#include "egg/gfx/eggStateGX.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTypes.h"

namespace EGG {

DrawPathLightMap::DrawPathLightMap()
    : field_0x18(6),
      mAlpha(0xFF),
      field_0x1C(0),
      field_0x20(0),
      field_0x24(0),
      mpTextureBuffer_0x28(nullptr),
      mpTextureBuffer_0x2C(nullptr),
      mpTextureBuffer_0x30(nullptr),
      mOldDrawFlag(0) {}

void DrawPathLightMap::internalResetForDraw() {
    freeTextureBuffers();
    mpTextureBuffer_0x28 = nullptr;
    mpTextureBuffer_0x2C = nullptr;
    mpTextureBuffer_0x30 = nullptr;
}

void DrawPathLightMap::internalDraw(u16 idx) {
    const Screen::DataEfb &efb = GlobalDrawState::getScreen().GetDataEfb();

    switch (idx) {
        case 0: {
            if (field_0x18 & 1) {
                mpTextureBuffer_0x30 = TextureBuffer::alloc(efb.vp.x2, efb.vp.y2, GX_TF_RGBA8);
                mpTextureBuffer_0x30->onCapFlag(1);
                mpTextureBuffer_0x30->onCapFlag(2);
                mpTextureBuffer_0x30->setFilt(GX_NEAR, GX_NEAR);
                mpTextureBuffer_0x30->capture(efb.vp.x1, efb.vp.y1, false, -1);
            }
            mOldDrawFlag = GlobalDrawState::getDrawFlag();
            GlobalDrawState::onDrawFlag(0x1 | 0x2);
        } break;
        case 1: {
            GlobalDrawState::setDrawFlag(mOldDrawFlag);
            GXTexFmt fmt = GX_TF_RGBA8;

            if (field_0x20 == 1) {
                fmt = GX_TF_RGB5A3;
                if (field_0x1C == 1) {
                    fmt = GX_TF_RGB565;
                }
            }

            int capW = efb.vp.x1;
            int capH = efb.vp.y1;

            mpTextureBuffer_0x28 = TextureBuffer::alloc(efb.vp.x2, efb.vp.y2, fmt);

            mpTextureBuffer_0x28->onCapFlag(1);
            mpTextureBuffer_0x28->onCapFlag(2);
            if (!(field_0x18 & 2)) {
                mpTextureBuffer_0x28->onCapFlag(4);
            }
            mpTextureBuffer_0x28->setClearColor(StateGX::getClearEfb());

            mpTextureBuffer_0x28->capture(capW, capH, false, -1);
            mpTextureBuffer_0x28->setFilt(GX_NEAR, GX_NEAR);

            if (mpTextureBuffer_0x30) {
                nw4r::math::MTX34 forDL;

                StateGX::ScopedColor _color(true);
                StateGX::ScopedAlpha _alpha(true);
                StateGX::ScopedDither _dither(false);

                PostEffectBase::setProjection(GetScreen());

                PSMTXScale(forDL, GetScreen().GetWidth(), GetScreen().GetHeight(), 1.0f);

                mpTextureBuffer_0x30->load(GX_TEXMAP0);
                DrawGX::BeginDrawScreen(true, true, false);

                DrawGX::SetBlendMode(DrawGX::BLEND_14);

                DrawGX::DrawDL16(forDL, DrawGX::WHITE);

                mpTextureBuffer_0x30->free();
                mpTextureBuffer_0x30 = nullptr;
            }

            if (field_0x18 & 2) {
                mpTextureBuffer_0x2C = TextureBuffer::alloc(efb.vp.x2, efb.vp.y2, GX_TF_Z24X8);
                mpTextureBuffer_0x2C->onCapFlag(4);

                if (isFlag0x2()) {
                    mpTextureBuffer_0x2C->onCapFlag(0x10);
                } else {
                    mpTextureBuffer_0x2C->offCapFlag(0x10);
                }

                mpTextureBuffer_0x2C->capture(capW, capH, false, -1);
                mpTextureBuffer_0x2C->setFilt(GX_NEAR, GX_NEAR);
            }
            StateGX::invalidateTexAllGX();
        } break;
        case 2: {
            if (mpTextureBuffer_0x28) {
                if (mAlpha != 0) {
                    mpTextureBuffer_0x28->load(GX_TEXMAP0);

                    nw4r::math::MTX34 forDL;
                    PostEffectBase::setProjection(GetScreen());
                    PSMTXScale(forDL, GetScreen().GetWidth(), GetScreen().GetHeight(), 1.0f);

                    GXColor tevColor = {0xFF, 0xFF, 0xFF, mAlpha};

                    StateGX::ScopedColor _color(true);
                    StateGX::ScopedAlpha _alpha(false);
                    StateGX::ScopedDither _dither(true);

                    if (mpTextureBuffer_0x2C) {
                        mpTextureBuffer_0x2C->load(GX_TEXMAP1);
                        DrawGX::SetMat_ColorChannel(DrawGX::COLORCHAN_1);
                        DrawGX::SetMat_TexGen(DrawGX::TEXGEN_1);
                        DrawGX::SetMat_Ind();
                        GXSetTevKColor(GX_KCOLOR0, (GXColor){0});

                        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                        GXSetNumTevStages(2);
                        GXSetTevDirect(GX_TEVSTAGE0);
                        GXSetTevDirect(GX_TEVSTAGE1);
                        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);

                        if (field_0x1C == 0) {
                            GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
                            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
                            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_KONST, GX_CA_RASA, GX_CA_ZERO);
                            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                        } else {
                            GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
                            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                        }

                        GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
                        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
                        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
                        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
                        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                        GXSetZTexture(GZ_ZT_REPLACE, GX_TF_Z24X8, 0);

                        GXSetZMode(true, field_0x1C == 0 ? GX_LEQUAL : GX_LESS, field_0x18 & 0x4 ? true : false);
                        GXSetZCompLoc(0);
                        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
                        DrawGX::SetBlendMode(DrawGX::BLEND_0);
                        DrawGX::SetVtxState(DrawGX::VTX_TYPE_11);
                    } else {
                        DrawGX::BeginDrawScreen(true, true, false);
                        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
                        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_RASA, GX_CA_ZERO);
                        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_A8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
                    }

                    DrawGX::DrawDL16(forDL, tevColor);
                    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
                }
                freeTextureBuffers();
            }
        } break;
    }
}

void DrawPathLightMap::freeTextureBuffers() {
    if (mpTextureBuffer_0x2C) {
        mpTextureBuffer_0x2C->free();
        mpTextureBuffer_0x2C = nullptr;
    }
    if (mpTextureBuffer_0x28) {
        mpTextureBuffer_0x28->free();
        mpTextureBuffer_0x28 = nullptr;
    }
}

} // namespace EGG
