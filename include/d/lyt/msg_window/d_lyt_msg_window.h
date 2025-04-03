#ifndef D_LYT_MSG_WINDOW_H
#define D_LYT_MSG_WINDOW_H

#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_auto_caption.h"
#include "d/lyt/d_lyt_auto_explain.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"
#include "d/lyt/msg_window/d_lyt_msg_window_stone.h"
#include "d/lyt/msg_window/d_lyt_msg_window_wood.h"
#include "s/s_State.hpp"
#include "sized_string.h"

class dLytMsgWindow_c {
public:
    dLytMsgWindow_c() : mStateMgr(*this) {}
    virtual ~dLytMsgWindow_c() {}

    bool build();
    bool remove();

private:

    void removeAllWindowSubtypes();

    static dLytMsgWindow_c *sInstance;

    STATE_FUNC_DECLARE(dLytMsgWindow_c, Invisible);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, OutputText);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyChangePage0);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyChangePage1);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyMsgEnd0);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyMsgEnd1);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyMsgEnd2);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeySelectQuestion);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, MapOpen);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, WaitKeyMapClose);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, MapClose);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, Out);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, ExplainIn);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, ExplainVisible);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, ExplainOut);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, DemoIn);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, DemoVisible);
    STATE_FUNC_DECLARE(dLytMsgWindow_c, DemoOut);


    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMsgWindow_c);
    /* 0x040 */ d2d::ResAccIf_c mResAcc1;
    /* 0x3B0 */ d2d::ResAccIf_c mResAcc2;

    /* 0x720 */ TextWindowUnk *mpMsgWindowUnk;
    /* 0x724 */ dTagProcessor_c *mpTagProcessor;

    /* 0x728 */ dLytMsgWindowSubtype *mpCurrentSubtype;
    /* 0x72C */ dLytMsgWindowSubtype *mpWindowTalk;
    /* 0x730 */ dLytMsgWindowSubtype *mpWindowLink;
    /* 0x734 */ dLytMsgWindowSubtype *mpWindowSword;
    /* 0x738 */ dLytMsgWindowWood_c *mpWindowWood;
    /* 0x73C */ dLytMsgWindowStone_c *mpWindowStone;
    /* 0x740 */ dLytMsgWindowSubtype *mpWindowGet;
    /* 0x744 */ dLytMsgWindowSubtype *mpWindowDemo;
    /* 0x748 */ dLytAutoExplain_c *mpAutoExplain;
    /* 0x74C */ dLytAutoCaption_c *mpAutoCaption;

    /* 0x774 */ s32 field_0x774;
    /* 0x778 */ s32 field_0x778;
    /* 0x77C */ s32 field_0x77C;
    /* 0x780 */ s32 field_0x780;
    /* 0x784 */ s32 field_0x784;

    /* 0x788 */ u16 mEntryPointToTrigger;
    /* 0x78A */ u16 mAlsoEntryPointToTrigger;

    /* 0x78C */ SizedString<0x40> mName;

    /* 0x80C */ u8 field_0x80C;
    /* 0x80D */ u8 field_0x80D;
    /* 0x80E */ u8 field_0x80E;
    /* 0x80F */ u8 mShowAutoMessage;
    /* 0x810 */ u8 field_0x810;
    /* 0x811 */ u8 field_0x811;
    /* 0x812 */ u8 field_0x812;
    /* 0x813 */ u8 field_0x813;
    /* 0x814 */ u8 field_0x814;
    /* 0x815 */ u8 field_0x815;
    /* 0x816 */ u8 field_0x816;
    /* 0x817 */ u8 field_0x817;
    /* 0x818 */ u8 field_0x818;
    /* 0x819 */ u8 field_0x819;
    /* 0x81A */ u8 field_0x81A;
    /* 0x81B */ u8 field_0x81B;
    /* 0x81C */ u8 field_0x81C;
    /* 0x81D */ u8 field_0x81D;
    /* 0x81E */ u8 field_0x81E;

    /* 0x820 */ u16 field_0x820;
    /* 0x824 */ void *field_0x824;
    /* 0x828 */ s32 field_0x828;

    /* 0x82C */ u32 mTextOptionSelection;
    /* 0x830 */ u32 mSpecialFiMenuValue;

    /* 0x834 */ dLytMsgWindowSelectBtn_c mSelectBtn;
    /* 0x1220 */ u8 field_0x1220;
};

#endif
