#ifndef D_LYT_METER_A_BTN_H
#define D_LYT_METER_A_BTN_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytMeterABtn_c : public d2d::dSubPane {
public:
    dLytMeterABtn_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytMeterABtn_c() {}
    void setContainerGroups(d2d::AnmGroup_c *g1, d2d::AnmGroup_c *g2) {
        mpContainerAnmGroup1 = g1;
        mpContainerAnmGroup2 = g2;
    }

    void setOwnerPane(nw4r::lyt::Pane *pane) {
        mpOwnerPane = pane;
    }

private:
    void setMessage(s32 id);

    STATE_FUNC_DECLARE(dLytMeterABtn_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, In);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Out);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Active);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Off);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterABtn_c);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytMeterABtn_c);
    /* 0x044 */ d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnm[3];
    /* 0x198 */ d2d::AnmGroup_c *mpContainerAnmGroup1;
    /* 0x19C */ d2d::AnmGroup_c *mpContainerAnmGroup2;
    /* 0x1A0 */ nw4r::lyt::Pane *mpPane;
    /* 0x1A4 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x1A8 */ dTextBox_c *mpTextBoxes[2];
    /* 0x1B0 */ dWindow_c *mpWindow[1];
    /* 0x1B4 */ dTextBox_c *mpSizeBox[1];
    /* 0x1B8 */ s32 field_0x1B8;
    /* 0x1BC */ s32 field_0x1BC;
    /* 0x1C0 */ BOOL field_0x1C0;
    /* 0x1C4 */ s32 field_0x1C4;
    /* 0x1C8 */ u8 field_0x1C8;
    /* 0x1C9 */ u8 field_0x1C9;
    /* 0x1CA */ u8 field_0x1CA;
    /* 0x1CB */ bool mOldTextState;
};

#endif
