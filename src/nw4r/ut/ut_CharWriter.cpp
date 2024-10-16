// ported from https://github.com/kiwi515/ogws/blob/master/src/nw4r/ut/ut_CharWriter.cpp

#include "nw4r/ut.h"

namespace {

static void SetupGXCommon() {
    static const nw4r::ut::Color fog = 0;

    GXSetFog(GX_FOG_NONE, fog, 0.0f, 0.0f, 0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX_IDENT, FALSE, GX_DUALMTX_IDENT);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

} // namespace

namespace nw4r {
namespace ut {

CharWriter::CharWriter() : mAlpha(255), mIsWidthFixed(false), mFixedWidth(0.0f), mFont(NULL) {
    mLoadingTexture.Reset();
    ResetColorMapping();
    SetGradationMode(GRADMODE_NONE);
    SetTextColor(Color(0xFFFFFFFF));
    SetScale(1.0f, 1.0f);
    SetCursor(0.0f, 0.0f, 0.0f);
    EnableLinearFilter(true, true);
}

CharWriter::~CharWriter() {}

void CharWriter::SetupGX() {
    ResetTextureCache();

    if (mColorMapping.min != 0x00000000 || mColorMapping.max != 0xFFFFFFFF) {
        SetupGXWithColorMapping(mColorMapping.min, mColorMapping.max);
    } else if (mFont != NULL) {
        switch (mFont->GetTextureFormat()) {
            case GX_TF_I4:
            case GX_TF_I8:     SetupGXForI(); break;
            case GX_TF_IA4:
            case GX_TF_IA8:    SetupGXDefault(); break;
            case GX_TF_RGB565:
            case GX_TF_RGB5A3:
            case GX_TF_RGBA8:  SetupGXForRGBA(); break;
            default:           SetupGXDefault(); break;
        }
    } else {
        SetupGXDefault();
    }
}

void CharWriter::SetFontSize(f32 width, f32 height) {
    SetScale(width / mFont->GetWidth(), height / mFont->GetHeight());
}

void CharWriter::SetFontSize(f32 height) {
    f32 scale = height / mFont->GetHeight();
    SetScale(scale, scale);
}

f32 CharWriter::GetFontWidth() const {
    return mScale.x * mFont->GetWidth();
}

f32 CharWriter::GetFontHeight() const {
    return mScale.y * mFont->GetHeight();
}

f32 CharWriter::GetFontAscent() const {
    return mScale.y * mFont->GetAscent();
}

f32 CharWriter::GetFontDescent() const {
    return mScale.y * mFont->GetDescent();
}

void CharWriter::EnableLinearFilter(bool atSmall, bool atLarge) {
    mFilter.atSmall = atSmall ? GX_LINEAR : GX_NEAR;
    mFilter.atLarge = atLarge ? GX_LINEAR : GX_NEAR;
}

f32 CharWriter::Print(u16 ch) {
    f32 width;
    f32 left;

    Glyph glyph;
    mFont->GetGlyph(&glyph, ch);

    if (mIsWidthFixed) {
        width = mFixedWidth;
        left = (width - glyph.widths.charWidth * mScale.x) / 2 + (glyph.widths.leftSpacing * mScale.x);
    } else {
        width = glyph.widths.charWidth * mScale.x;
        left = glyph.widths.leftSpacing * mScale.x;
    }

    PrintGlyph(mCursorPos.x + left, mCursorPos.y, mCursorPos.z, glyph);
    mCursorPos.x += width;

    return width;
}

void CharWriter::PrintGlyph(f32 x, f32 y, f32 z, const Glyph &glyph) {
    f32 x2 = x + (glyph.widths.glyphWidth * mScale.x);
    f32 y2 = y + (glyph.height * mScale.y);

    u32 posLeft = glyph.cellX;
    u16 texLeft = 0x8000 * posLeft / glyph.texWidth;

    u32 posTop = glyph.cellY;
    u16 texTop = 0x8000 * posTop / glyph.texHeight;

    u32 posRight = posLeft + glyph.widths.glyphWidth;
    u16 texRight = 0x8000 * posRight / glyph.texWidth;

    u32 posBottom = posTop + glyph.height;
    u16 texBottom = 0x8000 * posBottom / glyph.texHeight;

    LoadTexture(glyph, GX_TEXMAP0);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    {
        GXPosition3f32(x, y, z);
        GXColor1u32(mVertexColor.tl);
        GXTexCoord2s16(texLeft, texTop);

        GXPosition3f32(x2, y, z);
        GXColor1u32(mVertexColor.tr);
        GXTexCoord2s16(texRight, texTop);

        GXPosition3f32(x2, y2, z);
        GXColor1u32(mVertexColor.br);
        GXTexCoord2s16(texRight, texBottom);

        GXPosition3f32(x, y2, z);
        GXColor1u32(mVertexColor.bl);
        GXTexCoord2s16(texLeft, texBottom);
    }
    // GXEnd();
}

void CharWriter::LoadTexture(const Glyph &glyph, GXTexMapID slot) {
    LoadingTexture loadingTexture;

    loadingTexture.slot = slot;
    loadingTexture.texture = glyph.texture;
    loadingTexture.filter = mFilter;

    if (loadingTexture != mLoadingTexture) {
        GXTexObj texObj;
        GXInitTexObj(&texObj, glyph.texture, glyph.texWidth, glyph.texHeight, glyph.format, GX_CLAMP, GX_CLAMP, FALSE);
        GXInitTexObjLOD(&texObj, mFilter.atSmall, mFilter.atLarge, 0.0f, 0.0f, 0.0f, FALSE, FALSE, GX_ANISO_1);
        GXLoadTexObj(&texObj, slot);

        mLoadingTexture = loadingTexture;
    }
}

void CharWriter::UpdateVertexColor() {
    // clang-format off
    mVertexColor.tl = mTextColor.start;
    mVertexColor.tr = mTextColor.gradMode != GRADMODE_H    ? mTextColor.start : mTextColor.end;
    mVertexColor.bl = mTextColor.gradMode != GRADMODE_V    ? mTextColor.start : mTextColor.end;
    mVertexColor.br = mTextColor.gradMode == GRADMODE_NONE ? mTextColor.start : mTextColor.end;
    // clang-format on

    mVertexColor.tl.a = (mVertexColor.tl.a * mAlpha) / 255, mVertexColor.tr.a = (mVertexColor.tr.a * mAlpha) / 255;
    mVertexColor.bl.a = (mVertexColor.bl.a * mAlpha) / 255;
    mVertexColor.br.a = (mVertexColor.br.a * mAlpha) / 255;
}

void CharWriter::SetupVertexFormat() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 15);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

void CharWriter::SetupGXDefault() {
    SetupGXCommon();

    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

    SetupVertexFormat();
}

void CharWriter::SetupGXWithColorMapping(Color min, Color max) {
    SetupGXCommon();

    GXSetNumTevStages(2);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);

    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

    GXSetTevColor(GX_TEVREG0, min);
    GXSetTevColor(GX_TEVREG1, max);

    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

    SetupVertexFormat();
}

void CharWriter::SetupGXForI() {
    SetupGXCommon();

    GXSetNumTevStages(1);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, TRUE, GX_TEVPREV);

    SetupVertexFormat();
}

void CharWriter::SetupGXForRGBA() {
    SetupGXDefault();
}

CharWriter::LoadingTexture CharWriter::mLoadingTexture;

} // namespace ut
} // namespace nw4r
