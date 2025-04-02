#include "d/lyt/d_textbox.h"

#include "common.h"
#include "d/d_font_manager.h"
#include "d/d_tag_processor.h"
#include "d/d_textunk.h"
#include "libms/msgfile.h"
#include "m/m_color.h"
#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/lyt/lyt_textBox.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_CharWriter.h"
#include "nw4r/ut/ut_Font.h"
#include "nw4r/ut/ut_Rect.h"
#include "rvl/GX/GXPixel.h"
#include "rvl/GX/GXTev.h"
#include "rvl/GX/GXTypes.h"

#include <cstddef>
#include <cstring>

dTextBox_c::dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &resBlockSet)
    : nw4r::lyt::TextBox(pBlock, resBlockSet), mMyFontSize(0.0f, 0.0f), mMyTextScale(0.0f, 0.0f) {
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

static int CalcLineStrNum(
    f32 *pWidth, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbOver
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
        resizeTextToFit(GetSize().width);
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
        resizeTextToFit(GetSize().width);
    }
}

f32 dTextBox_c::GetLineWidth(f32 *pOutSpacing) const {
    if ((int)mTextLen <= 0) {
        return 0.0f;
    }

    f32 widthThisLine = 0.0f;
    int numCharsThisLine = 0;
    int maxNumCharacters = 0;
    const wchar_t *buf = GetString();
    f32 maxWidth = widthThisLine;
    const nw4r::ut::Font *f = GetFont();
    f32 charBaseWidth = mFontSize.width / f->GetWidth();
    wchar_t c;
    while ((c = *buf, c != '\0')) {
        if (c == L'\x0E') {
            u8 commandLen = buf[3];
            buf += commandLen / 2 + 4;
        } else if (c == L'\x0F') {
            buf += 3;
        } else if (c == L'\n') {
            if (widthThisLine > maxWidth) {
                maxWidth = widthThisLine;
                maxNumCharacters = numCharsThisLine;
            }
            widthThisLine = 0.0f;
            numCharsThisLine = 0;
            buf++;
        } else {
            if (mIsWidthFixed == true) {
                widthThisLine += mFixedWidth;
            } else {
                int charWid = f->GetCharWidth(c);
                widthThisLine += charBaseWidth * charWid;
            }
            numCharsThisLine++;
            buf++;
        }
    }

    if (widthThisLine > maxWidth) {
        maxWidth = widthThisLine;
        maxNumCharacters = numCharsThisLine;
    }

    f32 maxSpaceWidth = (maxNumCharacters - 1) * GetCharSpace();
    maxWidth += maxSpaceWidth + 1.0f;
    if (pOutSpacing != nullptr) {
        *pOutSpacing = maxSpaceWidth;
    }

    return maxWidth;
}

f32 dTextBox_c::GetLinesHeight() const {
    if ((int)mTextLen <= 0) {
        return 0.0f;
    }

    const wchar_t *buf = GetString();
    const nw4r::ut::Font *f = GetFont();

    f32 assumedDefaultSize = 35.0f;

    f32 f3 = UnkTextThing::getFn800B1F70();
    nw4r::lyt::Size size = mMyFontSize;
    int fontHeight = f->GetHeight();
    f3 *= (size.height / assumedDefaultSize) * fontHeight;
    f32 lineBaseHeight = f3;

    f32 totalHeight = 0.0f;

    wchar_t c;
    while ((c = *buf, c != '\0')) {
        if (c == L'\x0E') {
            u8 commandLen = buf[3];
            s32 skip = commandLen / 2 + 4;
            u32 command = *((u32 *)(buf + 1));
            if (command == 0x10008) {
                f32 nullHeight = dTagProcessor_c::fn_800B8040(0, mWindowSubtype);
                f32 cmdHeight = dTagProcessor_c::fn_800B8040(*((u8 *)(buf + 4)), mWindowSubtype);
                lineBaseHeight = f3 * (cmdHeight / nullHeight);
            }

            buf += skip;
        } else if (c == L'\x0F') {
            buf += 3;
        } else if (c == L'\n') {
            totalHeight += lineBaseHeight + GetLineSpace();
            buf++;
        } else {
            buf++;
        }
    }

    totalHeight += lineBaseHeight;

    return totalHeight;
}

extern "C" f32 lbl_805751A4;
f32 dTextBox_c::GetUnkWidthRatio() const {
    f32 tmp = lbl_805751A4;
    return GetLineWidth(nullptr) / tmp;
}

