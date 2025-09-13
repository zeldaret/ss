#ifndef D_LYT_PAUSE_TEXT_H
#define D_LYT_PAUSE_TEXT_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_a_btn.h"
#include "s/s_State.hpp"
#include "sized_string.h"

/** 2D UI - Pause menu - item info popup */
class dLytPauseText_c {
public:
    static const char *getCurrentPauseLabel(SizedString<32> &buf);
    static const wchar_t *getCurrentPausePopupName();
    static const wchar_t *getCurrentPausePopupCaption();

    dLytPauseText_c();
    virtual ~dLytPauseText_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    void requestIn();
    void requestOut();

    bool isChangingState() const {
        return mIsChangingState;
    }

private:
    void startAnim(int idx);
    void stopAnim(int idx);
    void updateText();

    bool isCurrentItemWallet() const;
    void calcTextLines(dTextBox_c *textBox, const wchar_t *text);

    STATE_FUNC_DECLARE(dLytPauseText_c, None);
    STATE_FUNC_DECLARE(dLytPauseText_c, In);
    STATE_FUNC_DECLARE(dLytPauseText_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseText_c, Out);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytPauseText_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[3];
    /* 0x190 */ d2d::ResAccIf_c mResAcc;
    /* 0x500 */ dLytCommonABtn_c mABtn;
    /* 0x6B0 */ d2d::SubPaneList mSubpaneList;
    /* 0x6BC */ d2d::SubPaneListNode mSubpanes[1];
    /* 0x6CC */ s32 mStep;
    /* 0x6D0 */ f32 mLineSpace;
    /* 0x6D4 */ u8 mNumTextLines;
    /* 0x6D5 */ bool mInRequest;
    /* 0x6D6 */ bool mOutRequest;
    /* 0x6D7 */ bool mIsChangingState;
    /* 0x6D8 */ bool field_0x6D8;
};

#endif
