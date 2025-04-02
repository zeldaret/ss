#include "d/lyt/d_textbox.h"

#include "common.h"
#include "d/d_font_manager.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "libms/msgfile.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_textBox.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_Rect.h"

#include <cstring>

dTextBox_c::dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &resBlockSet)
    : nw4r::lyt::TextBox(pBlock, resBlockSet),
      mMyFontSize(0.0f, 0.0f),
      mMyTextScale(0.0f, 0.0f),
      field_0x188(0.0f),
      field_0x18C(0.0f),
      field_0x190(0.0f),
      field_0x194(0.0f) {
    const nw4r::lyt::res::Font *fonts = nw4r::lyt::detail::ConvertOffsToPtr<nw4r::lyt::res::Font>(
        resBlockSet.pFontList, sizeof(nw4r::lyt::res::FontList)
    );
    const char *fontName = nw4r::lyt::detail::ConvertOffsToPtr<char>(fonts, fonts[pBlock->fontIdx].nameStrOffset);
    for (u32 i = 0; i < 5; i++) {
        const char *name = dFontMng_c::getFontName(i);
        if (!std::strcmp(fontName, name)) {
            mFontIdx = i;
            break;
        }
    }
    init();
}

///////////////////////////////////////////////////////////////////////////
// Begin largely copied from nw4r::lyt::TextBox
///////////////////////////////////////////////////////////////////////////

inline u8 ClampColor(s16 colVal) {
    // The CLAMP macro was reversed for this -_-
    return colVal < 0 ? 0 : (colVal > 0xFF ? 0xFF : colVal);
}

nw4r::ut::Color GetColor(const GXColorS10 &src) {
    GXColor dst;
    dst.r = ClampColor(src.r);
    dst.g = ClampColor(src.g);
    dst.b = ClampColor(src.b);
    dst.a = ClampColor(src.a);
    return dst;
}

static void CalcStringRect(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> &textWriter, const wchar_t *str, int length,
    bool *pbWideScreenUnk, f32 maxWidth
);

static void CalcStringRectImpl(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbWideScreenUnk
);

static int CalcLineRectImpl(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbOver, bool *pbWideScreenUnk
);

nw4r::ut::Rect dTextBox_c::GetMyTextDrawRect(nw4r::ut::TextWriterBase<wchar_t> *pWriter, bool *pbWideScreenUnk) const {
    nw4r::ut::Rect textRect;
    pWriter->SetCursor(0.0f, 0.0f);
    pWriter->SetFont(*mpFont);
    pWriter->SetFontSize(GetFontSize().width, GetFontSize().height);
    pWriter->SetLineSpace(mLineSpace);
    pWriter->SetCharSpace(mCharSpace);
    if (mIsWidthFixed == true) {
        pWriter->EnableFixedWidth(true);
        pWriter->SetFixedWidth(mFixedWidth);
    }
    if (GetTagProcessor() != nullptr) {
        pWriter->SetTagProcessor(GetTagProcessor());
    }
    CalcStringRect(&textRect, *pWriter, mTextBuf, mTextLen, pbWideScreenUnk, FLT_MAX);

    nw4r::lyt::Size textSize(textRect.GetWidth(), textRect.GetHeight());
    nw4r::math::VEC2 ltPos = GetVtxPos();

    // Adjust for Alignment (left, center, right)
    ltPos.x += (mSize.width - textSize.width) * GetTextMagH();
    ltPos.y -= (mSize.height - textSize.height) * GetTextMagV();

    // Apply the new pos
    textRect.left = ltPos.x;
    textRect.top = ltPos.y;
    textRect.right = ltPos.x + textSize.width;
    textRect.bottom = ltPos.y - textSize.height;

    return textRect;
}

static void CalcStringRect(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> &textWriter, const wchar_t *str, int length,
    bool *pbWideScreenUnk, f32 maxWidth
) {
    nw4r::ut::TextWriterBase<wchar_t> writerCpy(textWriter);
    CalcStringRectImpl(pRect, &writerCpy, str, length, maxWidth, pbWideScreenUnk);
}

static void CalcStringRectImpl(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbWideScreenUnk
) {
    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;
    pTextWriter->SetCursor(0.0f, 0.0f);
    int remain = length;
    const wchar_t *pos = str;
    int i = 0;
    do {
        nw4r::ut::Rect rect;
        bool bOver;
        int read =
            CalcLineRectImpl(&rect, pTextWriter, pos, remain, maxWidth, &bOver, i == 0 ? pbWideScreenUnk : nullptr);
        pos += read;
        remain -= read;
        pRect->left = nw4r::ut::Min(pRect->left, rect.left);
        pRect->top = nw4r::ut::Min(pRect->top, rect.top);
        pRect->right = nw4r::ut::Max(pRect->right, rect.right);
        pRect->bottom = nw4r::ut::Max(pRect->bottom, rect.bottom);
        if (bOver) {
            CalcLineRectImpl(&rect, pTextWriter, L"\n", 1, maxWidth, &bOver, nullptr);
            pRect->left = nw4r::ut::Min(pRect->left, rect.left);
            pRect->top = nw4r::ut::Min(pRect->top, rect.top);
            pRect->right = nw4r::ut::Max(pRect->right, rect.right);
            pRect->bottom = nw4r::ut::Max(pRect->bottom, rect.bottom);
        }
        i++;
    } while (remain > 0);
}