f32 dTextBox_c::resizeTextToFit(f32 maxWidth) {
    if (!hasDynamicText()) {
        return 0.0f;
    }

    nw4r::lyt::Size size;
    size = GetFontSize();

    f32 spacing = 0.0f;
    f32 computedTextWidth = GetLineWidth(&spacing);
    if (maxWidth >= computedTextWidth) {
        return 0.0f;
    }

    if (spacing < 0.0f) {
        spacing = 0.0f;
    }
    f32 resizeFactor = (maxWidth - spacing) / computedTextWidth;
    size.width *= resizeFactor;
    size.height = size.height;
    SetFontSize(size);
    return resizeFactor * 100.0f;
}

void dTextBox_c::init() {
    mpLytBase = nullptr;
    mIsWidthFixed = false;
    mFixedWidth = 0.0f;
    mHasTextWriter = true;
    field_0x1F6 = 0;
    field_0x1F7 = 0;
    field_0x1F9 = false;
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

bool dTextBox_c::hasDynamicText() {
    u16 myNum = GetExtUserDataNum();
    if (myNum == 0) {
        return false;
    }

    const nw4r::lyt::res::ExtUserData *myList = GetExtUserData();
    for (int i = 0; i < myNum; i++) {
        SizedString<0x40> userDatName;
        userDatName = myList->GetName();
        if (userDatName == "embed" && myList->GetType() == nw4r::lyt::res::TYPE_INT) {
            return true;
        }

        if (userDatName == "textScale" && myList->GetType() == nw4r::lyt::res::TYPE_INT) {
            return true;
        }

        if (userDatName == "copy" && myList->GetType() == nw4r::lyt::res::TYPE_STRING) {
            field_0x1F6 = true;
            dTextBox_c *other = mpLytBase->getTextBox(myList->GetString());
            if (other != nullptr) {
                u16 otherNum = other->GetExtUserDataNum();
                if (otherNum == 0) {
                    break;
                }
                const nw4r::lyt::res::ExtUserData *otherList = other->GetExtUserData();
                for (int j = 0; j < otherNum; j++) {
                    userDatName = otherList->GetName();
                    if (userDatName == "embed") {
                        return true;
                    }
                    if (userDatName == "textScale") {
                        return true;
                    }
                    otherList++;
                }
            }
        }
        myList++;
    }

    return false;
}

void dTextBox_c::someDebugCheckMaybe() {
    field_0x1F7 = 0;

    // Everything below this is effectively no-ops, since
    // we check and compare a bunch of things but don't actually
    // do anything.

    u16 myNum = GetExtUserDataNum();
    if (!myNum) {
        return;
    }

    const nw4r::lyt::res::ExtUserData *myList = GetExtUserData();
    for (int i = 0; i < myNum; myList++, i++) {
        SizedString<0x40> userDatName;
        userDatName = myList->GetName();

        if (userDatName == "copy" && myList->GetType() == nw4r::lyt::res::TYPE_STRING) {
            dTextBox_c *other = mpLytBase->getTextBox(myList->GetString());
            if (other != nullptr) {
                u16 otherNum = other->GetExtUserDataNum();
                if (otherNum == 0) {
                    continue;
                }
                const nw4r::lyt::res::ExtUserData *otherList = other->GetExtUserData();
                for (int j = 0; j < otherNum; j++) {
                    SizedString<0x40> otherName;
                    otherName = otherList->GetName();
                    otherList++;
                }
            }
        }
    }

    return;
}

// Also largely copied from nw4r::lyt::TextBox
void dTextBox_c::DrawSelf(const nw4r::lyt::DrawInfo &drawInfo) {
    if (!mTextBuf || !mpFont || !mpMaterial) {
        return;
    }
    LoadMtx(drawInfo);
    nw4r::ut::TextWriterBase<wchar_t> writer;
    nw4r::ut::Rect textRect;
    if (mHasTextWriter) {
        field_0x1F9 = false;
        someDebugCheckMaybe();
        nw4r::ut::Rect resultRect = GetMyTextDrawRect(&writer, &field_0x1F9);
        textRect = resultRect;
        mTextWriter = writer;
        field_0x188 = textRect;
    } else {
        writer = mTextWriter;
        textRect = field_0x188;
    }

    if (mpMyTagProcessor != nullptr) {
        mpMyTagProcessor->setMsgWindowSubtype(mWindowSubtype);
        mpMyTagProcessor->setField_0xEE1(field_0x1F6);
        mpMyTagProcessor->setField_0xEE2(0);
        if (mAlpha != 0) {
            mpMyTagProcessor->setField_0xEE3(1);
        } else {
            mpMyTagProcessor->setField_0xEE3(0);
        }
    }

    bool unk1;
    if (field_0x1F8 == 2) {
        unk1 = field_0x1F9;
    } else {
        unk1 = field_0x1F8 == 1;
    }
    if (field_0x1F7 == 1 || (field_0x1F7 == 2 && unk1)) {
        f32 f = GetFontSize().height * UnkTextThing::getField0x75C();
        textRect.top -= f;
        textRect.bottom -= f;
    } else if (field_0x1F7 == 3 && unk1) {
        f32 f = GetFontSize().height * UnkTextThing::getField0x760();
        textRect.top -= f;
        textRect.bottom -= f;
    }

    nw4r::ut::Color c1 = nw4r::lyt::detail::MultipleAlpha(GetTextColor(0), GetGlobalAlpha());
    nw4r::ut::Color c2 = nw4r::lyt::detail::MultipleAlpha(GetTextColor(2), GetGlobalAlpha());

    writer.SetGradationMode(c1 != c2 ? nw4r::ut::CharWriter::GRADMODE_V : nw4r::ut::CharWriter::GRADMODE_NONE);
    writer.SetTextColor(c1, c2);

    nw4r::ut::Color minCol = GetColor(mpMaterial->GetTevColor(0));
    nw4r::ut::Color maxCol = GetColor(mpMaterial->GetTevColor(1));
    writer.SetColorMapping(minCol, maxCol);

    if (mFontIdx <= 3) {
        setupGX();
        if (mpMyTagProcessor != nullptr) {
            mpMyTagProcessor->setTextbox(this);
        }
    } else {
        writer.SetupGX();
    }
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);

    f32 hMag = GetTextAlignMag();
    const wchar_t *strPos = mTextBuf;
    writer.SetCursor(textRect.left, -textRect.top);
    f32 texWidth = textRect.GetWidth();

    int i = 0;

    if (field_0x1FA != 0) {
        // The original variant from nw4r::lyt
        int remain = mTextLen;
        while (remain > 0) {
            bool bOver;
            f32 lineWidth;
            int lineStrNum = CalcLineStrNum(&lineWidth, &writer, strPos, remain, mSize.width, &bOver);
            f32 textPosX = textRect.left + hMag * (texWidth - lineWidth);
            writer.SetCursorX(textPosX);
            writer.PrintMutable(strPos, lineStrNum);
            if (bOver) {
                writer.PrintMutable(L"\n", 1);
            }
            strPos += lineStrNum;
            remain -= lineStrNum;
        }
    } else if (!mHasTextWriter && field_0x201 == 1) {
        // Taking parameters from the three arrays, instead of calculating them
        int remain = mTextLen;
        while (remain > 0 && i < 10) {
            bool bOver;
            f32 lineWidth = field_0x1C4[i];
            int lineStrNum = field_0x198[i];
            bOver = field_0x1EC[i];
            f32 textPosX = textRect.left + hMag * (texWidth - lineWidth);
            writer.SetCursorX(textPosX);
            writer.PrintMutable(strPos, lineStrNum);
            if (bOver) {
                writer.PrintMutable(L"\n", 1);
            }
            strPos += lineStrNum;
            remain -= lineStrNum;
            i++;
        }
    } else {
        // Copying parameters to the three arrays
        int remain = mTextLen;
        while (remain > 0 && i < 10) {
            bool bOver;
            f32 lineWidth;
            int lineStrNum = CalcLineStrNum(&lineWidth, &writer, strPos, remain, mSize.width, &bOver);
            f32 textPosX = textRect.left + hMag * (texWidth - lineWidth);
            writer.SetCursorX(textPosX);
            writer.PrintMutable(strPos, lineStrNum);
            if (mHasTextWriter && field_0x201 == 1 && i < 10) {
                field_0x1C4[i] = lineWidth;
                field_0x198[i] = lineStrNum;
                field_0x1EC[i] = bOver;
            }
            if (bOver) {
                writer.PrintMutable(L"\n", 1);
            }
            strPos += lineStrNum;
            remain -= lineStrNum;
            i++;
        }
    }

    /*
        const wchar_t *strPos = mTextBuf;
        writer.SetCursor(textRect.left, -textRect.top);
        f32 texWidth = textRect.GetWidth();
        int remain = mTextLen;

        while (remain > 0) {
            bool bOver;
            f32 lineWidth;
            int lineStrNum = CalcLineStrNum(&lineWidth, &writer, strPos, remain, mSize.width, &bOver);
            f32 textPosX = textRect.left + hMag * (texWidth - lineWidth);
            writer.SetCursorX(textPosX);
            writer.PrintMutable(strPos, lineStrNum);
            if (bOver) {
                writer.PrintMutable(L"\n", 1);
            }
            strPos += lineStrNum;
            remain -= lineStrNum;
        }
    */

    mHasTextWriter = false;
}

