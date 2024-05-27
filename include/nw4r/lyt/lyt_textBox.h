#ifndef LYT_TEXT_BOX_H
#define LYT_TEXT_BOX_H

#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {

namespace lyt {
class TextBox : public Pane {
public:
    TextBox(u16 allocStrLen);
    TextBox(const wchar_t *str, const ut::Font *pFont);
    TextBox(u16, const wchar_t *str, u16, const ut::Font *pFont);
    TextBox(const res::TextBox *pBlock, const ResBlockSet &resBlockSet);

    void Init(u16 allocStrLen);

    ut::Rect GetTextDrawRect() const;
    ut::Rect GetTextDrawRect(const DrawInfo &drawInfo) const;
    u16 GetStringBufferLength() const;
    u16 SetStringImpl(const wchar_t *str, u16 dstIdx, u32 strLen);
    const ut::Font *GetFont() const;
    void SetFont(const ut::Font *pFont);
    ut::Rect GetTextDrawRect(ut::TextWriterBase<wchar_t> *pWriter) const;
    f32 GetTextMagH() const;
    f32 GetTextMagV() const;
    f32 GetTextAlignMag() const;
    UNKTYPE MakeDrawFlag() const;

    ut::Color GetTextColor(u32 type) const {
        return mTextColors[type / 2];
    }
    void SetTextColor(u32 type, ut::Color value) {
        mTextColors[type] = value;
    }
    u8 GetTextPositionH() const {
        return detail::GetHorizontalPosition(mTextPosition);
    }
    void SetTextPositionH(u8 val) {
        detail::SetHorizontalPosition(&mTextPosition, val);
    }
    u8 GetTextPositionV() const {
        return detail::GetVerticalPosition(mTextPosition);
    }
    void SetTextPositionV(u8 val) {
        detail::SetVerticalPosition(&mTextPosition, val);
    }

    const Size &GetFontSize() const {
        return mFontSize;
    }

    void SetFontSize(const Size &fontSize) {
        mFontSize = fontSize;
    }
    ut::TagProcessorBase<wchar_t> *GetTagProcessor() const {
        return mpTagProcessor;
    }
    void SetTagProcessor(ut::TagProcessorBase<wchar_t> *pTagProcessor) {
        mpTagProcessor = pTagProcessor;
    }

    virtual ~TextBox();
    NW4R_UT_RTTI_DECL(TextBox);
    virtual void DrawSelf(const DrawInfo &drawInfo);                   // at 0x18
    virtual ut::Color GetVtxColor(u32 idx) const;                      // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color value);                // at 0x28
    virtual u8 GetVtxColorElement(u32 idx) const;                      // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value);                // at 0x38
    virtual void LoadMtx(const DrawInfo &drawInfo);                    // at 0x70
    virtual void AllocStringBuffer(u16 minLen);                        // at 0x
    virtual void FreeStringBuffer();                                   // at 0x
    virtual u16 SetString(const wchar_t *str, u16 dstIdx);             // at 0x
    virtual u16 SetString(const wchar_t *str, u16 dstIdx, u16 strLen); // at 0x

protected:
    wchar_t *mTextBuf;                             // at 0xD8 // ptr to is Guess
    ut::Color mTextColors[2];                      // at 0xDC
    const ut::Font *mpFont;                        // at 0xE4
    Size mFontSize;                                // at 0xE8
    f32 mLineSpace;                                // at 0xF0
    f32 mCharSpace;                                // at 0xF4
    ut::TagProcessorBase<wchar_t> *mpTagProcessor; // at 0xF8
    u16 mTextBufBytes;                             // at 0xFC
    u16 mTextLen;                                  // at 0xFE
    u8 mTextPosition;                              // at 0x100
    struct {                                       //
        u8 bAllocFont : 1;                         //
        u8 textAlignment : 2;                      //
    } mBits;                                       // at 0x101
};

} // namespace lyt
} // namespace nw4r
#endif
