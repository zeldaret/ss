#ifndef LYT_TEXT_BOX_H
#define LYT_TEXT_BOX_H

#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {

namespace lyt {
namespace res {

struct TextBox : public Pane {
    u16 textBufBytes;  // at 0x4C
    u16 materialIdx;   // at 0x50
    u16 fontIdx;       // at 0x52
    u8 textPosition;   // at 0x54
    u8 textAlignment;  // at 0x55
    u8 padding[2];     // at 0x56
    u32 textStrOffset; // at 0x58
    u32 textCols[2];   // at 0x5C
    Size fontSize;     // at 0x64
    f32 charSpace;     // at 0x6C
    f32 lineSpace;     // at 0x70
};

} // namespace res

class TextBox : public Pane {
public:
    void SetTagProcessor(ut::TagProcessorBase<wchar_t> *pTagProcessor) {
        mpTagProcessor = pTagProcessor;
    }

    TextBox(const res::TextBox *, const ResBlockSet &resBlockSet);
    virtual ~TextBox();
    NW4R_UT_RTTI_DECL(TextBox);

    wchar_t *mTextBuf;                             // at 0xD8 // ptr to is Guess
    ut::Color mTextColors[2];                      // at 0xDC
    ut::Font *mpFont;                              // at 0xE4
    Size mFontSize;                                // at 0xE8
    f32 mLineSpace;                                // at 0xF0
    f32 mCharSpace;                                // at 0xF4
    ut::TagProcessorBase<wchar_t> *mpTagProcessor; // at 0xF8
    u16 mTextBufBytes;                             // at 0xFC
    u16 mTextLen;                                  // at 0xFE
    u8 mTextPosition;                              // at 0x100
    struct {                                       //
        u8 bAllocFont : 1;                         //
        u8 textAligntment : 1;                     //
    } mBits;                                       // at 0x101
};

} // namespace lyt
} // namespace nw4r
#endif
