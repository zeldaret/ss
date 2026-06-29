#ifndef D_LYT_DEMO_TITLE_H
#define D_LYT_DEMO_TITLE_H

#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"
#include "d/lyt/d_lyt_base.h"

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

    bool getmIsAnimating(){
        return mIsAnimating;
    }

    d2d::LytBase_c getmLytBase(){
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

    void fn_802B0C20();
    void fn_802B0CA0();
    void fn_802B0CB0();
    void fn_802B0CC0();
    void fn_802B0D20();
    void fn_802B0D30();
    void fn_802B0D40();
    void fn_802B0DB0();
    void fn_802B0DC0();
    void fn_802B0DD0();
    void fn_802B0E30();
    void fn_802B0E40();
    void fn_802B0E50();
    void fn_802B0E60();
    bool fn_802B1270(d2d::ResAccIf_c *resAcc);

private:
    STATE_FUNC_DECLARE(dLytDemoTitle_c, None);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, In);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, Move);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, Out);
    STATE_FUNC_DECLARE(dLytDemoTitle_c, End);

    
    /* 0x08c */ UI_STATE_MGR_DECLARE(dLytDemoTitle_c);
    /* 0x040 */ u8 padding[0xcc-0x0c8];
    /* 0x0cc */ nw4r::lyt::ArcResourceAccessor mAccessor;
    /* 0x17c */ nw4r::lyt::FontRefLink mFontRefLinks[5];
    /* 0x438 */ dLytDemoTitleMain_c mMain;
    /* 0x58c */ bool mIsAnimating;
    /* 0x58d */ bool field_0x58d;
};
#endif
