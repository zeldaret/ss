#ifndef D_LYT_CURSOR_STICK_H
#define D_LYT_CURSOR_STICK_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dLytCursorStick_c {
public:
    dLytCursorStick_c() : mStateMgr(*this, sStateID::null) {
        sInstance = this;
    }
    ~dLytCursorStick_c() {
        sInstance = nullptr;
    }

    static dLytCursorStick_c *sInstance;
    static bool draw();
    static bool similarToDraw();

private:
    STATE_FUNC_DECLARE(dLytCursorStick_c, Off);
    STATE_FUNC_DECLARE(dLytCursorStick_c, On);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytCursorStick_c);
    /* 0x03C */ d2d::ResAccIf_c mResAcc;
    /* 0x3AC */ d2d::dLytSub mLyt;
    /* 0x440 */ d2d::AnmGroup_c mAnmGroups[1];
    /* 0x480 */ u8 field_0x480[0x49C - 0x480];
};

#endif
