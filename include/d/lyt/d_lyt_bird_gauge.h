#ifndef D_LYT_BIRD_GAUGE_H
#define D_LYT_BIRD_GAUGE_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytBirdGaugeMain_c {
public:
    dLytBirdGaugeMain_c();
    virtual ~dLytBirdGaugeMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool execute();
    bool remove();

    const d2d::dLytSub &getLyt() const {
        return mLyt;
    }

    d2d::dLytSub &getLyt() {
        return mLyt;
    }

    void cancelAnimation();

    bool isAnimating() const {
        return mIsAnimating;
    }

    s32 getDisplayedDashes() const {
        return mNumDisplayedDashes;
    }

    void changeToIn();
    void changeToDash();
    void changeToRecovery();
    void changeToOut();

private:
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeDash);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeRecovery);
    STATE_FUNC_DECLARE(dLytBirdGaugeMain_c, ModeOut);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytBirdGaugeMain_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[8];
    /* 0x2D4 */ bool mIsAnimating;
    /* 0x2D8 */ s32 mNumDisplayedDashes;
};

class dLytBirdGauge_c : public d2d::dSubPane {
public:
    dLytBirdGauge_c() : mStateMgr(*this, sStateID::null) {}
    virtual bool build(d2d::ResAccIf_c *resAcc) override;
    virtual bool remove() override;
    virtual bool execute() override;
    virtual nw4r::lyt::Pane *getPane() override {
        return mMain.getLyt().getLayout()->GetRootPane();
    }
    virtual d2d::LytBase_c *getLyt() override {
        return &mMain.getLyt();
    }
    virtual const char *getName() const override {
        return mMain.getLyt().getName();
    }

    virtual ~dLytBirdGauge_c() {}

    void hide();

    void setNumDashes(s32 num) {
        mGameStateDashes = num;
    }

    void setField_0x690(bool val) {
        field_0x690 = val;
    }

    bool getField_0x692() const {
        return field_0x692;
    }

    bool getField_0x693() const {
        return field_0x693;
    }

private:
    static dLytBirdGauge_c *sInstance;

    STATE_FUNC_DECLARE(dLytBirdGauge_c, In);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, None);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Dash);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Recovery);
    STATE_FUNC_DECLARE(dLytBirdGauge_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytBirdGauge_c);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytBirdGauge_c);
    /* 0x008 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B4 */ dLytBirdGaugeMain_c mMain;
    /* 0x690 */ bool field_0x690;
    /* 0x691 */ bool field_0x691;
    /* 0x692 */ bool field_0x692;
    /* 0x693 */ bool field_0x693;
    /* 0x694 */ s32 mGameStateDashes;
};

#endif
