#ifndef D_LYT_METER_A_BTN_H
#define D_LYT_METER_A_BTN_H

#include <d/lyt/d2d.h>
#include <d/lyt/meter/d_lyt_meter_base.h>
#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>

class dLytMeterABtn_c : public dLytMeterBase {
public:
    dLytMeterABtn_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool LytMeter0x14() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mLyt.getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mLyt;
    }
    virtual const char *getName() const {
        return mLyt.getName();
    }

    virtual ~dLytMeterABtn_c() {}
    void setContainerGroups(d2d::AnmGroup_c *g1, d2d::AnmGroup_c *g2) {
        mpContainerAnmGroup1 = g1;
        mpContainerAnmGroup2 = g2;
    }
private:
    void fn_800E0870(u32);

    STATE_FUNC_DECLARE(dLytMeterABtn_c, InvisibleWait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, In);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Out);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Wait);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, On);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Active);
    STATE_FUNC_DECLARE(dLytMeterABtn_c, Off);

    UI_STATE_MGR_DECLARE(dLytMeterABtn_c);
    d2d::dLytSub mLyt;
    /* 0x0D8 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x198 */ d2d::AnmGroup_c *mpContainerAnmGroup1;
    /* 0x19C */ d2d::AnmGroup_c *mpContainerAnmGroup2;
    /* 0x1A0 */ nw4r::lyt::Pane *mpPane;
    /* 0x1A4 */ void *field_0x1A4;
    /* 0x1A8 */ dTextBox_c *mpTextBoxes[2];
    /* 0x1B0 */ dWindow_c *mpWindow;
    /* 0x1B4 */ dTextBox_c *mpSizeBox;
    /* 0x1B8 */ u32 field_0x1B8;
    /* 0x1BC */ u32 field_0x1BC;
    /* 0x1C0 */ u32 field_0x1C0;
    /* 0x1C4 */ u32 field_0x1C4;
    /* 0x1C8 */ u8 field_0x1C8;
    /* 0x1C9 */ u8 field_0x1C9;
    /* 0x1CA */ u8 field_0x1CA;
    /* 0x1CB */ u8 field_0x1CB;
};

#endif
