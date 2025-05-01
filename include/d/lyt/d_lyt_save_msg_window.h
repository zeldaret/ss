#ifndef D_LYT_SAVE_MSG_WINDOW_H
#define D_LYT_SAVE_MSG_WINDOW_H

#include "d/lyt/d2d.h"
#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dLytSaveMsgWindow_c {
public:
    dLytSaveMsgWindow_c();
    virtual ~dLytSaveMsgWindow_c() {}

    bool build(bool);
    bool remove();
    bool execute();
    bool draw();
    bool drawNow();

    void reset();
    bool setProperties(const char *message, s32, s32, u8);
    bool checkIsWait();
    void setNumLines(s32 numLines) {
        mNumLines = numLines;
    }

    u8 getWillFinishOut() const {
        return mWillFinishOut;
    }

private:
    void resetProperties();
    void setupSaveTextMaybe();
    void updateSaveTextMaybe();
    void updateSaveText();
    void setAnmFrame(s32 group, f32 frame);
    void unbindAnm(s32 group);

    STATE_FUNC_DECLARE(dLytSaveMsgWindow_c, Init);
    STATE_FUNC_DECLARE(dLytSaveMsgWindow_c, In);
    STATE_FUNC_DECLARE(dLytSaveMsgWindow_c, Wait);
    STATE_FUNC_DECLARE(dLytSaveMsgWindow_c, Out);

    /* 0x0004 */ UI_STATE_MGR_DECLARE(dLytSaveMsgWindow_c);
    /* 0x0040 */ d2d::ResAccIf_c mResAcc;
    /* 0x03B0 */ d2d::LytBase_c mLyt;
    /* 0x0440 */ d2d::AnmGroup_c mAnm[9];
    /* 0x0680 */ d2d::SubPaneList mSubpaneList;
    /* 0x068C */ d2d::SubPaneListNode mSubpanes[1];
    /* 0x069C */ dLytMsgWindowSelectBtn_c mBtn;
    /* 0x1070 */ SizedString<64> mLabel;
    /* 0x10B0 */ s32 field_0x10B0;
    /* 0x10B4 */ s32 field_0x10B4;
    /* 0x10B8 */ s32 field_0x10B8;
    /* 0x10BC */ s32 field_0x10BC;
    /* 0x10C0 */ s32 field_0x10C0;
    /* 0x10C4 */ s32 field_0x10C4;
    /* 0x10C8 */ s32 field_0x10C8;
    /* 0x10CC */ s32 mNumLines;
    /* 0x10D0 */ s32 mSaveobjId;
    /* 0x10D4 */ f32 mLineSpace;
    /* 0x10D8 */ u8 field_0x10D8;
    /* 0x10D9 */ u8 mWillFinishOut;
    /* 0x10DA */ u8 field_0x10DA;
    /* 0x10DB */ u8 field_0x10DB;
    /* 0x10DC */ u8 field_0x10DC;
    /* 0x10DD */ u8 field_0x10DD;
};

#endif
