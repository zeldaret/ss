#ifndef D_LYT_MAP_CAPTURE_H
#define D_LYT_MAP_CAPTURE_H

#include "common.h"
#include "s/s_State.hpp"
#include "s/s_StateID.hpp"

class dLytMapCapture_c {
public:
    dLytMapCapture_c() :mStateMgr(*this, sStateID::null) {
        field_0x040 = 0;
        field_0x070 = 0.0f;
        field_0x074 = 0.0f;
        field_0x078 = 0;
        field_0x079 = 0;
        field_0x06C = 0.0f;
        field_0x068 = 0.0f;
        field_0x064 = 0.0f;
        mStateMgr.changeState(StateID_RenderingWait);
    }
    virtual ~dLytMapCapture_c() {}

    STATE_FUNC_DECLARE(dLytMapCapture_c, RenderingWait);
    STATE_FUNC_DECLARE(dLytMapCapture_c, RenderingWaitStep2);

private:
    /* 0x004 */ UI_STATE_MGR_DECLARE(dLytMapCapture_c);
    /* 0x040*/ UNKWORD field_0x040;
    /* 0x064 */ f32 field_0x064;
    /* 0x068 */ f32 field_0x068;
    /* 0x06C */ f32 field_0x06C;
    /* 0x070 */ f32 field_0x070;
    /* 0x074 */ f32 field_0x074;
    /* 0x078 */ u8 field_0x078;
    /* 0x079 */ u8 field_0x079;
};

#endif