void dTextBox_c::loadTextFormatVars() {
    u16 myNum = GetExtUserDataNum();
    if (myNum == 0) {
        return;
    }

    const nw4r::lyt::res::ExtUserData *myList = GetExtUserData();
    for (int i = 0; i < myNum; i++) {
        SizedString<0x40> userDatName;
        userDatName = myList->GetName();
        if (userDatName == "fontWidth" && myList->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
            // If this text box has a "fontWidth" user datum of type float,
            // we use that as a fixed font width.
            mIsWidthFixed = true;
            mFixedWidth = myList->GetFloat();
            return;
        }

        // If this text box has a "copy" user datum, find a text box
        // of the given name and use its font width, if any
        if (userDatName == "copy") {
            dTextBox_c *other = mpLytBase->getTextBox(myList->GetString());
            if (other != nullptr) {
                u16 otherNum = other->GetExtUserDataNum();
                if (otherNum == 0) {
                    return;
                }
                const nw4r::lyt::res::ExtUserData *otherList = other->GetExtUserData();
                for (int j = 0; j < otherNum; j++) {
                    userDatName = otherList->GetName();
                    if (userDatName == "fontWidth" && otherList->GetType() == nw4r::lyt::res::TYPE_FLOAT) {
                        mIsWidthFixed = true;
                        mFixedWidth = otherList->GetFloat();
                        break;
                    }
                    otherList++;
                }
            }
        }
        myList++;
    }
}

