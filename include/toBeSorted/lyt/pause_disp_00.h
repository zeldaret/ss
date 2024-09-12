#ifndef LYT_PAUSE_DISP_00_H
#define LYT_PAUSE_DISP_00_H

#include <s/s_State.hpp>
#include <s/s_StateMgr.hpp>
#include <toBeSorted/lyt/d2d.h>


class dLytPauseDisp00_c {
public:
    dLytPauseDisp00_c();
    virtual ~dLytPauseDisp00_c();

    void init();

    STATE_FUNC_DECLARE(dLytPauseDisp00_c, None);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, In);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Select);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Ring);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseDisp00_c, Out);

private:


    void displayElement(int idx, float value);

    STATE_MGR_DECLARE(dLytPauseDisp00_c);
    d2d::LytBase_c mLytBase;
    d2d::dLytStructA field_0x00D0[0x7E];
    d2d::dLytStructB field_0x2050[0x18];
    /* 0xE110 */ nw4r::ut::LinkList<d2d::dLytStructB, 0> mList;
    d2d::dLytStructC field_0xE11C[0x18];
    d2d::dLytStructD field_0xE29C;
};

#endif