extern "C" bool NeedsScreenAdjustment();

static int CalcLineRectImpl(
    nw4r::ut::Rect *pRect, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbOver, bool *pbWideScreenUnk
) {
    nw4r::ut::PrintContext<wchar_t> context = {pTextWriter, str, 0.0f, 0.0f, 0};
    f32 x = 0.0f;

    const nw4r::ut::Font *font = pTextWriter->GetFont();
    bool bCharSpace = false;
    nw4r::ut::CharStrmReader reader = font->GetCharStrmReader();

    const wchar_t *prStrPos = (const wchar_t *)reader.GetCurrentPos();

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = nw4r::ut::Min(0.0f, pTextWriter->GetLineHeight());
    pRect->bottom = nw4r::ut::Max(0.0f, pTextWriter->GetLineHeight());

    *pbOver = false;

    reader.Set(str);
    nw4r::ut::Rect prMaxRect = *pRect;
    u16 code = reader.Next();
    while (((const wchar_t *)reader.GetCurrentPos() - str) <= length) {
        if (code < ' ') {
            nw4r::ut::Operation operation;
            nw4r::ut::Rect rect(x, 0.0f, 0.0f, 0.0f);

            context.str = (const wchar_t *)reader.GetCurrentPos();
            context.flags = !bCharSpace;

            pTextWriter->SetCursorX(x);
            // ??????????
            if (pbWideScreenUnk != nullptr && ((u32 *)context.str)[0] == 0 && NeedsScreenAdjustment()) {
                *pbWideScreenUnk = true;
            }
            operation = pTextWriter->GetTagProcessor()->CalcRect(&rect, code, &context);
            reader.Set(context.str);

            pRect->left = nw4r::ut::Min(pRect->left, rect.left);
            pRect->top = nw4r::ut::Min(pRect->top, rect.top);
            pRect->right = nw4r::ut::Max(pRect->right, rect.right);
            pRect->bottom = nw4r::ut::Max(pRect->bottom, rect.bottom);

            x = pTextWriter->GetCursorX();

            if (pRect->GetWidth() > maxWidth) {
                *pbOver = true;
                break;
            }
            if (operation == nw4r::ut::OPERATION_END_DRAW) {
                return length;
            } else if (operation == nw4r::ut::OPERATION_NO_CHAR_SPACE) {
                bCharSpace = false;
            } else if (operation == nw4r::ut::OPERATION_CHAR_SPACE) {
                bCharSpace = true;
            } else if (operation == nw4r::ut::OPERATION_NEXT_LINE) {
                break;
            }

        } else {
            if (bCharSpace) {
                x += pTextWriter->GetCharSpace();
            }
            bCharSpace = true;
            if (pTextWriter->IsWidthFixed()) {
                x += pTextWriter->GetFixedWidth();
            } else {
                x += pTextWriter->GetFont()->GetCharWidth(code) * pTextWriter->GetScaleH();
            }
            pRect->left = nw4r::ut::Min(pRect->left, x);
            pRect->right = nw4r::ut::Max(pRect->right, x);

            if (pRect->GetWidth() > maxWidth) {
                *pbOver = true;
                break;
            }
        }
        prStrPos = (const wchar_t *)reader.GetCurrentPos();
        code = reader.Next();
        prMaxRect = *pRect;
    }

    if (*pbOver) {
        if (prStrPos) {
            *pRect = prMaxRect;
            return (prStrPos - str);
        }
    }
    return ((const wchar_t *)reader.GetCurrentPos() - str);
}

///////////////////////////////////////////////////////////////////////////
// End largely copied from nw4r::lyt::TextBox
///////////////////////////////////////////////////////////////////////////

void dTextBox_c::setTextWithGlobalTextProcessor(const wchar_t *str, void *unk, ...) {
    va_list l;
    va_start(l, str);
    setTextWithGlobalTextProcessorV(str, unk, l);
    va_end(l);
}

extern "C" dTagProcessor_c *GLOBAL_TEXT_MANAGER;

void dTextBox_c::setTextWithGlobalTextProcessorV(const wchar_t *str, void *unk, va_list list) {
    SetFontSize(mMyTextScale);
    mHasTextWriter = true;
    mpMyTagProcessor = GLOBAL_TEXT_MANAGER;
    u32 outLen = 0;
    mWindowSubtype = mpMyTagProcessor->getMsgWindowSubtype();
    mpMyTagProcessor->setField_0xEE1(field_0x1F6);
    mpMyTagProcessor->setField_0xEE2(0);
    mpMyTagProcessor->setField_0xEE3(1);

    static wchar_t destBuffer[0x200];
    mpMyTagProcessor->formatV(nullptr, str, destBuffer, ARRAY_LENGTH(destBuffer), &outLen, unk, list);
    SetString(destBuffer, 0, outLen);
}

