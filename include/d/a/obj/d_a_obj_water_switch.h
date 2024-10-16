#ifndef D_A_OBJ_WATER_SWITCH_H
#define D_A_OBJ_WATER_SWITCH_H

#include "d/a/obj/d_a_obj_base.h"
#include "s/s_State.hpp"
#include "s/s_StateMgr.hpp"

class dAcOwaterSwitch_c : public dAcObjBase_c {
public:
    dAcOwaterSwitch_c() : mStateMgr(*this, sStateID::null) {}
    virtual ~dAcOwaterSwitch_c() {}

    STATE_FUNC_DECLARE(dAcOwaterSwitch_c, Off);
    STATE_FUNC_DECLARE(dAcOwaterSwitch_c, Wait1);
    STATE_FUNC_DECLARE(dAcOwaterSwitch_c, Wait2);
    STATE_FUNC_DECLARE(dAcOwaterSwitch_c, Wait3);
    STATE_FUNC_DECLARE(dAcOwaterSwitch_c, On);

private:
    /* 0x??? */ STATE_MGR_DECLARE(dAcOwaterSwitch_c);
};

#endif
