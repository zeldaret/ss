#ifndef D_TEXTWINDOW_UNK_H
#define D_TEXTWINDOW_UNK_H

#include "d/d_tag_processor.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "toBeSorted/lyt_vec2f.h"

// Size 0x1554
// Probably responsible for emitting characters
// one by one in the text boxes
class TextWindowUnk {
public:
    TextWindowUnk(dTagProcessor_c *tagProcessor);
    virtual ~TextWindowUnk() {}

    void fn_800B2130(const char *, dTextBox_c *, void *, bool);
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
    bool checkLastLineReachedMaybe() const;

private:
    /* 0x0004 */ dTagProcessor_c *mpTagProcessor;
    /* 0x0008 */ wchar_t mRawTextBuffer[1023];
    /* 0x0806 */ wchar_t mProcessedTextBuffer[1023];
    /* 0x1004 */ wchar_t mUnkBuffer[571]; // ???
    /* 0x147A */ u16 field_0x147A;
    /* 0x147C */ u16 field_0x147C;
    /* 0x147E */ u8 _0x147E[0x1550 - 0x147E];
    /* 0x1550 */ void *field_0x1550;
};

struct dLytMsgWindowCharData {
    /* 0x00 */ f32 posX;
    /* 0x04 */ f32 posY;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ s16 field_0x0C;
    /* 0x0E */ wchar_t character;
    /* 0x10 */ s8 displayTimerMaybe;
};

class MsgWindowBlurRelated {
public:
    MsgWindowBlurRelated() {}
    virtual ~MsgWindowBlurRelated() {}

    void build();
    void remove();
    void drawTextBlur(dLytTextLight *thing, LytVec2f size, u8 alpha);

    dLytMsgWindowCharData *getCharData() {
        return mData;
    }

private:
    dLytMsgWindowCharData mData[1023];
};

#endif
