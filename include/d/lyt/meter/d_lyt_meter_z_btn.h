#ifndef D_LYT_METER_Z_BTN_H
#define D_LYT_METER_Z_BTN_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterZBtn_c : public d2d::dSubPane {
public:
    dLytMeterZBtn_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterZBtn_c() {}

    bool isCalling() const;

private:
    void setMessage(s32 id);

    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Active);
    STATE_FUNC_DECLARE(dLytMeterZBtn_c, Off);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMeterZBtn_c);
    /* 0x04C */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[3];
    /* 0x198 */ nw4r::lyt::Pane *mpPane;
    /* 0x19C */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x1A0 */ dTextBox_c *mpTextBoxes[2];
    /* 0x1A8 */ dWindow_c *mpWindow[1];
    /* 0x1AC */ dTextBox_c *mpSizeBox[1];
    /* 0x1B0 */ s32 field_0x1B0;
    /* 0x1B4 */ s32 field_0x1B4;
    /* 0x1B8 */ BOOL field_0x1B8;
    /* 0x1BC */ s32 field_0x1BC;
    /* 0x1C0 */ s32 mOnDelay;
    /* 0x1C4 */ bool field_0x1C4;
    /* 0x1C5 */ u8 field_0x1C5;
    /* 0x1C6 */ u8 field_0x1C6;
};

#endif
