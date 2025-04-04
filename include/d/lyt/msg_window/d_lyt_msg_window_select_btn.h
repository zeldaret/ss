#ifndef D_LYT_MSG_WINDOW_SELECT_BTN_H
#define D_LYT_MSG_WINDOW_SELECT_BTN_H

#include "d/d_tag_processor.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

// Size 0x54?
struct SelectBtnHelper {
    SelectBtnHelper();
    ~SelectBtnHelper();

    /* 0x00 */ nw4r::lyt::Pane *panes[9];
    /* 0x24 */ u8 _0x24[0x34 - 0x24];
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ s32 field_0x44;
    /* 0x48 */ s32 field_0x48;
    /* 0x4C */ s32 field_0x4C;
    /* 0x50 */ s8 field_0x50;
    /* 0x51 */ s8 field_0x51;
    /* 0x52 */ s8 field_0x52;

    f32 fn_8011D690(s32);
    void fn_8011C970();
    u8 fn_8011CAC0();
};

class dLytMsgWindowSelectBtnParts_c {
    friend class dLytMsgWindowSelectBtn_c;

public:
    dLytMsgWindowSelectBtnParts_c() : mStateMgr(*this) {}

    void init();
    void execute();

private:
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Wait);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, On);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Select);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtnParts_c, Off);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMsgWindowSelectBtnParts_c);
    /* 0x3C */ d2d::AnmGroup_c *mpAnms[3];
    /* 0x48 */ u32 field_0x48;
    /* 0x4C */ u32 field_0x4C;
    /* 0x50 */ u8 field_0x50;
};

class dLytMsgWindowSelectBtn_c : public d2d::dSubPane {
public:
    dLytMsgWindowSelectBtn_c() : mStateMgr(*this) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const override {
        return mLyt.getName();
    }

    virtual ~dLytMsgWindowSelectBtn_c() {}

    bool draw();

    s32 getField_0x9B4() const {
        return field_0x9B4;
    }

    void setField_0x9B8(s32 value) {
        field_0x9B8 = value;
    }

    void setField_0x9BC(s32 value) {
        field_0x9BC = value;
    }

    void setField_0x990(s32 value) {
        mBtnHelper.field_0x4C = value;
    }

    void setField_0x998(dTagProcessor_c *value) {
        mpTagProcessor = value;
    }

    void fn_8011E5D0(u32, bool);

    dTextBox_c *getSelectTextBox(s32 option, s32 idx) const {
        return mpSelectTextBoxes[option][idx];
    }

    BOOL isStateWait() {
        return *mStateMgr.getStateID() == StateID_Wait;
    }

private:
    s32 fn_8011FE50();

    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, In);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitSelect);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, CursorInOut);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitDecide);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, WaitCancel);
    STATE_FUNC_DECLARE(dLytMsgWindowSelectBtn_c, Out);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMsgWindowSelectBtn_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[27];
    /* 0x798 */ nw4r::lyt::Pane *mpSelectPanes[7];
    /* 0x7B4 */ dTextBox_c *mpSelectTextBoxes[4][2];
    /* 0x7D4 */ dTextBox_c *mpDecideTextBoxes[2];
    /* 0x7DC */ nw4r::lyt::Pane *mpBoundings[4];
    /* 0x7EC */ dWindow_c *mpWindow;
    /* 0x7F0 */ dTextBox_c *mpSizeBox;
    /* 0x7F4 */ dLytMsgWindowSelectBtnParts_c mParts[4];
    /* 0x944 */ SelectBtnHelper mBtnHelper;
    /* 0x998 */ dTagProcessor_c *mpTagProcessor;
    /* 0x99C */ s32 field_0x99C;
    /* 0x9A0 */ s32 field_0x9A0;
    /* 0x9A4 */ u8 field_0x9A4;
    /* 0x9A8 */ s32 field_0x9A8;
    /* 0x9AC */ void *field_0x9AC;
    /* 0x9B0 */ s32 field_0x9B0;
    /* 0x9B0 */ s32 field_0x9B4;
    /* 0x9B8 */ s32 field_0x9B8;
    /* 0x9BC */ s32 field_0x9BC;
    /* 0x9C0 */ s32 field_0x9C0;
    /* 0x9C4 */ void *field_0x9C4;
    /* 0x9C8 */ s32 field_0x9C8;
    /* 0x9CC */ s8 field_0x9CC;
    /* 0x9CD */ u8 field_0x9CD;
    /* 0x9CE */ u8 field_0x9CE;
    /* 0x9CF */ u8 field_0x9CF;
    /* 0x9D0 */ u8 field_0x9D0;
    /* 0x9D1 */ u8 field_0x9D1;
};

#endif
