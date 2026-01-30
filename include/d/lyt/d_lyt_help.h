#ifndef D_LYT_HELP_H
#define D_LYT_HELP_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "m/m2d.h"
#include "s/s_State.hpp"

class dLytHelpMain_c {
public:
    dLytHelpMain_c();
    virtual ~dLytHelpMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

    void modeRequestIn(s32 textIndex);
    void modeRequestWait();
    void modeRequestOut();
    void modeRequestEnd();
    void modeRequestNone();

    bool isIdle() const {
        return mIsIdle;
    }

    void setText(s32 textIndex);
private:

    STATE_FUNC_DECLARE(dLytHelpMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytHelpMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytHelpMain_c, ModeWait);
    STATE_FUNC_DECLARE(dLytHelpMain_c, ModeOut);
    STATE_FUNC_DECLARE(dLytHelpMain_c, ModeEnd);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytHelpMain_c);
    /* 0x040 */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[3];
    /* 0x190 */ dTextBox_c *mpTextBoxesTitleS[3];
    /* 0x19C */ dTextBox_c *mpTextBoxesTitle[3];
    /* 0x1A8 */ dTextBox_c *mpTextBoxesS[3];
    /* 0x1B4 */ dTextBox_c *mpTextBoxes[3];
    /* 0x1C0 */ bool mIsIdle;
    /* 0x1C4 */ s32 mTextIndex;
    /* 0x1C8 */ s32 field_0x1C8;
    /* 0x1CC */ s32 field_0x1CC;
};

class dLytHelp_c : public m2d::Base_c {
public:
    dLytHelp_c() : mStateMgr(*this) {
        sInstance = this;
    }
    virtual ~dLytHelp_c() {
        sInstance = nullptr;
    }
    virtual void draw() override;
    virtual bool build();
    virtual bool remove();
    virtual bool execute();

    bool setText(s32 textIndex);
    void changeText(s32 textIndex);

    static dLytHelp_c *GetInstance() {
        return sInstance;
    }

    bool isStateEnd() const {
        return *mStateMgr.getStateID() == StateID_End;
    }

    void requestOut() {
        mOutRequest = true;
    }

    bool isWaiting() const {
        return mIsWaiting;
    }

private:
    static dLytHelp_c *sInstance;

    STATE_FUNC_DECLARE(dLytHelp_c, None);
    STATE_FUNC_DECLARE(dLytHelp_c, In);
    STATE_FUNC_DECLARE(dLytHelp_c, Wait);
    STATE_FUNC_DECLARE(dLytHelp_c, Out);
    STATE_FUNC_DECLARE(dLytHelp_c, End);

    STATE_MGR_DEFINE_UTIL_ISSTATE(dLytHelp_c);

    /* 0x010 */ UI_STATE_MGR_DECLARE(dLytHelp_c);
    /* 0x04C */ d2d::ResAccIf_c mResAcc;
    /* 0x3BC */ dLytHelpMain_c *mpMain;
    /* 0x3C0 */ bool field_0x3C0;
    /* 0x3C1 */ bool mOutRequest;
    /* 0x3C2 */ bool mIsWaiting;
    /* 0x3C4 */ s32 mTextIndex;
    /* 0x3C8 */ s32 field_0x3C8;
};

#endif
