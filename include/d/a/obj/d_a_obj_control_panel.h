#ifndef D_A_OBJ_CONTROL_PANEL_H
#define D_A_OBJ_CONTROL_PANEL_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOCtrlPnl_c : public dAcObjBase_c {
public:
    dAcOCtrlPnl_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOCtrlPnl_c() {}

    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, Wait);
    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, Control);
    STATE_FUNC_DECLARE(dAcOCtrlPnl_c, EndEvent);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOCtrlPnl_c);
};

#endif