void dTextBox_c::setupGX() const {
    static mColor sFogColor = mColor(0);
    GXSetFog(GX_FOG_NONE, sFogColor, 0.0f, 0.0f, 0.0f, 0.0f);
    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetZTexture(0, GX_TF_Z8, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, false, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3c, 0, 0x7d);
    GXSetNumIndStages(0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetNumTevStages(2);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

    // TODO the logic is wrong - what it should actually look like:

    /*
    GXColorS10 c1 = mpMaterial->GetTevColor(1);
    GXColorS10 c0 = mpMaterial->GetTevColor(0);
    GXColorS10 c2 = {-(c1.r / 2), -(c1.g / 2), -(c1.b / 2), -(c1.a / 2)};

    GXSetTevColorS10(GX_TEVREG0, c0);
    GXSetTevColorS10(GX_TEVREG1, c1);
    GXSetTevColorS10(GX_TEVREG2, c2);
    */

    // This makes the instruction patterns match best

    GXColorS10 c0 = mpMaterial->GetTevColor(1);
    GXColorS10 c1 = mpMaterial->GetTevColor(0);
    GXColorS10 c2 = {-(c0.r / 2), -(c0.g / 2), -(c0.b / 2), -(c0.a / 2)};

    GXSetTevColorS10(GX_TEVREG0, c0);
    GXSetTevColorS10(GX_TEVREG1, c1);
    GXSetTevColorS10(GX_TEVREG2, c2);

    if ((mFontIdx == 1) || (mFontIdx == 3)) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_C2);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
    } else if ((mFontIdx == 0) || (mFontIdx == 2)) {
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_C2);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_A2);
    }

    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

static int CalcLineStrNum(
    f32 *pWidth, nw4r::ut::TextWriterBase<wchar_t> *pTextWriter, const wchar_t *str, int length, f32 maxWidth,
    bool *pbOver
) {
    nw4r::ut::Rect rect;
    nw4r::ut::TextWriterBase<wchar_t> writerCpy(*pTextWriter);
    writerCpy.SetCursor(0.0f, 0.0f);
    int ret = CalcLineRectImpl(&rect, &writerCpy, str, length, maxWidth, pbOver, nullptr);
    *pWidth = rect.right - rect.left;
    return ret;
}

void dTextBox_c::MySetFontSize(const nw4r::lyt::Size &value) {
    mMyFontSize = value;
}

void dTextBox_c::MySetScale(const nw4r::lyt::Size &value) {
    mMyTextScale = value;
}
