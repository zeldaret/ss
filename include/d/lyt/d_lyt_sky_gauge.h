#ifndef D_LYT_SKY_GAUGE_H
#define D_LYT_SKY_GAUGE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"

class dLytSkyGauge_c;

class dLytSkyGaugeMain_c {
public:
    dLytSkyGaugeMain_c();
    virtual ~dLytSkyGaugeMain_c() {}

    bool build(d2d::ResAccIf_c *resAcc);
    bool execute();
    bool remove();

    void changeToIn();
    void changeToMove();
    void changeToNone();
    void changeToOut();

    void setHeight(f32 height);

    bool wantsMove() {
        return mWantsModeChange;
    }

    const d2d::dLytSub &getLyt() const {
        return mLyt;
    }

    d2d::dLytSub &getLyt() {
        return mLyt;
    }

private:
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeMove);
    STATE_FUNC_DECLARE(dLytSkyGaugeMain_c, ModeOut);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytSkyGaugeMain_c);
    /* 0x040 */ d2d::dLytSub mLyt;
    /* 0x0D4 */ d2d::AnmGroup_c mAnmGroups[3];
    /* 0x194 */ bool mWantsModeChange;
    /* 0x195 */ bool field_0x195; // unused
};

class dLytSkyGauge_c : public d2d::dSubPane {
public:
    dLytSkyGauge_c() : mStateMgr(*this, sStateID::null) {}
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

    virtual ~dLytSkyGauge_c() {}

    void setHeight(f32 height);

    static dLytSkyGauge_c *sInstance;
    void setWantsIn() {
        mWantsIn = true;
    }
    void setWantsOut() {
        mWantsOut = true;
    }

private:
    STATE_FUNC_DECLARE(dLytSkyGauge_c, None);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, In);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, Move);
    STATE_FUNC_DECLARE(dLytSkyGauge_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytSkyGauge_c);

    /* 0x008 */ UI_STATE_MGR_DECLARE(dLytSkyGauge_c);
    /* 0x008 */ d2d::ResAccIf_c mResAcc;
    /* 0x3B4 */ dLytSkyGaugeMain_c mMain;
    /* 0x54C */ bool mWantsIn;
    /* 0x54D */ bool mWantsOut;
};

#endif
