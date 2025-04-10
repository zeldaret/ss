#ifndef D_LYT_TEXTBOX_H
#define D_LYT_TEXTBOX_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "libms/msgfile.h"
#include "nw4r/lyt/lyt_drawInfo.h"
#include "nw4r/lyt/lyt_textBox.h"
#include "nw4r/lyt/lyt_types.h"
#include "nw4r/ut/ut_Rect.h"
#include "nw4r/ut/ut_TextWriterBase.h"

class dTagProcessor_c;

class dTextBox_c : public nw4r::lyt::TextBox {
    friend class dWindow_c;

public:
    dTextBox_c(const nw4r::lyt::res::TextBox *pBlock, const nw4r::lyt::ResBlockSet &resBlockSet);
    virtual void DrawSelf(const nw4r::lyt::DrawInfo &drawInfo) override; // at 0x18

    f32 GetLineWidth(f32 *pOutSpacing) const;
    f32 GetLinesHeight() const;
    f32 GetUnkWidthRatio() const;

    void setLytBase(d2d::LytBase_c *lytBase) {
        mpLytBase = lytBase;
    }

    void SetScale(f32 scale) {
        nw4r::lyt::Size value(mMyTextScale);
        value.width *= scale;
        value.height *= scale;
        mMyScale = scale;
        MySetScale(value);
        nw4r::lyt::TextBox::SetFontSize(value);
    }

    void set0x1F6(u8 val) {
        field_0x1F6 = val;
    }

    void set0x1F8(u8 val) {
        field_0x1F8 = val;
    }

    f32 getMyScale() const {
        return mMyScale;
    }

    void setMessageWithGlobalTextProcessor2(const char *message, void *unk, ...) {
        va_list l;
        va_start(l, str);
        setMessageWithGlobalTextProcessorV(message, nullptr, unk, l);
        va_end(l);
    }

    void setTextWithGlobalTextProcessor(const wchar_t *str);
    void setTextWithGlobalTextProcessor(const wchar_t *str, void *, ...);
    void setMessageWithGlobalTextProcessor(const char *labelId, void *unused, void *, ...);
    void setMessageWithGlobalTextProcessorV(const char *labelId, void *unused, void *, va_list list);
    void setTextWithTextProcessor(const wchar_t *str, dTagProcessor_c *tagProcessor, void *, ...);
    s32 calcTextLines(const wchar_t *src, dTagProcessor_c *tagProcessor);

    void setMessageWithGlobalTextProcessorAndMsbtInfo(
        const MsbtInfo *info, const char *labelId, wchar_t *destBuf, u32 maxLen
    );

    void loadTextFormatVars();
    void setupGX() const;

    // @bug: This does not implement UT's RTTI, so casts to dTextBox_c will
    // succeed even if all you have is a lyt::TextBox
private:
    void init();
    void MySetFontSize(const nw4r::lyt::Size &value);
    void MySetScale(const nw4r::lyt::Size &value);

    void setTextWithGlobalTextProcessorV(const wchar_t *str, void *, va_list list);
    void setTextWithTextProcessorV(const wchar_t *str, dTagProcessor_c *tagProcessor, void *, va_list list);

    nw4r::ut::Rect GetMyTextDrawRect(nw4r::ut::TextWriterBase<wchar_t> *pWriter, bool *pbWideScreenUnk) const;

    f32 resizeTextToFit(f32 maxWidth);
    bool hasDynamicText();
    void someDebugCheckMaybe();

    int GetStoredLineStrNum(int i, f32 *pLineWidth, bool *pbOver) const {
        *pbOver = mbOvers[i];
        *pLineWidth = mLineWidths[i];
        return mLineStrNums[i];
    }

    void SetStoredLineStrNum(int i, f32 lineWidth, int lineStrNum, bool bOver) {
        mLineWidths[i] = lineWidth;
        mLineStrNums[i] = lineStrNum;
        mbOvers[i] = bOver;
    }

    /* 0x104 */ d2d::LytBase_c *mpLytBase;
    /* 0x108 */ f32 mFixedWidth;
    /* 0x10C */ u8 mIsWidthFixed;
    /* 0x114 */ nw4r::lyt::Size mMyFontSize;
    /* 0x118 */ nw4r::lyt::Size mMyTextScale;
    /* 0x120 */ f32 mMyScale;
    /* 0x124 */ nw4r::ut::TextWriterBase<wchar_t> mTextWriter;
    /* 0x188 */ nw4r::ut::Rect field_0x188;
    /* 0x198 */ u32 mLineStrNums[10];
    /* 0x1C0 */ u8 mFontIdx;
    /* 0x1C1 */ u8 mWindowSubtype;
    /* 0x1C4 */ f32 mLineWidths[10];
    /* 0x1EC */ bool mbOvers[10];
    /* 0x1F6 */ u8 field_0x1F6;
    /* 0x1F7 */ u8 field_0x1F7;
    /* 0x1F8 */ u8 field_0x1F8;
    /* 0x1F9 */ bool field_0x1F9;
    /* 0x1FA */ u8 field_0x1FA;
    /* 0x1FC */ dTagProcessor_c *mpMyTagProcessor;
    /* 0x200 */ bool mHasTextWriter;
    /* 0x201 */ bool field_0x201;
};

#endif
