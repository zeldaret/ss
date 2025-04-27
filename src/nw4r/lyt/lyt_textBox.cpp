#include "nw4r/lyt/lyt_textBox.h"

#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/ut.h" // IWYU pragma: export
#include "wstring.h"

namespace nw4r {
namespace lyt {

NW4R_UT_RTTI_DEF_DERIVED(TextBox, Pane);

namespace {
// ReverseYAxis__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPQ34nw4r4math5MTX34
void ReverseYAxis(math::MTX34 *pMtx) {
    // Based on impl in LoadMtx
    pMtx->_01 = -pMtx->_01;
    pMtx->_11 = -pMtx->_11;
    pMtx->_21 = -pMtx->_21;
}

// ClampColor__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@Fs
u8 ClampColor(s16 colVal) {
    // The CLAMP macro was reversed for this -_-
    return colVal < 0 ? 0 : (colVal > 0xFF ? 0xFF : colVal);
}

// GetColor__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FRC11_GXColorS10
ut::Color GetColor(const GXColorS10 &src) {
    GXColor dst;
    dst.r = ClampColor(src.r);
    dst.g = ClampColor(src.g);
    dst.b = ClampColor(src.b);
    dst.a = ClampColor(src.a);
    return dst;
}

// Protos
template <typename T>
void CalcStringRect(ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth);
template <typename T>
int CalcLineStrNum(
    f32 *pWidth, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth, bool *pbOver
);
template <typename T>
int CalcLineRectImpl(
    ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth, bool *pbOver
);
template <typename T>
void CalcStringRectImpl(ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth);

template <typename T>
void CalcStringRect(ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth) {
    ut::TextWriterBase<T> myCopy = *pTextWriter;
    CalcStringRectImpl<T>(pRect, &myCopy, str, length, maxWidth);
}

template <typename T>
int CalcLineStrNum(
    f32 *pWidth, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth, bool *pbOver
) {
    ut::Rect rect;
    ut::TextWriterBase<T> myCopy = *pTextWriter;
    myCopy.SetCursor(0.0f, 0.0f);
    int ret = CalcLineRectImpl(&rect, &myCopy, str, length, maxWidth, pbOver);

    *pWidth = rect.GetWidth();
    return ret;
}

template <typename T>
int CalcLineRectImpl(
    ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth, bool *pbOver
) {
    ut::PrintContext<T> context = {pTextWriter, str, 0.0f, 0.0f, 0};
    f32 x = 0.0f;

    const ut::Font *font = pTextWriter->GetFont();
    bool bCharSpace = false;
    ut::CharStrmReader reader = font->GetCharStrmReader();

    const T *prStrPos = (const T *)reader.GetCurrentPos();

    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = ut::Min(0.0f, pTextWriter->GetLineHeight());
    pRect->bottom = ut::Max(0.0f, pTextWriter->GetLineHeight());

    *pbOver = false;

    reader.Set(str);
    ut::Rect prMaxRect = *pRect;
    u16 code = reader.Next();
    while (((const T *)reader.GetCurrentPos() - str) <= length) {
        if (code < ' ') {
            ut::Operation operation;
            ut::Rect rect(x, 0.0f, 0.0f, 0.0f);

            context.str = (const T *)reader.GetCurrentPos();
            context.flags = !bCharSpace;

            pTextWriter->SetCursorX(x);
            operation = pTextWriter->GetTagProcessor()->CalcRect(&rect, code, &context);
            reader.Set(context.str);

            pRect->left = ut::Min(pRect->left, rect.left);
            pRect->top = ut::Min(pRect->top, rect.top);
            pRect->right = ut::Max(pRect->right, rect.right);
            pRect->bottom = ut::Max(pRect->bottom, rect.bottom);

            x = pTextWriter->GetCursorX();

            if (pRect->GetWidth() > maxWidth) {
                *pbOver = true;
                break;
            }
            if (operation == ut::OPERATION_END_DRAW) {
                return length;
            } else if (operation == ut::OPERATION_NO_CHAR_SPACE) {
                bCharSpace = false;
            } else if (operation == ut::OPERATION_CHAR_SPACE) {
                bCharSpace = true;
            } else if (operation == ut::OPERATION_NEXT_LINE) {
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
            pRect->left = ut::Min(pRect->left, x);
            pRect->right = ut::Max(pRect->right, x);

            if (pRect->GetWidth() > maxWidth) {
                *pbOver = true;
                break;
            }
        }
        prStrPos = (const T *)reader.GetCurrentPos();
        code = reader.Next();
        prMaxRect = *pRect;
    }

    if (*pbOver) {
        if (prStrPos) {
            *pRect = prMaxRect;
            return (prStrPos - str);
        }
    }
    return ((const T *)reader.GetCurrentPos() - str);
}
template <typename T>
void CalcStringRectImpl(ut::Rect *pRect, ut::TextWriterBase<T> *pTextWriter, const T *str, int length, f32 maxWidth) {
    pRect->left = 0.0f;
    pRect->right = 0.0f;
    pRect->top = 0.0f;
    pRect->bottom = 0.0f;
    pTextWriter->SetCursor(0.0f, 0.0f);
    int remain = length;
    const T *pos = str;
    do {
        ut::Rect rect;
        bool bOver;
        int read = CalcLineRectImpl(&rect, pTextWriter, pos, remain, maxWidth, &bOver);
        pos += read;
        remain -= read;
        pRect->left = ut::Min(pRect->left, rect.left);
        pRect->top = ut::Min(pRect->top, rect.top);
        pRect->right = ut::Max(pRect->right, rect.right);
        pRect->bottom = ut::Max(pRect->bottom, rect.bottom);
        if (bOver) {
            CalcLineRectImpl(&rect, pTextWriter, L"\n", 1, maxWidth, &bOver);
            pRect->left = ut::Min(pRect->left, rect.left);
            pRect->top = ut::Min(pRect->top, rect.top);
            pRect->right = ut::Max(pRect->right, rect.right);
            pRect->bottom = ut::Max(pRect->bottom, rect.bottom);
        }
    } while (remain > 0);
}
} // namespace

// __ct__Q34nw4r3lyt7TextBoxFUs
TextBox::TextBox(u16 allocStrLen) {
    // TODO: Not in SS
}

// __ct__Q34nw4r3lyt7TextBoxFUsPCwPCQ34nw4r2ut4Font
TextBox::TextBox(const wchar_t *str, const ut::Font *pFont) {
    // TODO: Not in SS
}

// __ct__Q34nw4r3lyt7TextBoxFUsPCwUsPCQ34nw4r2ut4Font
TextBox::TextBox(u16, const wchar_t *str, u16, const ut::Font *pFont) {
    // TODO: Not in SS
}

// __ct__Q34nw4r3lyt7TextBoxFPCQ44nw4r3lyt3res7TextBoxRCQ34nw4r3lyt11ResBlockSet
TextBox::TextBox(const res::TextBox *pBlock, const ResBlockSet &resBlockSet)
    : Pane(pBlock), mTextColors(), mFontSize() {
    u16 allocStrBufferLen = pBlock->textBufBytes / 2;
    if (allocStrBufferLen > 0) {
        allocStrBufferLen -= 1;
    }
    Init(allocStrBufferLen);
    if (pBlock->textStrBytes >= 2 && mTextBuf) {
        u16 resStringLen = pBlock->textStrBytes / 2 - 1;
        const wchar_t *pBlockText = detail::ConvertOffsToPtr<wchar_t>(pBlock, pBlock->textStrOffset);
        SetString(pBlockText, 0, resStringLen);
    }
    mTextColors[0] = pBlock->textCols[0];
    mTextColors[1] = pBlock->textCols[1];
    mFontSize = pBlock->fontSize;
    mTextPosition = pBlock->textPosition;
    mBits.textAlignment = pBlock->textAlignment;
    mCharSpace = pBlock->charSpace;
    mLineSpace = pBlock->lineSpace;

    int i = pBlock->fontIdx;
    const res::Font *fonts = detail::ConvertOffsToPtr<res::Font>(resBlockSet.pFontList, sizeof(res::FontList));
    const char *fontName = detail::ConvertOffsToPtr<char>(fonts, fonts[i].nameStrOffset);
    ut::Font *pFont = resBlockSet.pResAccessor->GetFont(fontName);
    if (pFont) {
        mpFont = pFont;
    } else {
        void *fontRes = resBlockSet.pResAccessor->GetResource('font', fontName, nullptr);
        if (fontRes) {
            ut::ResFont *pResFont = Layout::NewObj<ut::ResFont>();
            if (pResFont) {
                bool bSucces = pResFont->SetResource(fontRes);
                mpFont = pResFont;
                mBits.bAllocFont = true;
            }
        }
    }
    const u32 *matOffsTbl = detail::ConvertOffsToPtr<u32>(resBlockSet.pMaterialList, sizeof(res::MaterialList));
    const res::Material *pResMaterial =
        detail::ConvertOffsToPtr<res::Material>(resBlockSet.pMaterialList, matOffsTbl[pBlock->materialIdx]);
    mpMaterial = Layout::NewObj<Material>(pResMaterial, resBlockSet);
}

// Init__Q34nw4r3lyt7TextBoxFUs
void TextBox::Init(u16 allocStrLen) {
    mTextBuf = nullptr;
    mTextBufBytes = 0;
    mTextLen = 0;
    mpFont = nullptr;
    mFontSize = Size(0.0f, 0.0f);
    mLineSpace = 0.0f;
    mCharSpace = 0.0f;
    mpTagProcessor = nullptr;
    SetTextPositionH(1);
    SetTextPositionV(1);
    memset(&mBits, 0, sizeof(mBits));
    if (allocStrLen != 0) {
        AllocStringBuffer(allocStrLen);
    }
}

// __dt__Q34nw4r3lyt7TextBoxFv
TextBox::~TextBox() {
    SetFont(nullptr);
    if (mpMaterial && !mpMaterial->IsUserAllocated()) {
        Layout::DeleteObj(mpMaterial);
        mpMaterial = nullptr;
    }
    FreeStringBuffer();
}

// GetVtxColor__Q34nw4r3lyt7TextBoxCFUl
ut::Color TextBox::GetVtxColor(u32 idx) const {
    return GetTextColor(idx);
}

// SetVtxColor__Q34nw4r3lyt7TextBoxFUlQ34nw4r2ut5Color
void TextBox::SetVtxColor(u32 idx, ut::Color value) {
    SetTextColor(idx / 2, value);
}

// GetVtxColorElement__Q34nw4r3lyt7TextBoxCFUl
u8 TextBox::GetVtxColorElement(u32 idx) const {
    return ((u8 *)mTextColors)[((idx / 2) & ~3) + (idx & 3)];
}

// SetVtxColorElement__Q34nw4r3lyt7TextBoxFUlUc
void TextBox::SetVtxColorElement(u32 idx, u8 value) {
    ((u8 *)mTextColors)[((idx / 2) & ~3) + (idx & 3)] = value;
}

// 1] IDK which one is accurate, both symbols exist
// GetTextDrawRect__Q34nw4r3lyt7TextBoxCFv
ut::Rect TextBox::GetTextDrawRect() const {
    ut::TextWriterBase<wchar_t> temp;
    return GetTextDrawRect(&temp);
}

// 2] IDK which one is accurate, both symbols exist
// GetTextDrawRect__Q34nw4r3lyt7TextBoxCFRCQ34nw4r3lyt8DrawInfo
ut::Rect TextBox::GetTextDrawRect(const DrawInfo &drawInfo) const {
    // Not in SS
    return GetTextDrawRect();
}

// DrawSelf__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo
void TextBox::DrawSelf(const DrawInfo &drawInfo) {
    if (!mTextBuf || !mpFont || !mpMaterial) {
        return;
    }
    LoadMtx(drawInfo);
    ut::TextWriterBase<wchar_t> writer;
    ut::Rect textRect = GetTextDrawRect(&writer);
    ut::Color topCol = detail::MultipleAlpha(mTextColors[0], mGlbAlpha);
    ut::Color btmCol = detail::MultipleAlpha(mTextColors[1], mGlbAlpha);
    writer.SetGradationMode(topCol != btmCol ? ut::CharWriter::GRADMODE_V : ut::CharWriter::GRADMODE_NONE);
    writer.SetTextColor(topCol, btmCol);

    ut::Color minCol = GetColor(mpMaterial->GetTevColor(0));
    ut::Color maxCol = GetColor(mpMaterial->GetTevColor(1));
    writer.SetColorMapping(minCol, maxCol);
    writer.SetupGX();
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    f32 hMag = GetTextAlignMag();

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
}

// CalcLineStrNum<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPfPQ34nw4r2ut17TextWriterBase<w>PCwifPb_i
// CalcLineRectImpl<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPQ34nw4r2ut4RectPQ34nw4r2ut17TextWriterBase<w>PCwifPb_i

// GetStringBufferLength__Q34nw4r3lyt7TextBoxCFv
u16 TextBox::GetStringBufferLength() const {
    if (mTextBufBytes == 0) {
        return 0;
    }
    return mTextBufBytes / sizeof(wchar_t) - 1;
}

// AllocStringBuffer__Q34nw4r3lyt7TextBoxFUs
void TextBox::AllocStringBuffer(u16 minLen) {
    if (minLen == 0) {
        return;
    }

    u32 allocLen = minLen;
    u32 texBufBytes = (minLen + 1) * sizeof(wchar_t);

    if (texBufBytes > mTextBufBytes) {
        FreeStringBuffer();
        mTextBuf = Layout::NewArray<wchar_t>(allocLen + 1);
        if (mTextBuf) {
            mTextBufBytes = texBufBytes;
        }
    }
}

// FreeStringBuffer__Q34nw4r3lyt7TextBoxFv
void TextBox::FreeStringBuffer() {
    if (mTextBuf) {
        Layout::DeletePrimArray(mTextBuf);
        mTextBuf = nullptr;
        mTextBufBytes = 0;
    }
}

// SetString__Q34nw4r3lyt7TextBoxFPCwUs
u16 TextBox::SetString(const wchar_t *str, u16 dstIdx) {
    return SetStringImpl(str, dstIdx, wcslen(str));
}

// SetString__Q34nw4r3lyt7TextBoxFPCwUsUs
u16 TextBox::SetString(const wchar_t *str, u16 dstIdx, u16 strLen) {
    return SetStringImpl(str, dstIdx, strLen);
}

// SetStringImpl__Q34nw4r3lyt7TextBoxFPCwUsUl
u16 TextBox::SetStringImpl(const wchar_t *str, u16 dstIdx, u32 strLen) {
    if (!mTextBuf) {
        return 0;
    }
    u16 bufLen = GetStringBufferLength();
    if (dstIdx >= bufLen) {
        return 0;
    }
    u32 cpLen = ut::Min<u32>(strLen, bufLen - dstIdx);
    memcpy(&mTextBuf[dstIdx], str, cpLen * sizeof(wchar_t));
    mTextLen = dstIdx + cpLen;
    mTextBuf[mTextLen] = L'\0';
    return cpLen;
}

// GetFont__Q34nw4r3lyt7TextBoxCFv
const ut::Font *TextBox::GetFont() const {
    return mpFont;
}

// SetFont__Q34nw4r3lyt7TextBoxFPCQ34nw4r2ut4Font
void TextBox::SetFont(const ut::Font *pFont) {
    if (mBits.bAllocFont) {
        Layout::DeleteObj(const_cast<ut::Font *>(mpFont));
        mBits.bAllocFont = false;
    }
    mpFont = pFont;
    if (mpFont) {
        SetFontSize(Size(mpFont->GetWidth(), mpFont->GetHeight()));
    } else {
        SetFontSize(Size(0.0f, 0.0f));
    }
}

// LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo
void TextBox::LoadMtx(const DrawInfo &drawInfo) {
    math::MTX34 mtx;
    if (drawInfo.IsMultipleViewMtxOnDraw()) {
        PSMTXConcat(drawInfo.GetViewMtx(), mGlbMtx, mtx);
    } else {
        PSMTXCopy(mGlbMtx, mtx);
    }

    ReverseYAxis(&mtx);

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

// GetTextDrawRect__Q34nw4r3lyt7TextBoxCFPQ34nw4r2ut17TextWriterBase<w>
// BBA map failed me here xD, probably different impl
ut::Rect TextBox::GetTextDrawRect(ut::TextWriterBase<wchar_t> *pWriter) const {
    ut::Rect textRect;
    pWriter->SetCursor(0.0f, 0.0f);
    pWriter->SetFont(*mpFont);
    pWriter->SetFontSize(mFontSize.width, mFontSize.height);
    pWriter->SetLineSpace(mLineSpace);
    pWriter->SetCharSpace(mCharSpace);
    if (mpTagProcessor) {
        pWriter->SetTagProcessor(mpTagProcessor);
    }
    CalcStringRect(&textRect, pWriter, mTextBuf, mTextLen, mSize.width);

    Size textSize(textRect.GetWidth(), textRect.GetHeight());
    math::VEC2 ltPos = GetVtxPos();

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

// CalcStringRect<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPQ34nw4r2ut4RectPQ34nw4r2ut17TextWriterBase<w>PCwif_v
// CalcStringRectImpl<w>__Q34nw4r3lyt25@unnamed@lyt_textBox_cpp@FPQ34nw4r2ut4RectPQ34nw4r2ut17TextWriterBase<w>PCwif_v

// GetTextMagH__Q34nw4r3lyt7TextBoxCFv
f32 TextBox::GetTextMagH() const {
    switch (GetTextPositionH()) {
        default: return 0.0f;
        case 1:  return 0.5f;
        case 2:  return 1.0f;
    }
}

// GetTextMagV__Q34nw4r3lyt7TextBoxCFv
f32 TextBox::GetTextMagV() const {
    switch (GetTextPositionV()) {
        default: return 0.0f;
        case 1:  return 0.5f;
        case 2:  return 1.0f;
    }
}

// GetTextAlignMag__Q34nw4r3lyt7TextBoxCFv
f32 TextBox::GetTextAlignMag() const {
    switch (mBits.textAlignment) {
        default: return GetTextMagH();
        case 1:  return 0.0f;
        case 2:  return 0.5f;
        case 3:  return 1.0f;
    }
}

// MakeDrawFlag__Q34nw4r3lyt7TextBoxCFv
UNKTYPE TextBox::MakeDrawFlag() const {
    // TODO: Not In SS
    // Calls GetTextPositionH()
}

} // namespace lyt
} // namespace nw4r