void dTextBox_c::setTextWithTextProcessor(const wchar_t *str, dTagProcessor_c *tagProcessor, void *unk, ...) {
    va_list l;
    va_start(l, str);
    setTextWithTextProcessorV(str, tagProcessor, unk, l);
    va_end(l);
}

void dTextBox_c::setTextWithTextProcessorV(const wchar_t *str, dTagProcessor_c *tagProcessor, void *unk, va_list list) {
    SetFontSize(mMyTextScale);
    mHasTextWriter = true;
    u32 outLen = 0;
    mpMyTagProcessor = tagProcessor;
    mWindowSubtype = mpMyTagProcessor->getMsgWindowSubtype();
    mpMyTagProcessor->setField_0xEE1(field_0x1F6);
    mpMyTagProcessor->setField_0xEE2(0);
    mpMyTagProcessor->setField_0xEE3(1);

    static wchar_t destBuffer[0x200];
    mpMyTagProcessor->formatV(nullptr, str, destBuffer, ARRAY_LENGTH(destBuffer), &outLen, unk, list);
    SetString(destBuffer, 0, outLen);
}

s32 dTextBox_c::calcTextLines(const wchar_t *src, dTagProcessor_c *tagProcessor) {
    static wchar_t destBuffer[0x200];
    u32 outLen = 0;
    tagProcessor->format(this, src, destBuffer, ARRAY_LENGTH(destBuffer), &outLen, nullptr);
    return tagProcessor->getNumLinesMaybe();
}

extern "C" const wchar_t *getTextMessageByLabel(const char *labelId, int, int, int);

void dTextBox_c::setMessageWithGlobalTextProcessor(const char *labelId, void *unused, void *unk, ...) {
    va_list l;
    va_start(l, str);
    setMessageWithGlobalTextProcessorV(labelId, unused, unk, l);
    va_end(l);
}

void dTextBox_c::setMessageWithGlobalTextProcessorV(const char *labelId, void *unused, void *unk, va_list list) {
    const wchar_t *text = getTextMessageByLabel(labelId, 1, 0, 0);
    setTextWithGlobalTextProcessorV(text, unk, list);
    if (GetFont() != nullptr) {
        fn_800AFD60(GetSize().width);
    }
}

void dTextBox_c::setMessageWithGlobalTextProcessorAndMsbtInfo(
    const MsbtInfo *info, const char *labelId, wchar_t *destBuf, u32 maxLen
) {
    const wchar_t *src = LMS_GetTextByLabel(info, labelId);
    SetFontSize(mMyTextScale);
    mHasTextWriter = true;
    field_0x1FA = 1;
    u32 outLen = 0;
    mpMyTagProcessor = GLOBAL_TEXT_MANAGER;
    mpMyTagProcessor->setMsgWindowSubtype(0x22);
    mWindowSubtype = mpMyTagProcessor->getMsgWindowSubtype();
    mpMyTagProcessor->setField_0xEE1(field_0x1F6);
    mpMyTagProcessor->setField_0xEE2(0);
    mpMyTagProcessor->setField_0xEE3(1);

    mpMyTagProcessor->format(nullptr, src, destBuf, maxLen, &outLen, 0);
    SetString(destBuf, 0, outLen);
}

void dTextBox_c::setTextWithGlobalTextProcessor(const wchar_t *str) {
    setTextWithGlobalTextProcessor(str, nullptr);
    if (GetFont() != nullptr) {
        fn_800AFD60(GetSize().width);
    }
}

void dTextBox_c::init() {
    mpLytBase = nullptr;
    mIsWidthFixed = false;
    mFixedWidth = 0.0f;
    mHasTextWriter = true;
    field_0x1F6 = 0;
    field_0x1F7 = 0;
    field_0x1F9 = 0;
    field_0x1FA = 0;
    field_0x1F8 = 2;
    mpMyTagProcessor = nullptr;
    MySetFontSize(GetFontSize());
    for (int i = 0; i < 10; i++) {
        field_0x198[i] = 0;
        field_0x1C4[i] = 0.0f;
        field_0x1EC[i] = 0;
    }

    f32 f1 = 35.0f;
    if (mFontIdx > 1 && mFontIdx < 4) {
        f1 = 41.0f;
    }

    nw4r::lyt::Size sz = GetFontSize();
    f32 scale = UnkTextThing::getFn800B1F70();

    f32 height = sz.height / f1;
    sz.width = GetFont()->GetWidth() * height * scale;
    sz.height = GetFont()->GetHeight() * height * scale;
    SetFontSize(sz);
    mMyTextScale = sz;
    mMyScale = 1.0f;
    mWindowSubtype = 0x24;
    field_0x201 = 1;
}

void dTextBox_c::DrawSelf(const nw4r::lyt::DrawInfo &drawInfo) {
    if (!mTextBuf || !mpFont || !mpMaterial) {
        return;
    }
    LoadMtx(drawInfo);
    nw4r::ut::TextWriterBase<wchar_t> writer;
    // TODO
}

void dTextBox_c::MySetFontSize(const nw4r::lyt::Size &value) {
    mMyFontSize = value;
}

void dTextBox_c::MySetScale(const nw4r::lyt::Size &value) {
    mMyTextScale = value;
}
