#ifndef D_LYT_METER_PARTS_H
#define D_LYT_METER_PARTS_H

#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_pane.h"
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

    bool shouldCall() const;

    void setOwnerPane(nw4r::lyt::Pane *pane) {
        mpOwnerPane = pane;
    }

private:
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter1Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter1Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnm[3];
    /* 0x198 */ dWindow_c *mpWindow[1];
    /* 0x19C */ dTextBox_c *mpSizeBox[1];
    /* 0x1A0 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x1A4 */ s32 mUnuseDelay;
    /* 0x1A8 */ s32 mCallCount;
    /* 0x1AC */ bool mShouldCall;
    /* 0x1AD */ bool mHasInitedCall;
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

    bool shouldCall() const;

    void setOwnerPane(nw4r::lyt::Pane *pane) {
        mpOwnerPane = pane;
    }

private:
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Wait);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, ToUnuse);
    STATE_FUNC_DECLARE(dLytMeter2Button_c, Unuse);

    UI_STATE_MGR_DECLARE(dLytMeter2Button_c);
    d2d::dLytSub mLyt;
    d2d::AnmGroup_c mAnm[3];
    /* 0x198 */ dWindow_c *mpWindow[1];
    /* 0x19C */ dTextBox_c *mpSizeBox[1];
    /* 0x1A0 */ nw4r::lyt::Pane *mpOwnerPane;
    /* 0x1A4 */ s32 mUnuseDelay;
    /* 0x1A8 */ s32 mCallCount;
    /* 0x1AC */ bool mShouldCall;
    /* 0x1AD */ bool mHasInitedCall;
};

class dLytMeterParts_c {
public:
    dLytMeterParts_c() : mStateMgr(*this, sStateID::null) {}

    void build(s32 index);
    void execute();

    void setAnmGroups(d2d::AnmGroup_c *inAnm, d2d::AnmGroup_c *outAnm) {
        mpAnmIn = inAnm;
        mpAnmOut = outAnm;
    }

    void setShouldBeVisible(bool val) {
        mShouldBeVisible = val;
    }

private:
    STATE_FUNC_DECLARE(dLytMeterParts_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, In);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterParts_c, Out);

    /* 0x00 */ UI_STATE_MGR_DECLARE(dLytMeterParts_c);
    /* 0x3C */ d2d::AnmGroup_c *mpAnmIn;
    /* 0x40 */ d2d::AnmGroup_c *mpAnmOut;
    /* 0x44 */ s32 mIndex;
    /* 0x48 */ bool mShouldBeVisible;
};

#endif
