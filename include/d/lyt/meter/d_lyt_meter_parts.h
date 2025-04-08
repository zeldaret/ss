#ifndef D_LYT_METER_PARTS_H
#define D_LYT_METER_PARTS_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytMeter1Button_c : public d2d::dSubPane {
public:
    dLytMeter1Button_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeter1Button_c() {}
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

private:
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter1Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnmGroups[3];
    /* 0x198 */ dWindow_c *mpWindow;
    /* 0x19C */ dTextBox_c *mpTextbox;
    /* 0x1A0 */ u32 field_0x1A0;
    /* 0x1A4 */ s32 field_0x1A4;
    /* 0x1A8 */ s32 field_0x1A8;
    /* 0x1AC */ u8 field_0x1AC;
    /* 0x1AD */ u8 field_0x1AD;
};

class dLytMeter2Button_c : public d2d::dSubPane {
public:
    dLytMeter2Button_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeter2Button_c() {}
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

private:
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter2Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnmGroups[3];
    /* 0x198 */ dWindow_c *mpWindow;
    /* 0x19C */ dTextBox_c *mpTextbox;
    /* 0x1A0 */ u32 field_0x1A0;
    /* 0x1A4 */ s32 field_0x1A4;
    /* 0x1A8 */ s32 field_0x1A8;
    /* 0x1AC */ u8 field_0x1AC;
    /* 0x1AD */ u8 field_0x1AD;
};

class dLytMeterParts_c {
public:
    dLytMeterParts_c() : mStateMgr(*this, sStateID::null) {}

private:
    STATE_FUNC_DECLARE(dLytMeterParts_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, In);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Out);

    UI_STATE_MGR_DECLARE(dLytMeterParts_c);
    u8 field_0x3C[0x10];
};

#endif
