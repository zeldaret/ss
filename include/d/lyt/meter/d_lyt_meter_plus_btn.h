#ifndef D_LYT_METER_PLUS_BTN_H
#define D_LYT_METER_PLUS_BTN_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

class dLytMeterPlusBtn_c : public d2d::dSubPane {
public:
    dLytMeterPlusBtn_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterPlusBtn_c() {}

    bool isCalling() const;
    void setCall(bool shouldCall);

private:
    void setMessage(s32 id);
    bool isMapOpen() const;
    static bool hasSpecificMapMark();

    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeterPlusBtn_c, Unuse);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterPlusBtn_c);

    /* 0x00C */ UI_STATE_MGR_DECLARE(dLytMeterPlusBtn_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[3];
    /* 0x198 */ nw4r::lyt::Pane *mpPane[1];
    /* 0x19C */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x1A0 */ dTextBox_c *mpTextBoxes[2];
    /* 0x1A8 */ dWindow_c *mpWindow[1];
    /* 0x1AC */ dTextBox_c *mpSizeBox[1];
    /* 0x1B0 */ s32 field_0x1B0;
    /* 0x1B4 */ s32 field_0x1B4;
    /* 0x1B8 */ s32 mCallCount;
    /* 0x1BC */ s32 field_0x1BC;
    /* 0x1C0 */ bool field_0x1C0;
    /* 0x1C1 */ bool field_0x1C1;
    /* 0x1C2 */ bool mShouldCall;
    /* 0x1C3 */ bool field_0x1C3;
};

#endif
