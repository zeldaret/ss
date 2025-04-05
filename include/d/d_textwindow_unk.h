#ifndef D_TEXTWINDOW_UNK_H
#define D_TEXTWINDOW_UNK_H

#include "d/d_tag_processor.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "sized_string.h"
#include "toBeSorted/lyt_vec2f.h"

struct dLytMsgWindowCharData {
    /* 0x00 */ f32 posX;
    /* 0x04 */ f32 posY;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ s16 field_0x0C;
    /* 0x0E */ wchar_t character;
    /* 0x10 */ s8 displayTimerMaybe;

    void reset() {
        posX = 0.0f;
        posY = 0.0f;
        field_0x08 = dTagProcessor_c::fn_800B8040(0, 0);
        character = L'\0';
        field_0x0C = 1023;
        displayTimerMaybe = -1;
    }
};

// Size 0x1554
// Probably responsible for emitting characters
// one by one in the text boxes
class TextWindowUnk {
public:
    TextWindowUnk(dTagProcessor_c *tagProcessor);
    virtual ~TextWindowUnk() {}

    bool fn_800B2130(const char *, dTextBox_c *, dLytMsgWindowCharData *data, bool);
    void fn_800B2AA0();
    void textAdvancingRelated(bool, bool);

    const wchar_t *getProcessedText() const {
        return mProcessedTextBuffer;
    }

    u16 getField_0x147A() const {
        return field_0x147A;
    }

    u16 getField_0x147C() const {
        return field_0x147C;
    }

    bool checkEndReached() const;
    bool checkLastLineReached();
    bool reset();

private:
    /* 0x0004 */ dTagProcessor_c *mpTagProcessor;
    /* 0x0008 */ wchar_t mRawTextBuffer[1023];
    /* 0x0806 */ wchar_t mProcessedTextBuffer[1023];
    /* 0x1004 */ wchar_t mUnkBuffer[511]; // ???
    /* 0x1402 */ SizedString<64> field_0x1402;
    /* 0x1442 */ u8 _0x1442[0x144C - 0x1442];

    /* 0x144C */ s32 mRawTextPos;
    /* 0x1450 */ s32 mRawTextEndPos;
    /* 0x1454 */ s32 mCurrentLine;
    /* 0x1458 */ s32 field_0x1458;
    /* 0x145C */ u8 _0x145C[0x1478 - 0x145C];
    /* 0x1478 */ u16 field_0x1478;
    /* 0x147A */ u16 field_0x147A;
    /* 0x147C */ u16 field_0x147C;
    /* 0x147E */ u8 _0x147E[0x1480 - 0x147E];
    /* 0x1480 */ dTextBox_c *mpTextBox;
    /* 0x1484 */ LineData mLineWidths;
    /* 0x1550 */ dLytMsgWindowCharData *mpCharacterData;
};

class MsgWindowBlurRelated {
public:
    MsgWindowBlurRelated() {}
    virtual ~MsgWindowBlurRelated() {}

    void build();
    void remove();
    void drawTextBlur(dLytTextLight *thing, LytVec2f size, u8 alpha);
    void reset();

    dLytMsgWindowCharData *getCharData() {
        return mData;
    }

private:
    dLytMsgWindowCharData mData[1023];
};

#endif
