#ifndef LYT_PAUSE_DISP_01_H
#define LYT_PAUSE_DISP_01_H

#include "d/d_cursor_hit_check.h"
#include "d/lyt/d2d.h"
#include "d/lyt/d_lyt_common_icon_material.h"
#include "s/s_State.hpp"

class dLytPauseDisp01_c {
public:
    dLytPauseDisp01_c();
    virtual ~dLytPauseDisp01_c() {}

    void init();

    STATE_FUNC_DECLARE(dLytPauseDisp01_c, None);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, In);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Wait);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Select);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, GetDemo);
    STATE_FUNC_DECLARE(dLytPauseDisp01_c, Out);

private:
    void displayElement(int idx, float value);

    /* 0x0004 */ STATE_MGR_DECLARE(dLytPauseDisp01_c);
    /* 0x0040 */ d2d::LytBase_c mLytBase;
    /* 0x00D0 */ d2d::AnmGroup_c field_0x00D0[15];
    /* 0x0490 */ dLytCommonIconMaterial_c field_0x2050[28];
    /* 0x96B0 */ d2d::SubPaneList mSubpanes;
    /* 0x96BC */ d2d::SubPaneListNode field_0xE11C[28];
    /* 0x987C */ dCursorHitCheckLyt_c field_0xE29C;
};

#endif
