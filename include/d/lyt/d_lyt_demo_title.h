#ifndef D_LYT_DEMO_TITLE_H
#define D_LYT_DEMO_TITLE_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_base.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"


class dLytDemoTitleMain_c {
public:
    dLytDemoTitleMain_c();
    virtual ~dLytDemoTitleMain_c();

    bool fn_802B0760(d2d::ResAccIf_c *resAcc);
    bool fn_802B0860();
    bool fn_802B0950();
    bool fn_802B0980();
    void fn_802B09F0();
    void fn_802B0A50();
    void fn_802B0B00();
    void fn_802B0B50();
    void fn_802B0BD0();
    void fn_802B0C20();

    bool getmIsAnimating() {
        return mIsAnimating;
    }

    d2d::LytBase_c getmLytBase() {
        return mLytBase;
    }

private:
    STATE_FUNC_DECLARE(dLytDemoTitleMain_c, ModeNone);
    STATE_FUNC_DECLARE(dLytDemoTitleMain_c, ModeIn);
    STATE_FUNC_DECLARE(dLytDemoTitleMain_c, ModeMove);
    STATE_FUNC_DECLARE(dLytDemoTitleMain_c, ModeOut);
    STATE_FUNC_DECLARE(dLytDemoTitleMain_c, ModeEnd);

    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytDemoTitleMain_c);
    /* 0x040 */ d2d::LytBase_c mLytBase;
    /* 0x0d0 */ d2d::AnmGroup_c mAnmGroups[2];
    /* 0x150 */ bool mIsAnimating;
};

class dLytDemoTitle_c : public dLytBase_c {
public:
    dLytDemoTitle_c();
    virtual ~dLytDemoTitle_c();

    bool fn_802B1270();
    bool fn_802B1300();
    bool fn_802B13A0();
    bool fn_802B1410();

private:
    static dLytDemoTitle_c *sInstance;

    STATE_FUNC_DECLARE(dLytDemoTitle_c, None);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, In);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, Move);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, Out);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, End);

    /* 0x08c */ UI_STATE_MGR_DECLARE(dLytDemoTitle_c);
    /* 0x040 */ d2d::ResAccIf_c resAcc;
    /* 0x438 */ dLytDemoTitleMain_c mMain;
    /* 0x58c */ bool mIsAnimating;
    /* 0x58d */ bool field_0x58d;
};
#endif
