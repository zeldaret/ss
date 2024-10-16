#ifndef D_A_OBJ_TIME_STAGE_BG_H
#define D_A_OBJ_TIME_STAGE_BG_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOTimeStageBg_c : public dAcObjBase_c {
public:
    dAcOTimeStageBg_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOTimeStageBg_c() {}

    STATE_FUNC_DECLARE(dAcOTimeStageBg_c, Wait);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOTimeStageBg_c);
};

#endif
