#ifndef LYT_TEXT_BOX_H
#define LYT_TEXT_BOX_H

#include <nw4r/lyt/lyt_pane.h>

namespace nw4r {

namespace lyt {
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
