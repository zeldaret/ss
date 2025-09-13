#ifndef D_LYT_MSG_WINDOW_H
#define D_LYT_MSG_WINDOW_H

#include "d/d_tag_processor.h"
#include "d/d_textwindow_unk.h"
#include "d/lyt/d2d.h"
#include "d/lyt/msg_window/d_lyt_msg_window_common.h"
#include "d/lyt/msg_window/d_lyt_msg_window_select_btn.h"
#include "m/m_vec.h"
#include "s/s_State.hpp"
#include "sized_string.h"

// Forward declarations required for vtable order
class dLytMsgWindowTalk_c;
class dLytMsgWindowLink_c;
class dLytMsgWindowGet_c;
class dLytMsgWindowSword_c;
class dLytMsgWindowWood_c;
class dLytMsgWindowStone_c;
class dLytMsgWindowDemo_c;
class dLytAutoExplain_c;
class dLytAutoCaption_c;

struct dLytMsgWindowCharData;

class dLytMsgWindow_c {
public:
    enum MsgWindow_e {
        MSG_WINDOW_0 = 0,
        MSG_WINDOW_1 = 1,

        MSG_WINDOW_SWORD_FI = 2,
        MSG_WINDOW_SWORD_GHIRAHIM = 3,
        MSG_WINDOW_SWORD_LASTBOSS = 4,
        MSG_WINDOW_GET = 5,
        MSG_WINDOW_WOOD = 6,
        MSG_WINDOW_STONE = 7,

        MSG_WINDOW_8 = 8,

        MSG_WINDOW_LINK = 9,

        MSG_WINDOW_10 = 10,

        MSG_WINDOW_22 = 22,

        MSG_WINDOW_DEMO = 30,
        MSG_WINDOW_31 = 31,

        MSG_WINDOW_34 = 34,

        // Maybe none
        MSG_WINDOW_36 = 36,
    };

    dLytMsgWindow_c() : mStateMgr(*this) {}
    virtual ~dLytMsgWindow_c() {}

    bool build();
    bool remove();
    bool execute();
    bool draw();

    bool isVisible() const;
    bool isOutputtingText() const;

    bool setCurrentLabelName(const char *name, bool storeFile);
    void setCurrentEntrypointName(const char *name);
    void setCurrentFlowFilename(const char *name);

    void setNumericArg0(s32 arg);
    void setNumericArgs(s32 *arg, s32 argCount);

    static dLytMsgWindow_c *getInstance() {
        return sInstance;
    }

    SizedString<64> getLabel() const {
        return mNameCopy;
    }

    const char *fn_80117390(bool) const;

    // This function appears to be related to a compiler quirk.
    // 0x800D7B40 is in d_lyt_meter, but calling a static method
    // on an instance via dLytMsgWindow_c::getInstance()->fn_800D7B40()
    // causes the method to be emitted there.
    static u16 fn_800D7B40() {
        return sInstance->mEntryPointToTrigger;
    }

    dTagProcessor_c *getTagProcessor() const {
        return mpTagProcessor;
    }

    s32 getMsgIdx() const {
        return mMsgIdx;
    }

    u8 getField_0x80D() const {
        return field_0x80D;
    }

    u8 getField_0x815() const {
        return field_0x815;
    }

    s32 getTextOptionSelection() const {
        return mTextOptionSelection;
    }

    void onFlag0x820(u16 flag) {
        field_0x820 |= flag;
    }

private:
    bool setTextToDisplay(const wchar_t *text);
    void createSubMsgManager(u8 type);
    void removeSubMsgManagers();
    bool fn_8011A5D0() const;

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
    /* 0x72C */ dLytMsgWindowTalk_c *mpWindowTalk;
    /* 0x730 */ dLytMsgWindowLink_c *mpWindowLink;
    /* 0x734 */ dLytMsgWindowSword_c *mpWindowSword;
    /* 0x738 */ dLytMsgWindowWood_c *mpWindowWood;
    /* 0x73C */ dLytMsgWindowStone_c *mpWindowStone;
    /* 0x740 */ dLytMsgWindowGet_c *mpWindowGet;
    /* 0x744 */ dLytMsgWindowDemo_c *mpWindowDemo;
    /* 0x748 */ dLytAutoExplain_c *mpAutoExplain;
    /* 0x74C */ dLytAutoCaption_c *mpAutoCaption;

    /* 0x750 */ u8 _0x750[0x75C - 0x750];
    /* 0x75C */ mVec3_c field_0x75C;
    /* 0x768 */ mVec3_c field_0x768;
    /* 0x774 */ s32 field_0x774;
    /* 0x778 */ s32 field_0x778;
    /* 0x77C */ s32 field_0x77C;
    /* 0x780 */ s32 field_0x780;
    /* 0x784 */ s32 mMsgIdx;

    /* 0x788 */ u16 mEntryPointToTrigger;
    /* 0x78A */ u16 mAlsoEntryPointToTrigger;

    /* 0x78C */ SizedString<0x40> mName;
    /* 0x7CC */ SizedString<0x40> mNameCopy;

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
    /* 0x824 */ s32 field_0x824;
    /* 0x828 */ dLytMsgWindowCharData *field_0x828;

    /* 0x82C */ u32 mTextOptionSelection;
    /* 0x830 */ u32 mSpecialFiMenuValue;

    /* 0x834 */ dLytMsgWindowSelectBtn_c mSelectBtn;
    /* 0x1208 */ SizedString<8> mCurrentEntrypointName;
    /* 0x1210 */ SizedString<16> mCurrentFlowFileName;
    /* 0x1220 */ u8 field_0x1220;
};

#endif
