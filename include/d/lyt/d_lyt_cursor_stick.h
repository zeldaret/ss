#ifndef D_LYT_CURSOR_STICK_H
#define D_LYT_CURSOR_STICK_H

#include "common.h"
#include "d/lyt/d2d.h"
#include "nw4r/lyt/lyt_pane.h"
#include "s/s_State.hpp"

/** 2D UI - element outline when selecting with cursor */
class dLytCursorStick_c {
public:
    dLytCursorStick_c() : mStateMgr(*this, sStateID::null) {
        sInstance = this;
    }
    ~dLytCursorStick_c() {
        sInstance = nullptr;
    }

    static bool build();
    static bool remove();
    static bool execute();
    static bool draw();
    static bool drawDirectly();

    bool setPriority(u8 priority);
    void setTargetPane(nw4r::lyt::Pane *pane);

private:
    bool build_();
    bool remove_();
    bool execute_();
    bool draw_();
    bool drawDirectly_();
    static dLytCursorStick_c *sInstance;

    STATE_FUNC_DECLARE(dLytCursorStick_c, Off);
    STATE_FUNC_DECLARE(dLytCursorStick_c, On);

    void positionToTarget();

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytCursorStick_c);
    /* 0x03C */ d2d::ResAccIf_c mResAcc;
    /* 0x3AC */ d2d::dLytSub mLyt;
    /* 0x440 */ d2d::AnmGroup_c mAnm[1];
    /* 0x480 */ nw4r::lyt::Pane *mpPanes[5];
    /* 0x494 */ nw4r::lyt::Pane *mpTargetPane;
    /* 0x498 */ bool mShouldBeOn;
};

#endif
