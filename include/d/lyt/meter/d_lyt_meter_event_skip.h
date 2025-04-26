#ifndef D_LYT_METER_EVENT_SKIP_H
#define D_LYT_METER_EVENT_SKIP_H

#include "d/lyt/d2d.h"
#include "d/lyt/d_textbox.h"
#include "d/lyt/d_window.h"
#include "s/s_State.hpp"

class dLytMeterEventSkip_c {
public:
    dLytMeterEventSkip_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dLytMeterEventSkip_c() {}
    bool build(d2d::ResAccIf_c *resAcc);
    bool remove();
    bool execute();
    bool draw();

private:
    bool shouldPromptForSkip() const;

    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Invisible);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, In);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Visible);
    STATE_FUNC_DECLARE(dLytMeterEventSkip_c, Out);

    STATE_MGR_DEFINE_UTIL_CHANGESTATE(dLytMeterEventSkip_c);

    /* 0x000 */ UI_STATE_MGR_DECLARE(dLytMeterEventSkip_c);
    /* 0x03C */ d2d::LytBase_c mLyt;
    /* 0x0D0 */ d2d::AnmGroup_c mAnm[2];
    /* 0x150 */ dTextBox_c *mpTextBoxes[2];
    /* 0x158 */ dWindow_c *mpWindow;
    /* 0x15C */ dTextBox_c *mpSizeBox;
    /* 0x160 */ bool mVisible;
};

#endif
